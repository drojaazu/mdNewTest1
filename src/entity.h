#define FACING_DOWN 0
#define FACING_UP 1
#define FACING_DOWN_DIAG 2
#define FACING_UP_DIAG 3
#define FACING_SIDE 4

#define ANIM_WAIT_DOWN 0
#define ANIM_WALK_DOWN 1
#define ANIM_WAIT_DOWN_DIAG 2
#define ANIM_WALK_DOWN_DIAG 3
#define ANIM_WAIT_UP_DIAG 4
#define ANIM_WALK_UP_DIAG 5
#define ANIM_WAIT_SIDE 6
#define ANIM_WALK_SIDE 7
#define ANIM_WAIT_UP 8
#define ANIM_WALK_UP 9

// game characters
#define ENT_ARLE 0
#define ENT_CAR 1

#include <genesis.h>

enum dir {
  up = 1,
  down = 2,
  left = 4,
  right = 8,
  up_left = 5,
  up_right = 9,
  down_left = 6,
  down_right = 10
};

enum anim {
  down_wait,
  down_walk,
  down_wait_diag,
  down_walk_diag,
  up_wait_diag,
  up_walk_diag,
  side_wait,
  side_walk,
  up_wait,
  up_walk
};

struct Entity {
  Sprite* sprite;

  fix16 posx;
  fix16 posy;
  fix16 vel;
  fix16 maxspd;

  enum dir dir;
  enum anim anim;
};

void set_anim(struct Entity* ent);

void initEnt(struct Entity*, enum dir, fix16, fix16, fix16);
