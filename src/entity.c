#include "entity.h"

#include <genesis.h>

// graphics resources
#include "gfx.h"

void initEnt(struct Entity* ent, enum dir initFacing, fix16 initPosx,
             fix16 initPosy, fix16 initMaxSpeed) {
  ent->dir = initFacing;
  ent->posx = initPosx;
  ent->posy = initPosy;
  ent->vel = FIX16(0);

  ent->maxspd = initMaxSpeed;
};

void set_anim(struct Entity* ent) {
  bool hflip = FALSE;

  if (ent->vel > 0) {
    // walking anim
    switch (ent->dir) {
      case down:
        ent->anim = down_walk;
        break;
      case down_right:
        hflip = TRUE;
      case down_left:
        ent->anim = down_walk_diag;
        break;
      case up:
        ent->anim = up_walk;
        break;
      case up_right:
        hflip = TRUE;
      case up_left:
        ent->anim = up_walk_diag;
        break;
      case right:
        hflip = TRUE;
      case left:
        ent->anim = side_walk;
        break;
    }
  } else {
    // wait anim
    switch (ent->dir) {
      case down:
        ent->anim = down_wait;
        break;
      case down_right:
        hflip = TRUE;
      case down_left:
        ent->anim = down_wait_diag;
        break;
      case up:
        ent->anim = up_wait;
        break;
      case up_right:
        hflip = TRUE;
      case up_left:
        ent->anim = up_wait_diag;
        break;
      case right:
        hflip = TRUE;
      case left:
        ent->anim = side_wait;
        break;
    }
  }
  SPR_setHFlip(ent->sprite, hflip);
  SPR_setAnim(ent->sprite, ent->anim);
}