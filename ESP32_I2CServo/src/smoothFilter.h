#ifndef SMOOTHFILTER_H
#define SMOOTHFILTER_H

#include <Arduino.h>

// Tamanho da média. 10 a 20 é um bom valor.
#define FILTER_WINDOW_SIZE 15 

class SmoothFilter {
  private:
    int readings[FILTER_WINDOW_SIZE]; 
    int readIndex;
    long total;
    int pin;

  public:
    SmoothFilter(int pino) {
      pin = pino;
      readIndex = 0;
      total = 0;
      // Inicializa tudo com 0
      for (int i = 0; i < FILTER_WINDOW_SIZE; i++) {
        readings[i] = 0;
      }
      // Configura o pino
      pinMode(pin, INPUT);
    }

    // Função que retorna o valor analógico limpo (Média Móvel)
    int read() {
      // Remove o valor mais antigo do total
      total = total - readings[readIndex];

      // Lê o sensor atual
      int val = analogRead(pin);
      
      // Guarda no buffer
      readings[readIndex] = val;

      // Adiciona ao total
      total = total + readings[readIndex];

      // Avança o índice
      readIndex = readIndex + 1;
      if (readIndex >= FILTER_WINDOW_SIZE) {
        readIndex = 0;
      }

      // Retorna a média
      return total / FILTER_WINDOW_SIZE;
    }
};

#endif