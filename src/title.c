#include <genesis.h>
#include "title.h"
#include "global.h"

#include "../res/gfx_title.h"

u8 titleMode = 0;
u8 progMode;
u16 joy1Input;

void doTitle() {
    switch(titleMode) {
        case 0:
            initTitle();
        break;
        case 1:
            titleInput();
        break;
    }
}

void initTitle() {

    u16 tileIdx = TILE_USERINDEX;

    SYS_disableInts();

    VDP_setPalette(PAL0, palette_black);
    VDP_setPalette(PAL1, imgTitleArle.palette->data);
    VDP_setPalette(PAL2, imgTitleCar.palette->data);
    VDP_setBackgroundColor(1);

    VDP_setTextPriority(1);
    VDP_setTextPalette(PAL1);

    SYS_enableInts();

    VDP_drawImageEx(VDP_PLAN_B, &imgTitleArle, TILE_ATTR_FULL(PAL1, 0, 0, 0, tileIdx), 8, 8, 0, 0 );
    tileIdx += imgTitleArle.tileset->numTile;
    VDP_drawImageEx(VDP_PLAN_A, &imgTitleCar, TILE_ATTR_FULL(PAL2, 0, 0, 0, tileIdx), 6, 21, 0,0);

    VDP_drawText("CARBUNCLE CHASE!!", 16, 10);

    titleMode = 1;
}

void titleInput() {
    if (joy1Input & BUTTON_START) {
        VDP_clearPlan(VDP_PLAN_A, 1);
        VDP_clearPlan(VDP_PLAN_B, 1);
        progMode = MODE_STAGE;
    }
}