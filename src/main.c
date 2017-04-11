#include <genesis.h>
#include "main.h"
#include "global.h"

#include "walking.h"
#include "title.h"

// global program mode
u8 progMode = MODE_INIT;

u16 joy1Input;
u16 joy2Input;

int main()
{
    // main loop
    while(1) {
        VDP_waitVSync();
        readInput();
        switch(progMode) {
            case MODE_INIT :
                progMode = MODE_TITLE;
            break;
            case MODE_TITLE :
                // title
                doTitle();
            break;
            case MODE_STAGE :
                //walking
                doWalking();
            break;
        }

    }

    return(0);
}

static void readInput()
{
    joy1Input = JOY_readJoypad(JOY_1);
    joy2Input = JOY_readJoypad(JOY_2);
}



