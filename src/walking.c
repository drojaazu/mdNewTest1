#include "walking.h"

u16 joy1Input;

u8 walkingMode = 0;

struct Entity entArle;
struct Entity entCar;

s16 ytoward;
s16 xtoward;

fix16 car_posx;
fix16 car_posy;

fix16 p1_posx;
fix16 p1_posy;

enum dir p1_dir;

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
  initEnt(&entArle, FACING_DOWN, 0, 0, 100);
  initEnt(&entCar, FACING_DOWN, 0, 0, 60);

  VDP_setPalette(PAL0, palette_black);
  VDP_setPalette(PAL1, sprArle.palette->data);
  VDP_setPalette(PAL2, sprCar.palette->data);
  VDP_setPalette(PAL3, grassTilesPal.data);

  // VDP_setTextPlan(PLAN_A);
  // VDP_setTextPriority(1);
  // VDP_setTextPalette(PAL1);

  // sprite initialization

  entArle.sprite =
      SPR_addSprite(&sprArle, 0, 0, TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
  entCar.sprite =
      SPR_addSprite(&sprCar, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
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
  updateCamera(FIX16(0), FIX16(0));

  // VDP_fadeIn(0, (3 * 16) - 1, arleDef.palette->data, 20, FALSE);

  walkingMode = 1;
}

char car_posx_msg[6];
char car_posy_msg[6];

void processWalking() {
  updatePhysics();
  updateAnim();

  set_anim(&entArle);
  set_anim(&entCar);

  SPR_update();
  fix16ToStr(entArle.posx, car_posx_msg, 2);
  fix16ToStr(entArle.posy, car_posy_msg, 2);

  // VDP_drawText(car_posx_msg, 30, 2);
  // VDP_drawText(car_posy_msg, 30, 3);
}

fix16 camposx = 0;
fix16 camposy = 0;

void updateCamera(fix16 x, fix16 y) {
  if ((x != camposx) || (y != camposy)) {
    camposx = x;
    camposy = y;
    VDP_setHorizontalScroll(PLAN_A, fix16ToInt(-camposx));
    VDP_setVerticalScroll(PLAN_A, fix16ToInt(camposy));
  }
}

void updatePhysics() {
  p1_dir = joy1Input;
  if (p1_dir > 0) entArle.dir = p1_dir;

  // set player pos
  if (p1_dir > 0) {
    if (entArle.vel < entArle.maxspd) entArle.vel += 5;
  } else {
    entArle.vel -= 7;
    if (entArle.vel < 0) entArle.vel = 0;
  }

  if (entArle.dir & up)
    entArle.posy -= entArle.vel;
  else if (entArle.dir & down)
    entArle.posy += entArle.vel;

  if (entArle.dir & right)
    entArle.posx += entArle.vel;
  else if (entArle.dir & left)
    entArle.posx -= entArle.vel;

  // set car pos
  if (entCar.posx != (entArle.posx - 15) ||
      entCar.posy != (entArle.posy + 10)) {
    int destX = entArle.posx - 15;
    int destY = entArle.posy + 10;
    entCar.dir = 0;

    if (entCar.posx < destX)
      entCar.dir |= right;
    else if (entCar.posx > destX)
      entCar.dir |= left;
    else
      entCar.dir = entArle.dir;

    if (entCar.posy < destY)
      entCar.dir |= down;
    else if (entCar.posy > destY)
      entCar.dir |= up;
    else
      entCar.dir = entArle.dir;

    if (entCar.vel < entCar.maxspd) entCar.vel++;

    if (entCar.dir & up)
      entCar.posy -= entCar.vel;
    else if (entCar.dir & down)
      entCar.posy += entCar.vel;

    if (entCar.dir & right)
      entCar.posx += entCar.vel;
    else if (entCar.dir & left)
      entCar.posx -= entCar.vel;
  }

  SPR_setPosition(entArle.sprite, fix16ToInt(entArle.posx),
                  fix16ToInt(entArle.posy));
  SPR_setPosition(entCar.sprite, fix16ToInt(entCar.posx),
                  fix16ToInt(entCar.posy));

  fix16 px_scr, py_scr;
  fix16 npx_cam, npy_cam;

  // get sprite position on screen
  px_scr = entArle.posx - camposx;
  py_scr = entArle.posy - camposy;

  // calculate new camera position
  if (px_scr > FIX16(140))
    npx_cam = entArle.posx - FIX16(140);
  else if (px_scr < FIX16(80))
    npx_cam = entArle.posx - FIX16(80);
  else
    npx_cam = camposx;
  if (py_scr > FIX16(160))
    npy_cam = entArle.posy - FIX16(160);
  else if (py_scr < FIX16(100))
    npy_cam = entArle.posy - FIX16(100);
  else
    npy_cam = camposy;

  // clip camera position
  if (npx_cam < FIX16(0))
    npx_cam = FIX16(0);
  else if (npx_cam > FIX16(200))
    npx_cam = FIX16(200);
  if (npy_cam < FIX16(0))
    npy_cam = FIX16(0);
  else if (npy_cam > FIX16(100))
    npy_cam = FIX16(100);

  // set camera position
  updateCamera(npx_cam, npy_cam);
}

void updateAnim() {
  // update Arle
  set_anim(&entArle);

  // update Car
  set_anim(&entCar);
}
