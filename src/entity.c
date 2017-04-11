#include <genesis.h>
#include "entity.h"

// graphics resources
#include "gfx.h"

void initEnt(struct Entity* ent, char initFacing, fix32 initPosx, fix32 initPosy)
{
    ent->facing = initFacing;
    ent->posx = initPosx;
    ent->posy = initPosy;
    ent->spdx = FIX32(0);
    ent->spdy = FIX32(0);
    ent->moving = FALSE;
    ent->maxspd = FIX32(1);
    ent->hflip = FALSE;
};
