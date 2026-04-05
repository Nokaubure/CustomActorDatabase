#ifndef Z_OBJ_WARPSTONE_H
#define Z_OBJ_WARPSTONE_H

#include "global.h"

#define gEffFlash1DL 0x04010130
#define gOwlStatueClosedDL 0x060001D0
#define gOwlStatueHeadDL   0x06001110
#define gOwlStatueOpenedDL 0x06003770
struct ObjWarpstone;

typedef s32 (*ObjWarpstoneActionFunc)(struct ObjWarpstone* this, PlayState* play);

typedef enum {
    /* 0 */ SEK_MODEL_CLOSED,
    /* 1 */ SEK_MODEL_OPENED
} ObjectSekModels;

typedef enum {
    /* 25 */ OBJ_WARPSTONE_TIMER_OPEN_THRESHOLD = 25,
    /* 66 */ OBJ_WARPSTONE_TIMER_ACTIVATE_THRESHOLD = 66
} ObjWarpstoneTimerCutoffs;

typedef struct ObjWarpstone {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ ColliderCylinder collider; 
    /* 0x1A8 */ u8 isTalking;
    /* 0x1A9 */ u8 openingCSTimer;
    /* 0x1AA */ u8 modelIndex;
    /* 0x1AC */ ObjWarpstoneActionFunc actionFunc;
	/* 0x1AE */ u16 visibleDuration;
	u8 waitReset;
	u8 screenOpa;
	u8 talkTimer1;
} ObjWarpstone; // size = 0x1B0

#define OBJ_WARPSTONE_GET_ID(thisx) ((u16)((thisx)->params & 0xF))
//#define OBJ_WARPSTONE_IS_ACTIVATED(owlId) (((void)0, gSaveContext.save.saveInfo.playerData.owlActivationFlags) & (u16)gBitFlags[(owlId)])

#endif // Z_OBJ_WARPSTONE_H
