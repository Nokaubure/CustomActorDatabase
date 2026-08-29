#ifndef Z_EN_SLIME_H
#define Z_EN_SLIME_H

#include "global.h"
//#include "objects/object_slime/object_slime.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "mm/requirements.h"

#define EN_SLIME_LIMBPOS_COUNT 5

#define EN_SLIME_GET_TYPE(thisx) ((thisx)->params)
#define EN_SLIME_GET_ITEM(thisx) ((((thisx)->params)))
#define EN_SLIME_GET_REVIVE_TIME(thisx) ((((thisx)->params) >> 8) & 0xFF)
#define EN_SLIME_GET_KEYFLAG(thisx) ((((thisx)->params) >> 4) & 0xF)



struct EnSlime;

typedef void (*EnSlimeActionFunc)(struct EnSlime*, PlayState*);


typedef struct EnSlime {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnSlimeActionFunc actionFunc;
    /* 0x148 */ u8 iceBlockTimer;
    /* 0x149 */ u8 eyeTexIndex;
    /* 0x14A */ u8 drawDmgEffType;
    /* 0x14C */ s16 timer;
    /* 0x14E */ s16 idleRotY;
    /* 0x150 */ s16 reviveRotY;
    /* 0x152 */ s16 reviveTime;
    /* 0x154 */ union { 
                    Vec3f iceBlockSnapPos;
                    Vec3f wobbleRot;
                };
    /* 0x160 */ TexturePtr itemDropTex;
    /* 0x164 */ f32 drawDmgEffAlpha;
    /* 0x168 */ f32 drawDmgEffScale;
    /* 0x16C */ f32 drawDmgEffFrozenSteamScale;
    /* 0x170 */ f32 distLimit;
    /* 0x174 */ Vec3f reviveScale;
    /* 0x180 */ Vec3f limbPos[EN_SLIME_LIMBPOS_COUNT];
    /* 0x1BC */ ColliderCylinder collider;
    /* */       s16 keyFlag;
} EnSlime; // size = 0x208

typedef enum EnSlimeType {
    /* 0 */ EN_SLIME_TYPE_BLUE,
    /* 1 */ EN_SLIME_TYPE_GREEN,
    /* 2 */ EN_SLIME_TYPE_YELLOW,
    /* 3 */ EN_SLIME_TYPE_RED,
    /* 4 */ EN_SLIME_TYPE_FIRE,
    /* 5 */ EN_SLIME_TYPE_ICE,
    /* 6 */ EN_SLIME_TYPE_ELECTRIC,
    /* 7 */ EN_SLIME_TYPE_BLACK,
    /* 8 */ EN_SLIME_TYPE_ICE_2,
    /* 9 */ EN_SLIME_TYPE_KEY
} EnSlimeType;

typedef enum EnSlimeItem {
    /* 0 */ EN_SLIME_ITEM_NONE,
    /* 1 */ EN_SLIME_ITEM_MAGIC_SMALL,
    /* 2 */ EN_SLIME_ITEM_ARROW,
    /* 3 */ EN_SLIME_ITEM_HEART,
    /* 4 */ EN_SLIME_ITEM_DEKUNUT,
    /* 5 */ EN_SLIME_ITEM_ARROW_3,
    /* 6 */ EN_SLIME_ITEM_MAGIC_LARGE,
    /* 7 */ EN_SLIME_ITEM_BOMB,
    /* 8 */ EN_SLIME_ITEM_DEKUSTICK,
    /* 9 */ EN_SLIME_ITEM_KEY
} EnSlimeItem;

#define gChuchuBodyDL 0x060004C0
#define gChuchuEyesDL 0x06000650
#define gChuchuSlimeFlowTexAnim 0x06000828
#define gChuchuPuddleDL 0x06000A10
#define gChuchuEmptyDL 0x06000B68
#define gChuchuMouthTex 0x06000B70
#define gChuchuEyeSocketTex 0x06000D70
#define gChuchuEyeOpenTex 0x06000F70
#define gChuchuEyeHalfTex 0x06001070
#define gChuchuEyeClosedTex 0x06001170
#define gChuchuSlimeFlowTex 0x06001270
#define gChuchuSlimeBaseTex 0x06001470
#endif // Z_EN_SLIME_H
