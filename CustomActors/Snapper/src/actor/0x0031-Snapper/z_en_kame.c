/*
 * File: z_en_kame.c
 * Overlay: ovl_En_Kame
 * Description: Snapper Turtle
 */

#include "z_en_kame.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "mm/requirements.h"
#include <uLib.h>

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_2 | ACTOR_FLAG_10)

#define THIS ((EnKame*)thisx)

#define ACT_ID 0x0031
#define OBJ_ID 0x0078

void EnKame_Init(Actor* thisx, PlayState* play);
void EnKame_Destroy(Actor* thisx, PlayState* play);
void EnKame_Update(Actor* thisx, PlayState* play);
void EnKame_Draw(Actor* thisx, PlayState* play);

void func_80AD70A0(EnKame* this);
void func_80AD70EC(EnKame* this, PlayState* play);
void func_80AD71B4(EnKame* this);
void func_80AD7254(EnKame* this, PlayState* play);
void func_80AD73A8(EnKame* this);
void func_80AD7424(EnKame* this, PlayState* play);
void func_80AD76CC(EnKame* this);
void func_80AD7798(EnKame* this, PlayState* play);
void func_80AD792C(EnKame* this);
void func_80AD7948(EnKame* this, PlayState* play);
void func_80AD7B18(EnKame* this);
void func_80AD7B90(EnKame* this, PlayState* play);
void func_80AD7D40(EnKame* this, PlayState* play);
void func_80AD7DA4(EnKame* this);
void func_80AD7E0C(EnKame* this, PlayState* play);
void func_80AD7EC0(EnKame* this);
void func_80AD7F10(EnKame* this, PlayState* play);
void func_80AD7FF8(EnKame* this, PlayState* play);
void func_80AD810C(EnKame* this, PlayState* play);
void EnKame_SetupDie(EnKame* this, PlayState* play);
void func_80AD825C(EnKame* this, PlayState* play);
void func_80AD8364(EnKame* this);
void func_80AD8388(EnKame* this, PlayState* play);
void func_80AD8D64(Actor* thisx, PlayState* play);

ActorInit En_Kame_InitVars = {
    ACT_ID,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJ_ID,
    sizeof(EnKame),
    (ActorFunc)EnKame_Init,
    (ActorFunc)EnKame_Destroy,
    (ActorFunc)EnKame_Update,
    (ActorFunc)EnKame_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HARD,
        AT_NONE | AT_TYPE_ENEMY,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x04 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 35, 40, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 3, 15, 30, 80 };

static DamageTable sDamageTable = {
    /* Deku Nut       */ DMG_ENTRY(0, 0x1),
    /* Deku Stick     */ DMG_ENTRY(1, 0x0),
    /* Slingshot      */ DMG_ENTRY(1, 0x0),
    /* Explosive      */ DMG_ENTRY(1, 0xF),
    /* Boomerang      */ DMG_ENTRY(1, 0x0),
    /* Normal arrow   */ DMG_ENTRY(1, 0x0),
    /* Hammer swing   */ DMG_ENTRY(1, 0x0),
    /* Hookshot       */ DMG_ENTRY(0, 0xD),
    /* Kokiri sword   */ DMG_ENTRY(1, 0x0),
    /* Master sword   */ DMG_ENTRY(2, 0x0),
    /* Giant's Knife  */ DMG_ENTRY(4, 0x0),
    /* Fire arrow     */ DMG_ENTRY(2, 0x2),
    /* Ice arrow      */ DMG_ENTRY(2, 0x3),
    /* Light arrow    */ DMG_ENTRY(2, 0x4),
    /* Unk arrow 1    */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 2    */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 3    */ DMG_ENTRY(2, 0x0),
    /* Fire magic     */ DMG_ENTRY(2, 0x2),
    /* Ice magic      */ DMG_ENTRY(2, 0x3),
    /* Light magic    */ DMG_ENTRY(2, 0x6),
    /* Shield         */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray     */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin    */ DMG_ENTRY(1, 0x0),
    /* Giant spin     */ DMG_ENTRY(4, 0x0),
    /* Master spin    */ DMG_ENTRY(2, 0x0),
    /* Kokiri jump    */ DMG_ENTRY(2, 0x0),
    /* Giant jump     */ DMG_ENTRY(8, 0x0),
    /* Master jump    */ DMG_ENTRY(4, 0x0),
    /* Unknown 1      */ DMG_ENTRY(0, 0x0),
    /* Unblockable    */ DMG_ENTRY(0, 0x0),
    /* Hammer jump    */ DMG_ENTRY(4, 0x0),
    /* Unknown 2      */ DMG_ENTRY(8, 0xF),
};

static u32 sEyeTextures[] = { 0x060055a0, 0x060057a0, 0x060059a0,
                                   0x060057a0 };

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x71, ICHAIN_CONTINUE),
    ICHAIN_F32(gravity, -1, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 3500, ICHAIN_STOP),
};

static s32 D_80AD8E50 = false;

// Relative positions to spawn ice chunks when tektite is frozen
static Vec3f sIceChunks[12] = {
    { 20.0f, 20.0f, 0.0f },   { 10.0f, 40.0f, 10.0f },   { -10.0f, 40.0f, 10.0f }, { -20.0f, 20.0f, 0.0f },
    { 10.0f, 40.0f, -10.0f }, { -10.0f, 40.0f, -10.0f }, { 0.0f, 20.0f, -20.0f },  { 10.0f, 0.0f, 10.0f },
    { 10.0f, 0.0f, -10.0f },  { 0.0f, 20.0f, 20.0f },    { -10.0f, 0.0f, 10.0f },  { -10.0f, 0.0f, -10.0f },
};

void EnKame_Init(Actor* thisx, PlayState* play) {
    EnKame* this = THIS;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    SkelAnime_InitFlex(play, &this->skelAnime1, 0x06007c70, 0x06004210, this->jointTable1,
                       this->morphTable1, 13);
    SkelAnime_InitFlex(play, &this->skelAnime2, 0x06001a50, 0x06000b30, this->jointTable2,
                       this->morphTable2, 4);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 55.0f);
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    CollisionCheck_SetInfo(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);

    if (!D_80AD8E50) {
        s32 i;

        //for (i = 0; i < ARRAY_COUNT(sEyeTextures); i++) {
        //    sEyeTextures[i] = Lib_SegmentedToVirtual(sEyeTextures[i]);
        //}
        D_80AD8E50 = true;
    }

    func_80AD70A0(this);
}

void EnKame_Destroy(Actor* thisx, PlayState* play) {
    EnKame* this = THIS;

    Collider_DestroyCylinder(play, &this->collider);
}

void func_80AD6F34(EnKame* this) {
    if (this->unk_29C != 0) {
        this->unk_29C++;
        if (this->unk_29C == 4) {
            this->unk_29C = 0;
        }
    } else if (Rand_ZeroOne() < 0.05f) {
        this->unk_29C = 1;
    }
}

void func_80AD6F9C(EnKame* this) {
    this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX;
    this->drawDmgEffScale = 0.6f;
    this->drawDmgEffFrozenSteamScale = 0.90000004f;
    this->drawDmgEffAlpha = 1.0f;
    this->collider.base.colType = COLTYPE_HIT3;
    this->unk_2A2 = 80;
    this->actor.flags &= ~ACTOR_FLAG_10;
    Actor_SetColorFilter(&this->actor, 0x4000, 255, 0x0000, 80);
}

void func_80AD7018(EnKame* this, PlayState* play) {
    if (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) {
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
        this->collider.base.colType = COLTYPE_HIT6;
        this->drawDmgEffAlpha = 0.0f;
        Actor_SpawnIceEffects(play, &this->actor, this->limbPos, 10, 2, 0.3f, 0.2f);
        this->actor.flags |= ACTOR_FLAG_10;
    }
}

void func_80AD70A0(EnKame* this) {
    Animation_MorphToPlayOnce(&this->skelAnime1, 0x06004210, -5.0f);
    this->actor.speedXZ = 0.0f;
    this->actionFunc = func_80AD70EC;
}

void func_80AD70EC(EnKame* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (Animation_OnFrame(&this->skelAnime1, 10.0f)) {
        //Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_VOICE);
        SoundEffect_PlayOneshot(SOUND_ENEMY_SNAPPER_TALK, 1.0f, 1.0f, &this->actor.world.pos, 0.1f, 1000.0f, &this->actor);
        this->unk_2A0 = 40;
    }

    if ((this->actor.xzDistToPlayer < 240.0f)) {
        func_80AD73A8(this);
    } else if (SkelAnime_Update(&this->skelAnime1)) {
        func_80AD71B4(this);
    }
}

void func_80AD71B4(EnKame* this) {
    Animation_MorphToLoop(&this->skelAnime1, 0x0600823c, -5.0f);
    this->actor.speedXZ = 0.5f;
    this->unk_29E = Animation_GetLastFrame(0x0600823c) * ((s32)Rand_ZeroFloat(5.0f) + 3);
    this->unk_2A4 = this->actor.shape.rot.y;
    this->collider.base.acFlags |= (AC_HARD | AC_ON);
    this->collider.base.colType = COLTYPE_HARD;
    this->actionFunc = func_80AD7254;
}

void func_80AD7254(EnKame* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if ((this->actor.xzDistToPlayer < 240.0f)) {
        func_80AD73A8(this);
        return;
    }

    SkelAnime_Update(&this->skelAnime1);

    if (this->unk_2A4 != this->actor.shape.rot.y) {
        Math_ScaledStepToS(&this->actor.shape.rot.y, this->unk_2A4, 0x100);
        this->actor.world.rot.y = this->actor.shape.rot.y;
    } else if (Actor_WorldDistXZToPoint(&this->actor, &this->actor.home.pos) > 40.0f) {
        this->unk_2A4 = Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos) + ((s32)Rand_Next() >> 0x14);
    }

    this->unk_29E--;
    if (this->unk_29E == 0) {
        func_80AD70A0(this);
    } else if (Animation_OnFrame(&this->skelAnime1, 0.0f) || Animation_OnFrame(&this->skelAnime1, 15.0f)) {
        //Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_WALK);
        SoundEffect_PlayOneshot(SOUND_ENEMY_SNAPPER_STEP, 0.3f, 1.0f, &this->actor.world.pos, 0.1f, 1000.0f, &this->actor);
    }
}

void func_80AD73A8(EnKame* this) {
    Animation_MorphToPlayOnce(&this->skelAnime1, 0x06001c68, -3.0f);
    this->unk_29E = 0;
    this->unk_2AC = 1.0f;
    this->unk_2A8 = 1.0f;
    this->actor.speedXZ = 0.0f;
    if (this->unk_2A0 == 0) {
        //Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_VOICE);
        SoundEffect_PlayOneshot(SOUND_ENEMY_SNAPPER_TALK, 1.0f, 1.0f, &this->actor.world.pos, 0.1f, 1000.0f, &this->actor);
    }
    this->actionFunc = func_80AD7424;
}

void func_80AD7424(EnKame* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (SkelAnime_Update(&this->skelAnime1)) {
        if (((this->unk_29E == 0) || (this->actor.xzDistToPlayer < 120.0f))) {
            func_80AD76CC(this);
        } else {
            this->unk_29E--;
            if (this->unk_29E == 0) {
                func_80AD7B18(this);
            }
        }
    } else if (this->skelAnime1.curFrame > 2.0f) {
        this->unk_2AC = 1.5f - ((this->skelAnime1.curFrame - 2.0f) * (7.0f / 30));
        this->unk_2A8 = 1.5f - ((this->skelAnime1.curFrame - 2.0f) * (1.0f / 12));
    } else {
        f32 frame = this->skelAnime1.curFrame;

        this->unk_2AC = (0.25f * frame) + 1.0f;
        this->unk_2A8 = (0.25f * frame) + 1.0f;
    }
}

void func_80AD7568(EnKame* this) {
    this->actor.speedXZ = this->unk_2A6 * (5.0f / 7552);
    this->actor.shape.rot.z = this->unk_2A6 * 0.11016949f;
}

void func_80AD75A8(EnKame* this, PlayState* play) {
    static Color_RGBA8 D_80AD8E54 = { 250, 250, 250, 255 };
    static Color_RGBA8 D_80AD8E58 = { 180, 180, 180, 255 };
    static Vec3f D_80AD8E5C = { 0.0f, 0.75f, 0.0f };
    Vec3f gZeroVec3f = {0};

    if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && (this->actor.speedXZ >= 3.0f)) {
        if ((play->gameplayFrames % 2) == 0) {
            SurfaceSfxType surfaceSfxType =
                SurfaceType_GetSfxType(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId);
            
            if ((surfaceSfxType == SURFACE_SFX_TYPE_0) || (surfaceSfxType == SURFACE_SFX_TYPE_1)) {
                func_800286CC(play, &this->actor.world.pos, &D_80AD8E5C, &gZeroVec3f, 550, 100);
            } else if (surfaceSfxType == 15) {
                func_8002829C(play, &this->actor.world.pos, &D_80AD8E5C, &gZeroVec3f, &D_80AD8E54, &D_80AD8E58, 550,
                              100);
            }
        }
    }

    if (this->unk_2A6 > 0x1200) {
        func_8002F974(&this->actor, NA_SE_EN_PIHAT_SM_FLY - SFX_FLAG);
    }
}

void func_80AD76CC(EnKame* this) {
    if (this->actor.draw == EnKame_Draw) {
        this->actor.draw = func_80AD8D64;
        this->unk_2A6 = 0x3B00;
        this->unk_2AC = 0.5f;
        func_80AD7568(this);
        this->unk_29E = 15;
        this->actor.speedXZ = 0.0f;
        //Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_CUTTER_ON);
        SoundEffect_PlayOneshot(SOUND_ENEMY_SNAPPER_ATTACK_START, 1.0f, 1.0f, &this->actor.world.pos, 0.1f, 1000.0f, &this->actor);
        this->unk_2BC.y = this->actor.home.pos.y - 100.0f;
    } else {
        this->actor.world.rot.y = Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos);
        Math_Vec3f_Copy(&this->unk_2BC, &this->actor.home.pos);
        this->unk_29E = 0;
    }
    this->actor.flags |= ACTOR_FLAG_4;
    this->actionFunc = func_80AD7798;
}

void func_80AD7798(EnKame* this, PlayState* play) {
    if (this->unk_29E == 15) {
        this->unk_2AC += 0.2f;
        if (this->unk_2AC > 1.1f) {
            this->unk_29E--;
            this->collider.base.atFlags |= AT_ON;
            this->unk_2AC = 1.0f;
        }
    } else if (this->unk_29E > 0) {
        this->unk_29E--;
    } else if (Math_ScaledStepToS(&this->unk_2A6, 0x3B00, (s32)(this->unk_2A6 * 0.09f) + 45)) {
        if (this->unk_2BC.y < this->actor.home.pos.y) {
            this->actor.world.rot.y = this->actor.yawTowardsPlayer;
            this->unk_2BC.x = (Math_SinS(this->actor.world.rot.y) * 360.0f) + this->actor.world.pos.x;
            this->unk_2BC.z = (Math_CosS(this->actor.world.rot.y) * 360.0f) + this->actor.world.pos.z;
        }
        func_80AD792C(this);
    }

    this->actor.shape.rot.y += this->unk_2A6;

    if (!(this->unk_2BC.y < this->actor.home.pos.y) || (this->actionFunc != func_80AD7798)) {
        func_80AD7568(this);
    }
    func_80AD75A8(this, play);
}

void func_80AD792C(EnKame* this) {
    this->unk_29E = -1;
    this->actionFunc = func_80AD7948;
}

void func_80AD7948(EnKame* this, PlayState* play) {
    s32 temp_v1;

    this->actor.shape.rot.y += this->unk_2A6;
    func_80AD75A8(this, play);

    if (this->unk_29E == -1) {
        s16 temp_v0 = Actor_WorldYawTowardPoint(&this->actor, &this->unk_2BC) - this->actor.world.rot.y;

        temp_v1 = ABS_ALT(temp_v0);

        if ((this->actor.bgCheckFlags & BGCHECKFLAG_WALL) || (temp_v1 > 0x3000) ||
            (Actor_WorldDistXZToPoint(&this->actor, &this->unk_2BC) < 50.0f)) {
            s8 pad;

            if (this->unk_2BC.y < this->actor.home.pos.y) {
                this->unk_29E = 0x300;
            } else {
                this->unk_29E = 0;
            }
        }
        return;
    }

    if (Math_ScaledStepToS(&this->unk_2A6, this->unk_29E, (s32)(this->unk_2A6 * 0.09f) + 45)) {
        if (this->unk_29E == 0) {
            if (this->unk_2AC >= 1.0f) {
                //Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_CUTTER_OFF);
                SoundEffect_PlayOneshot(SOUND_ENEMY_SNAPPER_ATTACK_END, 0.5f, 1.0f, &this->actor.world.pos, 0.1f, 1000.0f, &this->actor);
            }
            this->unk_2AC -= 0.1f;
            this->collider.base.atFlags &= ~AT_ON;
            if (this->unk_2AC < 0.5f) {
                this->actor.flags &= ~ACTOR_FLAG_4;
                func_80AD7B18(this);
            }
        } else {
            func_80AD76CC(this);
        }
    }
    func_80AD7568(this);
}

void func_80AD7B18(EnKame* this) {
    this->actor.draw = EnKame_Draw;
    Animation_MorphToPlayOnce(&this->skelAnime1, 0x060031dc, -3.0f);
    this->actor.speedXZ = 0.0f;
    this->unk_2AC = 0.1f;
    this->unk_2A8 = 1.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->actionFunc = func_80AD7B90;
}

void func_80AD7B90(EnKame* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime1)) {
        func_80AD71B4(this);
    } else if (this->skelAnime1.curFrame > 7.0f) {
        this->unk_2AC = 1.5f - ((this->skelAnime1.curFrame - 7.0f) * (1.0f / 6));
        this->unk_2A8 = 1.5f - ((this->skelAnime1.curFrame - 7.0f) * (1.0f / 6));
    } else {
        f32 frame = this->skelAnime1.curFrame;

        this->unk_2AC = (0.2f * frame) + 0.1f;
        this->unk_2A8 = ((1.0f / 14) * frame) + 1.0f;
    }
}

void EnKame_SetupFlipToNormal(EnKame* this) {
        Animation_MorphToPlayOnce(&this->skelAnime1, 0x060035ec, -3.0f);
        this->unk_29E = 1;
        this->collider.info.bumper.dmgFlags &= ~0x8000;

    this->actor.draw = EnKame_Draw;
    this->actor.speedXZ = 0.0f;
    this->collider.base.acFlags &= ~AC_ON;
    this->collider.base.atFlags &= ~AT_ON;
    this->collider.base.atFlags &= ~(AT_BOUNCED | AT_HIT);
    this->actor.flags &= ~ACTOR_FLAG_4;
    this->actor.shape.rot.z = 0;
    //Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_REVERSE);
    SoundEffect_PlayOneshot(SOUND_ENEMY_SNAPPER_FLIP, 1.0f, 1.0f, &this->actor.world.pos, 0.1f, 1000.0f, &this->actor);
    this->actionFunc = func_80AD7D40;
}

void EnKame_SetupFlipOnBack(EnKame* this) {
    Animation_MorphToPlayOnce(&this->skelAnime1, 0x060039c0, -3.0f);
    this->unk_29E = 0;
    this->collider.info.bumper.dmgFlags |= 0x8000;


    this->actor.draw = EnKame_Draw;
    this->actor.speedXZ = 0.0f;
    this->collider.base.acFlags &= ~AC_ON;
    this->collider.base.atFlags &= ~AT_ON;
    this->collider.base.atFlags &= ~(AT_BOUNCED | AT_HIT);
    this->actor.flags &= ~ACTOR_FLAG_4;
    this->actor.shape.rot.z = 0;
    //Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_REVERSE);
    SoundEffect_PlayOneshot(SOUND_ENEMY_SNAPPER_FLIP, 1.0f, 1.0f, &this->actor.world.pos, 0.1f, 1000.0f, &this->actor);
    this->actionFunc = func_80AD7D40;
}

void EnKame_SetupFlip(EnKame* this) {
    if (this->actionFunc == func_80AD7E0C) {
        Animation_MorphToPlayOnce(&this->skelAnime1, 0x060035ec, -3.0f);
        this->unk_29E = 1;
        this->collider.info.bumper.dmgFlags &= ~0x8000;
    } else {
        Animation_MorphToPlayOnce(&this->skelAnime1, 0x060039c0, -3.0f);
        this->unk_29E = 0;
        this->collider.info.bumper.dmgFlags |= 0x8000;
    }

    this->actor.draw = EnKame_Draw;
    this->actor.speedXZ = 0.0f;
    this->collider.base.acFlags &= ~AC_ON;
    this->collider.base.atFlags &= ~AT_ON;
    this->collider.base.atFlags &= ~(AT_BOUNCED | AT_HIT);
    this->actor.flags &= ~ACTOR_FLAG_4;
    this->actor.shape.rot.z = 0;
    //Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_REVERSE);
    SoundEffect_PlayOneshot(SOUND_ENEMY_SNAPPER_FLIP, 1.0f, 1.0f, &this->actor.world.pos, 0.1f, 1000.0f, &this->actor);
    this->actionFunc = func_80AD7D40;
}

void func_80AD7D40(EnKame* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime1)) {
        if (this->unk_29E == 1) {
            func_80AD71B4(this);
        } else {
            this->unk_29E = 200;
            func_80AD7DA4(this);
        }
    }
}

void func_80AD7DA4(EnKame* this) {
    Animation_MorphToPlayOnce(&this->skelAnime1, 0x060027d8, -3.0f);
    this->collider.base.acFlags |= AC_ON;
    this->collider.base.acFlags &= ~AC_HARD;
    this->collider.base.colType = COLTYPE_HIT6;
    this->actor.speedXZ = 0.0f;
    this->actionFunc = func_80AD7E0C;
}

void func_80AD7E0C(EnKame* this, PlayState* play) {
    if (this->unk_29E > 0) {
        this->unk_29E--;
        if (SkelAnime_Update(&this->skelAnime1)) {
            if (Rand_ZeroOne() > 0.5f) {
                Animation_PlayOnce(&this->skelAnime1, 0x060027d8);
            } else {
                Animation_PlayOnce(&this->skelAnime1, 0x06002f88);
                //Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_ROAR);
                SoundEffect_PlayOneshot(SOUND_ENEMY_SNAPPER_STRUGGLE, 1.0f, 1.0f, &this->actor.world.pos, 0.1f, 1000.0f, &this->actor);
            }
        }
    } else {
        func_80AD7EC0(this);
    }
}

void func_80AD7EC0(EnKame* this) {
    Animation_MorphToPlayOnce(&this->skelAnime1, 0x06002510, -3.0f);
    //Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_WAKEUP);
    SoundEffect_PlayOneshot(SOUND_ENEMY_SNAPPER_RECOVER, 1.0f, 1.0f, &this->actor.world.pos, 0.1f, 1000.0f, &this->actor);
    this->actionFunc = func_80AD7F10;
}

void func_80AD7F10(EnKame* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime1)) {
        this->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        func_80AD71B4(this);
    } else if (this->skelAnime1.curFrame >= 10.0f) {
        this->actor.shape.shadowDraw = NULL;
        this->collider.base.acFlags &= ~AC_ON;
        this->collider.info.bumper.dmgFlags &= ~0x8000;
    }
}

void func_80AD7FA4(EnKame* this) {
    this->actor.speedXZ = 0.0f;
    if (this->actor.velocity.y > 0.0f) {
        this->actor.velocity.y = 0.0f;
    }
    Collider_UpdateCylinder(&this->actor, &this->collider);
    this->actionFunc = func_80AD7FF8;
}

void func_80AD7FF8(EnKame* this, PlayState* play) {
    if (this->unk_2A2 != 0) {
        this->unk_2A2--;
    }

    if (this->unk_2A2 == 0) {
        func_80AD7018(this, play);
        if (this->actor.colChkInfo.health == 0) {
            EnKame_SetupDie(this, NULL);
        } else {
            this->actor.world.rot.y = this->actor.shape.rot.y;
            func_80AD7DA4(this);
        }
    }
}

void EnKame_SetupDamage(EnKame* this) {
    s16 lastFrame = Animation_GetLastFrame(0x060008b4);

    Animation_Change(&this->skelAnime1, 0x060008b4, 1.0f, 0.0f, lastFrame, ANIMMODE_ONCE, -3.0f);
    Actor_SetColorFilter(&this->actor, 0x4000, 255, 0x0000, lastFrame);
    //Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_DAMAGE);
    SoundEffect_PlayOneshot(SOUND_ENEMY_SNAPPER_HIT, 1.0f, 1.0f, &this->actor.world.pos, 0.1f, 1000.0f, &this->actor);
    this->collider.base.acFlags &= ~AC_ON;
    this->actionFunc = func_80AD810C;
    this->flipTimer = 20;
}

void func_80AD810C(EnKame* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime1)) {
        func_80AD7DA4(this);
    }
}

void EnKame_SetupDie(EnKame* this, PlayState* play) {
    Animation_PlayLoop(&this->skelAnime1, 0x06000af4);
    Actor_SetColorFilter(&this->actor, 0x4000, 255, 0x0000, 20);
    this->collider.base.acFlags &= ~AC_ON;
    this->collider.base.atFlags &= ~AT_ON;
    this->collider.base.atFlags &= ~(AC_HARD | AC_HIT);
    this->actor.velocity.y = 15.0f;
    this->actor.speedXZ = 1.5f;
    if (play != NULL) {
        Enemy_StartFinishingBlow(play, &this->actor);
        if (this->actor.draw == func_80AD8D64) {
            this->actor.draw = EnKame_Draw;
        } else {
            Collider_UpdateCylinder(&this->actor, &this->collider);
        }
    }
    if (this->iceTimer != 0) {
    Actor_SpawnIceEffects(play, &this->actor, this->limbPos, 10, 2, 0.3f, 0.2f);
    }
    this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
    this->actor.flags &= ~ACTOR_FLAG_0;
    this->actor.flags |= ACTOR_FLAG_4;
    //Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_DEAD);
    SoundEffect_PlayOneshot(SOUND_ENEMY_SNAPPER_DIE, 1.0f, 1.0f, &this->actor.world.pos, 0.1f, 1000.0f, &this->actor);
    this->unk_29E = 0;
    this->actionFunc = func_80AD825C;
    this->flipTimer = 60;
}

void func_80AD825C(EnKame* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime1);
    if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && (this->actor.velocity.y < 0.0f)) {
        Audio_PlayActorSfx2(&this->actor, NA_SE_EN_DODO_M_GND);
        func_80AD8364(this);
    } else {
        if (this->unk_29E == 1) {
            this->actor.colorFilterTimer = 100;
        } else if (this->actor.colorFilterTimer == 0) {
            Actor_SetColorFilter(&this->actor, 0x0000, 0, 0x0000, 0);
        }
        this->actor.shape.rot.x += Rand_S16Offset(0x700, 0x1400);
        this->actor.shape.rot.y += (s16)Rand_ZeroFloat(5120.0f);
        this->actor.shape.rot.z += Rand_S16Offset(0x700, 0x1400);
    }
}

void func_80AD8364(EnKame* this) {
    this->unk_29E = 20;
    this->actor.speedXZ = 0.0f;
    this->actionFunc = func_80AD8388;
}

void func_80AD8388(EnKame* this, PlayState* play) {
    Vec3f sp34;
    Vec3f gZeroVec3f = {0};

    SkelAnime_Update(&this->skelAnime1);
    this->actor.colorFilterTimer = 100;
    if (this->unk_29E > 0) {
        this->unk_29E--;
        if (this->unk_29E == 0) {
            SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 21,
                                                        NA_SE_EN_EXTINCT - SFX_FLAG);
        }
    } else {
        this->actor.scale.x -= 0.001f;
        if (this->actor.scale.x <= 0.0f) {
            Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0x60);
            Actor_Kill(&this->actor);
        } else {
            this->actor.scale.y = this->actor.scale.x;
            this->actor.scale.z = this->actor.scale.x;
        }
        sp34.x = Rand_CenteredFloat(40.0f) + this->actor.world.pos.x;
        sp34.y = this->actor.world.pos.y + 15.0f;
        sp34.z = Rand_CenteredFloat(40.0f) + this->actor.world.pos.z;
        EffectSsDeadDb_Spawn(play, &sp34, &gZeroVec3f, &gZeroVec3f, 100, 0, 255, 255, 255, 255, 0, 0, 255, 1, 9,
                                 true);
    }
}

void func_80AD84C0(EnKame* this, PlayState* play) {
    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;

        Actor_SetDropFlag(&this->actor, &this->collider.info, true);
        if ((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) &&
            (this->collider.info.acHitInfo->toucher.dmgFlags & 0xDB0B3)) {
            return;
        }

        func_80AD7018(this, play);
        if (this->actor.colChkInfo.damageEffect == 13) {
            return;
        }

        if (this->actor.colChkInfo.damageEffect == 14) {
            EnKame_SetupDie(this, play);
        } else if (this->actor.colChkInfo.damageEffect == 15) {
            if (this->collider.base.acFlags & AC_HARD) {
                EnKame_SetupFlip(this);
            } else if (!Actor_ApplyDamage(&this->actor)) {
                EnKame_SetupDie(this, play);
            } else {
                EnKame_SetupDamage(this);
            }
        } else if ((this->actionFunc == func_80AD70EC) || (this->actionFunc == func_80AD7254)) {
            func_80AD73A8(this);
            this->unk_29E = 21;
        } else if (!(this->collider.base.acFlags & AC_HARD)) {
            if (this->actor.colChkInfo.damageEffect == 5) {
                this->unk_2A2 = 40;
                Actor_SetColorFilter(&this->actor, 0x0000, 255, 0x0000, 40);
                Audio_PlayActorSfx2(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
                this->drawDmgEffScale = 0.6f;
                this->drawDmgEffAlpha = 2.0f;
                this->drawDmgEffType = ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_SMALL;
                func_80AD7FA4(this);
            } else if (this->actor.colChkInfo.damageEffect == 0x1) {
                this->unk_2A2 = 40;
                Actor_SetColorFilter(&this->actor, 0x0000, 255, 0x0000, 40);
                Audio_PlayActorSfx2(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
                //this->iceTimer = 32;
                //SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 30, NA_SE_EV_ICE_FREEZE);
                func_80AD7FA4(this);
            } else if (this->actor.colChkInfo.damageEffect == 3) {
                func_80AD6F9C(this);
                if (!Actor_ApplyDamage(&this->actor)) {
                    this->unk_2A2 = 3;
                    this->collider.base.acFlags &= ~AC_ON;
                }
                func_80AD7FA4(this);
            } else {
                if (this->actor.colChkInfo.damageEffect == 2) {
                    this->drawDmgEffScale = 0.6f;
                    this->drawDmgEffAlpha = 4.0f;
                    this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
                } else if (this->actor.colChkInfo.damageEffect == 4) {
                    this->drawDmgEffScale = 0.6f;
                    this->drawDmgEffAlpha = 4.0f;
                    this->drawDmgEffType = ACTOR_DRAW_DMGEFF_LIGHT_ORBS;
                    Actor_Spawn(&play->actorCtx, play, 0x033B, this->collider.info.bumper.hitPos.x,
                                this->collider.info.bumper.hitPos.y, this->collider.info.bumper.hitPos.z, 0, 0, 0,
                                4);
                } else if (this->actor.colChkInfo.damageEffect == 6) {
                    this->drawDmgEffScale = 0.6f;
                    this->drawDmgEffAlpha = 4.0f;
                    this->drawDmgEffType = ACTOR_DRAW_DMGEFF_BLUE_LIGHT_ORBS;
                    Actor_Spawn(&play->actorCtx, play, 0x033B, this->collider.info.bumper.hitPos.x,
                                this->collider.info.bumper.hitPos.y, this->collider.info.bumper.hitPos.z, 0, 0, 0,
                                4);
                }

                if (!Actor_ApplyDamage(&this->actor)) {
                    EnKame_SetupDie(this, play);
                } else {
                    EnKame_SetupDamage(this);
                }
            }
        }
    }

    if (this->flipTimer == 0 && (play->actorCtx.unk_02 != 0) && (this->actor.xzDistToPlayer <= 400.0f) &&
            (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        func_80AD7018(this, play);
        if (this->actionFunc == func_80AD7E0C) {
        EnKame_SetupFlipToNormal(this);
        } else {
        EnKame_SetupFlipOnBack(this);
        }
        this->flipTimer = 20;
    }
}

void EnKame_Update(Actor* thisx, PlayState* play) {
    s32 pad;
    EnKame* this = THIS;
    u32 floorProperty;
    CollisionPoly* poly;
    f32 ray;
    s32 bgID;

    if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND_LEAVE)) {
        if ((ray = BgCheck_EntityRaycastDown3(&play->colCtx, &poly, &bgID, &this->actor.world.pos)) != BGCHECK_Y_MIN && poly) {
            u32 surface = SurfaceType_GetFloorProperty(&play->colCtx, poly, bgID);
            
            if (surface == FLOOR_PROPERTY_5 || surface == FLOOR_PROPERTY_12 ||
                SurfaceType_GetFloorType(&play->colCtx, poly, bgID) == FLOOR_TYPE_9) {
                Audio_PlayActorSfx2(&this->actor, NA_SE_EV_OBJECT_FALL);
                // void below, danger!
            }
        }
    }


    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
        floorProperty = SurfaceType_GetFloorProperty(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId);

        if ((floorProperty == FLOOR_PROPERTY_5) || (floorProperty == FLOOR_PROPERTY_12) ||
            SurfaceType_GetFloorType(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId) == FLOOR_TYPE_9) {
            if (this->actor.colChkInfo.health != 0) {
                EnKame_SetupDie(this, play);
                this->actor.colChkInfo.health = 0;
                //Actor_Kill(&this->actor);
                Enemy_StartFinishingBlow(play, &this->actor);
            }
            //return;
        }
    }


    func_80AD6F34(this);

    if (this->unk_2A0 != 0) {
        this->unk_2A0--;
    }

    if (this->flipTimer != 0)
        this->flipTimer -= 1;

    func_80AD84C0(this, play);

    if ((this->collider.base.atFlags & AT_HIT) && (this->collider.base.atFlags & AT_BOUNCED)) {
        this->collider.base.atFlags &= ~(AT_BOUNCED | AT_HIT);
        func_80AD76CC(this);
        if (Actor_WorldDistXZToPoint(&this->actor, &this->unk_2BC) < 50.0f) {
            this->collider.base.atFlags &= ~AT_ON;
        }
        this->unk_2A6 = 0x3B00;
        func_80AD7568(this);
    }

    this->actionFunc(this, play);

    Actor_MoveForward(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 40.0f, 60.0f, 40.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2 |
                                UPDBGCHECKINFO_FLAG_3 | UPDBGCHECKINFO_FLAG_4);

    if (this->actor.shape.shadowDraw != NULL) {
        Actor_SetFocus(&this->actor, 25.0f);
        Collider_UpdateCylinder(&this->actor, &this->collider);
    }

//    if (this->collider.base.atFlags & AT_ON) {
//        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
//    }

//    if (this->collider.base.acFlags & AC_ON) {
//        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
//    }

    CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);

    if (this->drawDmgEffAlpha > 0.0f) {
        if (this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) {
            Math_StepToF(&this->drawDmgEffAlpha, 0.0f, 0.05f);
            this->drawDmgEffScale = (this->drawDmgEffAlpha + 1.0f) * 0.3f;
            if (this->drawDmgEffScale > 0.6f) {
                this->drawDmgEffScale = 0.6f;
            } else {
                this->drawDmgEffScale = this->drawDmgEffScale;
            }
        } else if (!Math_StepToF(&this->drawDmgEffFrozenSteamScale, 0.6f, 0.015000001f)) {
            func_8002F974(&this->actor, NA_SE_EV_ICE_FREEZE - SFX_FLAG);
        }
    }
}

s32 func_80AD8A48(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnKame* this = THIS;

    if ((this->actionFunc == func_80AD7424) || (this->actionFunc == func_80AD7B90)) {
        if (limbIndex == 2) {
            Matrix_Scale(this->unk_2A8, this->unk_2AC, this->unk_2A8, MTXMODE_APPLY);
        } else if ((limbIndex == 11) || (limbIndex == 9) || (limbIndex == 7) || (limbIndex == 5)) {
            Matrix_Scale(this->unk_2A8, this->unk_2AC, this->unk_2AC, MTXMODE_APPLY);
        }
    }
    return false;
}

void func_80AD8AF8(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    static Vec3f D_80AD8E68[] = {
        { 1500.0f, 0.0f, -2000.0f }, { 1500.0f, 0.0f, 2000.0f }, { 1500.0f, 2000.0f, 0.0f },
        { 1500.0f, -2000.0f, 0.0f }, { 2500.0f, 0.0f, 0.0f },
    };
    static s8 D_80AD8EA4[] = { -1, -1, -1, 0, -1, -1, 1, -1, 2, -1, 3, -1, 4 };
    EnKame* this = THIS;

    if (D_80AD8EA4[limbIndex] != -1) {
        Matrix_MultZero(&this->limbPos[D_80AD8EA4[limbIndex]]);
    }

    if (limbIndex == 1) {
        s32 i;
        Vec3f* ptr;
        Vec3f* ptr2;

        if (this->actor.shape.shadowDraw == NULL) {
            Matrix_MultZero(&this->actor.world.pos);
        }

        ptr2 = D_80AD8E68;
        ptr = &this->limbPos[5];
        for (i = 0; i < ARRAY_COUNT(D_80AD8E68); i++) {
            Matrix_MultVec3f(ptr2, ptr);
            ptr2++;
            ptr++;
        }
    }
}

void EnKame_Draw(Actor* thisx, PlayState* play) {
    EnKame* this = THIS;
    Vec3f sp40;
    Vec3f icePos;
    s32 iceIndex;

    if (this->actor.shape.shadowDraw == 0) {
        Math_Vec3f_Copy(&sp40, &this->actor.world.pos);
    }

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyeTextures[this->unk_29C]));

    SkelAnime_DrawFlexOpa(play, this->skelAnime1.skeleton, this->skelAnime1.jointTable, this->skelAnime1.dListCount,
                          func_80AD8A48, func_80AD8AF8, &this->actor);
    Actor_DrawDamageEffects(play, &this->actor, this->limbPos, ARRAY_COUNT(this->limbPos), this->drawDmgEffScale,
                            this->drawDmgEffFrozenSteamScale, this->drawDmgEffAlpha, this->drawDmgEffType);

    if (this->actor.shape.shadowDraw == NULL) {
        ActorShadow_DrawCircle(&this->actor, NULL, play);
        Actor_SetFocus(&this->actor, 25.0f);
        Collider_UpdateCylinder(&this->actor, &this->collider);
        Math_Vec3f_Copy(&this->actor.world.pos, &sp40);
    }

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    if (this->iceTimer != 0) {
        thisx->colorFilterTimer++;
        this->iceTimer--;
        if ((this->iceTimer & 3) == 0) {
            iceIndex = this->iceTimer >> 2;

            icePos.x = sIceChunks[iceIndex].x + thisx->world.pos.x;
            icePos.y = sIceChunks[iceIndex].y + thisx->world.pos.y;
            icePos.z = sIceChunks[iceIndex].z + thisx->world.pos.z;
            EffectSsEnIce_SpawnFlyingVec3f(play, thisx, &icePos, 0x96, 0x96, 0x96, 0xFA, 0xEB, 0xF5, 0xFF, 1.3f);
        }
    }
}

s32 Enkame_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnKame* this = THIS;

    if (limbIndex == 1) {
        pos->y -= 700.0f;
    }

    if ((this->unk_2AC != 1.0f) && (limbIndex == 3)) {
        Matrix_Scale(1.0f, this->unk_2AC, this->unk_2AC, MTXMODE_APPLY);
    }
    return false;
}

void func_80AD8D64(Actor* thisx, PlayState* play) {
    EnKame* this = THIS;

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    SkelAnime_DrawFlexOpa(play, this->skelAnime2.skeleton, this->skelAnime2.jointTable, this->skelAnime2.dListCount,
                          Enkame_OverrideLimbDraw, NULL, &this->actor);
}
