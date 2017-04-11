#define FACING_DOWN       0
#define FACING_UP         1
#define FACING_DOWN_DIAG  2
#define FACING_UP_DIAG    3
#define FACING_SIDE       4

#define ANIM_WAIT_DOWN          0
#define ANIM_WALK_DOWN          1
#define ANIM_WAIT_DOWN_DIAG     2
#define ANIM_WALK_DOWN_DIAG     3
#define ANIM_WAIT_UP_DIAG       4
#define ANIM_WALK_UP_DIAG       5
#define ANIM_WAIT_SIDE          6
#define ANIM_WALK_SIDE          7
#define ANIM_WAIT_UP            8
#define ANIM_WALK_UP            9

// game characters
#define ENT_ARLE          0
#define ENT_CAR           1

struct Entity
{
    char facing;
    fix32 posx;
    fix32 posy;

    fix32 spdx;
    fix32 spdy;

    int moving;
    fix32 maxspd;
    int hflip;
};

void initEnt(struct Entity* ent, char initFacing, fix32 initPosx, fix32 initPosy);

