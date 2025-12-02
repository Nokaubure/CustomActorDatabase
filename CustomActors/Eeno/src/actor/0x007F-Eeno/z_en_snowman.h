#ifndef Z_EN_SNOWMAN_H
#define Z_EN_SNOWMAN_H

#include "global.h"
#include "ultra64.h"

#define EN_SNOWMAN_GET_TYPE(thisx) ((thisx)->params)
#define EN_SNOWMAN_GET_ATTACK_RANGE(thisx) (((thisx)->params >> 8) & 0xFF)

typedef enum {
    /* 0 */ EN_SNOWMAN_TYPE_SMALL,
    /* 1 */ EN_SNOWMAN_TYPE_LARGE,
    /* 2 */ EN_SNOWMAN_TYPE_SPLIT,
    /* 3 */ EN_SNOWMAN_TYPE_SMALL_SNOWBALL,
    /* 4 */ EN_SNOWMAN_TYPE_LARGE_SNOWBALL,
} EnSnowmanType;

struct EnSnowman;

typedef void (*EnSnowmanActionFunc)(struct EnSnowman*, PlayState*);

typedef struct EnSnowman {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ SkelAnime snowPileSkelAnime;
    /* 0x1CC */ Vec3s jointTable[13];
    /* 0x214 */ Vec3s morphTable[13];
    /* 0x25C */ Vec3s snowPileJointTable[4];
    /* 0x26E */ Vec3s snowPileMorphTable[4];
    /* 0x280 */ EnSnowmanActionFunc actionFunc;
    /* 0x284 */ EnSnowmanActionFunc prevActionFunc;
    /* 0x288 */ u8 isHoldingSnowball;
    /* 0x289 */ u8 combineState;
    /* 0x28A */ u8 turningOnSteepSlope;
    /* 0x28B */ u8 drawDmgEffType;
    /* 0x28C */ union {
                    s16 timer;
                    s16 snowballsToThrowBeforeIdling;
                } work;
    /* 0x28E */ s16 snowPileTargetRotY;
    /* 0x290 */ s16 combineTimer;
    /* 0x294 */ f32 eenoScale;
    /* 0x298 */ union {
                    f32 frameToStartHoldingSnowball;
                    f32 frameToThrowSnowball;
                    f32 targetScaleDuringCombine;
                } fwork;
    /* 0x29C */ f32 attackRange;
    /* 0x2A0 */ f32 drawDmgEffAlpha;
    /* 0x2A4 */ f32 drawDmgEffScale;
    /* 0x2A8 */ Vec3f combinePos;
    /* 0x2B4 */ Vec3f snowballPos;
    /* 0x2C0 */ Vec3f bodyPartsPos[9];
    /* 0x32C */ ColliderCylinder collider;
} EnSnowman; // size = 0x378

#define gEenoLargeSnowballCreateAnim 0x06000404
#define gEenoSmallSnowballCreateAnim 0x060007B4
#define gEenoLargeSnowballDL 0x060010B0
#define gEenoBodyBottomDL 0x06002428
#define gEenoBodyTopDL 0x06002640
#define gEenoRightUpperArmDL 0x060028F0
#define gEenoRightForearmDL 0x060029B0
#define gEenoRightHandDL 0x06002A50
#define gEenoLeftUpperArmDL 0x06002B20
#define gEenoLeftForearmDL 0x06002C70
#define gEenoLeftHandDL 0x06002D10
#define gEenoRightEyeDL 0x06002DE0
#define gEenoLeftEyeDL 0x06002E70
#define gEenoFaceTex 0x06002F00
#define gEenoUpperCheekTex 0x06003F00
#define gEenoBodyTex 0x06003F80
#define gEenoEyeTex 0x06004080
#define gEenoSmallSnowballDL 0x06004400
#define gEenoRootLimb 0x060044F0
#define gEenoBodyTopLimb 0x060044FC
#define gEenoLeftEyeLimb 0x06004508
#define gEenoRightEyeLimb 0x06004514
#define gEenoLeftUpperArmLimb 0x06004520
#define gEenoLeftForearmLimb 0x0600452C
#define gEenoLeftHandLimb 0x06004538
#define gEenoRightUpperArmLimb 0x06004544
#define gEenoRightForearmLimb 0x06004550
#define gEenoRightHandLimb 0x0600455C
#define gEenoBodyBottomLimb 0x06004568
#define gEenoSkel 0x060045A0
#define gEenoDamageAnim 0x06004628
#define gEenoSnowPileMoveAnim 0x060046D8
#define gEenoSnowPileBottomDL 0x06004960
#define gEenoSnowPileTopDL 0x060049E0
#define gEenoSnowPileBottomLimb 0x06004A70
#define gEenoSnowPileTopLimb 0x06004A7C
#define gEenoSnowPileSkel 0x06004A90
#define gEenoLargeSnowballThrowAnim 0x06004F14
#define gEenoSmallSnowballThrowAnim 0x0600544C
#define gEenoEmergeAnim 0x0600554C
#define gEenoIdleAnim 0x060058CC
#define gEenoSnowballFragment1DL 0x06005CB0
#define gEenoSnowballFragment2DL 0x06006190
#define gEenoSnowballFragment3DL 0x06006620
#define gEenoSnowballFragmentTex 0x06006730

#endif // Z_EN_SNOWMAN_H
