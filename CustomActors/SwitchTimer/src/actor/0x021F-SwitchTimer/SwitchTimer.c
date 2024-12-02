/**
 * by Nokaubure

Variable

& 0x003F : switch flag

Xrot = time in seconds / 10 (1 unit = 0.1 seconds), defaults at 1 second
Zrot = time before starting in seconds / 10 (1 unit = 0.1 seconds), defaults at 0 seconds

 **/


#define AVAL(base,type,offset)  (*(type*)((u8*)(base)+(offset)))

#include "z64math.h"

#include "ultra64.h"
#include "global.h"

#include "macros.h"

#define OBJ_ID 0x0001      
#define ACT_ID 0x001A

typedef struct {
    Actor actor;
    u16 switchflag;
    u8 active;
    u16 MaxTimer;
    u16 MaxStartTimer;
    u16 timer;
    u16 starttimer;

} SwitchTimer;


static void init(SwitchTimer *en, PlayState *global) {


    en->switchflag = (en->actor.params & 0x003F);
    en->MaxTimer = en->actor.world.rot.x * 2;
    en->MaxStartTimer = en->actor.world.rot.z * 2;
    if (en->timer == 0) en->timer = 10;
    
    if (Flags_GetSwitch(global, en->switchflag) != 0)
    {
        en->active = 1;
        en->starttimer = en->MaxStartTimer;
    }

}



static void update(SwitchTimer *en, PlayState *global) {

    if (en->active != 0 && !Flags_GetSwitch(global, en->switchflag))
    {
        en->active = 0;
    }
    else if (en->active == 0 && Flags_GetSwitch(global, en->switchflag))
    {
        if (en->MaxStartTimer > 0)
        {
            en->active = 1;
            en->starttimer = en->MaxStartTimer;
        }
        else
        {
            en->active = 2;
            en->timer = en->MaxTimer;
        }

    }

    if (en->active == 1)
    {
        en->starttimer--;

        if (en->starttimer == 0)
        {
            en->active = 2;
            en->timer = en->MaxTimer;
        }
    }
    else if (en->active == 2)
    {
        en->timer--;
        func_8002F994(&en->actor, en->timer);

        if (en->timer == 0)
        {
            Flags_UnsetSwitch(global, en->switchflag);
            en->active = 0;
        }
    }


}

static void dest(SwitchTimer *en, PlayState *global) {

    if (en->active > 0)
    {
        Flags_UnsetSwitch(global, en->switchflag);
    }

}



const ActorInit init_vars = {
    ACT_ID, 
    ACTORCAT_BG, 
    0x00000010,
    OBJ_ID,
    sizeof(SwitchTimer),
    (ActorFunc)init,
    (ActorFunc)dest,
    (ActorFunc)update,
    NULL
};







