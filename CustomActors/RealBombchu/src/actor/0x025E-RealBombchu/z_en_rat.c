/*
 * File: z_en_rat.c
 * Overlay: ovl_En_Rat
 * Description: Real Bombchu
 */

#include "z_en_rat.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "overlays/actors/ovl_En_Bom/z_en_bom.h"
#include "mm/requirements.h"
#include <uLib.h>

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_2 | ACTOR_FLAG_9)


#define COLORFILTER_COLORFLAG_NONE 0xC000
#define COLORFILTER_COLORFLAG_GRAY 0x8000
#define COLORFILTER_COLORFLAG_RED  0x4000
#define COLORFILTER_COLORFLAG_BLUE 0x0000

#define COLORFILTER_INTENSITY_FLAG 0x8000

#define COLORFILTER_BUFFLAG_XLU    0x2000
#define COLORFILTER_BUFFLAG_OPA    0x0000


#define THIS ((EnRat*)thisx)

#define ACT_ID 0x025E
#define OBJ_ID 0x021B

void EnRat_Init(Actor* thisx, PlayState* play);
void EnRat_Destroy(Actor* thisx, PlayState* play);
void EnRat_Update(Actor* thisx, PlayState* play);
void EnRat_Draw(Actor* thisx, PlayState* play);

void EnRat_InitializeAxes(EnRat* this);
void EnRat_UpdateRotation(EnRat* this);
void EnRat_Revive(EnRat* this, PlayState* play);
void EnRat_SetupIdle(EnRat* this);
void EnRat_Idle(EnRat* this, PlayState* play);
void EnRat_SetupSpottedPlayer(EnRat* this);
void EnRat_SpottedPlayer(EnRat* this, PlayState* play);
void EnRat_SetupChasePlayer(EnRat* this);
void EnRat_ChasePlayer(EnRat* this, PlayState* play);
void EnRat_Bounced(EnRat* this, PlayState* play);
void EnRat_Explode(EnRat* this, PlayState* play);
void EnRat_PostDetonation(EnRat* this, PlayState* play);

typedef enum {
    /* -2 */ EN_RAT_HOOK_STARTED = -2,
    /* -1 */ EN_RAT_HOOKED
} EnRatHookedState;

ActorInit En_Rat_InitVars = {
    /**/ ACT_ID,
    /**/ ACTORCAT_ENEMY,
    /**/ FLAGS,
    /**/ OBJ_ID,
    /**/ sizeof(EnRat),
    /**/ EnRat_Init,
    /**/ EnRat_Destroy,
    /**/ EnRat_Update,
    /**/ EnRat_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x00, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_HARD,
            BUMP_ON | BUMP_HOOKABLE,
            OCELEM_ON,
        },
        { 1, { { 0, 0, 0 }, 23 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementsInit,
};

typedef enum {
    /* 0x0 */ EN_RAT_DMGEFF_NONE,
    /* 0x1 */ EN_RAT_DMGEFF_STUN,
    /* 0xF */ EN_RAT_DMGEFF_HOOKSHOT = 0xF // Pulls the Real Bombchu towards the player
} EnRatDamageEffect;

static DamageTable sDamageTable = {
    /* Deku Nut       */ DMG_ENTRY(0, EN_RAT_DMGEFF_STUN),
    /* Deku Stick     */ DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    /* Slingshot      */ DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    /* Explosives     */ DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    /* Boomerang      */ DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    /* Normal arrow   */ DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    /* Hammer swing   */ DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    /* Hookshot       */ DMG_ENTRY(0, EN_RAT_DMGEFF_HOOKSHOT),
    /* Kokiri sword   */ DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    /* Master sword   */ DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    /* Giant's Knife  */ DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    /* Fire arrow     */ DMG_ENTRY(2, EN_RAT_DMGEFF_NONE),
    /* Ice arrow      */ DMG_ENTRY(2, EN_RAT_DMGEFF_NONE),
    /* Light arrow    */ DMG_ENTRY(2, EN_RAT_DMGEFF_NONE),
    /* Unk arrow 1    */ DMG_ENTRY(2, EN_RAT_DMGEFF_NONE),
    /* Unk arrow 2    */ DMG_ENTRY(2, EN_RAT_DMGEFF_NONE),
    /* Unk arrow 3    */ DMG_ENTRY(2, EN_RAT_DMGEFF_NONE),
    /* Fire magic     */ DMG_ENTRY(2, EN_RAT_DMGEFF_NONE),
    /* Ice magic      */ DMG_ENTRY(0, EN_RAT_DMGEFF_STUN),
    /* Light magic    */ DMG_ENTRY(2, EN_RAT_DMGEFF_NONE),
    /* Normal shield  */ DMG_ENTRY(0, EN_RAT_DMGEFF_NONE),
    /* Light ray      */ DMG_ENTRY(0, EN_RAT_DMGEFF_NONE),
    /* Kokiri spin    */ DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    /* Giant spin     */ DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    /* Master spin    */ DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    /* Kokiri jump    */ DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    /* Giant jump     */ DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    /* Master jump    */ DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    /* Unknown 1      */ DMG_ENTRY(0, EN_RAT_DMGEFF_NONE),
    /* Unblockable    */ DMG_ENTRY(0, EN_RAT_DMGEFF_NONE),
    /* Hammer jump    */ DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    /* Powder Keg     */ DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
};

static CollisionCheckInfoInit sColChkInfoInit = { 1, 30, 30, 50 };

static TexturePtr sSparkTextures[] = {
    gElectricSpark1Tex,
    gElectricSpark2Tex,
    gElectricSpark3Tex,
    gElectricSpark4Tex,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0xB6, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 15, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 5000, ICHAIN_STOP),
};

static EffectBlureInit2 sBlureInit = {
    0, 0, 0, { 250, 0, 0, 250 }, { 200, 0, 0, 130 }, { 150, 0, 0, 100 }, { 100, 0, 0, 50 }, 16,
    0, 0, 0, { 0, 0, 0, 0 },     { 0, 0, 0, 0 },
};

static s32 sTexturesDesegmented = false;

void EnRat_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    EnRat* this = THIS;
    s32 attackRange;
    s32 i;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    Collider_InitJntSph(play, &this->collider);
    Collider_SetJntSph(play, &this->collider, &this->actor, &sJntSphInit, &this->colliderItems);
    this->collider.elements[0].dim.worldSphere.radius = sJntSphElementsInit->dim.modelSphere.radius;

    attackRange = EN_RAT_GET_ATTACK_RANGE(&this->actor);
    if (EN_RAT_IS_OVERWORLD_TYPE(&this->actor)) {
        this->actor.params = EN_RAT_TYPE_OVERWORLD;
    } else {
        this->actor.params = EN_RAT_TYPE_DUNGEON;
    }

    SkelAnime_InitFlex(play, &this->skelAnime, 0x06001858, 0x0600026c, this->jointTable,
                       this->morphTable, REAL_BOMBCHU_LIMB_MAX);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 25.0f);

    if (EN_RAT_GET_TYPE(&this->actor) == EN_RAT_TYPE_DUNGEON) {
        Effect_Add(play, &this->blure1Index, EFFECT_BLURE2, 0, 0, &sBlureInit);
        Effect_Add(play, &this->blure2Index, EFFECT_BLURE2, 0, 0, &sBlureInit);
        this->timer = 30;
    } else {
        this->timer = 150;
    }

    CollisionCheck_SetInfo(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    EnRat_InitializeAxes(this);
    EnRat_UpdateRotation(this);

    if ((attackRange == 255) || (attackRange == 0)) {
        this->attackRange = 350.0f;
    } else if (EN_RAT_GET_TYPE(&this->actor) == EN_RAT_TYPE_DUNGEON) {
        this->attackRange = attackRange * 0.1f * 40.0f;
    } else {
        this->attackRange = attackRange * 0.5f * 40.0f;
    }

    if (!sTexturesDesegmented) {
        for (i = 0; i < ARRAY_COUNT(sSparkTextures); i++) {
            sSparkTextures[i] = Lib_SegmentedToVirtual(sSparkTextures[i]);
        }

        sTexturesDesegmented = true;
    }

    EnRat_SetupIdle(this);
}

void EnRat_Destroy(Actor* thisx, PlayState* play) {
    EnRat* this = THIS;

    if (EN_RAT_GET_TYPE(&this->actor) == EN_RAT_TYPE_DUNGEON) {
        Effect_Delete(play, this->blure1Index);
        Effect_Delete(play, this->blure2Index);
    }

    Collider_DestroyJntSph(play, &this->collider);
}

void EnRat_InitializeAxes(EnRat* this) {
    Matrix_RotateY(BINANG_TO_RAD(this->actor.shape.rot.y), MTXMODE_NEW);
    Matrix_RotateX(BINANG_TO_RAD(this->actor.shape.rot.x), MTXMODE_APPLY);
    Matrix_RotateZ(BINANG_TO_RAD(this->actor.shape.rot.z), MTXMODE_APPLY);

    Matrix_MultVecZ(1.0f, &this->axisForwards);
    Matrix_MultVecY(1.0f, &this->axisUp);
    Matrix_MultVecX(1.0f, &this->axisLeft);
}

/**
 * Returns true if floorPoly is valid for the Real Bombchu to move on, false otherwise.
 */
s32 EnRat_UpdateFloorPoly(EnRat* this, CollisionPoly* floorPoly, PlayState* play) {
    Vec3f normal;
    Vec3f vec;
    f32 angle;
    f32 magnitude;
    f32 normDotUp;

    this->actor.floorPoly = floorPoly;

    if (floorPoly != NULL) {
        normal.x = COLPOLY_GET_NORMAL(floorPoly->normal.x);
        normal.y = COLPOLY_GET_NORMAL(floorPoly->normal.y);
        normal.z = COLPOLY_GET_NORMAL(floorPoly->normal.z);
    } else {
        normal.x = 0.0f;
        normal.z = 0.0f;
        normal.y = 1.0f;
    }

    normDotUp = DOTXYZ(normal, this->axisUp);
    if (fabsf(normDotUp) >= 0.999f) {
        return false;
    }

    angle = Math_FAcosF(normDotUp);
    if (angle < 0.001f) {
        return false;
    }

    Math3D_Vec3f_Cross(&this->axisUp, &normal, &vec);

    magnitude = Math3D_Vec3fMagnitude(&vec);
    if (magnitude < 0.001f) {
        EnRat_Explode(this, play);
        return false;
    }

    Math_Vec3f_Scale(&vec, 1.0f / magnitude);
    Matrix_RotateAxisF(angle, &vec, MTXMODE_NEW);
    Matrix_MultVec3f(&this->axisLeft, &vec);
    Math_Vec3f_Copy(&this->axisLeft, &vec);
    Math3D_Vec3f_Cross(&this->axisLeft, &normal, &this->axisForwards);

    magnitude = Math3D_Vec3fMagnitude(&this->axisForwards);
    if (magnitude < 0.001f) {
        EnRat_Explode(this, play);
        return false;
    }

    Math_Vec3f_Scale(&this->axisForwards, 1.0f / magnitude);
    Math_Vec3f_Copy(&this->axisUp, &normal);
    return true;
}

void EnRat_UpdateRotation(EnRat* this) {
    MtxF mf;

    mf.xx = this->axisLeft.x;
    mf.yx = this->axisLeft.y;
    mf.zx = this->axisLeft.z;

    mf.xy = this->axisUp.x;
    mf.yy = this->axisUp.y;
    mf.zy = this->axisUp.z;

    mf.xz = this->axisForwards.x;
    mf.yz = this->axisForwards.y;
    mf.zz = this->axisForwards.z;

    Matrix_MtxFToYXZRot(&mf, &this->actor.world.rot, false);
    this->actor.world.rot.x = -this->actor.world.rot.x;
}

/**
 * Chooses to either look at the player (if the Real Bombchu has spotted the player or
 * is chasing after them) or at some other semi-random point (if the Bombchu is idle).
 */
void EnRat_ChooseDirection(EnRat* this) {
    Vec3f newAxisForwards;
    s16 angle;

    if (this->actionFunc != EnRat_Idle) {
        angle = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
        if (this->axisUp.y < -0.25f) {
            angle -= 0x8000;
        }
    } else {
        if (Actor_WorldDistXYZToPoint(&this->actor, &this->actor.home.pos) > 50.0f) {
            Vec3f homeInHome;
            Vec3f worldInHome;
            Vec3f worldPlusForwardInHome;
            Vec3f upInHome;

            // Set up matrix to unrotate, to make the "home rot" the new basis triad
            Matrix_RotateZ(BINANG_TO_RAD(-this->actor.home.rot.z), MTXMODE_NEW);
            Matrix_RotateX(BINANG_TO_RAD(-this->actor.home.rot.x), MTXMODE_APPLY);
            Matrix_RotateY(BINANG_TO_RAD(-this->actor.home.rot.y), MTXMODE_APPLY);

            // Unrotate axisUp into "home rot" triad and store in upfInHome
            Matrix_MultVec3f(&this->axisUp, &upInHome);

            // Move world.pos forward by axisForwards and store in homeInHome
            Math_Vec3f_Sum(&this->actor.world.pos, &this->axisForwards, &homeInHome);

            // Unrotate homeInHome into "home rot" triad and store in worldPlusForwardInHome
            Matrix_MultVec3f(&homeInHome, &worldPlusForwardInHome);

            // Unrotate home into "home rot" triad
            Matrix_MultVec3f(&this->actor.home.pos, &homeInHome);

            // Unrotate world into "home rot" triad
            Matrix_MultVec3f(&this->actor.world.pos, &worldInHome);

            angle = Math_Vec3f_Yaw(&worldInHome, &homeInHome) - Math_Vec3f_Yaw(&worldInHome, &worldPlusForwardInHome);
            if (upInHome.y < -0.25f) {
                angle -= 0x8000;
            }

            angle += (s16)(s32)Rand_CenteredFloat(0x800);
        } else {
            angle = (Rand_ZeroOne() < 0.1f) ? (s16)(s32)Rand_CenteredFloat(0x800) : 0;
        }
    }

    angle = CLAMP(angle, -0x800, 0x800);
    Matrix_RotateAxisF(BINANG_TO_RAD(angle), &this->axisUp, MTXMODE_NEW);
    Matrix_MultVec3f(&this->axisForwards, &newAxisForwards);
    Math_Vec3f_Copy(&this->axisForwards, &newAxisForwards);
    Math3D_Vec3f_Cross(&this->axisUp, &this->axisForwards, &this->axisLeft);
    this->shouldRotateOntoSurfaces = true;
}

/**
 * Returns true if the Real Bombchu is on a collision poly or is on the water's surface.
 */
s32 EnRat_IsOnCollisionPoly(PlayState* play, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** poly,
                            s32* bgId) {
    WaterBox* waterBox;
    s32 isOnWater;
    f32 waterSurface;

    if (WaterBox_GetSurface1(play, &play->colCtx, posB->x, posB->z, &waterSurface, &waterBox) &&
        (waterSurface <= posA->y) && (posB->y <= waterSurface)) {
        isOnWater = true;
    } else {
        isOnWater = false;
    }

    if (BgCheck_EntityLineTest1(&play->colCtx, posA, posB, posResult, poly, 1, 1, 1, 1, bgId)) {
        if (!(SurfaceType_GetWallFlags(&play->colCtx, *poly, *bgId) & (WALL_FLAG_4 | WALL_FLAG_5)) &&
            (!isOnWater || (waterSurface <= posResult->y))) {
            return true;
        }
    }

    if (isOnWater) {
        posResult->x = posB->x;
        posResult->y = waterSurface;
        posResult->z = posB->z;
        *poly = NULL;
        *bgId = BGCHECK_SCENE;
        return true;
    }

    return false;
}

s32 EnRat_IsTouchingSurface(EnRat* this, PlayState* play) {
    CollisionPoly* polySide = NULL;
    CollisionPoly* polyUpDown = NULL;
    s32 bgIdSide;
    s32 bgIdUpDown;
    s32 i;
    f32 lineLength;
    Vec3f posA;
    Vec3f posB;
    Vec3f posSide;
    Vec3f posUpDown;

    bgIdUpDown = bgIdSide = BGCHECK_SCENE;

    lineLength = 2.0f * this->actor.speedXZ;

    posA.x = this->actor.world.pos.x + (this->axisUp.x * 5.0f);
    posA.y = this->actor.world.pos.y + (this->axisUp.y * 5.0f);
    posA.z = this->actor.world.pos.z + (this->axisUp.z * 5.0f);

    posB.x = this->actor.world.pos.x - (this->axisUp.x * 4.0f);
    posB.y = this->actor.world.pos.y - (this->axisUp.y * 4.0f);
    posB.z = this->actor.world.pos.z - (this->axisUp.z * 4.0f);

    if (EnRat_IsOnCollisionPoly(play, &posA, &posB, &posUpDown, &polyUpDown, &bgIdUpDown)) {
        posB.x = (this->axisForwards.x * lineLength) + posA.x;
        posB.y = (this->axisForwards.y * lineLength) + posA.y;
        posB.z = (this->axisForwards.z * lineLength) + posA.z;

        if (EnRat_IsOnCollisionPoly(play, &posA, &posB, &posSide, &polySide, &bgIdSide)) {
            if ((polySide != NULL) && this->hasLostTrackOfPlayer) {
                return false;
            }

            this->shouldRotateOntoSurfaces |= EnRat_UpdateFloorPoly(this, polySide, play);
            Math_Vec3f_Copy(&this->actor.world.pos, &posSide);
            this->actor.floorBgId = bgIdSide;
            this->actor.speedXZ = 0.0f;
        } else {
            if (polyUpDown != this->actor.floorPoly) {
                this->shouldRotateOntoSurfaces |= EnRat_UpdateFloorPoly(this, polyUpDown, play);
            }

            Math_Vec3f_Copy(&this->actor.world.pos, &posUpDown);
            this->actor.floorBgId = bgIdUpDown;
        }
    } else {
        this->actor.speedXZ = 0.0f;
        lineLength *= 3.0f;
        Math_Vec3f_Copy(&posA, &posB);

        for (i = 0; i < 3; i++) {
            if (i == 0) {
                // backwards
                posB.x = posA.x - (this->axisForwards.x * lineLength);
                posB.y = posA.y - (this->axisForwards.y * lineLength);
                posB.z = posA.z - (this->axisForwards.z * lineLength);
            } else if (i == 1) {
                // left
                posB.x = posA.x + (this->axisLeft.x * lineLength);
                posB.y = posA.y + (this->axisLeft.y * lineLength);
                posB.z = posA.z + (this->axisLeft.z * lineLength);
            } else {
                // right
                posB.x = posA.x - (this->axisLeft.x * lineLength);
                posB.y = posA.y - (this->axisLeft.y * lineLength);
                posB.z = posA.z - (this->axisLeft.z * lineLength);
            }

            if (EnRat_IsOnCollisionPoly(play, &posA, &posB, &posSide, &polySide, &bgIdSide)) {
                this->shouldRotateOntoSurfaces |= EnRat_UpdateFloorPoly(this, polySide, play);
                Math_Vec3f_Copy(&this->actor.world.pos, &posSide);
                this->actor.floorBgId = bgIdSide;
                break;
            }
        }

        if (i == 3) {
            // no collision nearby
            return false;
        }
    }

    return true;
}

/**
 * Transform coordinates from actor coordinate space (origin at actor's world.pos, z-axis is facing
 * angle, i.e. shape.rot.y) to world space, according to current orientation.
 * `offset` is expected to already be at world scale.
 */
void EnRat_ActorCoordsToWorld(EnRat* this, Vec3f* offset, Vec3f* pos) {
    f32 x = offset->x + this->visualJitter;

    pos->x = this->actor.world.pos.x + (this->axisLeft.x * x) + (this->axisUp.x * offset->y) +
             (this->axisForwards.x * offset->z);
    pos->y = this->actor.world.pos.y + (this->axisLeft.y * x) + (this->axisUp.y * offset->y) +
             (this->axisForwards.y * offset->z);
    pos->z = this->actor.world.pos.z + (this->axisLeft.z * x) + (this->axisUp.z * offset->y) +
             (this->axisForwards.z * offset->z);
}

/**
 * This function will spawn splashes and ripples as the Real Bombchu runs across the water's surface.
 */
void EnRat_SpawnWaterEffects(EnRat* this, PlayState* play) {
    s32 pad;
    Vec3f splashPos;

    EffectSsGRipple_Spawn(play, &this->actor.world.pos, 70, 500, 0);
    splashPos.x = this->actor.world.pos.x - (this->axisForwards.x * 10.0f);
    splashPos.z = this->actor.world.pos.z - (this->axisForwards.z * 10.0f);
    splashPos.y = this->actor.world.pos.y + 5.0f;
    EffectSsGSplash_Spawn(play, &splashPos, NULL, NULL, 1, 450);
}

void EnRat_HandleNonSceneCollision(EnRat* this, PlayState* play) {
    s16 yaw = this->actor.shape.rot.y;
    f32 sin;
    f32 cos;
    f32 tempX;

    func_800433A4(&play->colCtx, this->actor.floorBgId, &this->actor);

    if (yaw != this->actor.shape.rot.y) {
        yaw = this->actor.shape.rot.y - yaw;

        sin = Math_SinS(yaw);
        cos = Math_CosS(yaw);

        tempX = this->axisForwards.x;
        this->axisForwards.x = (sin * this->axisForwards.z) + (cos * tempX);
        this->axisForwards.z = (cos * this->axisForwards.z) - (sin * tempX);

        tempX = this->axisUp.x;
        this->axisUp.x = (sin * this->axisUp.z) + (cos * tempX);
        this->axisUp.z = (cos * this->axisUp.z) - (sin * tempX);

        tempX = this->axisLeft.x;
        this->axisLeft.x = (sin * this->axisLeft.z) + (cos * tempX);
        this->axisLeft.z = (cos * this->axisLeft.z) - (sin * tempX);
    }
}

static Vec3f sSmokeAccel = { 0.0f, 0.6f, 0.0f };
static Color_RGBA8 sSmokeColor = { 255, 255, 255, 255 };

void EnRat_SpawnSmoke(EnRat* this, PlayState* play) {
Vec3f gZeroVec3f = {0};
    func_8002836C(play, &this->smokePos, &gZeroVec3f, &sSmokeAccel, &sSmokeColor, &sSmokeColor, 75, 7, 8);
}

void EnRat_SetupRevive(EnRat* this) {
    this->hasLostTrackOfPlayer = false;
    this->timer = 200;
    this->damageReaction.stunTimer = 0;
    Math_Vec3f_Copy(&this->actor.world.pos, &this->actor.home.pos);
    this->actor.shape.yOffset = 0.0f;
    this->actor.shape.rot.x = this->actor.home.rot.x;
    this->actor.shape.rot.y = this->actor.home.rot.y;
    this->actor.shape.rot.z = this->actor.home.rot.z;
    EnRat_InitializeAxes(this);
    EnRat_UpdateRotation(this);
    this->actor.flags &= ~ACTOR_FLAG_0;
    this->actor.speedXZ = 0.0f;
    Animation_PlayLoopSetSpeed(&this->skelAnime, 0x06000174, 0.0f);
    this->revivePosY = 2666.6667f;
    this->actor.draw = NULL;
    this->actor.shape.shadowDraw = NULL;
    this->actionFunc = EnRat_Revive;
}

/**
 * Makes the Real Bombchu respawn after a certain amount of time by tunneling up from underground.
 * Used only by Overworld-type Bombchu.
 */
void EnRat_Revive(EnRat* this, PlayState* play) {
    Vec3f gZeroVec3f = {0};
    if (this->timer > 0) {
        this->timer--;
        if (this->timer == 0) {
            this->actor.flags |= ACTOR_FLAG_0;
            this->actor.draw = EnRat_Draw;
            this->skelAnime.playSpeed = 1.0f;
        }
    } else {
        Math_StepToF(&this->revivePosY, 0.0f, 666.6667f);
        if (Animation_OnFrame(&this->skelAnime, 2.0f)) {
            func_800286CC(play, &this->actor.world.pos, &this->axisUp, &gZeroVec3f, 600, 100);
            this->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        }

        if (Animation_OnFrame(&this->skelAnime, 0.0f)) {
            this->actor.flags &= ~ACTOR_FLAG_4;
            this->timer = 150;
            EnRat_SetupIdle(this);
        }
    }
}

void EnRat_SetupIdle(EnRat* this) {
    Animation_PlayLoop(&this->skelAnime, 0x0600026c);
    this->animLoopCounter = 5;
    this->actor.speedXZ = 2.0f;
    this->actionFunc = EnRat_Idle;
}

/**
 * Move around randomly near the Bombchu's home until it spots the player.
 */
void EnRat_Idle(EnRat* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    this->actor.speedXZ = 2.0f;
    if (Animation_OnFrame(&this->skelAnime, 0.0f)) {
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_RIZA_ONGND);
        if (this->animLoopCounter != 0) {
            this->animLoopCounter--;
        }
    }

    if ((this->animLoopCounter == 0) && (Rand_ZeroOne() < 0.05f)) {
        //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_BOMCHU_VOICE);
        SoundEffect_PlayOneshot(SOUND_ENEMY_REAL_BOMBCHU_TALK, 0.5f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
        this->animLoopCounter = 5;
    }

    if (!(player->stateFlags3) && (this->actor.xzDistToPlayer < this->attackRange) &&
        Actor_IsFacingPlayer(&this->actor, 0x3800)) {
        EnRat_SetupSpottedPlayer(this);
    }
}

void EnRat_SetupSpottedPlayer(EnRat* this) {
    this->actor.flags |= ACTOR_FLAG_4;
    Animation_MorphToLoop(&this->skelAnime, 0x06000174, -5.0f);
    this->animLoopCounter = 3;
    this->actor.speedXZ = 0.0f;
    this->actionFunc = EnRat_SpottedPlayer;
}

/**
 * Play the "spotted" animation a few times, then start chasing the player.
 */
void EnRat_SpottedPlayer(EnRat* this, PlayState* play) {
    if ((this->animLoopCounter == 3) && Animation_OnFrame(&this->skelAnime, 5.0f)) {
        //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_BOMCHU_AIM);
        SoundEffect_PlayOneshot(SOUND_ENEMY_REAL_BOMBCHU_ALERT, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
    }

    if (Animation_OnFrame(&this->skelAnime, 0.0f)) {
        this->animLoopCounter--;
        if (this->animLoopCounter == 0) {
            EnRat_SetupChasePlayer(this);
        }
    }
}

void EnRat_UpdateSparkOffsets(EnRat* this) {
    s32 i;
    Vec3f* ptr;

    for (i = 0; i < ARRAY_COUNT(this->sparkOffsets); i++) {
        ptr = &this->sparkOffsets[i];
        ptr->x = Rand_CenteredFloat(6.0f);
        ptr->y = Rand_CenteredFloat(6.0f);
        ptr->z = Rand_CenteredFloat(6.0f);
    }
}

void EnRat_SetupChasePlayer(EnRat* this) {
    Animation_MorphToLoop(&this->skelAnime, 0x0600026c, -5.0f);
    this->actor.speedXZ = 6.1f;
    EnRat_UpdateSparkOffsets(this);
    this->actionFunc = EnRat_ChasePlayer;
}

static Vec3f sBlureP1Offset = { 0.0f, 10.5f, -9.0f };
static Vec3f sBlureP2LeftOffset = { 18.0f, 0.0f, -7.5f };
static Vec3f sBlureP2RightOffset = { -18.0f, 0.0f, -7.5f };
static Vec3f sDustVelocity = { 0.0f, 3.0f, 0.0f };

/**
 * Run towards the player. If the player puts on the Stone Mask or burrows into a Deku
 * Flower, the Real Bombchu will lose track of the player and run in a straight line.
 */
void EnRat_ChasePlayer(EnRat* this, PlayState* play) {
    Vec3f gZeroVec3f = {0};
    Player* player = GET_PLAYER(play);
    Vec3f blureP1;
    Vec3f blureP2;

    this->actor.speedXZ = 6.1f;
    if (this->hasLostTrackOfPlayer) {
        if (!(player->stateFlags3) &&
            Actor_IsFacingPlayer(&this->actor, 0x3000)) {
            this->hasLostTrackOfPlayer = false;
        }
    } else if ((player->stateFlags3)) {
        this->hasLostTrackOfPlayer = true;
    }

    if (Animation_OnFrame(&this->skelAnime, 0.0f)) {
        if (this->animLoopCounter != 0) {
            this->animLoopCounter--;
        }

        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_RIZA_ONGND);
    }

    DECR(this->timer);

    if ((this->timer == 0) && (EN_RAT_GET_TYPE(&this->actor) == EN_RAT_TYPE_DUNGEON)) {
        this->timer = 30;
    }

    EnRat_SpawnSmoke(this, play);
    this->visualJitter =
        (5.0f + (Rand_ZeroOne() * 3.0f)) * Math_SinS(((Rand_ZeroOne() * 0x200) + 0x3000) * this->timer);

    if (EN_RAT_GET_TYPE(&this->actor) == EN_RAT_TYPE_DUNGEON) {
        EnRat_ActorCoordsToWorld(this, &sBlureP1Offset, &blureP1);

        EnRat_ActorCoordsToWorld(this, &sBlureP2LeftOffset, &blureP2);
        EffectBlure_AddVertex(Effect_GetByIndex(this->blure1Index), &blureP1, &blureP2);

        EnRat_ActorCoordsToWorld(this, &sBlureP2RightOffset, &blureP2);
        EffectBlure_AddVertex(Effect_GetByIndex(this->blure2Index), &blureP1, &blureP2);
    } else if ((this->actor.floorPoly != NULL) && ((play->gameplayFrames % 4) == 0)) {
        func_800286CC(play, &this->actor.world.pos, &sDustVelocity, &gZeroVec3f, 550, 50);
    }

    if ((this->actor.floorPoly == NULL) && Animation_OnFrame(&this->skelAnime, 0.0f)) {
        EnRat_SpawnWaterEffects(this, play);
    }

    if ((this->animLoopCounter == 0) && (Rand_ZeroOne() < 0.05f)) {
        //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_BOMCHU_AIM);
        SoundEffect_PlayOneshot(SOUND_ENEMY_REAL_BOMBCHU_ALERT, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
        this->animLoopCounter = 5;
    }

    //Audio_PlayActorSfx2_Flagged(&this->actor, NA_SE_EN_BOMCHU_RUN - SFX_FLAG);
    SoundEffect_PlayHeld(SOUND_ENEMY_REAL_BOMBCHU_CHARGE, 1.0f, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 1000.0f, &this->actor);
    EnRat_UpdateSparkOffsets(this);
}

void EnRat_SetupBounced(EnRat* this) {
    this->actor.speedXZ = 5.0f;
    this->actor.velocity.y = 8.0f;
    this->actor.world.rot.y = this->actor.yawTowardsPlayer + 0x8000;
    this->actor.gravity = -1.0f;
    EnRat_UpdateSparkOffsets(this);
    this->actor.bgCheckFlags &= ~(BGCHECKFLAG_GROUND | BGCHECKFLAG_WALL | BGCHECKFLAG_CEILING);
    this->actionFunc = EnRat_Bounced;
}

/**
 * Fly backwards until the Real Bombchu touches a wall, ceiling, or floor, then explode.
 */
void EnRat_Bounced(EnRat* this, PlayState* play) {
    this->actor.shape.rot.x -= 0x700;
    Math_StepToF(&this->actor.shape.yOffset, 1700.0f, 170.0f);
    this->timer--;
    if (this->timer == 0) {
        this->timer = 30;
    }

    if (this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_WALL | BGCHECKFLAG_CEILING)) {
        EnRat_Explode(this, play);
    }
}

void EnRat_Explode(EnRat* this, PlayState* play) {
    EnBom* bomb =
        (EnBom*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOM, this->actor.world.pos.x, this->actor.world.pos.y,
                            this->actor.world.pos.z, 0, 0, 0, BOMB_BODY);

    if (bomb != NULL) {
        bomb->timer = 0;
    }

    if (EN_RAT_GET_TYPE(&this->actor) == EN_RAT_TYPE_OVERWORLD) {
        Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0x100);
    }

    this->actor.speedXZ = 0.0f;
    this->actionFunc = EnRat_PostDetonation;
}

/**
 * If the Real Bombchu is an Overworld-type Bombchu, this will set it up to revive.
 * Otherwise, it will simply kill the Bombchu.
 */
void EnRat_PostDetonation(EnRat* this, PlayState* play) {
    if (EN_RAT_GET_TYPE(&this->actor) == EN_RAT_TYPE_OVERWORLD) {
        EnRat_SetupRevive(this);
    } else {
        Actor_Kill(&this->actor);
    }
}

void EnRat_Update(Actor* thisx, PlayState* play) {
    s32 pad;
    EnRat* this = THIS;

    this->shouldRotateOntoSurfaces = false;
    if (this->damageReaction.stunTimer == 0) {
        SkelAnime_Update(&this->skelAnime);
    }

    if (this->collider.base.atFlags & AT_HIT) {
        this->collider.base.atFlags &= ~AT_HIT;
        this->collider.base.acFlags &= ~AC_HIT;
        this->collider.base.ocFlags1 &= ~OC1_HIT;
        if (this->collider.base.atFlags & AT_BOUNCED) {
            EnRat_SetupBounced(this);
        } else {
            EnRat_Explode(this, play);
            return;
        }
    } else if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;
        if (this->actor.colChkInfo.damageEffect == EN_RAT_DMGEFF_HOOKSHOT) {
            this->damageReaction.hookedState = EN_RAT_HOOK_STARTED;
        } else if (this->actor.colChkInfo.damageEffect == EN_RAT_DMGEFF_STUN) {
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
            Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 120, COLORFILTER_BUFFLAG_OPA, 40);
            if (this->actionFunc == EnRat_Bounced) {
                this->actor.speedXZ = 0.0f;
                if (this->actor.velocity.y > 0.0f) {
                    this->actor.velocity.y = 0.0f;
                }
            } else {
                this->damageReaction.stunTimer = 40;
            }
        } else {
            EnRat_Explode(this, play);
            return;
        }
    } else if (((this->collider.base.ocFlags1 & OC1_HIT) && (((this->collider.base.oc->category == ACTORCAT_ENEMY)) ||
                                                             (this->collider.base.oc->category == ACTORCAT_BOSS) ||
                                                             (this->collider.base.oc->category == ACTORCAT_PLAYER))) ||
               ((this->actionFunc == EnRat_ChasePlayer) && (this->timer == 0))) {
        this->collider.base.ocFlags1 &= ~OC1_HIT;
        EnRat_Explode(this, play);
        return;
    }

    this->actionFunc(this, play);

    if ((this->actionFunc != EnRat_PostDetonation) && (this->actionFunc != EnRat_Revive)) {
        if (this->damageReaction.stunTimer > 0) {
            this->damageReaction.stunTimer--;
        } else if (this->damageReaction.hookedState < 0) {
            if (this->damageReaction.hookedState == EN_RAT_HOOK_STARTED) {
                // The player just hit the Real Bombchu with the Hookshot.
                this->damageReaction.hookedState = EN_RAT_HOOKED;
            } else if (!CHECK_FLAG_ALL(this->actor.flags, ACTOR_FLAG_13)) {
                // The player has hooked the Real Bombchu for more than one frame, but
                // the actor flag indicating that the Hookshot is attached is *not* set.
                EnRat_Explode(this, play);
                return;
            }
        } else if (this->actionFunc != EnRat_Bounced) {
            if (this->actor.floorBgId != BGCHECK_SCENE) {
                EnRat_HandleNonSceneCollision(this, play);
            }

            if (!this->hasLostTrackOfPlayer) {
                EnRat_ChooseDirection(this);
            }

            if ((this->actionFunc != EnRat_SpottedPlayer) && !EnRat_IsTouchingSurface(this, play)) {
                EnRat_Explode(this, play);
                return;
            }

            if (this->shouldRotateOntoSurfaces) {
                EnRat_UpdateRotation(this);
                this->actor.shape.rot.x = -this->actor.world.rot.x;
                this->actor.shape.rot.y = this->actor.world.rot.y;
                this->actor.shape.rot.z = this->actor.world.rot.z;
            }

            func_8002D97C(&this->actor);
            this->actor.floorHeight = this->actor.world.pos.y;
        } else {
            Actor_MoveForward(&this->actor);
            Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 30.0f, 60.0f,
                                    UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2);
        }

        if (SurfaceType_IsWallDamage(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId)) {
            EnRat_Explode(this, play);
            return;
        }

        this->collider.elements[0].dim.worldSphere.center.x = this->actor.world.pos.x + (this->axisUp.x * 10.0f);
        this->collider.elements[0].dim.worldSphere.center.y = this->actor.world.pos.y + (this->axisUp.y * 10.0f);
        this->collider.elements[0].dim.worldSphere.center.z = this->actor.world.pos.z + (this->axisUp.z * 10.0f);

        if (this->actionFunc != EnRat_Revive) {
            if (this->damageReaction.stunTimer == 0) {
                CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
            }

            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
        }

        Actor_SetFocus(&this->actor, this->actor.shape.yOffset * 0.015f);
    }
}

s32 EnRat_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnRat* this = THIS;

    if (limbIndex == REAL_BOMBCHU_LIMB_BODY) {
        pos->y -= this->revivePosY;
    }

    if (limbIndex == REAL_BOMBCHU_LIMB_TAIL_END) {
        *dList = NULL;
    }

    return false;
}

void EnRat_PostLimbDraw(PlayState* play2, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    PlayState* play = play2;
    EnRat* this = THIS;
    MtxF* currentMatrixState;
    Vec3f* ptr;
    f32 redModifier;

    if (limbIndex == REAL_BOMBCHU_LIMB_TAIL_END) {
        OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

        Matrix_ReplaceRotation(&play->billboardMtxF);
        Matrix_MultZero(&this->smokePos);
        this->smokePos.y += 15.0f;
        currentMatrixState = Matrix_GetCurrent();

        if (this->actionFunc == EnRat_ChasePlayer) {
            s32 i;

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 150, 255);
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);
            Matrix_Scale(45.0f, 45.0f, 45.0f, MTXMODE_APPLY);

            for (i = 0; i < ARRAY_COUNT(this->sparkOffsets); i++) {
                ptr = &this->sparkOffsets[i];
                currentMatrixState->mf[3][0] = this->smokePos.x + ptr->x;
                currentMatrixState->mf[3][1] = this->smokePos.y + ptr->y;
                currentMatrixState->mf[3][2] = this->smokePos.z + ptr->z;
                gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPSegment(POLY_XLU_DISP++, 0x08, sSparkTextures[(play->gameplayFrames + i) & 3]);
                gSPDisplayList(POLY_XLU_DISP++, gEffSparkDL);
            }

            Matrix_Scale(1.0f / 45.0f, 1.0f / 45.0f, 1.0f / 45.0f, MTXMODE_APPLY);
            currentMatrixState->mf[3][0] = this->smokePos.x;
            currentMatrixState->mf[3][1] = this->smokePos.y - 15.0f;
            currentMatrixState->mf[3][2] = this->smokePos.z;
        }

        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gBombCapDL);
        if (EN_RAT_GET_TYPE(&this->actor) == EN_RAT_TYPE_DUNGEON) {
            redModifier = fabsf(Math_CosF(this->timer * (M_PI / 30.f)));
        } else {
            if (this->timer >= 120) {
                redModifier = fabsf(Math_CosF((this->timer % 30) * (M_PI / 30.0f)));
            } else if (this->timer >= 30) {
                redModifier = fabsf(Math_CosF((this->timer % 6) * (M_PI / 6.0f)));
            } else {
                redModifier = fabsf(Math_CosF((this->timer % 3) * (M_PI / 3.0f)));
            }
        }

        gDPSetEnvColor(POLY_OPA_DISP++, (s32)((1.0f - redModifier) * 255.0f), 0, 40, 255);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, (s32)((1.0f - redModifier) * 255.0f), 0, 40, 255);
        Matrix_RotateZYX(0x4000, 0, 0, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gBombBodyDL);

        CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
    }
}

void EnRat_Draw(Actor* thisx, PlayState* play) {
    EnRat* this = THIS;

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    Gfx_SetupDL_60NoCDXlu(play->state.gfxCtx);
    func_8002EBCC(&this->actor, play, 0);
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                          EnRat_OverrideLimbDraw, EnRat_PostLimbDraw, &this->actor);
}
