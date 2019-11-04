#ifndef _WALKING_H_
#define _WALKING_H_
#include <genesis.h>

#include "../res/gfx.h"
#include "entity.h"
#include "global.h"

void updateAnim();
void updatePhysics();

void doWalking();
void initStage();
void processWalking();
void updateCamera(fix16, fix16);

#endif  // _WALKING_H_