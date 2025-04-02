/*
 * File: z_en_mkk.c
 * Overlay: ovl_En_Mkk
 * Description: Black and White Boe (Name origin: Makkurokurosuke)
 */

#include "z_en_mkk.h"
#include <uLib.h>

#define ACT_ID 0x53
#define OBJ_ID 0x11

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_2 | ACTOR_FLAG_4)

#define THIS ((EnMkk*)thisx)

static Gfx SetupDLs[] = {

        /* SETUPDL_25 */
        gsDPPipeSync(),
        gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
        gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIA_PRIM2),
        gsDPSetOtherMode(G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                             G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
                         G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_AA_ZB_OPA_SURF2),
        gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
        gsSPEndDisplayList(),
};

static Gfx D_801AEFA0[] = {gsSPEndDisplayList()};


void EnMkk_Init(Actor* thisx, PlayState* play);
void EnMkk_Destroy(Actor* thisx, PlayState* play);
void EnMkk_Update(Actor* thisx, PlayState* play);
void EnMkk_Draw(Actor* thisx, PlayState* play);

void func_80A4E100(EnMkk* this, PlayState* play);
void func_80A4E1F0(EnMkk* this, PlayState* play);
void func_80A4E2E8(EnMkk* this, PlayState* play);
void func_80A4E60C(EnMkk* this, PlayState* play);
void func_80A4E72C(EnMkk* this, PlayState* play);
void func_80A4EE48(EnMkk* this, PlayState* play);
void func_80A4EF74(EnMkk* this, PlayState* play);
void func_80A4E0CC(EnMkk* this);
void func_80A4E190(EnMkk* this);
void func_80A4E2B8(EnMkk* this);
void func_80A4E58C(EnMkk* this);
void func_80A4EDF0(EnMkk* this);
void func_80A4EEF4(EnMkk* this);
void func_80A4F16C(Actor* thisx, PlayState* play);
void func_80A4F4C8(Actor* thisx, PlayState* play);
void func_80A4E67C(EnMkk* this);
void func_80A4E84C(EnMkk* this);
void func_80A4EBBC(EnMkk* this, PlayState* play);



ActorInit En_Mkk_InitVars = {
    ACT_ID,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJ_ID,
    sizeof(EnMkk),
    (ActorFunc)EnMkk_Init,
    (ActorFunc)EnMkk_Destroy,
    (ActorFunc)EnMkk_Update,
    (ActorFunc)EnMkk_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT3,
        AT_NONE | AT_TYPE_ENEMY,
        AC_NONE | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x04 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_HARD,
        BUMP_ON,
        OCELEM_ON,
    },
    { 18, 32, 0, { 0, 0, 0 } },
};

typedef struct EnMkkDlists {
    /* 0x00 */ Gfx* unk0;
    /* 0x04 */ Gfx* unk4;
    /* 0x08 */ Gfx* unk8;
    /* 0x0C */ Gfx* unkC;
} EnMkkDlists; // size = 0x10

static CollisionCheckInfoInit sColChkInfoInit = { 1, 15, 30, 10 };

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(1, 0xF),
    /* Deku stick    */ DMG_ENTRY(1, 0xF),
    /* Slingshot     */ DMG_ENTRY(1, 0xF),
    /* Explosive     */ DMG_ENTRY(1, 0xF),
    /* Boomerang     */ DMG_ENTRY(1, 0xF),
    /* Normal arrow  */ DMG_ENTRY(1, 0xF),
    /* Hammer swing  */ DMG_ENTRY(1, 0xF),
    /* Hookshot      */ DMG_ENTRY(1, 0xF),
    /* Kokiri sword  */ DMG_ENTRY(1, 0xF),
    /* Master sword  */ DMG_ENTRY(1, 0xF),
    /* Giant's Knife */ DMG_ENTRY(1, 0xF),
    /* Fire arrow    */ DMG_ENTRY(2, 0xF),
    /* Ice arrow     */ DMG_ENTRY(2, 0xF),
    /* Light arrow   */ DMG_ENTRY(2, 0xF),
    /* Unk arrow 1   */ DMG_ENTRY(2, 0xF),
    /* Unk arrow 2   */ DMG_ENTRY(2, 0xF),
    /* Unk arrow 3   */ DMG_ENTRY(2, 0xF),
    /* Fire magic    */ DMG_ENTRY(1, 0xF),
    /* Ice magic     */ DMG_ENTRY(1, 0xF),
    /* Light magic   */ DMG_ENTRY(1, 0xF),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(1, 0xF),
    /* Giant spin    */ DMG_ENTRY(1, 0xF),
    /* Master spin   */ DMG_ENTRY(1, 0xF),
    /* Kokiri jump   */ DMG_ENTRY(1, 0xF),
    /* Giant jump    */ DMG_ENTRY(1, 0xF),
    /* Master jump   */ DMG_ENTRY(1, 0xF),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(1, 0xF),
    /* Powder Keg    */ DMG_ENTRY(8, 0xF),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32_DIV1000(gravity, -500, ICHAIN_CONTINUE),
    ICHAIN_F32(minVelocityY, -5, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 3000, ICHAIN_STOP),
};

static Color_RGBA8 D_80A4F780 = { 250, 250, 250, 255 };

static Color_RGBA8 D_80A4F784 = { 180, 180, 180, 255 };

static Vec3f D_80A4F788 = { 0.0f, 0.45f, 0.0f };

static s16 sEffPrimColorR[] = { { 50 }, { 255 } };
static s16 sEffPrimColorG[] = { { 50 }, { 255 } };
static s16 sEffPrimColorB[] = { { 50 }, { 255 } };
static s16 sEffEnvColorR[] = { { 200 }, { 255 } };
static s16 sEffEnvColorG[] = { { 200 }, { 255 } };
static s16 sEffEnvColorB[] = { { 200 }, { 255 } };

static Color_RGBA8 sEffPrimColors[] = { { 50, 50, 50, 255 }, { 255, 255, 255, 255 } };

static Color_RGBA8 sEffEnvColors[] = { { 200, 200, 200, 255 }, { 255, 255, 255, 255 } };

static EnMkkDlists sBoeDLists[] = {
    { 0x06000030, 0x060000b0, 0x060000c8, 0x06000140 },
    { 0x060001f0, 0x06000278, 0x06000290, 0x06000310 },
};

static Color_RGBA8 D_80A4F7C4[] = {
    { 255, 255, 255, 255 }, { 128, 128, 128, 255 }, { 0, 0, 0, 255 },
    { 0, 0, 0, 255 },       { 128, 128, 128, 255 }, { 255, 255, 255, 255 },
};

void EnMkk_Init(Actor* thisx, PlayState* play) {

    EnMkk* this = THIS;
    s32 paramsFF00;
    s32 params2;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    this->collider.dim.radius = sCylinderInit.dim.radius;
    CollisionCheck_SetInfo(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    ActorShape_Init(&this->actor.shape, 1000.0f, NULL, 0.0f);

    this->primColorSelect = 0;
    this->unk_149 = 0;
    this->unk_152 = (this->actor.shape.rot.x >= 0) ? this->actor.shape.rot.x : this->actor.shape.rot.x * -0x50;

    this->unk_14C = CLAMP(this->actor.shape.rot.z, 0, 16);

    this->actor.shape.rot.z = 0;
    this->actor.world.rot.z = 0;
    this->actor.shape.rot.x = 0;
    this->actor.world.rot.x = 0;

    Math_Vec3f_Copy(&this->unk_154, &this->actor.world.pos);
    Math_Vec3f_Copy(&this->unk_160, &this->actor.world.pos);
    this->unk_14B = ENMKK_GET_4(thisx) ? 8 : 0;

    paramsFF00 = ENMKK_GET_FF00(&this->actor);
    params2 = ENMKK_GET_2(&this->actor);
    this->actor.params &= 1;

    if (this->actor.params == 1) {
        this->actor.naviEnemyId = 0x73;
    } else {
        this->actor.naviEnemyId = 0x72;
    }

    if ((paramsFF00 == 0) || (paramsFF00 == 255)) {
        func_80A4E0CC(this);
        this->unk_178 = 30000.0f;
        if (params2 > 0) {
            this->unk_14B |= 4;
        }
    } else {
        this->unk_178 = paramsFF00 * 40.0f * 0.1f;
        func_80A4EDF0(this);
        this->unk_14E = 0;
    }
}

void EnMkk_Destroy(Actor* thisx, PlayState* play) {
    EnMkk* this = THIS;

    Collider_DestroyCylinder(play, &this->collider);
}

void func_80A4E0CC(EnMkk* this) {
    this->alpha = 0;
    this->unk_14B |= 3;
    this->actor.flags &= ~ACTOR_FLAG_0;
    this->actionFunc = func_80A4E100;
}

void func_80A4E100(EnMkk* this, PlayState* play) {
    s32 newAlpha;

    if (this->unk_14B & 4) {
        newAlpha = this->alpha + 15;
    } else {
        newAlpha = this->alpha + 5;
    }
    if (newAlpha >= 255) {
        this->primColorSelect = 3;
        this->collider.base.acFlags |= AC_ON;
        this->alpha = 255;
        this->actor.flags |= ACTOR_FLAG_0;
        this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
        this->unk_14B &= ~4;
        func_80A4E190(this);
    } else {
        this->alpha = newAlpha;
    }
}

void func_80A4E190(EnMkk* this) {
    this->unk_14E = (s32)(Rand_ZeroOne() * 20.0f) + 10;
    if (this->unk_149 != 0) {
        this->unk_149--;
    }
    this->actionFunc = func_80A4E1F0;
}

void func_80A4E1F0(EnMkk* this, PlayState* play) {
    this->unk_14E--;
    if (this->unk_14E == 0) {
        func_80A4E2B8(this);
    }
}

void func_80A4E22C(EnMkk* this, PlayState* play) {
    Vec3f pos;
    Vec3f zeroVec3f = {0};

    pos.x = this->actor.world.pos.x;
    pos.y = this->actor.world.pos.y + 15.0f;
    pos.z = this->actor.world.pos.z;
    func_8002829C(play, &pos, &zeroVec3f, &D_80A4F788, &D_80A4F780, &D_80A4F784, 350, 20);
}

void func_80A4E2B8(EnMkk* this) {
    this->unk_14E = 30;
    this->unk_150 = this->actor.shape.rot.y;
    this->unk_14B &= ~1;
    this->actionFunc = func_80A4E2E8;
    Collider_UpdateCylinder(&this->actor, &this->collider);
}

void func_80A4E2E8(EnMkk* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 sp20;

    this->unk_14E--;
    if ((this->actor.params == 1) && (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && (this->actor.speedXZ > 2.5f) &&
        ((play->gameplayFrames % 3) == 0)) {
        func_80A4E22C(this, play);
    }
    if (this->unk_14E > 0) {
        Math_StepToF(&this->actor.speedXZ, 5.0f, 0.7f);
        sp20 = false;
    } else {
        sp20 = Math_StepToF(&this->actor.speedXZ, 0.0f, 0.7f);
    }
    if ((player->stateFlags2 & 0x80) || (player->actor.freezeTimer > 0)) {
        Math_ScaledStepToS(&this->unk_150, this->actor.yawTowardsPlayer + 0x8000, 0x400);
    } else {
        Math_ScaledStepToS(&this->unk_150, this->actor.yawTowardsPlayer, 0x400);
    }
    this->actor.shape.rot.y =
        (s32)(sinf(this->unk_14E * ((2 * M_PI) / 15)) * (614.4f * this->actor.speedXZ)) + this->unk_150;
    SoundEffect_PlayHeld(SOUND_ENEMY_BOE_MOVE, 0.3f, 1.0f, 1.0f, &this->actor.world.pos, 0.1f, 1000.0f, this);
    if (sp20) {
        this->unk_14B &= ~2;
        func_80A4E190(this);
    } else if ((this->unk_149 == 0) && (!(player->stateFlags3 & 0x100)) &&
               (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) &&
               (Actor_IsFacingPlayer(&this->actor, 0x1800)) && (this->actor.xzDistToPlayer < 120.0f) &&
               (fabsf(this->actor.yDistToPlayer) < 100.0f)) {
        func_80A4E58C(this);
    }
}

void func_80A4E58C(EnMkk* this) {
    this->unk_14B |= 1;
    this->actor.speedXZ = 3.0f;
    this->actor.velocity.y = 5.0f;
    SoundEffect_PlayOneshot(SOUND_ENEMY_BOE_ATTACK, 1.0f, 1.0f, &this->actor.world.pos, 0.1f, 500.0f, this);
    this->collider.base.atFlags |= AT_ON;
    Math_ScaledStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0x800);
    this->actionFunc = func_80A4E60C;
}

void func_80A4E60C(EnMkk* this, PlayState* play) {
    if (this->collider.base.atFlags & AT_HIT) {
        this->collider.base.atFlags &= ~(AT_ON | AT_HIT);
    }
    if ((this->actor.velocity.y < 0.0f) && (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        this->unk_149 = 2;
        this->collider.base.atFlags &= ~AT_ON;
        func_80A4E2B8(this);
    }
}

void func_80A4E67C(EnMkk* this) {
    this->unk_14B |= 1;
    this->actor.flags &= ~ACTOR_FLAG_0;
    this->collider.base.acFlags &= ~AC_ON;
    this->actor.flags |= ACTOR_FLAG_4;
    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_PO_DEAD);
    this->alpha = 254;
    Collider_UpdateCylinder(&this->actor, &this->collider);
    this->actor.speedXZ = -7.0f;
    this->actor.shape.rot.y = this->actor.world.rot.y;
    this->actor.velocity.y = 5.0f;
    this->actor.gravity = -1.3f;
    this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
    this->actionFunc = func_80A4E72C;
}

void func_80A4E72C(EnMkk* this, PlayState* play) {
    Vec3f temp;
    Vec3f zeroVec3f = {0};

    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        if (this->actor.velocity.y > -1.0f) {
            temp.x = this->actor.world.pos.x;
            temp.y = this->actor.world.pos.y + 15.0f;
            temp.z = this->actor.world.pos.z;
            
            if (this->actor.params == 1) {
                EffectSsDeadDb_Spawn(play, &temp, &zeroVec3f, &zeroVec3f, 100, 0, 255, 255, 255, 255, 255, 255, 255, 255, 13, 1);  
            } else {
                EffectSsDeadDb_Spawn(play, &temp, &zeroVec3f, &zeroVec3f, 100, 0, 50, 50, 50, 255, 200, 200, 200, 255, 13, 1);
            } 
            
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_EXTINCT);
            if (this->unk_14C != 0) {
                 Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, this->unk_14C * 0x10);
            }
            func_80A4EEF4(this);
        } else {
            this->actor.velocity.y *= -0.8f;
            this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
        }
    }
}

void func_80A4E84C(EnMkk* this) {
    if (this->unk_14B & 3) {
        Vec3f sp34;
        f32 temp_fv0 = Math_Vec3f_DistXYZ(&this->actor.world.pos, &this->actor.prevPos);

        Math_Vec3f_Copy(&sp34, &this->unk_154);
        if (temp_fv0 < 0.001f) {
            Math_Vec3f_Copy(&this->unk_154, &this->actor.prevPos);
        } else {
            temp_fv0 = 1.0f / temp_fv0;

            this->unk_154.x =
                ((this->actor.prevPos.x - this->actor.world.pos.x) * temp_fv0 * 10.0f) + this->actor.world.pos.x;
            this->unk_154.y =
                ((this->actor.prevPos.y - this->actor.world.pos.y) * temp_fv0 * 10.0f) + this->actor.world.pos.y;
            this->unk_154.z =
                ((this->actor.prevPos.z - this->actor.world.pos.z) * temp_fv0 * 10.0f) + this->actor.world.pos.z;
        }
        temp_fv0 = Math_Vec3f_DistXYZ(&this->unk_154, &sp34);
        if (temp_fv0 < 0.001f) {
            Math_Vec3f_Copy(&this->unk_160, &sp34);
        } else {
            temp_fv0 = 1.0f / temp_fv0;

            this->unk_160.x = ((sp34.x - this->unk_154.x) * temp_fv0 * 12.0f) + this->unk_154.x;
            this->unk_160.y = ((sp34.y - this->unk_154.y) * temp_fv0 * 12.0f) + this->unk_154.y;
            this->unk_160.z = ((sp34.z - this->unk_154.z) * temp_fv0 * 12.0f) + this->unk_154.z;
        }
    } else {
        this->unk_160.y = this->unk_154.y;
        this->unk_154.y = this->actor.world.pos.y;
        this->unk_154.x = this->actor.world.pos.x -
                          10.0f * Math_SinS(this->actor.shape.rot.y +
                                            (s32)(1228.8f * this->actor.speedXZ * sinf(this->unk_14E * (M_PI / 5))));
        this->unk_154.z = this->actor.world.pos.z -
                          10.0f * Math_CosS(this->actor.shape.rot.y +
                                            (s32)(1228.8f * this->actor.speedXZ * sinf(this->unk_14E * (M_PI / 5))));
        this->unk_160.x = this->unk_154.x -
                          12.0f * Math_SinS(this->actor.shape.rot.y -
                                            (s32)(1228.8f * this->actor.speedXZ * sinf(this->unk_14E * (M_PI / 5))));
        this->unk_160.z = this->unk_154.z -
                          12.0f * Math_CosS(this->actor.shape.rot.y -
                                            (s32)(1228.8f * this->actor.speedXZ * sinf(this->unk_14E * (M_PI / 5))));
    }
}

void func_80A4EBBC(EnMkk* this, PlayState* play) {
    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlag(&this->actor, &this->collider.info, true);
        Enemy_StartFinishingBlow(play, &this->actor);
        func_80A4E67C(this);
    }
}

void EnMkk_Update(Actor* thisx, PlayState* play) {
    s32 pad;
    Player* player;
    EnMkk* this = THIS;

    if (this->primColorSelect > 0) {
        this->primColorSelect--;
    } else if ((Rand_ZeroOne() < 0.075f) && (this->actor.params != 1)) {
        this->primColorSelect = 6;
    }

    func_80A4EBBC(this, play);
    this->actionFunc(this, play);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    Actor_MoveForward(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 20.0f, 20.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_3 |
                                UPDBGCHECKINFO_FLAG_4);

    if (this->actor.params == 0) {
        func_80A4E84C(this);
    }

    if (Actor_IsFacingPlayer(&this->actor, 0x3000)) {
        player = GET_PLAYER(play);
        this->actor.shape.rot.x = Actor_WorldPitchTowardPoint(&this->actor, &player->actor.focus.pos);
        this->actor.shape.rot.x = CLAMP(this->actor.shape.rot.x, -0x1800, 0x1800);
    }


    Actor_SetFocus(&this->actor, 10.0f);
    this->collider.dim.pos.x = this->actor.focus.pos.x;
    this->collider.dim.pos.y = this->actor.focus.pos.y;
    this->collider.dim.pos.z = this->actor.focus.pos.z;

    Collider_UpdateCylinder(&this->actor, &this->collider);

    if (this->collider.base.atFlags & AT_ON) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    }
    if (this->collider.base.acFlags & AT_ON) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
}

void func_80A4EDF0(EnMkk* this) {
    this->alpha = 0;
    this->unk_14B |= 3;
    this->actor.flags &= ~ACTOR_FLAG_0;
    this->actor.draw = NULL;
    this->actor.update = func_80A4F16C;
    this->actor.gravity = -0.5f;
    this->unk_14E = this->unk_152;
    this->actionFunc = func_80A4EE48;
}

void func_80A4EE48(EnMkk* this, PlayState* play) {
    if (this->unk_14E > 0) {
        this->unk_14E--;
        if (this->unk_14E == 0) {
            this->actor.flags &= ~ACTOR_FLAG_4;
        }
    } else if ((this->actor.xzDistToPlayer < this->unk_178)) {
        this->actor.update = EnMkk_Update;
        this->actor.draw = EnMkk_Draw;
        this->actor.flags &= ~ACTOR_FLAG_4;
        func_80A4E0CC(this);
    }
}

void func_80A4EEF4(EnMkk* this) {
    Math_Vec3f_Copy(&this->unk_16C, &this->actor.world.pos);
    Math_Vec3f_Copy(&this->unk_154, &this->actor.world.pos);
    Math_Vec3f_Copy(&this->unk_160, &this->actor.world.pos);
    Actor_SetScale(&this->actor, 0.005f);
    this->actor.update = func_80A4F16C;
    this->actor.draw = func_80A4F4C8;
    this->actionFunc = func_80A4EF74;
}

void func_80A4EF74(EnMkk* this, PlayState* play) {
    s32 newAlpha = this->alpha - 20;

    if (newAlpha <= 0) {
        if (this->unk_14B & 8) {
            Actor_SetScale(&this->actor, 0.01f);
            this->primColorSelect = 0;
            this->unk_149 = 0;
            Math_Vec3f_Copy(&this->actor.world.pos, &this->actor.home.pos);
            Math_Vec3f_Copy(&this->unk_154, &this->actor.world.pos);
            Math_Vec3f_Copy(&this->unk_160, &this->actor.world.pos);
            this->actor.speedXZ = 0.0f;
            this->actor.velocity.y = 0.0f;
            func_80A4EDF0(this);
        } else {
            Actor_Kill(&this->actor);
        }
    } else {
        this->alpha = newAlpha;
        this->actor.world.pos.x += 0.3f + (3.0f * Rand_ZeroOne());
        this->actor.world.pos.y += 0.5f + (3.5f * Rand_ZeroOne());
        this->unk_16C.x -= 0.3f + (3.0f * Rand_ZeroOne());
        this->unk_16C.y += 0.5f + (3.5f * Rand_ZeroOne());
        this->unk_154.z += 0.3f + (3.0f * Rand_ZeroOne());
        this->unk_154.y += 0.5f + (3.5f * Rand_ZeroOne());
        this->unk_160.z -= 0.3f + (3.0f * Rand_ZeroOne());
        this->unk_160.y += 0.5f + (3.5f * Rand_ZeroOne());
    }
}

void func_80A4F16C(Actor* thisx, PlayState* play) {
    EnMkk* this = THIS;

    this->actionFunc(this, play);
}

void EnMkk_Draw(Actor* thisx, PlayState* play) {
    EnMkkDlists* dLists = &sBoeDLists[thisx->params];
    Gfx* gfx;
    Color_RGBA8* primColors;
    EnMkk* this = THIS;

    
    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    if (this->actor.projectedPos.z > 0.0f) {
        MtxF* matrix;


        if (this->alpha == 255) {
            primColors = &D_80A4F7C4[this->primColorSelect];
            gfx = POLY_OPA_DISP;
            gSPDisplayList(&gfx[0], SetupDLs);
            gDPSetPrimColor(&gfx[1], 0, 0xFF, primColors->r, primColors->g, primColors->b, primColors->a);
            gSPSegment(&gfx[2], 0x08, D_801AEFA0);
            gSPMatrix(&gfx[3], Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(&gfx[4], dLists->unkC);
            POLY_OPA_DISP = &gfx[5];
        }
        gfx = POLY_XLU_DISP;
        gSPDisplayList(&gfx[0], SetupDLs);
        gDPSetEnvColor(&gfx[1], 255, 255, 255, this->alpha);
        gSPDisplayList(&gfx[2], dLists->unk0);
        Matrix_ReplaceRotation(&play->billboardMtxF);
        gSPMatrix(&gfx[3], Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(&gfx[4], dLists->unk4);
        if (thisx->params == 0) {
            matrix = Matrix_GetCurrent();
            matrix->mf[3][0] = this->unk_154.x;
            matrix->mf[3][1] = this->unk_154.y + 8.5f;
            matrix->mf[3][2] = this->unk_154.z;

            Matrix_Scale(0.85f, 0.85f, 0.85f, MTXMODE_APPLY);
            gSPMatrix(&gfx[5], Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(&gfx[6], dLists->unk4);

            matrix->mf[3][0] = this->unk_160.x;
            matrix->mf[3][1] = this->unk_160.y + 7.2250004f;
            matrix->mf[3][2] = this->unk_160.z;

            Matrix_Scale(0.85f, 0.85f, 0.85f, MTXMODE_APPLY);
            gSPMatrix(&gfx[7], Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(&gfx[8], dLists->unk4);
            gSPDisplayList(&gfx[9], dLists->unk8);

            POLY_XLU_DISP = &gfx[10];
        } else {
            gSPDisplayList(&gfx[5], dLists->unk8);
            POLY_XLU_DISP = &gfx[6];
        }
    }
    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

void func_80A4F4C8(Actor* thisx, PlayState* play) {
    s32 pad;
    Gfx* gfx;
    MtxF* matrix;
    EnMkkDlists* dLists = &sBoeDLists[thisx->params];
    EnMkk* this = THIS;

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    gfx = POLY_XLU_DISP;
    gSPDisplayList(&gfx[0], SetupDLs);
    gDPSetEnvColor(&gfx[1], 255, 255, 255, this->alpha);
    gSPDisplayList(&gfx[2], dLists->unk0);
    Matrix_ReplaceRotation(&play->billboardMtxF);
    gSPMatrix(&gfx[3], Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(&gfx[4], dLists->unk4);

    matrix = Matrix_GetCurrent();
    matrix->mf[3][0] = this->unk_154.x;
    matrix->mf[3][1] = this->unk_154.y + 5.0f;
    matrix->mf[3][2] = this->unk_154.z;

    gSPMatrix(&gfx[5], Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(&gfx[6], dLists->unk4);

    matrix->mf[3][0] = this->unk_160.x;
    matrix->mf[3][1] = this->unk_160.y + 5.0f;
    matrix->mf[3][2] = this->unk_160.z;

    gSPMatrix(&gfx[7], Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(&gfx[8], dLists->unk4);

    matrix->mf[3][0] = this->unk_16C.x;
    matrix->mf[3][1] = this->unk_16C.y + 5.0f;
    matrix->mf[3][2] = this->unk_16C.z;

    gSPMatrix(&gfx[9], Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(&gfx[10], dLists->unk4);
    gSPDisplayList(&gfx[11], dLists->unk8);
    POLY_XLU_DISP = &gfx[12];

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}
