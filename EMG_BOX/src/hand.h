#ifndef HAND_H
#define HAND_H

extern int THUMB_POS;
extern int INDEX_POS;
extern int MIDDLE_POS;
extern int RING_POS;
extern int LITTLE_POS;
extern int GESTURE;
extern int ACTION_MODE;
extern bool HAND_STATE;
extern bool FEEDBACK_STATE;
extern int THRESHOLD_THUMB;
extern int THRESHOLD_INDEX;
extern int THRESHOLD_MIDDLE;
extern int THRESHOLD_RING;
extern int THRESHOLD_LITTLE;
extern int EMG_TOGGLE_TIME;

int setFingerPos(int finger, int value);
void setGesture(int value);
void updateToggleTime(int value);
void setHandState(bool value);
void setFeedbackState(bool value);
int setThresholdFinger(int finger, int value);
void initHand();
#endif