#ifndef ShockFungus_H
#define ShockFungus_H

#include <ulib.h>
#define OBJ_H "object/0x0201-ShockFungus.h"
#include OBJ_H

struct ShockFungus;

typedef void (*ShockFungusActionFunc)(struct ShockFungus*, PlayState*);

#define STATE_STAND 0
#define STATE_WALK 1
#define STATE_ATTACK 2
#define STATE_DEAD 3
#define STATE_STUN 4
#define STATE_SPORE 5
#define STATE_ROLL 6

typedef struct ShockFungus {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ SkelAnime skelAnime;
        /*  */ Vec3s jointTable[CUBE_LIMB_MAX];
        /*  */ Vec3s morphTable[CUBE_LIMB_MAX];
        /*  */ ColliderCylinder Hitbox;
        /*  */ ColliderCylinder Hurtbox;
        /*  */ ColliderCylinder Sporehitbox;
        /*  */ ShockFungusActionFunc actionFunc;
        /*  */ s16 invincibilityTimer;
               s16 fireTimer;
        /*  */ s16 Timer;
               s16 SporeTimer;
               u8 state;
               u16 aggro;
               u8 eyeTexIndex;
               s16 blinkTimer;
               u8 deadtimer;
               u8 type;
               u8 musicOn;
               u8 rollcd;
} ShockFungus;

#endif