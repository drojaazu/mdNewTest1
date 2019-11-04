#include "walking.h"

#include "../res/gfx_walking.h"
#include "entity.h"
#include "global.h"

u16 joy1Input;

u8 walkingMode = 0;

Sprite sprites[2];
struct Entity entArle;
struct Entity entCar;

s16 ytoward;
s16 xtoward;

fix32 car_posx;
fix32 car_posy;

fix32 p1_posx;
fix32 p1_posy;

void doWalking() {
  switch (walkingMode) {
    case 0:
      initStage();
      break;
    case 1:
      processWalking();
      break;
  }
}

void initStage() {
  // disable interrupt when accessing VDP
  SYS_disableInts();
  // initialization
  VDP_setScreenWidth320();
  SPR_init(0, 0, 0);

  // entity initialization
  initEnt(&entArle, FACING_DOWN, 0, 0);
  initEnt(&entCar, FACING_DOWN, 0, 0);

  VDP_setPalette(PAL0, palette_black);
  VDP_setPalette(PAL1, sprArle.palette->data);
  VDP_setPalette(PAL2, sprCar.palette->data);
  VDP_setPalette(PAL3, grassTilesPal.data);

  VDP_setTextPalette(PAL1);

  // sprite initialization

  sprites[0] =
      *(SPR_addSprite(&sprArle, 0, 0, TILE_ATTR(PAL1, TRUE, FALSE, FALSE)));
  sprites[1] =
      *(SPR_addSprite(&sprCar, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE)));
  SPR_update();

  // set up bg tiles
  VDP_loadTileSet(&grassTiles, TILE_USERINDEX, 0);

  // VDP_fillTileMapRect(VDP_PLAN_A, TILE_ATTR_FULL(PAL3, 0, 0, 0,
  // TILE_USERINDEX), 0, 0, 40, 40); VDP_fillTileMapRect(VDP_PLAN_A,
  // TILE_ATTR_FULL(PAL3, 0, 0, 0, TILE_USERINDEX+1), 1, 12, 5, 4);
  u8 grassX, grassY;

  for (grassY = 0; grassY < 40; grassY += 2) {
    for (grassX = 0; grassX < 40; grassX += 2) {
      VDP_setTileMapXY(PLAN_A, TILE_ATTR_FULL(PAL3, 0, 0, 0, TILE_USERINDEX),
                       grassX, grassY);
      VDP_setTileMapXY(PLAN_A,
                       TILE_ATTR_FULL(PAL3, 0, 0, 0, TILE_USERINDEX + 1),
                       grassX + 1, grassY);
      VDP_setTileMapXY(PLAN_A,
                       TILE_ATTR_FULL(PAL3, 0, 0, 0, TILE_USERINDEX + 2),
                       grassX, grassY + 1);
      VDP_setTileMapXY(PLAN_A,
                       TILE_ATTR_FULL(PAL3, 0, 0, 0, TILE_USERINDEX + 3),
                       grassX + 1, grassY + 1);
    }
  }

  SYS_enableInts();
  updateCamera(FIX32(0), FIX32(0));

  // VDP_fadeIn(0, (3 * 16) - 1, arleDef.palette->data, 20, FALSE);

  walkingMode = 1;
}

char car_posx_msg[6];
char car_posy_msg[6];

void processWalking() {
  updatePhysics();
  updateAnim();

  SPR_update();
  SPR_setPosition(&sprites[0], 10, 10);
  fix32ToStr(entCar.posx, car_posx_msg, 2);
  fix32ToStr(entCar.posy, car_posy_msg, 2);

  // VDP_drawText(car_posx_msg, 30, 2);
  // VDP_drawText(car_posy_msg, 30, 3);
}

fix32 camposx = 0;
fix32 camposy = 0;

void updateCamera(fix32 x, fix32 y) {
  if ((x != camposx) || (y != camposy)) {
    camposx = x;
    camposy = y;
    VDP_setHorizontalScroll(PLAN_A, fix32ToInt(-camposx));
    VDP_setVerticalScroll(PLAN_A, fix32ToInt(camposy));
  }
}

void updatePhysics() {
  if (joy1Input & BUTTON_UP)
    ytoward = -1;
  else if (joy1Input & BUTTON_DOWN)
    ytoward = 1;
  else
    ytoward = 0;

  if (joy1Input & BUTTON_LEFT)
    xtoward = -1;
  else if (joy1Input & BUTTON_RIGHT)
    xtoward = 1;
  else
    xtoward = 0;

  // set player pos
  entArle.posx += FIX32(xtoward);
  entArle.posy += FIX32(ytoward);
  if (xtoward != 0 || ytoward != 0)
    entArle.moving = TRUE;
  else
    entArle.moving = FALSE;

  if (ytoward > 0) {
    if (xtoward > 0) {
      entArle.hflip = TRUE;
      entArle.facing = FACING_DOWN_DIAG;
    } else if (xtoward < 0) {
      entArle.hflip = FALSE;
      entArle.facing = FACING_DOWN_DIAG;
    } else {
      entArle.hflip = FALSE;
      entArle.facing = FACING_DOWN;
    }
  } else if (ytoward < 0) {
    if (xtoward > 0) {
      entArle.hflip = TRUE;
      entArle.facing = FACING_UP_DIAG;
    } else if (xtoward < 0) {
      entArle.hflip = FALSE;
      entArle.facing = FACING_UP_DIAG;
    } else {
      entArle.hflip = FALSE;
      entArle.facing = FACING_UP;
    }
  } else if (xtoward != 0) {
    entArle.facing = FACING_SIDE;
    if (xtoward > 0) {
      entArle.hflip = TRUE;
    } else if (xtoward < 0) {
      entArle.hflip = FALSE;
    }
  }

  // set car pos
  int movXleft = FALSE;
  int movXright = FALSE;
  int movYup = FALSE;
  int movYdown = FALSE;

  int destX = 0;
  int destY = 0;
  if (entArle.hflip)
    destX = entArle.posx + FIX32(10);
  else
    destX = entArle.posx - FIX32(10);

  if (entArle.facing == FACING_UP || entArle.facing == FACING_UP_DIAG) {
    destY = entArle.posy + FIX32(15);
  } else {
    destY = entArle.posy - FIX32(15);
  }

  if (entCar.posx != destX || entCar.posy != destY) {
    entCar.moving = TRUE;
    if (entCar.posx < destX) {
      entCar.posx += FIX32(0.5);
      movXleft = TRUE;
      entCar.hflip = TRUE;
    } else if (entCar.posx > destX) {
      entCar.posx -= FIX32(0.5);
      movXright = TRUE;
      entCar.hflip = FALSE;
    }
    if (entCar.posy < destY) {
      entCar.posy += FIX32(0.5);
      movYdown = TRUE;
    } else if (entCar.posy > destY) {
      entCar.posy -= FIX32(0.5);
      movYup = TRUE;
    }
    if (movXleft || movXright) {
      if (movYdown) {
        entCar.facing = FACING_DOWN_DIAG;
      } else if (movYup) {
        entCar.facing = FACING_UP_DIAG;
      } else {
        entCar.facing = FACING_SIDE;
      }
    } else if (movYup || movYdown) {
      if (movYup)
        entCar.facing = FACING_UP;
      else if (movYdown)
        entCar.facing = FACING_DOWN;
    }
    entCar.moving = TRUE;

    fix32 px_scr, py_scr;
    fix32 npx_cam, npy_cam;

    // get sprite position on screen
    px_scr = entArle.posx - camposx;
    py_scr = entArle.posy - camposy;

    // calculate new camera position
    if (px_scr > FIX32(140))
      npx_cam = entArle.posx - FIX32(140);
    else if (px_scr < FIX32(80))
      npx_cam = entArle.posx - FIX32(80);
    else
      npx_cam = camposx;
    if (py_scr > FIX32(160))
      npy_cam = entArle.posy - FIX32(160);
    else if (py_scr < FIX32(100))
      npy_cam = entArle.posy - FIX32(100);
    else
      npy_cam = camposy;

    // clip camera position
    if (npx_cam < FIX32(0))
      npx_cam = FIX32(0);
    else if (npx_cam > FIX32(200))
      npx_cam = FIX32(200);
    if (npy_cam < FIX32(0))
      npy_cam = FIX32(0);
    else if (npy_cam > FIX32(100))
      npy_cam = FIX32(100);

    // set camera position
    updateCamera(npx_cam, npy_cam);

  } else {
    entCar.moving = FALSE;
    entCar.facing = entArle.facing;
  }

  SPR_setPosition(&sprites[0], fix32ToInt(entArle.posx),
                  fix32ToInt(entArle.posy));
  SPR_setPosition(&sprites[1], fix32ToInt(entCar.posx),
                  fix32ToInt(entCar.posy));
}

void updateAnim() {
  // update Arle
  if (entArle.moving) {
    switch (entArle.facing) {
      case FACING_DOWN:
        SPR_setAnim(&sprites[0], ANIM_WALK_DOWN);
        break;
      case FACING_DOWN_DIAG:
        SPR_setAnim(&sprites[0], ANIM_WALK_DOWN_DIAG);
        break;
      case FACING_UP:
        SPR_setAnim(&sprites[0], ANIM_WALK_UP);
        break;
      case FACING_UP_DIAG:
        SPR_setAnim(&sprites[0], ANIM_WALK_UP_DIAG);
        break;
      case FACING_SIDE:
        SPR_setAnim(&sprites[0], ANIM_WALK_SIDE);
        break;
    }
  } else {
    switch (entArle.facing) {
      case FACING_DOWN:
        SPR_setAnim(&sprites[0], ANIM_WAIT_DOWN);
        break;
      case FACING_DOWN_DIAG:
        SPR_setAnim(&sprites[0], ANIM_WAIT_DOWN_DIAG);
        break;
      case FACING_UP:
        SPR_setAnim(&sprites[0], ANIM_WAIT_UP);
        break;
      case FACING_UP_DIAG:
        SPR_setAnim(&sprites[0], ANIM_WAIT_UP_DIAG);
        break;
      case FACING_SIDE:
        SPR_setAnim(&sprites[0], ANIM_WAIT_SIDE);
        break;
    }
  }

  // update Car
  if (entCar.moving) {
    switch (entCar.facing) {
      case FACING_DOWN:
        SPR_setAnim(&sprites[1], ANIM_WALK_DOWN);
        break;
      case FACING_DOWN_DIAG:
        SPR_setAnim(&sprites[1], ANIM_WALK_DOWN_DIAG);
        break;
      case FACING_UP:
        SPR_setAnim(&sprites[1], ANIM_WALK_UP);
        break;
      case FACING_UP_DIAG:
        SPR_setAnim(&sprites[1], ANIM_WALK_UP_DIAG);
        break;
      case FACING_SIDE:
        SPR_setAnim(&sprites[1], ANIM_WALK_SIDE);
        break;
    }
  } else {
    switch (entCar.facing) {
      case FACING_DOWN:
        SPR_setAnim(&sprites[1], ANIM_WAIT_DOWN);
        break;
      case FACING_DOWN_DIAG:
        SPR_setAnim(&sprites[1], ANIM_WAIT_DOWN_DIAG);
        break;
      case FACING_UP:
        SPR_setAnim(&sprites[1], ANIM_WAIT_UP);
        break;
      case FACING_UP_DIAG:
        SPR_setAnim(&sprites[1], ANIM_WAIT_UP_DIAG);
        break;
      case FACING_SIDE:
        SPR_setAnim(&sprites[1], ANIM_WAIT_SIDE);
        break;
    }
  }

  if (entCar.hflip)
    SPR_setVRAMTileIndex(&sprites[1], TILE_ATTR(PAL2, TRUE, FALSE, TRUE));
  else
    SPR_setVRAMTileIndex(&sprites[1], TILE_ATTR(PAL2, TRUE, FALSE, FALSE));

  if (entArle.hflip)
    SPR_setVRAMTileIndex(&sprites[0], TILE_ATTR(PAL1, TRUE, FALSE, TRUE));
  else
    SPR_setVRAMTileIndex(&sprites[0], TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
}
