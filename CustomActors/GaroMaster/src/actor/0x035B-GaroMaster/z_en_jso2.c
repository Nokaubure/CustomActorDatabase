/*
 * File: z_en_jso2.c
 * Overlay: ovl_En_Jso2
 * Description: Garo Master
 */

#include "z_en_jso2.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "mm/requirements.h"
#include <uLib.h>


#define FLAGS                                                                                            \
    (ACTOR_FLAG_0 | ACTOR_FLAG_2 | ACTOR_FLAG_4 | ACTOR_FLAG_5 | ACTOR_FLAG_20 | \
     ACTOR_FLAG_31)

#define THIS ((EnJso2*)thisx)

#define ACT_ID 0x035B
#define ACT2_ID 0x035C
#define OBJ_ID 0x022E

void EnJso2_Init(Actor* thisx, PlayState* play);
void EnJso2_Destroy(Actor* thisx, PlayState* play);
void EnJso2_Update(Actor* thisx, PlayState* play);
void EnJso2_Draw(Actor* thisx, PlayState* play2);

void func_80A776E0(EnJso2* this, s32 animIndex);
void func_80A778D8(EnJso2* this);
void func_80A778F8(EnJso2* this, PlayState* play);
void func_80A78588(EnJso2* this);
void func_80A785E4(EnJso2* this, PlayState* play);
void func_80A787FC(EnJso2* this, PlayState* play);
void func_80A78868(EnJso2* this, PlayState* play);
void func_80A78ACC(EnJso2* this, PlayState* play);
void func_80A78B04(EnJso2* this);
void func_80A78B70(EnJso2* this, PlayState* play);
void func_80A78C08(EnJso2* this);
void func_80A78C7C(EnJso2* this, PlayState* play);
void func_80A78E8C(EnJso2* this);
void func_80A78F04(EnJso2* this, PlayState* play);
void func_80A78F80(EnJso2* this, PlayState* play);
void func_80A79038(EnJso2* this, PlayState* play);
void func_80A790E4(EnJso2* this, PlayState* play);
void func_80A7919C(EnJso2* this, PlayState* play);
void func_80A79300(EnJso2* this);
void func_80A79364(EnJso2* this, PlayState* play);
void func_80A79450(EnJso2* this);
void func_80A794C8(EnJso2* this, PlayState* play);
void func_80A79600(EnJso2* this, PlayState* play);
void func_80A7980C(EnJso2* this, PlayState* play);
void func_80A798C8(EnJso2* this, PlayState* play);
void func_80A79A84(EnJso2* this, PlayState* play);
void func_80A79BA0(EnJso2* this, PlayState* play);
void func_80A79864(EnJso2* this);
void func_80A79B60(EnJso2* this);
void func_80A7A0D0(EnJso2* this);
void func_80A7A124(EnJso2* this, PlayState* play);
void func_80A7A2EC(EnJso2* this, PlayState* play);
s32 func_80A7AA48(PlayState* play, s32 arg1, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx);
void func_80A7AA9C(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx);

// static DamageTable sDamageTable = {
static DamageTable D_80A7B4F0 = {
    /* Deku Nut       */ DMG_ENTRY(0, 0x1),
    /* Deku Stick     */ DMG_ENTRY(1, 0xF),
    /* Slingshot      */ DMG_ENTRY(1, 0xF),
    /* Explosives     */ DMG_ENTRY(1, 0xF),
    /* Boomerang      */ DMG_ENTRY(1, 0xF),
    /* Normal arrow   */ DMG_ENTRY(1, 0xF),
    /* Hammer Swing   */ DMG_ENTRY(4, 0xF),
    /* Hookshot       */ DMG_ENTRY(0, 0x1),
    /* Kokiri Sword   */ DMG_ENTRY(1, 0xF),
    /* Master Sword   */ DMG_ENTRY(2, 0xF),
    /* Giant's Knife  */ DMG_ENTRY(4, 0xF),
    /* Fire arrow     */ DMG_ENTRY(2, 0x2),
    /* Ice arrow      */ DMG_ENTRY(2, 0x3),
    /* Light arrow    */ DMG_ENTRY(2, 0x4),
    /* Unk arrow 1    */ DMG_ENTRY(2, 0xF),
    /* Unk arrow 1    */ DMG_ENTRY(2, 0xF),
    /* Unk arrow 1    */ DMG_ENTRY(2, 0xF),
    /* Fire magic     */ DMG_ENTRY(2, 0x2),
    /* Ice magic      */ DMG_ENTRY(2, 0x3),
    /* Light magic    */ DMG_ENTRY(2, 0x6),
    /* Normal shield  */ DMG_ENTRY(0, 0x0),
    /* Light ray      */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin    */ DMG_ENTRY(2, 0xF),
    /* Giant spin     */ DMG_ENTRY(8, 0xF),
    /* Master spin    */ DMG_ENTRY(4, 0xF),
    /* Kokiri jump    */ DMG_ENTRY(2, 0xF),
    /* Giant jump     */ DMG_ENTRY(8, 0xF),
    /* Master jump    */ DMG_ENTRY(4, 0xF),
    /* Unknown 1      */ DMG_ENTRY(0, 0x0),
    /* Unblockable    */ DMG_ENTRY(0, 0x0),
    /* Hammer jump    */ DMG_ENTRY(4, 0xF),
    /* Powder Keg     */ DMG_ENTRY(8, 0xF),
};

Vec3f D_80A7B510[] = {
    { 0.003f, 0.003f, 0.003f }, { 0.002f, 0.002f, 0.002f }, { 0.001f, 0.001f, 0.001f },
    { 0.003f, 0.003f, 0.003f }, { 0.002f, 0.002f, 0.002f }, { 0.001f, 0.001f, 0.001f },
};

static Vec3f D_80A7B558 = { 800.0f, -20.0f, -50.0f };
static Vec3f D_80A7B564 = { 500.0f, -20.0f, -70.0f };
static Vec3f D_80A7B570 = { 300.0f, -20.0f, -90.0f };
static Vec3f D_80A7B57C = { 800.0f, -20.0f, 50.0f };
static Vec3f D_80A7B588 = { 500.0f, -20.0f, 70.0f };
static Vec3f D_80A7B594 = { 300.0f, -20.0f, 90.0f };
static Vec3f D_80A7B5A0 = { 600.0f, -100.0f, 100.0f };
static Vec3f D_80A7B5AC = { 300.0f, -100.0f, 80.0f };
static Vec3f D_80A7B5B8 = { 100.0f, -100.0f, 60.0f };
static Vec3f D_80A7B5C4 = { 600.0f, -100.0f, -100.0f };
static Vec3f D_80A7B5D0 = { 300.0f, -100.0f, -80.0f };
static Vec3f D_80A7B5DC = { 100.0f, -100.0f, -60.0f };

ActorInit En_Jso2_InitVars = {
    /**/ ACT_ID,
    /**/ ACTORCAT_ENEMY,
    /**/ FLAGS,
    /**/ OBJ_ID,
    /**/ sizeof(EnJso2),
    /**/ EnJso2_Init,
    /**/ EnJso2_Destroy,
    /**/ EnJso2_Update,
    /**/ EnJso2_Draw,
};

// static ColliderCylinderInit sCylinderInit = {
static ColliderCylinderInit D_80A7B608 = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x08, 0x04 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 22, 70, 0, { 0, 0, 0 } },
};

// static ColliderQuadInit sQuadInit = {
static ColliderQuadInit D_80A7B634 = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x01, 0x10 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL | TOUCH_UNK7,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

static AnimationHeader* D_80A7B684[0x16] = {
    0x06003530,  0x0600378c,
    0x060038ac,  0x06003d5c,
    0x060044f8,  0x06004858,
    0x060081f4,  0x0600ae80,
    0x0600b5f0,  0x06005d5c,
    0x0600603c,  0x06007b04,
    0x0600788c,  0x06005778,
    0x060072ac,  0x06004384,
    0x060071e0,  0x0600b1dc,
    0x060070bc,  0x060067f0,
    0x060063a4,  0x06004018,
};

static u8 D_80A7B6DC[] = { 2, 2, 0, 2, 0, 2, 0, 0, 0, 2, 2, 0, 0, 2, 0, 2, 2, 0, 2, 2, 0, 2 };

static Vec3s D_80A7B6F4 = { 350, -20, -3430 };

static Vec3f D_80A7B6FC = { 0.0f, 0.0f, 0.0f };

static Vec3f D_80A7B708 = { 1600.0f, 0.0f, 0.0f };

static Vec3f D_80A7B714 = { 0.0f, 0.0f, 0.0f };

static Vec3f D_80A7B720 = { 1700.0f, 0.0f, 0.0f };

static Vec3f D_80A7B72C = { 0.0f, 0.0f, 0.0f };

static s16 D_80A7B738[] = { 128, 0, 0, 0, 0, 128, 0, 0, 0, 0, 128, 0, 0, 0, 0, 128 };

void EnJso2_Init(Actor* thisx, PlayState* play) {
    EnJso2* this = THIS;
    EffectBlureInit1 rightSwordBlureInit;
    EffectBlureInit1 leftSwordBlureInit;

    this->actor.naviEnemyId = 0x6E;
    this->actor.targetMode = 5;
    this->actor.colChkInfo.mass = 0x50;
    this->actor.colChkInfo.health = 0xE;
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
    this->actor.colChkInfo.damageTable = &D_80A7B4F0;
    this->actor.shape.shadowScale = 0.0f;
    SkelAnime_InitFlex(play, &this->skelAnime, 0x06003168, 0x060081f4, this->jointTable, this->morphTable,
                       20);
    Collider_InitCylinder(play, &this->unkEF4);
    Collider_InitQuad(play, &this->unkF40);
    Collider_InitQuad(play, &this->unkFC0);
    Collider_SetCylinder(play, &this->unkEF4, &this->actor, &D_80A7B608);
    Collider_SetQuad(play, &this->unkF40, &this->actor, &D_80A7B634);
    Collider_SetQuad(play, &this->unkFC0, &this->actor, &D_80A7B634);

    rightSwordBlureInit.p1StartColor[0] = leftSwordBlureInit.p1StartColor[0] = 255;
    rightSwordBlureInit.p1StartColor[1] = leftSwordBlureInit.p1StartColor[1] = 0;
    rightSwordBlureInit.p1StartColor[2] = leftSwordBlureInit.p1StartColor[2] = 0;
    rightSwordBlureInit.p1StartColor[3] = leftSwordBlureInit.p1StartColor[3] = 128;

    rightSwordBlureInit.p2StartColor[0] = leftSwordBlureInit.p2StartColor[0] = 255;
    rightSwordBlureInit.p2StartColor[1] = leftSwordBlureInit.p2StartColor[1] = 0;
    rightSwordBlureInit.p2StartColor[2] = leftSwordBlureInit.p2StartColor[2] = 0;

    rightSwordBlureInit.p1EndColor[0] = leftSwordBlureInit.p1EndColor[0] = 255;
    rightSwordBlureInit.p1EndColor[1] = leftSwordBlureInit.p1EndColor[1] = 0;
    rightSwordBlureInit.p1EndColor[2] = leftSwordBlureInit.p1EndColor[2] = 0;

    rightSwordBlureInit.p2EndColor[0] = leftSwordBlureInit.p2EndColor[0] = 255;
    rightSwordBlureInit.p2EndColor[1] = leftSwordBlureInit.p2EndColor[1] = 0;
    rightSwordBlureInit.p2EndColor[2] = leftSwordBlureInit.p2EndColor[2] = 255;

    // clang-format off
    rightSwordBlureInit.p2StartColor[3] = leftSwordBlureInit.p2StartColor[3] = 64;
    rightSwordBlureInit.p1EndColor[3] = leftSwordBlureInit.p1EndColor[3] = 
    rightSwordBlureInit.p2EndColor[3] = leftSwordBlureInit.p2EndColor[3] = 0;
    // clang-format on

    rightSwordBlureInit.elemDuration = leftSwordBlureInit.elemDuration = 8;

    rightSwordBlureInit.unkFlag = leftSwordBlureInit.unkFlag = 0;

    rightSwordBlureInit.calcMode = leftSwordBlureInit.calcMode = 2;

    Effect_Add(play, &this->unk_380, 1, 0, 0, &rightSwordBlureInit);
    Effect_Add(play, &this->unk_384, 1, 0, 0, &leftSwordBlureInit);

    this->unk378 = 0.042f;
    this->unk29C = this->actor.params;
    this->unk_38E = Rand_S16Offset(0, 7);
    this->unk_2B0 = 0xC;
    this->unk366 = 0xFF;

    if (this->unk29C == 0) {
        this->actor.draw = NULL;
        this->actor.flags |= 0x08000000;
        this->actor.flags &= ~1;
        this->actor.shape.yOffset = 0.0f;
        func_80A778D8(this);
    } else {
        this->actor.gravity = -3.0f;
        this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        this->actor.shape.yOffset = 960.0f;
        func_80A78588(this);
    }
}

void EnJso2_Destroy(Actor* thisx, PlayState* play) {
    EnJso2* this = THIS;

    Collider_DestroyCylinder(play, &this->unkEF4);
    Collider_DestroyQuad(play, &this->unkF40);
    Collider_DestroyQuad(play, &this->unkFC0);
    Effect_Delete(play, this->unk_380);
    Effect_Delete(play, this->unk_384);
    func_800F483C(127, 0);
    func_800F5B58();
    //Audio_RestorePrevBgm();
}

void func_80A776E0(EnJso2* this, s32 animIndex) {
    f32 morphFrames;

    this->unk374 = Animation_GetLastFrame(D_80A7B684[animIndex]);
    this->unk1040 = animIndex;
    morphFrames = 1.0f;

    if (animIndex == 8) {
        morphFrames = 2.0f;
    }
    Animation_Change(&this->skelAnime, D_80A7B684[animIndex], morphFrames, 0.0f, this->unk374, D_80A7B6DC[animIndex],
                     -2.0f);
}

void func_80A77790(EnJso2* this, PlayState* play) {
    if (this->unk1048 != 0) {
        Math_ApproachF(&this->unk1054.x, this->unk1078.x, this->unk294, this->unk298);
        Math_ApproachF(&this->unk1054.y, this->unk1078.y, this->unk294, this->unk298);
        Math_ApproachF(&this->unk1054.z, this->unk1078.z, this->unk294, this->unk298);
        Math_ApproachF(&this->unk1060.x, this->unk1084.x, this->unk294, this->unk298);
        Math_ApproachF(&this->unk1060.y, this->unk1084.y, this->unk294, this->unk298);
        Math_ApproachF(&this->unk1060.z, this->unk1084.z, this->unk294, this->unk298);
        Math_ApproachF(&this->unk104C, this->unk1050, 0.3f, 10.0f);
        Play_CameraSetAtEye(play, this->unk1048, &this->unk1060, &this->unk1054);
        Play_CameraSetFov(play, this->unk1048, this->unk104C);
    }
}

s32 func_80A77880(PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 5) && (Message_ShouldAdvance(play) != 0)) {
        Message_CloseTextbox(play);
        return 1;
    } else {
        return 0;
    }
}

void func_80A778D8(EnJso2* this) {
    this->unk284 = 0;
    this->unk36C = 2;
    this->actionFunc = func_80A778F8;
}

void func_80A778F8(EnJso2* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 curFrame = this->skelAnime.curFrame;
    Vec3f spawnPos;
    s32 i;

    switch (this->unk1046) {
        case 0:
            if (this->actor.xzDistToPlayer < 400.0f) {
                func_800F483C(0, 10);
                this->unk1046++;
                this->actor.gravity = 0.0f;
            }
            break;

        case 1:
            break;

        case 2:
            this->actor.world.pos.x = -285.0f;
            this->actor.world.pos.y = 535.0f;
            this->actor.world.pos.z = -3425.0f;

            this->unk1078.x = 82.0f;
            this->unk1078.y = -516.0f;
            this->unk1078.z = 800.0f;

            Math_Vec3f_Copy(&spawnPos, &this->unk1078);
            OLib_Vec3fAdd(&this->actor.world, &spawnPos, &this->unk1078, 1);

            this->unk1084.x = this->actor.world.pos.x - 90.0f;
            this->unk1084.y = this->actor.world.pos.y - 591.0f;
            this->unk1084.z = this->actor.world.pos.z + 100.0f;

            Math_Vec3f_Copy(&this->unk1054, &this->unk1078);
            Math_Vec3f_Copy(&this->unk1060, &this->unk1084);

            /*
            if (func_80122760(play, &this->unk_27C, 60.0f) != 0) {
                func_8002DF54(play, NULL, 19);
                this->unk1044 = 10;
                this->unk1046++;
            }
            */
            break;

        case 3: {
            Vec3f unk1078Copy;
            s32 i;

            if (this->unk1044 == 1) {
                for (i = 0; i < 2; i++) {
                    Math_Vec3f_Copy(&unk1078Copy, &this->actor.home.pos);
                    unk1078Copy.x += Rand_CenteredFloat(80.0f);
                    unk1078Copy.y = this->actor.world.pos.y + (i * 120.0f);
                    unk1078Copy.z += Rand_CenteredFloat(80.0f);
                    Actor_Spawn(&play->actorCtx, play, ACT2_ID, unk1078Copy.x, unk1078Copy.y, unk1078Copy.z, 0,
                                this->actor.world.rot.y, 0, 2);
                }
                this->unk1044 = 25;
                this->unk1046++;
            }
        } break;

        case 4:
            if (this->unk1044 == 0) {
                func_8002DF54(play, NULL, 4);
                this->unk1044 = 20;
                this->unk1046++;
            }
            break;

        case 5:
            if (this->unk1044 == 0) {
                this->unk1044 = 20;
                this->unk1046++;
            }
            break;

        case 6:
            this->unk1078.x = 82.0f;
            this->unk1078.y = -533.0f;
            this->unk1078.z = 800.0f;

            Math_Vec3f_Copy(&spawnPos, &this->unk1078);
            OLib_Vec3fAdd(&this->actor.world, &spawnPos, &this->unk1078, 1);

            this->unk1084.x = this->actor.focus.pos.x - 80.0f;
            this->unk1084.y = this->actor.focus.pos.y - 360.0f;
            this->unk1084.z = this->actor.focus.pos.z + 100.0f;

            if (this->unk1044 == 0) {
                this->unk1044 = 5;
                this->unk1046++;
            }
            break;

        case 7:
            if (this->unk1044 == 0) {
                //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_ANSATSUSYA_LAUGH);
                SoundEffect_PlayOneshot(SOUND_ENEMY_GARO_LAUGH, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor);            
                this->unk1044 = 0x2D;
                this->unk1046++;
            }
            break;

        case 8:
            if (this->unk1044 == 0) {
                this->actor.speedXZ = 3.0f;
                this->actor.velocity.y = 10.0f;
                this->actor.gravity = -1.0f;

                func_80A776E0(this, 16);

                this->unk294 = 0.4f;
                this->unk298 = 40.0f;

                Audio_PlayActorSfx2(&this->actor, NA_SE_EN_PO_ROLL);
                Math_Vec3f_Copy(&this->unk_2B8, &this->unk1054);

                this->unk1044 = 25;
                this->unk1046++;
            }
            break;

        case 9: {
            this->unk_38C++;

            if (this->unk_38C >= 20) {
                this->unk_38C = 0;
            }
            if (this->unk_388 < 19) {
                this->unk_388++;
            }

            Math_Vec3f_Copy(&this->unk_390[this->unk_38C], &this->actor.world.pos);
            Math_Vec3s_Copy(&this->unk_480[this->unk_38C], &this->actor.world.rot);

            this->unk_390[this->unk_38C].y += 40.0f;

            for (i = 0; i < 20; i++) {
                this->unk_4F8[this->unk_38C][i] = this->jointTable[i];
            }

            Math_ApproachF(&this->actor.shape.shadowScale, 17.0f, 0.4f, 4.0f);

            if (this->unk1044 == 0) {
                this->unk1078.x = this->unk_2B8.x - 490.0f;
                this->unk1078.y = this->unk_2B8.y;
                this->unk1078.z = this->unk_2B8.z + 100.0f;
            }

            this->unk1084.x = this->actor.focus.pos.x - 80.0f;
            this->unk1084.y = this->actor.focus.pos.y - 130.0f;
            this->unk1084.z = this->actor.focus.pos.z;

            if (this->actor.bgCheckFlags & 1) {
                //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_ANSATSUSYA_ONGND);
                SoundEffect_PlayOneshot(SOUND_ENEMY_GARO_ON_GROUND, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor);            
                this->actor.gravity = 0.0f;
                this->actor.velocity.y = 0.0f;
                this->actor.speedXZ = 0.0f;
                func_80A776E0(this, 15);
                Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, this->actor.shape.shadowScale, 0xA,
                                         4.0f, 500, 50, 1);
                this->unk1044 = 1;
                Audio_PlayActorSfx2(&this->actor, NA_SE_EN_DODO_M_GND);
                this->unk1046++;
            }
        } break;

        case 10:
            this->unk_38C++;
            if (this->unk_38C >= 0x14) {
                this->unk_38C = 0;
            }
            if (this->unk_388 < 0x13) {
                this->unk_388++;
            }

            Math_Vec3f_Copy(&this->unk_390[this->unk_38C], &this->actor.world.pos);
            Math_Vec3s_Copy(&this->unk_480[this->unk_38C], &this->actor.world.rot);

            this->unk_390[this->unk_38C].y += 40.0f;

            for (i = 0; i < 20; i++) {
                this->unk_4F8[this->unk_38C][i] = this->jointTable[i];
            }

            if (this->unk1044 == 0) {
                this->unk1078.x = this->unk_2B8.x - 518.0f;
                this->unk1078.y = this->unk_2B8.y - 11.0f;
                this->unk1078.z = this->unk_2B8.z + 100.0f;
            }

            this->unk1084.x = this->actor.focus.pos.x + 20.0f;
            this->unk1084.y = this->actor.focus.pos.y - 50.0f;
            this->unk1084.z = this->actor.focus.pos.z;

            if (this->unk374 <= curFrame) {
                this->unk_388 = 0;
                this->unk_38C = 0;
                this->unk1044 = 0x14;
                func_80A776E0(this, 0x12);
                this->unk1046++;
            }
            break;

        case 11:
            if (Animation_OnFrame(&this->skelAnime, 17.0f) != 0) {
                //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_ANSATSUSYA_SWORD);
                SoundEffect_PlayOneshot(SOUND_ENEMY_GARO_SWORD, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor);            
                Audio_PlayActorSfx2(&this->actor, NA_SE_EV_FLAME_IGNITION);
                this->unk36C = 0;
                func_800F483C(127, 0);
                func_800F5ACC(NA_BGM_MINI_BOSS);
            }

            if (this->unk1044 == 0) {
                this->unk1078.x = this->unk_2B8.x - 470.0f;
                this->unk1078.y = this->unk_2B8.y - 10.0f;
                this->unk1078.z = this->unk_2B8.z + 100.0f;
                this->unk1084.x = this->actor.focus.pos.x - 80.0f;
                this->unk1084.y = this->actor.focus.pos.y - 30.0f;
                this->unk1084.z = this->actor.focus.pos.z;

                if (this->unk36C == 0) {
                    for (i = 0; i < ARRAY_COUNT(D_80A7B510); i++) {
                        Math_ApproachF(&this->unk_EAC[i].x, D_80A7B510[i].x, 0.3f, 0.0005f);
                        this->unk_EAC[i].y = this->unk_EAC[i].x;
                        this->unk_EAC[i].z = this->unk_EAC[i].x;
                    }
                }
            }

            if (this->unk374 <= curFrame) {
                this->actor.flags &= ~ACTOR_FLAG_20;
                this->actor.gravity = -3.0f;
                this->actor.flags &= ~ACTOR_FLAG_27;
                this->actor.flags |= ACTOR_FLAG_0;
                func_80A787FC(this, play);
            }
            break;
    }
    this->actor.shape.yOffset = 960.0f;
    func_80A77790(this, play);
}

void func_80A78588(EnJso2* this) {
    this->unk36C = 2;
    this->unkEF4.base.acFlags |= AC_HARD;
    this->actor.flags &= ~(ACTOR_FLAG_20);
    func_80A776E0(this, 13);
    this->actionFunc = func_80A785E4;
}

void func_80A785E4(EnJso2* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;
    s32 i;

    this->actor.world.rot.y = this->actor.shape.rot.y = this->actor.yawTowardsPlayer;

    switch (this->unk1046) {
        case 0:
            Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, this->actor.shape.shadowScale, 1, 8.0f,
                                     500, 10, 1);
            func_800F483C(0, 10);
            SoundEffect_PlayOneshot(SOUND_ENEMY_GARO_APPEAR, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor); 
            this->unk1046++;
            break;

        case 1:
            if (Animation_OnFrame(&this->skelAnime, 18.0f) != 0) {
                Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, this->actor.shape.shadowScale, 1,
                                         8.0f, 500, 10, 1);
            }

            Math_ApproachF(&this->actor.shape.shadowScale, 17.0f, 0.4f, 4.0f);

            if (Animation_OnFrame(&this->skelAnime, 45.0f)) {
                func_800F483C(127, 0);
                func_800F5ACC(NA_BGM_MINI_BOSS);
                SoundEffect_PlayOneshot(SOUND_ENEMY_GARO_SWORD, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor); 
                Audio_PlayActorSfx2(&this->actor, NA_SE_EV_FLAME_IGNITION);
                this->unk36C = 0;
            }

            if (this->unk36C == 0) {
                for (i = 0; i < ARRAY_COUNT(D_80A7B510); i++) {
                    Math_ApproachF(&this->unk_EAC[i].x, D_80A7B510[i].x, 0.3f, 0.0005f);
                    this->unk_EAC[i].y = this->unk_EAC[i].x;
                    this->unk_EAC[i].z = this->unk_EAC[i].x;
                }
            }

            if (curFrame >= this->unk374) {
                func_80A787FC(this, play);
            }
            break;
    }
}

void func_80A787FC(EnJso2* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    func_80A776E0(this, 8);

    this->unk286 = player->actor.shape.rot.y;
    this->unk288 = 0x258;
    this->unk284 = 3;
    this->unk370 = 0;
    this->unkEF4.base.acFlags |= AC_HARD;
    this->actionFunc = func_80A78868;
}

void func_80A78868(EnJso2* this, PlayState* play) {
    f32 sp44 = this->skelAnime.curFrame;
    Player* player = GET_PLAYER(play);
    Vec3f sp34;
    SkelAnime* sp28 = &this->skelAnime;

    Audio_PlayActorSfx2(&this->actor, NA_SE_EN_PO_FLY - SFX_FLAG);

    if (sp44 < this->unk374) {
        SkelAnime_Update(&this->skelAnime);
    } else if (this->actor.bgCheckFlags & 1) {
        SkelAnime_Update(&this->skelAnime);
    }

    if (Animation_OnFrame(&this->skelAnime, 6.0f) != 0) {
        this->actor.velocity.y = 10.0f;
        if (!(play->gameplayFrames & 1)) {
            //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_ANSATSUSYA_CRYING);
        }
    }

    if (Animation_OnFrame(sp28, 12.0f) != 0) {
        SoundEffect_PlayOneshot(SOUND_ENEMY_GARO_SKIP, 0.1f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor); 
        this->actor.speedXZ = 0.0f;
        if (Rand_ZeroFloat(1.0f) < 0.5f) {
            this->unk288 = -this->unk288;
        }
    }

    if (this->unk28E == 0) {
        this->actor.speedXZ = 0.0f;
        func_80A78B04(this);
        return;
    }

    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0xA, 0xFA0, 0x14);
    Math_ApproachF(&this->actor.speedXZ, 5.0f, 0.3f, 2.0f);

    this->unk286 += this->unk288;

    sp34.x = (Math_SinS(this->unk286) * 200.0f) + player->actor.world.pos.x;
    sp34.y = this->actor.world.pos.y;
    sp34.z = (Math_CosS(this->unk286) * 200.0f) + player->actor.world.pos.z;

    Math_SmoothStepToS(&this->actor.world.rot.y, Math_Vec3f_Yaw(&this->actor.world.pos, &sp34), 0xA, 0xFA0, (s16)0x14);
}

void func_80A78A70(EnJso2* this) {
    func_80A776E0(this, 5);
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    Audio_PlayActorSfx2(&this->actor, NA_SE_IT_SHIELD_BOUND);
    this->unkEF4.base.acFlags |= AC_HARD;
    this->unk284 = 4;
    this->actionFunc = func_80A78ACC;
}

void func_80A78ACC(EnJso2* this, PlayState* play) {
    f32 temp = this->skelAnime.curFrame;

    if (temp >= this->unk374) {
        func_80A787FC(this, play);
    }
}

void func_80A78B04(EnJso2* this) {
    func_80A776E0(this, 0);
    this->actor.world.rot.y = -this->actor.yawTowardsPlayer;
    this->unkEF4.base.acFlags |= AC_HARD;
    this->unk284 = 5;
    this->actionFunc = func_80A78B70;
    this->actor.speedXZ = 10.0f;
    this->actor.velocity.y = 20.0f;
}

void func_80A78B70(EnJso2* this, PlayState* play) {
    this->actor.world.rot.x += 0x1770;
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 10, 4000, 20);
    if (!(this->actor.velocity.y > 0.0f) && (this->actor.bgCheckFlags & 1)) {
        this->actor.world.rot.x = 0;
        this->actor.velocity.y = 0.0f;
        this->actor.speedXZ = 0.0f;
        this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        func_80A78C08(this);
    }
}

void func_80A78C08(EnJso2* this) {
    this->unk28A = 40;
    this->unkEF4.base.acFlags |= AC_HARD;
    this->actor.speedXZ = 15.0f;
    this->actor.velocity.y = 13.0f;
    SoundEffect_PlayOneshot(SOUND_ENEMY_GARO_APPEAR, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor); 
    SoundEffect_PlayHeld(SOUND_ENEMY_GARO_DASH, 1.0f, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor); 
    this->unk284 = 6;
    this->actionFunc = func_80A78C7C;
}

void func_80A78C7C(EnJso2* this, PlayState* play) {
    f32 temp_fv1 = this->skelAnime.curFrame;
    s16 temp_v0_2;
    Vec3f sp2C;
    s32 temp = 1;

    if ((this->unkF40.base.atFlags & AT_BOUNCED) || (this->unkFC0.base.atFlags & AT_BOUNCED)) {
        this->unkF40.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        this->unkFC0.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        Matrix_RotateY(this->actor.yawTowardsPlayer, MTXMODE_NEW);
        Matrix_MultVecZ(-10.0f, &sp2C);
        Math_Vec3f_Copy(&this->unkE58, &sp2C);
        this->unk368 = temp;
        this->unk28A = 0;
        //AudioSfx_SetChannelIO(&this->actor.projectedPos, NA_SE_EN_ANSATSUSYA_DASH_2, 0);
        SoundEffect_PlayHeld(SOUND_ENEMY_GARO_DASH, 1.0f, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor); 
        func_80A79864(this);
        return;
    }

    if ((this->actor.velocity.y < 0.0f) && (this->actor.bgCheckFlags & 1)) {
        if (Rand_ZeroOne() < ((gGameInfo->data[0x976] * 0.1f) + 0.7f)) {
            this->actor.velocity.y = 13.0f;
        } else {
            //AudioSfx_SetChannelIO(&this->actor.projectedPos, NA_SE_EN_ANSATSUSYA_DASH_2, 0);
            SoundEffect_PlayHeld(SOUND_ENEMY_GARO_DASH, 1.0f, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor); 
            this->unk368 = temp;
            this->unk370 = 1;
            this->actor.speedXZ = 0.0f;
            func_80A78E8C(this);
            return;
        }
    }

    if (!(temp_fv1 < this->unk374)) {
        temp_v0_2 = ABS_ALT((s16)(this->actor.yawTowardsPlayer - this->actor.shape.rot.y));

        if (((this->unk28A == 0) || (this->actor.xzDistToPlayer < 120.0f)) || (temp_v0_2 > 0x4300)) {
            //AudioSfx_SetChannelIO(&this->actor.projectedPos, NA_SE_EN_ANSATSUSYA_DASH_2, 0);
            SoundEffect_PlayHeld(SOUND_ENEMY_GARO_DASH, 1.0f, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor); 
            Math_ApproachZeroF(&this->actor.speedXZ, 0.3f, 3.0f);
            func_80A790E4(this, play);
        }
    }
}

void func_80A78E8C(EnJso2* this) {
    func_80A776E0(this, 0);
    this->unk28A = 20;
    this->actor.speedXZ = 0.0f;
    this->actor.gravity = 0.0f;
    this->actor.velocity.y = 10.0f;
    //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_ANSATSUSYA_JUMP);
    SoundEffect_PlayOneshot(SOUND_ENEMY_GARO_JUMP, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor); 
    this->actor.flags |= ACTOR_FLAG_27;
    this->unk284 = 0xF;
    this->actionFunc = func_80A78F04;
}

void func_80A78F04(EnJso2* this, PlayState* play) {
    this->actor.shape.rot.y -= 0x1D4C;
    Math_ApproachZeroF(&this->unk378, 0.3f, 0.01f);
    Math_ApproachZeroF(&this->actor.shape.shadowScale, 0.3f, 3.0f);
    if (this->unk28A == 0) {
        func_80A78F80(this, play);
    }
}

void func_80A78F80(EnJso2* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    Math_Vec3f_Copy(&this->actor.world.pos, &player->actor.world.pos);
    this->actor.world.pos.y += 300.0f + BREG(0x34);
    this->actor.velocity.y = 0.0f;
    this->actor.gravity = BREG(0x35) + -3.0f;
    //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_ANSATSUSYA_FALL);
    SoundEffect_PlayOneshot(SOUND_ENEMY_GARO_FALL, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor); 
    this->unk284 = 16;
    this->actionFunc = func_80A79038;
}

void func_80A79038(EnJso2* this, PlayState* play) {
    Math_ApproachF(&this->unk378, 0.042f, 0.3f, 0.03f);
    Math_ApproachF(&this->actor.shape.shadowScale, 17.0f, 0.4f, 4.0f);
    if (this->actor.bgCheckFlags & 1) {
        this->unk370 = 0;
        this->unk378 = 0.042f;
        this->actor.shape.shadowScale = 17.0f;
        this->actor.flags &= ~(ACTOR_FLAG_27);
        func_80A790E4(this, play);
    }
}

void func_80A790E4(EnJso2* this, PlayState* play) {
    func_80A776E0(this, 1);
    Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, this->actor.shape.shadowScale, 1, 8.0f, 500,
                             10, 1);
    Math_ApproachZeroF(&this->actor.speedXZ, 0.3f, 3.0f);
    this->unk371 = 0;
    Audio_PlayActorSfx2(&this->actor, NA_SE_IT_SWORD_SWING_HARD);
    this->unk284 = 7;
    this->unkEF4.base.acFlags &= ~(AC_HARD);
    this->actionFunc = func_80A7919C;
}

void func_80A7919C(EnJso2* this, PlayState* play) {
    Vec3f sp44;
    f32 sp40 = this->skelAnime.curFrame;

    Math_ApproachZeroF(&this->actor.speedXZ, 0.5f, 5.0f);

    if (!(play->gameplayFrames % 8)) {
        Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, this->actor.shape.shadowScale, 1, 8.0f,
                                 500, 10, 1);
    }

    if ((this->unkF40.base.atFlags & AT_HIT) || (this->unkFC0.base.atFlags & AT_HIT) != 0) {
        this->unk371 = 1;
        this->unkF40.base.atFlags &= ~(AT_HIT);
        this->unkFC0.base.atFlags &= ~(AT_HIT);
    }

    if ((this->unkF40.base.atFlags & AT_BOUNCED) || (this->unkFC0.base.atFlags & 4)) {
        this->unkF40.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        this->unkFC0.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        Matrix_RotateY(this->actor.yawTowardsPlayer, MTXMODE_NEW);
        Matrix_MultVecZ(-10.0f, &sp44);
        Math_Vec3f_Copy(&this->unkE58, &sp44);
        this->unk368 = 1;
        func_80A79864(this);
    } else if (this->unk374 <= sp40) {
        this->unk368 = 1;
        this->actor.speedXZ = 0.0f;
        func_80A79450(this);
    }
}

void func_80A79300(EnJso2* this) {
    func_80A776E0(this, 14);
    this->unk371 = 0;
    Audio_PlayActorSfx2(&this->actor, NA_SE_IT_SWORD_SWING_HARD);
    this->unk368 = 0;
    this->unk28E = 0xF;
    this->unk284 = 8;
    this->actionFunc = func_80A79364;
    this->actor.speedXZ = 12.0f;
}

void func_80A79364(EnJso2* this, PlayState* play) {
    this->actor.shape.rot.y -= 0x1770;
    Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, this->actor.shape.shadowScale, 1, 4.0f, 300, 5,
                             1);
    Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1, 4000, 20);

    if ((this->unk28E == 0) || ((this->unkF40.base.atFlags & AT_HIT) != 0) ||
        (this->unkF40.base.atFlags & AT_BOUNCED) || ((this->unkFC0.base.atFlags & AT_HIT) != 0) ||
        (this->unkFC0.base.atFlags & AT_BOUNCED)) {
        this->unkF40.base.atFlags &= ~(AT_BOUNCED | AT_HIT);
        this->unkFC0.base.atFlags &= ~(AT_BOUNCED | AT_HIT);
        func_80A79864(this);
    }
}

void func_80A79450(EnJso2* this) {
    if (this->unk371 != 0) {
        func_80A776E0(this, 2);
        this->unk290 = 20;
    } else {
        func_80A776E0(this, 12);
        this->unk290 = 40;
    }
    this->unkEF4.base.acFlags &= ~(AC_HARD);
    this->unk284 = 9;
    this->actionFunc = func_80A794C8;
}

void func_80A794C8(EnJso2* this, PlayState* play) {
    if (this->unk290 == 0) {
        this->unk28E = Rand_S16Offset(30, 30);
        this->unk371 = 0;
        this->unkEF4.base.acFlags |= AC_HARD;
        func_80A787FC(this, play);
    }
}

void func_80A79524(EnJso2* this) {
    Vec3f vec;

    //AudioSfx_SetChannelIO(&this->actor.projectedPos, NA_SE_EN_ANSATSUSYA_DASH_2, 0);
    SoundEffect_PlayHeld(SOUND_ENEMY_GARO_DASH, 1.0f, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor); 
    func_80A776E0(this, 4);

    this->unk290 = 0x1E;
    this->unkEF4.base.acFlags &= ~(AC_HARD);
    this->actor.speedXZ = 0.0f;

    Matrix_RotateY(this->actor.yawTowardsPlayer, MTXMODE_NEW);
    Matrix_MultVecZ(-10.0f, &vec);
    Math_Vec3f_Copy(&this->unkE58, &vec);

    if (((this->unk2A2 == 11) || (this->unk2A2 == 10)) && (this->unk2A0 == 0)) {
        this->unk2A0 = 0;
        this->unk2A2 = 0;
    }

    if ((this->unk2A2 != 11) && (this->unk2A2 != 10)) {
        this->unk290 = 40;
    }

    this->unk284 = 10;
    this->actionFunc = func_80A79600;
}

void func_80A79600(EnJso2* this, PlayState* play) {
    if (this->unk2A2 == 11) {
        if ((this->unk2A0 != 0) && (this->unk2A0 < 0x3C)) {
            this->unk2A2 = 0xA;
        }
    }

    if ((this->unk290 == 0) && (this->unk2A0 == 0)) {
        if ((this->unk2A2 == 11) || (this->unk2A2 == 10)) {
            Actor_SpawnIceEffects(play, &this->actor, this->unk2D4, 0xC, 2, 0.7f, 0.4f);
            this->unk2A0 = 0;
            this->unk2A2 = 0;
        }
        func_80A79864(this);
    }
}

void func_80A796BC(EnJso2* this, PlayState* play) {
    Vec3f vec;

    //AudioSfx_SetChannelIO(&this->actor.projectedPos, NA_SE_EN_ANSATSUSYA_DASH_2, 0);
    SoundEffect_PlayHeld(SOUND_ENEMY_GARO_DASH, 1.0f, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor); 
    func_80A776E0(this, 4);

    this->unk371 = 0;
    this->actor.velocity.y = 10.0f;
    this->actor.speedXZ = 0.0f;

    Matrix_RotateY(this->actor.yawTowardsPlayer, MTXMODE_NEW);
    Matrix_MultVecZ(-20.0f, &vec);
    Math_Vec3f_Copy(&this->unkE58, &vec);

    if (((this->unk2A2 == 11) || (this->unk2A2 == 10)) && (this->unk2A0 != 0)) {
        Actor_SpawnIceEffects(play, &this->actor, this->unk2D4, 0xC, 2, 0.7f, 0.4f);
        this->unk2A0 = 0;
        this->unk2A2 = 0;
    }

    CollisionCheck_GreenBlood(play, NULL, &this->actor.focus.pos);
    CollisionCheck_GreenBlood(play, NULL, &this->actor.focus.pos);
    CollisionCheck_GreenBlood(play, NULL, &this->actor.focus.pos);

    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 0xFF, 0, 8);
    //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_ANSATSUSYA_DAMAGE);
    SoundEffect_PlayOneshot(SOUND_ENEMY_GARO_DAMAGE, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor); 
    this->unk284 = 0xB;
    this->actionFunc = func_80A7980C;
}

void func_80A7980C(EnJso2* this, PlayState* play) {
    if (!(this->actor.velocity.y > 0.0f) && (this->actor.colorFilterTimer == 0) && (this->actor.bgCheckFlags & 1)) {
        func_80A79300(this);
    }
}

void func_80A79864(EnJso2* this) {
    func_80A776E0(this, 3);
    this->actor.world.rot.y *= -1;
    this->unk370 = 0;
    this->unk284 = 0xC;
    this->actionFunc = func_80A798C8;
    this->actor.speedXZ = 7.0f;
    this->actor.velocity.y = 20.0f;
}

void func_80A798C8(EnJso2* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 10, 3000, 20);

    if (this->actor.bgCheckFlags & 1) {
        this->actor.speedXZ = 0.0f;
    }

    if ((this->unk374 <= curFrame) && (this->actor.bgCheckFlags & 1)) {
        this->actor.world.rot.x = 0;
        this->actor.velocity.y = 0.0f;
        this->actor.speedXZ = 0.0f;
        this->actor.world.rot.y = this->actor.shape.rot.y;
        this->unk28E = Rand_S16Offset(10, 10);
        func_80A787FC(this, play);
    }
}

void func_80A7998C(EnJso2* this, PlayState* play) {
    Vec3f gZeroVec3f = {0};
    //AudioSfx_SetChannelIO(&this->actor.projectedPos, NA_SE_EN_ANSATSUSYA_DASH_2, 0);
    SoundEffect_PlayHeld(SOUND_ENEMY_GARO_DASH, 1.0f, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor); 
    func_80A776E0(this, 4);

    if (((this->unk2A2 == 0xB) || (this->unk2A2 == 0xA)) && (this->unk2A0 == 0)) {
        this->unk2A2 = 0;
    }

    this->actor.shape.rot.z = 0;
    this->actor.flags |= ACTOR_FLAG_27;
    this->actor.flags &= ~(ACTOR_FLAG_0 | ACTOR_FLAG_2);
    this->actor.speedXZ = 0.0f;
    this->unk368 = 1;
    this->unk290 = 0x1E;
    this->unk36C = 2;
    this->actor.world.rot.z = this->actor.shape.rot.z;
    this->actor.shape.rot.x = this->actor.shape.rot.z;
    this->actor.world.rot.x = this->actor.shape.rot.z;

    Enemy_StartFinishingBlow(play, &this->actor);
    //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_ANSATSUSYA_DEAD);
    SoundEffect_PlayOneshot(SOUND_ENEMY_GARO_DEAD, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor); 
    Math_Vec3f_Copy(&this->unkE58, &gZeroVec3f);

    this->unk284 = 0xD;
    this->actionFunc = func_80A79A84;
}

void func_80A79A84(EnJso2* this, PlayState* play) {
    Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 10, 4000, 20);

    if ((this->unk2A2 == 0xB) || (this->unk2A2 == 0xA)) {
        if (this->unk2A0 != 0) {
            Actor_SpawnIceEffects(play, &this->actor, this->unk2D4, 0xC, 2, 0.7f, 0.4f);
            this->unk2A0 = 0;
            this->unk2A2 = 0;
        } else {
            return;
        }
    }

    if (this->unk290 == 0) {
        this->unk1050 = 60.0f;
        this->unk104C = 60.0f;
        if (this->unk29C == 0) {
            func_80A79B60(this);
            return;
        }
        func_80A7A0D0(this);
    }
}

void func_80A79B60(EnJso2* this) {
    this->unk1046 = 0;
    this->unk1044 = 0;
    this->actor.flags |= ACTOR_FLAG_20;
    this->unk290 = 0x1E;
    this->unk284 = 0xE;
    this->actionFunc = func_80A79BA0;
}

void func_80A79BA0(EnJso2* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 curframe = this->skelAnime.curFrame;
    Vec3f sp4C;

    if ((this->unk1040 == 0x15) && (curframe >= this->unk374) && !this->unk2D0) {
        this->unk2D0 = Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACT2_ID, this->unk2C4.x,
                                          this->unk2C4.y, this->unk2C4.z, 0, 0, 0, 3);
    }

    if (this->unk2D0) {
        this->unk2D0->world.pos.x = this->unk2C4.x;
        this->unk2D0->world.pos.y = this->unk2C4.y;
        this->unk2D0->world.pos.z = this->unk2C4.z;
    }

    switch (this->unk1046) {
        case 0:
                //CutsceneManager_StartWithPlayerCs(this->actor.csId, &this->actor);
                func_8002DF54(play, &this->actor, 7);
                this->actor.world.rot.y = this->actor.shape.rot.y =
                    Math_Vec3f_Yaw(&this->actor.world.pos, &this->actor.home.pos);
                func_800F483C(0, 10);
                func_80A776E0(this, 0x13);
                func_8002DF54(play, &this->actor, 7);
                this->unk294 = 0.4f;
                this->unk1046++;
                this->unk298 = 40.0f;
            break;

        case 1:
            if (curframe >= this->unk374) {
                func_80A776E0(this, 0x14);
                this->unk1046++;
            }
            break;

        case 2:
            if (curframe >= this->unk374) {
                this->actor.textId = 0x13AE;
                Message_StartTextbox(play, this->actor.textId, &this->actor);
                this->unk1046++;
            }
            break;

        case 3:
            if (func_80A77880(play) != 0) {
                if (this->actor.textId == 0x13AE) {
                    this->actor.textId = 0x13AF;
                } else if (this->actor.textId == 0x13AF) {
                    this->actor.textId = 0x13B0;
                    func_80A776E0(this, 0x15);
                } else if (this->actor.textId == 0x13B0) {
                    play->msgCtx.msgLength = 0;
                    if (this->unk2D0) {
                        this->unk2D0->world.rot.z = 1;
                        func_8002DF54(play, &this->actor, 0x2F);
                        this->unk2B4 = 1;
                    }
                    this->unk1044 = 0x1E;
                    this->unk1046++;
                    break;
                }

                Message_ContinueTextbox(play, this->actor.textId);
            }
            break;

        case 4:
            Math_SmoothStepToS(&this->unk366, 0, 1, 0xF, 0x32);
            Math_ApproachZeroF(&this->actor.shape.shadowScale, 0.3f, 3.0f);
            if (this->unk1044 == 0) {
                this->actor.textId = 0x13B1;
                Message_StartTextbox(play, this->actor.textId, &this->actor);
                //Audio_PlayActorSfx2(&this->actor, NA_SE_EN_ANSATSUSYA_LAUGH);
                SoundEffect_PlayOneshot(SOUND_ENEMY_GARO_LAUGH, 1.0f, 1.0f, &this->actor.world.pos, 0.0f, 2000.0f, &this->actor); 
                this->unk290 = 0x32;
                this->unk1046++;
            }
            break;

        case 5:
            func_8002DF54(play, &this->actor, 7);
            if (this->unk290 == 0) {
                //CutsceneManager_Stop(this->actor.csId);
                func_8002DF54(play, &this->actor, 6);
                Actor_Kill(&this->actor);
            }
            break;

        default:
            break;
    }

    if (this->unk1048 != 0) {
        player->actor.world.pos.x = (Math_SinS(this->actor.world.rot.y) * 170.0f) + this->actor.world.pos.x;
        player->actor.world.pos.z = (Math_CosS(this->actor.world.rot.y) * 170.0f) + this->actor.world.pos.z;
        player->actor.world.rot.y = player->actor.shape.rot.y = this->actor.world.rot.y + 0x8000;

        Matrix_RotateY((BREG(49) << 8) + this->actor.shape.rot.y + 0x1000, MTXMODE_NEW);
        Matrix_MultVecZ(BREG(48) + 230.0f, &sp4C);
        this->unk1078.x = this->actor.world.pos.x + sp4C.x;
        this->unk1078.y = BREG(50) + -43.0f + this->actor.world.pos.y + 50.0f;
        this->unk1078.z = this->actor.world.pos.z + sp4C.z;
        this->unk1084.x = player->actor.world.pos.x + ((this->actor.world.pos.x - player->actor.world.pos.x) * 0.5f);
        this->unk1084.y = BREG(51) + 6.0f + player->actor.world.pos.y + 5.0f;
        this->unk1084.z = player->actor.world.pos.z + ((this->actor.world.pos.z - player->actor.world.pos.z) * 0.5f);
    }
    func_80A77790(this, play);
}

void func_80A7A0D0(EnJso2* this) {
    this->unk1044 = 0;
    func_800F483C(0, 10);
    func_80A776E0(this, 19);
    this->unk284 = 14;
    this->actionFunc = func_80A7A124;
}

void func_80A7A124(EnJso2* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0xA, 0xFA0, 0x14);

    if ((this->unk1040 == 19) && (this->unk374 <= curFrame)) {
        this->unk104A = 0;
        func_80A776E0(this, 20);
    }

    if ((this->unk1040 == 20) && (this->unk374 <= curFrame)) {
        this->unk104A++;
        if (this->unk104A >= 2) {
            this->unk104A = 0;
            func_80A776E0(this, 21);
        }
    }

    if ((this->unk1040 == 0x15) && (this->unk374 <= curFrame)) {
        if (this->unk2D0 == NULL) {
            this->unk2D0 = Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACT2_ID, this->unk2C4.x,
                                              this->unk2C4.y, this->unk2C4.z, 0, 0, 0, 4);
        } else if (this->unk104A >= 10) {
            if (this->unk2D0 != NULL) {
                this->unk2D0->world.rot.z = 1;
                this->unk2B4 = 1;
                this->actionFunc = func_80A7A2EC;
                return;
            }
        } else {
            this->unk104A++;
        }
    }

    if (this->unk2D0 != NULL) {
        this->unk2D0->world.pos.x = this->unk2C4.x;
        this->unk2D0->world.pos.y = this->unk2C4.y;
        this->unk2D0->world.pos.z = this->unk2C4.z;
    }
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->unkEF4.base);
}

void func_80A7A2EC(EnJso2* this, PlayState* play) {
    Math_SmoothStepToS(&this->unk366, 0, 1, 0xF, 0x32);
    Math_ApproachZeroF(&this->actor.shape.shadowScale, 0.3f, 3.0f);
    if (this->unk366 < 2) {
        Actor_Kill(&this->actor);
    }
}

void func_80A7A360(EnJso2* this, PlayState* play) {
    s32 var_a3 = false;

    if ((this->unk284 != 11) && (this->unk284 != 12) && (this->unk284 != 13) && (this->unk284 != 14) &&
        this->unkEF4.base.acFlags & AT_HIT) {
        this->unkEF4.base.acFlags &= ~(AT_HIT);
        if ((this->actor.colChkInfo.damageEffect == 1) || (this->actor.colChkInfo.damageEffect == 5)) {
            this->actor.world.rot.x = this->actor.shape.rot.x = 0;
            if (((this->unk2A2 != 0xB) && (this->unk2A2 != 0xA)) || (this->unk2A0 == 0)) {
                Audio_PlayActorSfx2(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
                if (this->actor.colChkInfo.damageEffect == 5) {
                    this->unk2A0 = 40;
                    this->unk2A2 = 32;
                }
                Actor_SetColorFilter(&this->actor, 0, 255, 0, 40);
                func_80A79524(this);
            }
        } else {
            switch (this->unk284) {
                case 2:
                case 3:
                case 4:
                    this->actor.speedXZ = 0.0f;
                    func_80A78A70(this);
                    break;
                case 7:
                case 9:
                case 10:
                    switch (this->actor.colChkInfo.damageEffect) {
                        case 15:
                            var_a3 = true;
                            break;
                        case 2:
                            this->unk2A0 = 40;
                            this->unk2A2 = 0;
                            var_a3 = true;
                            break;

                        case 4:
                            if (((this->unk2A2 != 11) && (this->unk2A2 != 10)) || (this->unk2A0 == 0)) {
                                Actor_Spawn(&play->actorCtx, play, 0x33B, this->actor.focus.pos.x,
                                            this->actor.focus.pos.y, this->actor.focus.pos.z, 0, 0, 0, 4);
                                this->unk2A0 = 20;
                                this->unk2A2 = 20;
                                var_a3 = true;
                            }
                            break;

                        case 6:
                            if (((this->unk2A2 != 11) && (this->unk2A2 != 10)) || (this->unk2A0 == 0)) {
                                Actor_Spawn(&play->actorCtx, play, 0x33B, this->actor.focus.pos.x,
                                            this->actor.focus.pos.y, this->actor.focus.pos.z, 0, 0, 0, 4);
                                this->unk2A0 = 21;
                                this->unk2A2 = 21;
                                var_a3 = true;
                            }
                            break;

                        case 3:
                            if (((this->unk2A2 != 0xB) && (this->unk2A2 != 0xA)) || (this->unk2A0 == 0)) {
                                Actor_ApplyDamage(&this->actor);
                                this->unk2A0 = 0x50;
                                this->unk2A2 = 0xB;
                                this->unk2A4 = 0.0f;
                                this->unk2A8 = 1.5f;
                            }

                            if (this->actor.colChkInfo.health <= 0) {
                                func_80A7998C(this, play);
                            } else {
                                func_80A79524(this);
                            }
                            break;

                        default:
                            break;
                    }
                default:
                    break;
            }

            if (var_a3) {
                Actor_ApplyDamage(&this->actor);
                if (this->actor.colChkInfo.health > 0) {
                    func_80A796BC(this, play);
                } else {
                    func_80A7998C(this, play);
                }
            }
        }
    }
}

void EnJso2_Update(Actor* thisx, PlayState* play) {
    s32 pad;
    s32 i;
    EnJso2* this = (EnJso2*)thisx;

    if (this->unk284 != 3) {
        SkelAnime_Update(&this->skelAnime);
    }

    DECR(this->unk28A);
    DECR(this->unk28E);
    DECR(this->unk290);
    DECR(this->unk1044);
    DECR(this->unk2A0);

    func_80A7A360(this, play);
    Actor_SetScale(&this->actor, this->unk378);
    this->actionFunc(this, play);
    Actor_SetFocus(&this->actor, 80.0f);
    Actor_MoveForward(&this->actor);

    if (this->actor.bgCheckFlags & 1) {
        this->actor.world.pos.x += this->unkE58.x;
        this->actor.world.pos.z += this->unkE58.z;
        Math_ApproachZeroF(&this->unkE58.x, 1.0f, 2.0f);
        Math_ApproachZeroF(&this->unkE58.z, 1.0f, 2.0f);
    }

    Actor_UpdateBgCheckInfo(play, &this->actor, 35.0f, 60.0f, 60.0f, 0x1D);

    if ((this->unk284 == 5) || (this->unk284 == 6) || (this->unk284 == 15) || (this->unk284 == 16)) {
        this->unk_38C++;
        if (this->unk_38C >= 20) {
            this->unk_38C = 0;
        }

        if (this->unk_388 < 19) {
            this->unk_388++;
        }
        Math_Vec3f_Copy(&this->unk_390[this->unk_38C], &this->actor.world.pos);
        Math_Vec3s_Copy(&this->unk_480[this->unk_38C], &this->actor.world.rot);

        this->unk_390[this->unk_38C].y += 40.0f;

        for (i = 0; i < 20; i++) {
            this->unk_4F8[this->unk_38C][i] = this->jointTable[i];
        }
    } else if (this->unk284 != 0) {
        this->unk_388 = 0;
    }

    if ((this->unk284 != 3) && (this->unk284 != 5) && (this->unk284 != 0xB) && (this->unk284 != 8) &&
        (this->unk284 != 0xF) && (this->unk284 != 0xC)) {
        this->actor.shape.rot.y = this->actor.world.rot.y;
    }

    this->actor.shape.rot.x = this->actor.world.rot.x;

    Collider_UpdateCylinder(&this->actor, &this->unkEF4);

    if ((this->unk284 != 0) && (this->unk284 != 8) && (this->unk284 != 0xF)) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->unkEF4.base);

        if ((this->unk284 != 1) && (this->unk284 != 8)) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->unkEF4.base);
        }
    }
    if (((this->unk284 == 7) || (this->unk284 == 16) || (this->unk284 == 6) || (this->unk284 == 8)) &&
        (this->unk371 == 0) && (this->unk36C == 0)) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->unkF40.base);
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->unkFC0.base);
    }
}

s32 func_80A7AA48(PlayState* play, s32 arg1, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnJso2* this = THIS;

    if (this->unk36C == 2) {
        if ((arg1 == 4) && (this->unk284 != 14)) {
            *dList = NULL;
        }
        if (arg1 == 6) {
            *dList = NULL;
        }
    }
    return 0;
}

void func_80A7AA9C(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    EnJso2* this = THIS;
    Vec3f sp68;
    Vec3f sp5C;
    Vec3f sp50 = D_80A7B6FC;

    if (limbIndex == 4) {
        Matrix_Translate(0.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        Math_Vec3f_Copy(&this->unkFC0.dim.quad[3], &this->unkFC0.dim.quad[1]);
        Math_Vec3f_Copy(&this->unkFC0.dim.quad[2], this->unkFC0.dim.quad);
        Matrix_MultVec3f(&D_80A7B720, &this->unkFC0.dim.quad[1]);
        Matrix_MultVec3f(&D_80A7B72C, this->unkFC0.dim.quad);
        Collider_SetQuadVertices(&this->unkFC0, this->unkFC0.dim.quad, &this->unkFC0.dim.quad[1],
                                 &this->unkFC0.dim.quad[2], &this->unkFC0.dim.quad[3]);
        Matrix_MultVec3f(&D_80A7B708, &sp68);
        Matrix_MultVec3f(&D_80A7B714, &sp5C);

        if ((this->unk284 == 7) || (this->unk284 == 9)) {
            Matrix_MultVec3f(&D_80A7B5A0, &this->unk_E64[0]);
            Matrix_MultVec3f(&D_80A7B5AC, &this->unk_E64[1]);
            Matrix_MultVec3f(&D_80A7B5B8, &this->unk_E64[2]);
        } else {
            Matrix_MultVec3f(&D_80A7B558, &this->unk_E64[0]);
            Matrix_MultVec3f(&D_80A7B564, &this->unk_E64[1]);
            Matrix_MultVec3f(&D_80A7B570, &this->unk_E64[2]);
        }

        if (((this->unk284 == 7) || (this->unk284 == 8) || (this->unk284 == 6) || (this->unk284 == 16)) &&
            (this->unk368 == 0)) {
            EffectBlure_AddVertex(Effect_GetByIndex(this->unk_384), &sp68, &sp5C);
        } else if (this->unk368 == 1) {
            EffectBlure_AddSpace(Effect_GetByIndex(this->unk_384));
        }
    }

    if (limbIndex == 6) {
        Matrix_Translate(0.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        Math_Vec3f_Copy(&this->unkF40.dim.quad[3], &this->unkF40.dim.quad[1]);
        Math_Vec3f_Copy(&this->unkF40.dim.quad[2], this->unkF40.dim.quad);
        Matrix_MultVec3f(&D_80A7B720, &this->unkF40.dim.quad[1]);
        Matrix_MultVec3f(&D_80A7B72C, this->unkF40.dim.quad);
        Collider_SetQuadVertices(&this->unkF40, this->unkF40.dim.quad, &this->unkF40.dim.quad[1],
                                 &this->unkF40.dim.quad[2], &this->unkF40.dim.quad[3]);
        Matrix_MultVec3f(&D_80A7B708, &sp68);
        Matrix_MultVec3f(&D_80A7B714, &sp5C);

        if ((this->unk284 == 7) || (this->unk284 == 9)) {
            Matrix_MultVec3f(&D_80A7B5C4, &this->unk_E64[3]);
            Matrix_MultVec3f(&D_80A7B5D0, &this->unk_E64[4]);
            Matrix_MultVec3f(&D_80A7B5DC, &this->unk_E64[5]);
        } else {
            Matrix_MultVec3f(&D_80A7B57C, &this->unk_E64[3]);
            Matrix_MultVec3f(&D_80A7B588, &this->unk_E64[4]);
            Matrix_MultVec3f(&D_80A7B594, &this->unk_E64[5]);
        }

        if (((this->unk284 == 7) || (this->unk284 == 8) || (this->unk284 == 6) || (this->unk284 == 16)) &&
            (this->unk368 == 0)) {
            EffectBlure_AddVertex(Effect_GetByIndex(this->unk_380), &sp68, &sp5C);
        } else if (this->unk368 == 1) {
            EffectBlure_AddSpace(Effect_GetByIndex(this->unk_380));
            this->unk368 = 0;
        }
    }

    if (limbIndex == 10) {
        sp50.x = 900.0f;
        sp50.y = 50.0f;
        sp50.z = -330.0f;
        Matrix_MultVec3f(&sp50, &this->unk2C4);
    }

    if ((this->unk284 != 14) && ((limbIndex == 4) || (limbIndex == 6) || (limbIndex == 7) || (limbIndex == 8) ||
                                 (limbIndex == 9) || (limbIndex == 10) || (limbIndex == 11) || (limbIndex == 12) ||
                                 (limbIndex == 14) || (limbIndex == 16) || (limbIndex == 17) || (limbIndex == 19))) {

        Matrix_MultZero(&this->unk2D4[this->unk364]);

        if (++this->unk364 >= 12) {
            this->unk364 = 0;
        }
    }

    if (limbIndex == 12) {
        Matrix_Push();
        Matrix_Scale(1.0f, 1.0f, 1.0f, MTXMODE_APPLY);

        OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        //! FAKE:
        if (1) {}

        gSPDisplayList(POLY_OPA_DISP++, 0x06002ed8);
        Matrix_Pop();

        CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
    }
}

void EnJso2_Draw(Actor* thisx, PlayState* play2) {
    EnJso2* this = THIS;
    PlayState* play = play2;
    s32 i;
    s32 pad;

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    Gfx_SetupDL_25Xlu(play->state.gfxCtx);
    Gfx_SetupDL_25Opa(play->state.gfxCtx);

    if ((this->unk2B4) == 0) {
        Scene_SetRenderModeXlu(play, 0, 1);
        SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                              func_80A7AA48, func_80A7AA9C, &this->actor);
    } else {
        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, this->unk366);
        Scene_SetRenderModeXlu(play, 1, 2);
        POLY_XLU_DISP = SkelAnime_DrawFlex(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                           this->skelAnime.dListCount, NULL, NULL, &this->actor, POLY_XLU_DISP);
    }

    if (this->unk_388 > 0) {
        s32 index = this->unk_38C;

        for (i = 0; i < this->unk_388; i++) {
            if (D_80A7B738[i] == 0) {
                continue;
            }
            Matrix_Translate(this->unk_390[index].x, this->unk_390[index].y, this->unk_390[index].z, MTXMODE_NEW);
            Matrix_Scale(this->unk378, this->unk378, this->unk378, MTXMODE_APPLY);
            Matrix_RotateY(this->unk_480[index].y, MTXMODE_APPLY);
            Matrix_RotateX(this->unk_480[index].x, MTXMODE_APPLY);
            Matrix_RotateZ(this->unk_480[index].z, MTXMODE_APPLY);
            gDPPipeSync(POLY_XLU_DISP++);
            gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, D_80A7B738[i]);
            Scene_SetRenderModeXlu(play, 1, 2);
            POLY_XLU_DISP = SkelAnime_DrawFlex(play, this->skelAnime.skeleton, this->unk_4F8[index],
                                               this->skelAnime.dListCount, NULL, NULL, &this->actor, POLY_XLU_DISP);
            index--;
            if (index < 0) {
                index = 19;
            }
        }
    }

    if (((this->unk284 < 15) && (this->unk36C == 0)) &&
        (((this->unk2A2 != 11) && (this->unk2A2 != 10)) ||
         (((this->unk2A2 == 11) || (this->unk2A2 == 10)) && (this->unk2A0 == 0)))) {
        for (i = 0; i < 6; i++) {
            Matrix_Push();
            Matrix_Translate(this->unk_E64[i].x, this->unk_E64[i].y, this->unk_E64[i].z, MTXMODE_NEW);
            Matrix_Scale(this->unk_EAC[i].x, this->unk_EAC[i].y, this->unk_EAC[i].z, MTXMODE_APPLY);

            gSPSegment(POLY_XLU_DISP++, 0x08,
                       Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 0, 32, 64, 1, 0,
                                        ((this->unk_38E * 10) - (play->state.frames * 20)) & 0x1FF, 32, 128));
            gDPPipeSync(POLY_XLU_DISP++);

            gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, 170, 255);

            gDPSetEnvColor(POLY_XLU_DISP++, 255, 50, 0, 255);

            Matrix_Mult(&play->billboardMtxF, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);
            Matrix_Pop();
        }
    }

    if (this->unk2A0 != 0) {
        f32 temp_fv1 = this->unk2A0 * 0.05f;

        if ((this->unk2A2 == 0xB) || (this->unk2A2 == 0xA)) {
            this->unk2A4 += 0.3f;
            if (this->unk2A4 > 0.5f) {
                this->unk2A4 = 0.5f;
            }
            Math_ApproachF(&this->unk2A8, this->unk2A4, 0.1f, 0.04f);
        }
        Actor_DrawDamageEffects(play, &this->actor, this->unk2D4, this->unk_2B0, this->unk2A4, this->unk2A8, temp_fv1,
                                this->unk2A2);
    }

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}
