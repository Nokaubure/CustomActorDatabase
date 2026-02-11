#ifndef Z_EN_KAME_H
#define Z_EN_KAME_H

#include "global.h"

struct EnKame;

typedef void (*EnKameActionFunc)(struct EnKame*, PlayState*);

typedef struct EnKame {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime1;
    /* 0x188 */ Vec3s jointTable1[13];
    /* 0x1D6 */ Vec3s morphTable1[13];
    /* 0x224 */ SkelAnime skelAnime2;
    /* 0x268 */ Vec3s jointTable2[4];
    /* 0x280 */ Vec3s morphTable2[4];
    /* 0x298 */ EnKameActionFunc actionFunc;
    /* 0x29C */ u8 unk_29C;
    /* 0x29D */ u8 drawDmgEffType;
    /* 0x29E */ s16 unk_29E;
    /* 0x2A0 */ s16 unk_2A0;
    /* 0x2A2 */ s16 unk_2A2;
    /* 0x2A4 */ s16 unk_2A4;
    /* 0x2A6 */ s16 unk_2A6;
    /* 0x2A8 */ f32 unk_2A8;
    /* 0x2AC */ f32 unk_2AC;
    /* 0x2B0 */ f32 drawDmgEffAlpha;
    /* 0x2B4 */ f32 drawDmgEffScale;
    /* 0x2B8 */ f32 drawDmgEffFrozenSteamScale;
    /* 0x2BC */ Vec3f unk_2BC;
    /* 0x2C8 */ Vec3f limbPos[10];
    /* 0x340 */ ColliderCylinder collider;
    /*  */ u8 flipTimer;
    /*  */ u8 iceTimer;
} EnKame; // size = 0x38C

#define gSnapperRearUpAnim 0x06000440
#define gSnapperDamageAnim 0x060008B4
#define gSnapperDeathAnim 0x06000AF4
#define gSpikedSnapperIdleAnim 0x06000B30
#define gSpikedSnapperBodyDL 0x06001550
#define gSpikedSnapperShellDL 0x06001800
#define gSpikedSnapperSpikesDL 0x060018E0
#define gSpikedSnapperBodyLimb 0x06001A20
#define gSpikedSnapperShellLimb 0x06001A2C
#define gSpikedSnapperSpikesLimb 0x06001A38
#define gSpikedSnapperSkel 0x06001A50
#define gSnapperRetreatIntoShellAnim 0x06001C68
#define gSnapperFlipUprightAnim 0x06002510
#define gSnapperWiggleLegsAnim 0x060027D8
#define gSnapperFailToFlipUprightAnim 0x06002F88
#define gSnapperEmergeFromShellAnim 0x060031DC
#define gSnapperBouncedUprightAnim 0x060035EC
#define gSnapperEyeTexAnim 0x06003624
#define gSnapperFlipOverAnim 0x060039C0
#define gSnapperIdleAnim 0x06004210
#define gSnapperShellTex 0x06004220
#define gSnapperBodyTex 0x06004620
#define gSnapperLegTex 0x06004820
#define gSnapperHeadTex 0x06004C20
#define gSnapperMouthTex 0x06005020
#define gSnapperHoleTex 0x060050A0
#define gSnapperSpikeSkinTex 0x060052A0
#define gSnapperSpikeTex 0x060053A0
#define gSnapperEyeOpenTex 0x060055A0
#define gSnapperEyeHalfTex 0x060057A0
#define gSnapperEyeClosedTex 0x060059A0
#define gSnapperBodyDL 0x06006C50
#define gSnapperBackRightLegDL 0x06006DE8
#define gSnapperBackRightFootDL 0x06006F30
#define gSnapperBackLeftLegDL 0x06007040
#define gSnapperBackLeftFootDL 0x06007188
#define gSnapperFrontRightLegDL 0x06007298
#define gSnapperFrontRightFootDL 0x060073E0
#define gSnapperFrontLeftLegDL 0x060074F0
#define gSnapperFrontLeftFootDL 0x06007638
#define gSnapperHeadDL 0x06007748
#define gSnapperEyesDL 0x06007938
#define gSnapperJawDL 0x060079C8
#define gSnapperBodyLimb 0x06007BB0
#define gSnapperHeadLimb 0x06007BBC
#define gSnapperJawLimb 0x06007BC8
#define gSnapperEyesLimb 0x06007BD4
#define gSnapperFrontLeftLegLimb 0x06007BE0
#define gSnapperFrontLeftFootLimb 0x06007BEC
#define gSnapperFrontRightLegLimb 0x06007BF8
#define gSnapperFrontRightFootLimb 0x06007C04
#define gSnapperBackLeftLegLimb 0x06007C10
#define gSnapperBackLeftFootLimb 0x06007C1C
#define gSnapperBackRightLegLimb 0x06007C28
#define gSnapperBackRightFootLimb 0x06007C34
#define gSnapperSkel 0x06007C70
#define gSnapperWalkAnim 0x0600823C
#endif // Z_EN_KAME_H
