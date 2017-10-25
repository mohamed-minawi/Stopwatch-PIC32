#ifndef MC_DISPATCHER_H
#define MC_DISPATCHER_H

void QueTask(void(*ptr)(void));
void QueDelay(void(*ptr)(void), int);
void ReRunMe(int);
void InitMulti();
void Dispatch();
void FinishDelay();

#endif