#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// Inclua seus headers
#include "buttons.h"
#include "display.h"
#include "emg.h"
#include "hand.h"
#include "now.h"
#include "prefs.h"

// --- DEFINIÇÕES DE FREERTOS ---

// Handles para as tarefas
TaskHandle_t TaskSensorHandle = NULL;
TaskHandle_t TaskInterfaceHandle = NULL;

// Mutex para proteger variáveis globais compartilhadas
SemaphoreHandle_t xGlobalStateMutex;

// --- PROTÓTIPOS DAS TAREFAS ---
void TaskSensorLogic(void *pvParameters);
void TaskInterfaceComms(void *pvParameters);

void setup()
{
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    Serial.begin(115200);

    // 1. Criação do Mutex antes de qualquer tarefa
    xGlobalStateMutex = xSemaphoreCreateMutex();

    // 2. Inicializações de Hardware (Single Threaded - Seguro fazer aqui)
    initDisplay();
    delay(1000);
    initButtons();
    delay(1000);
    initHand();
    delay(1000);
    initEMG(); // Calibrações iniciais
    delay(1000);
    initNow(); // Inicializa ESP-NOW

    // Opcional: Salvar configs iniciais se necessário (debug)
    // savePrefs();

    // 3. Criação das Tarefas

    // TAREFA 1: ALTA PRIORIDADE / TEMPO REAL (Core 1)
    // Responsável por: Ler EMG, Tratar Botões, Calcular Lógica da Mão
    xTaskCreatePinnedToCore(
        TaskSensorLogic,   // Função da tarefa
        "SensorLogic",     // Nome
        4096,              // Stack Size (memória)
        NULL,              // Parâmetros
        2,                 // Prioridade (2 = Alta)
        &TaskSensorHandle, // Handle
        1                  // Core ID (1 = App Core, padrão Arduino)
    );

    // TAREFA 2: BAIXA PRIORIDADE / I/O LENTO (Core 0)
    // Responsável por: Atualizar OLED, Enviar ESP-NOW
    xTaskCreatePinnedToCore(
        TaskInterfaceComms,   // Função da tarefa
        "InterfaceComms",     // Nome
        8192,                 // Stack Size (Display precisa de mais RAM)
        NULL,                 // Parâmetros
        1,                    // Prioridade (1 = Baixa)
        &TaskInterfaceHandle, // Handle
        0                     // Core ID (0 = Pro Core, junto com WiFi)
    );

    Serial.println("Sistema FreeRTOS Iniciado...");
}

void loop()
{
    // No FreeRTOS, o loop() padrão deve ficar vazio ou ser deletado.
    // A tarefa "Loop" do Arduino roda com prioridade 1 no Core 1.
    vTaskDelete(NULL); // Mata a tarefa loop padrão para economizar ciclos
}

// --- IMPLEMENTAÇÃO DAS TAREFAS ---

/* * TAREFA DE LÓGICA RÁPIDA (CORE 1)
 * Executa a cada 10ms (100Hz) ou mais rápido se necessário.
 * Foca em ler sensores e registrar inputs.
 */
void TaskSensorLogic(void *pvParameters)
{
    static unsigned long lastToggleTime = 0;
    bool inCooldown = false;
    static bool lastEmgActive = false;

    for (;;)
    {
        // 1. Atualizar Botões (OneButton precisa de tick frequente)
        // Protegemos com Mutex pois botões alteram ACTION_PAGE que o Display lê
        if (xSemaphoreTake(xGlobalStateMutex, (TickType_t)10) == pdTRUE)
        {
            btnTicks();
            xSemaphoreGive(xGlobalStateMutex);
        }

        // 2. Processar Lógica EMG (Leitura analógica e limites)
        // Nota: emgLines altera HAND_STATE e GESTURE
        // Podemos ler o analógico FORA do mutex (rápido), mas atualizar estados DENTRO

        // Pequena otimização: Ler analógico fora do bloqueio para não travar o Display
        int rawEmg = analogRead(ANALOG_EMG_1_PIN);
        //(Se você alterar emg.cpp para receber valor)

        if (xSemaphoreTake(xGlobalStateMutex, (TickType_t)10) == pdTRUE)
        {
            // A lógica atual de emgLines faz leitura e lógica juntas.
            // Idealmente refatore emgLines, mas por enquanto chamamos direto.

            // Exemplo: Atualiza lógica se estamos na página correta ou sempre
            // Você pode decidir se o EMG roda sempre ou só em certas páginas
            // Assumindo que roda sempre para manter o estado atualizado:
            if (ACTION_MODE == 0)
            {
                emgLines(rawEmg, false, TIME_OUT);
            }
            else if (ACTION_MODE == 4 || ACTION_MODE == 2)
            {
                unsigned long currentMillis = millis();

                if (inCooldown)
                {
                    unsigned long timePassed = currentMillis - lastToggleTime;

                    if (timePassed > EMG_TOGGLE_TIME)
                    {
                        inCooldown = false;
                        COOLDOWN_VAL = 0;
                    }
                    else
                    {
                        long remaining = EMG_TOGGLE_TIME - timePassed;
                        COOLDOWN_VAL = map(remaining, 0, EMG_TOGGLE_TIME, 0, 100);
                    }
                }
                else
                {
                    COOLDOWN_VAL = 0;
                }

                bool currentEmgActive = emgLinesReturn(rawEmg, TIME_OUT);

                if (!inCooldown)
                {

                    if (currentEmgActive && !lastEmgActive)
                    {
                        GESTURE += 1;
                        if (GESTURE > 5)
                        {
                            GESTURE = 0;
                        }
                        HAND_STATE = !HAND_STATE;
                        inCooldown = true;
                        lastToggleTime = currentMillis;
                        COOLDOWN_VAL = 100;
                    }
                }
                lastEmgActive = currentEmgActive;
            }
            else
            {
                inCooldown = false;
                COOLDOWN_VAL = 0;
            }
            xSemaphoreGive(xGlobalStateMutex);
        }
        Serial.println(HAND_STATE ? "Closed" : "Open");
        Serial.println(GESTURE);
        // Delay para permitir que o Watchdog do processador respire e definir a frequência
        vTaskDelay(pdMS_TO_TICKS(10)); // ~10ms delay (100 ticks por segundo para botões é ótimo)
    }
}

/* * TAREFA DE INTERFACE E COMUNICAÇÃO (CORE 0)
 * Executa a cada 50ms (20fps) para o display.
 * Display OLED I2C é lento, não queremos travar o sensor EMG.
 */
void TaskInterfaceComms(void *pvParameters)
{
    unsigned long lastCommsTime = 0;
    const unsigned long commsInterval = 300;

    for (;;)
    {

        // 1. Atualizar Display
        // O display lê muitas variaveis globais. Precisamos garantir que
        // o TaskSensorLogic não mude os valores no meio da renderização da tela.
        if (xSemaphoreTake(xGlobalStateMutex, (TickType_t)20) == pdTRUE)
        {
            menu(); // Chama a função principal de display
            xSemaphoreGive(xGlobalStateMutex);
        }

        // 2. Enviar ESP-NOW (A cada 300ms)
        // Usamos millis() aqui dentro ou um contador simples
        unsigned long now = millis();
        if (now - lastCommsTime > commsInterval)
        {
            lastCommsTime = now;

            // Preparar dados e enviar
            if (xSemaphoreTake(xGlobalStateMutex, (TickType_t)10) == pdTRUE)
            {
                sendPackMillis(0); // Passamos 0 pois controlamos o tempo aqui fora
                xSemaphoreGive(xGlobalStateMutex);
            }
        }

        // Frequência de atualização da tela ~30ms (33 FPS)
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}