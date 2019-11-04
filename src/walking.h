#ifndef _WALKING_H_
#define _WALKING_H_
#include <genesis.h>
void updateAnim();
void updatePhysics();

void doWalking();
void initStage();
void processWalking();
void updateCamera(fix32, fix32);

#endif  // _WALKING_H_