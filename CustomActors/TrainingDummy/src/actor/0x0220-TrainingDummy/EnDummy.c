#include "EnDummy.h"
#include "functions.h"

void EnDummy_Init(EnDummy* this, PlayState* play);
void EnDummy_Destroy(EnDummy* this, PlayState* play);
void EnDummy_Update(EnDummy* this, PlayState* play);
void EnDummy_Draw(EnDummy* this, PlayState* play);

void EnDummy_Recoil(EnDummy* this, PlayState* play);
void EnDummy_Particle(EnDummy* this, PlayState* play);

#define ACT_ID 0x0220
#define OBJ_ID 0x0220


const ActorInit sEnDummy_InitVars = {
    .id           = ACT_ID,
    .category     = ACTORCAT_BG,
    .flags        = (ACTOR_FLAG_0 | ACTOR_FLAG_2 | ACTOR_FLAG_10), // Targetable // Enemy // Hookshotable
    .objectId     = OBJ_ID,
    .instanceSize = sizeof(EnDummy),
    .init         = (ActorFunc)EnDummy_Init,
    .destroy      = (ActorFunc)EnDummy_Destroy,
    .update       = (ActorFunc)EnDummy_Update,
    .draw         = (ActorFunc)EnDummy_Draw
};

static void* sNumberTextures[] = { // Store Number Textures
    gEnDummy_TexNumber0,
    gEnDummy_TexNumber1,
    gEnDummy_TexNumber2,
    gEnDummy_TexNumber3,
    gEnDummy_TexNumber4,
    gEnDummy_TexNumber5,
    gEnDummy_TexNumber6,
    gEnDummy_TexNumber7,
    gEnDummy_TexNumber8,
    gEnDummy_TexNumber9,
};

typedef enum {
    /* 0x0 */ Dummy_DMGEFF_NONE,
    /* 0x1 */ Dummy_DMGEFF_FIRE,
    /* 0x2 */ Dummy_DMGEFF_ICE,
    /* 0x3 */ Dummy_DMGEFF_LIGHT,
    /* 0x4 */ Dummy_DMGEFF_STUN,
    /* 0x4 */ Dummy_DMGEFF_PANCAKE,
} DummyDamageEffect;

Color_RGB8 DamageColorsEnv[] = { // Display Damage Numbers with corresponding color to their effect
    {0xFF,0xFF,0xFF}, // white
    {0xFF,0x2F,0x3C}, // red
    {0x41,0x77,0xFF}, // blue
    {0xFF,0xE1,0x73}, // yellow
    {0x08,0xE2,0xFF}, // light blue
    {0xF4,0xAC,0x4A}, // pancake colored
};

typedef enum {
    /* 0x0 */ None,    // No Effect
    /* 0x1 */ Stunned, // No Movement
} movementState;

static DamageTable sDamageTable = { // Basic damage table with effects
    /* Deku nut      */ DMG_ENTRY(0, Dummy_DMGEFF_STUN),
    /* Deku stick    */ DMG_ENTRY(2, 0x0),
    /* Slingshot     */ DMG_ENTRY(1, 0x0),
    /* Explosive     */ DMG_ENTRY(2, 0x0),
    /* Boomerang     */ DMG_ENTRY(0, Dummy_DMGEFF_STUN),
    /* Normal arrow  */ DMG_ENTRY(2, 0x0),
    /* Hammer swing  */ DMG_ENTRY(2, Dummy_DMGEFF_PANCAKE),
    /* Hookshot      */ DMG_ENTRY(0, Dummy_DMGEFF_STUN),
    /* Kokiri sword  */ DMG_ENTRY(1, 0x0),
    /* Master sword  */ DMG_ENTRY(2, 0x0),
    /* Giant's Knife */ DMG_ENTRY(4, 0x0),
    /* Fire arrow    */ DMG_ENTRY(2, Dummy_DMGEFF_FIRE),
    /* Ice arrow     */ DMG_ENTRY(4, Dummy_DMGEFF_ICE),
    /* Light arrow   */ DMG_ENTRY(2, Dummy_DMGEFF_LIGHT),
    /* Unk arrow 1   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(2, 0x0),
    /* Fire magic    */ DMG_ENTRY(0, Dummy_DMGEFF_FIRE),
    /* Ice magic     */ DMG_ENTRY(3, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(1, 0x0),
    /* Giant spin    */ DMG_ENTRY(4, 0x0),
    /* Master spin   */ DMG_ENTRY(2, 0x0),
    /* Kokiri jump   */ DMG_ENTRY(2, 0x0),
    /* Giant jump    */ DMG_ENTRY(8, 0x0),
    /* Master jump   */ DMG_ENTRY(4, 0x0),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(4, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

typedef enum {
    /* 0x0 */ AnimIdle,
    /* 0x1 */ AnimRecoil,
} DummyAnims;

//typedef struct {
//    /* 0x00 */ AnimationHeader* animation;
//    /* 0x04 */ f32              playSpeed;
//    /* 0x08 */ f32              startFrame;
//    /* 0x0C */ f32              frameCount;
//    /* 0x10 */ u8               mode;
//    /* 0x14 */ f32              morphFrames;
//} AnimationInfo; // size = 0x18

AnimationInfo sDummyAnims[] = {
    { gEnDummy_AnimIdle,   1, 0, -1, ANIMMODE_ONCE,  8 },
    { gEnDummy_AnimRecoil, 1, 0, -1, ANIMMODE_ONCE, -1 },
};

void EnDummy_SetAction(EnDummy* this, EnDummyFunc func) {
  this->func = func;
}

s32 CanSee(Actor* actor, Actor* targetActor, PlayState* play, s16 viewConeAngle, f32 range ,f32 heightLimit) {
    CollisionPoly* colPoly;
    s32 bgId;
    Vec3f colPoint;

    // Check to see if targetActor is within range of actor
    if (Math_Vec3f_DistXYZ(&actor->world.pos, &targetActor->world.pos) > range) {
        return false;
    }

    // Check to see if heightDifference between target is within height limit
    if (ABS(actor->pos.y - targetActor->pos.y) > heightLimit) {
        return false;
    }

    // Check to see if the yawToTarget is within view cone
    if (ABS((s16)(Math_Vec3f_Yaw(&actor->world.pos, &targetActor->world.pos) - actor->shape.rot.y)) > viewConeAngle) {
        return false;
    }

    // check to see if the line between actor and targetActor does not intersect a collision poly
    if (BgCheck_EntityLineTest1(&play->colCtx, &actor->world.pos, &targetActor->world.pos, &colPoint, &colPoly, true,
                                false, false, true, &bgId)) {
        return false;
    }

    return true;
}

void EnDummy_Idle(EnDummy* this, PlayState* play) {
    // Waiting
}

void EnDummy_Recoil(EnDummy* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (this->skelAnime.curFrame == 6){

        if (CanSee(&this->actor,&player->actor,play,DEG_TO_BINANG(45),30.0f,50.0f)){
            func_8002F6D4(play, &this->actor, 7.0f, this->actor.yawTowardsPlayer, 5.0f, 0x00);
        }
    }

    // Return to idle after taking normal damage
    if (this->skelAnime.animation == gEnDummy_AnimRecoil && Animation_OnFrame(&this->skelAnime, Animation_GetLastFrame(gEnDummy_AnimRecoil))){
        Animation_ChangeByInfo(&this->skelAnime, sDummyAnims, AnimIdle);
        EnDummy_SetAction(this, EnDummy_Idle);
    }
}

void EnDummy_Particle(EnDummy* this, PlayState* play) {
    u8 newnum = this->numparticles;

    this->numparticles += Rand_S16Offset(2, 4); //(Minimum, Rand Addition Max)
    this->numparticles = CLAMP_MAX(this->numparticles, ParticleMax);
    newnum = this->numparticles - newnum;
    
    if (newnum == 0)
        return;
    
    for (int i = 0; i < ParticleMax && newnum > 0; ++i) {
        EnDummyParticle* p = &this->particles[i];
        
        if (p->timer == 0) {
            newnum--;
            p->pos = this->actor.pos;
            p->pos.x += Rand_CenteredFloat(15.0f);
            p->pos.y += 40.0f + Rand_CenteredFloat(5.0f);
            p->pos.z += Rand_CenteredFloat(15.0f);
            p->vel.x = Rand_CenteredFloat(6.0f);
            p->vel.y = Rand_CenteredFloat(3.0f) + 6.0f;
            p->vel.z = Rand_CenteredFloat(6.0f);
            p->rot.x += Rand_S16Offset(0, 0x4000);
            p->rot.y += Rand_S16Offset(0, 0x4000);
            p->rot.z += Rand_S16Offset(0, 0x4000);
            p->timer = Rand_S16Offset(20,55);
        }
    }
}

void EnDummy_ParticleDraw(EnDummy* this, PlayState* play) {

    if (this->numparticles!= 0)
        gSPDisplayList(POLY_XLU_DISP++, gEnDummy_MtlParticleEffect1);

    for (int i = 0; i < ParticleMax && this->numparticles > 0; ++i) {

        EnDummyParticle* p = &this->particles[i];
        
        Vec3f prevpos = p->pos;
        
        if (p->timer == 0)
            continue;
        
        p->timer--;
        
        if (p->timer == 0)
            this->numparticles--;
        
        p->pos = Vec3f_Add(p->pos, p->vel);
        p->vel = Vec3f_MulVal(Vec3f_Sub(p->pos, prevpos), 0.93f);
        p->vel.y -= particleGravity;
        p->rot.x += Rand_S16Offset(500, 200);
        p->rot.y += Rand_S16Offset(500, 200);
        p->rot.z += Rand_S16Offset(500, 200);
        
        Matrix_Translate(UnfoldVec3f(p->pos), MTXMODE_NEW);
        Matrix_RotateZYX(UnfoldVec3f(p->rot), MTXMODE_APPLY);
        Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);

        gSPMatrix(POLY_XLU_DISP++, NEW_MATRIX(), G_MTX_LOAD);
        gSPDisplayList(POLY_XLU_DISP++, gEnDummy_DlParticleEffect1 + 1);
    }
}

void EnDummy_NumberParticle(EnDummy* this, PlayState* play) {
    u8 newnum = this->numberNumparticles;

    this->numberNumparticles += 1; //(Minimum, Rand Addition Max)
    this->numberNumparticles = CLAMP_MAX(this->numberNumparticles, NumberParticleMax);
    newnum = this->numberNumparticles - newnum;
    
    if (newnum == 0)
        return;
    
    for (int i = 0; i < NumberParticleMax && newnum > 0; ++i) {
        EnDummyNumberParticle* p = &this->numberParticles[i];
        
        if (p->timer == 0) {
            newnum--;
            p->damage = this->actor.colChkInfo.damage; // store damage value
            p->damageEffect = this->actor.colChkInfo.damageEffect; // store damage effect value
            p->pos = this->actor.pos;
            p->pos.x += Rand_CenteredFloat(20.0f);
            p->pos.y += 90.0f;
            p->pos.z += Rand_CenteredFloat(20.0f);
            p->vel.x = Rand_CenteredFloat(6.0f);
            p->vel.z = Rand_CenteredFloat(6.0f);
            p->timer = Rand_S16Offset(20,35);
        }
    }
}

void EnDummy_NumberParticleDraw(EnDummy* this, PlayState* play) {

    for (int i = 0; i < NumberParticleMax && this->numberNumparticles > 0; ++i) {

        EnDummyNumberParticle* p = &this->numberParticles[i];
        
        Vec3f prevpos = p->pos;
        
        if (p->timer == 0)
            continue;
        
        p->timer--;
        
        if (p->timer == 0)
            this->numberNumparticles--;
        
        p->pos = Vec3f_Add(p->pos, p->vel);
        p->vel = Vec3f_MulVal(Vec3f_Sub(p->pos, prevpos), 0.93f);
        p->vel.y -= NumberParticleGravity;
        
        Matrix_Translate(UnfoldVec3f(p->pos), MTXMODE_NEW);
        Matrix_ReplaceRotation(&play->billboardMtxF);
        Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);

        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sNumberTextures[p->damage]));
        gSPMatrix(POLY_XLU_DISP++, NEW_MATRIX(), G_MTX_LOAD);

        gDPSetEnvColor(POLY_XLU_DISP++, DamageColorsEnv[p->damageEffect].r, DamageColorsEnv[p->damageEffect].g, DamageColorsEnv[p->damageEffect].b, 255);
        gSPDisplayList(POLY_XLU_DISP++, gEnDummy_DlParticleEffect2);
    }
}

void DrawDebugCone(Actor* actor, f32 range, s32 viewConeAngle, GraphicsContext* gfxCtx, s32 canSeeResult) {
    Vtx* vtx;
    f32 scale = range * 0.01f;

    Gfx_SetupDL_25Xlu(__gfxCtx);
    Matrix_RotateY(BINANG_TO_RAD(actor->shape.rot.y), MTXMODE_APPLY);
    Matrix_Translate(actor->world.pos.x, actor->world.pos.y + 4.0f, actor->world.pos.z, MTXMODE_NEW);
    gSPMatrix(POLY_XLU_DISP++, NEW_MATRIX(), G_MTX_LOAD);

    vtx = Graph_Alloc(gfxCtx, 6 * sizeof(Vtx));

    if (vtx == NULL) {
        goto end;
    }

    // Apex Point
    vtx[0].v.ob[0] = 0;
    vtx[0].v.ob[1] = 0;
    vtx[0].v.ob[2] = 0;

    // Left Point
    vtx[1].v.ob[0] = range * 1.25f * Math_SinS(actor->shape.rot.y - viewConeAngle);
    vtx[1].v.ob[1] = 0;
    vtx[1].v.ob[2] = range * 1.25f * Math_CosS(actor->shape.rot.y - viewConeAngle);

    // Right Point
    vtx[2].v.ob[0] = range * 1.25f * Math_SinS(actor->shape.rot.y + viewConeAngle);
    vtx[2].v.ob[1] = 0;
    vtx[2].v.ob[2] = range * 1.25f * Math_CosS(actor->shape.rot.y + viewConeAngle);

    // Left Bounds
    vtx[3].v.ob[0] = range * 1.25f * Math_SinS(actor->shape.rot.y - 0x4000);
    vtx[3].v.ob[1] = 0;
    vtx[3].v.ob[2] = range * 1.25f * Math_CosS(actor->shape.rot.y - 0x4000);

    // Right Bounds
    vtx[4].v.ob[0] = range * 1.25f * Math_SinS(actor->shape.rot.y + 0x4000);
    vtx[4].v.ob[1] = 0;
    vtx[4].v.ob[2] = range * 1.25f * Math_CosS(actor->shape.rot.y + 0x4000);

    // Back Bounds
    vtx[5].v.ob[0] = range * 1.6f * Math_SinS(actor->shape.rot.y + 0x8000);
    vtx[5].v.ob[1] = 0;
    vtx[5].v.ob[2] = range * 1.6f * Math_CosS(actor->shape.rot.y + 0x8000);

    gDPSetRenderMode(POLY_XLU_DISP++, G_RM_PASS, G_RM_AA_ZB_OPA_TERR); // Set render mode to mask circle
    gSPTexture(POLY_XLU_DISP++, 0, 0, 0, G_TX_RENDERTILE, G_OFF); // No Texture
    gDPSetCombineLERP(POLY_XLU_DISP++, PRIMITIVE, 0, PRIMITIVE, 0, PRIMITIVE, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED);
    gSPClearGeometryMode(POLY_XLU_DISP++, G_CULL_BOTH);
    gSPSetGeometryMode(POLY_XLU_DISP++, G_ZBUFFER);

    gSPVertex(POLY_XLU_DISP++, vtx, 6, 0);

    // 180 total degrees draw extra back triangle
    if (viewConeAngle <= 0x4000){
        gSP1Triangle(POLY_XLU_DISP++, 0, 1, 3, 0);
        gSP1Triangle(POLY_XLU_DISP++, 0, 2, 4, 0);
        gSP1Triangle(POLY_XLU_DISP++, 3, 4, 5, 0);
    } else{
        gSP1Triangle(POLY_XLU_DISP++, 0, 1, 5, 0);
        gSP1Triangle(POLY_XLU_DISP++, 0, 2, 5, 0);
    }

    end:

    Matrix_Translate(actor->world.pos.x, actor->world.pos.y + 3.0f, actor->world.pos.z, MTXMODE_NEW);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, NEW_MATRIX(), G_MTX_LOAD);

    if (canSeeResult){
        gDPSetEnvColor(POLY_XLU_DISP++,0xFF,0x7F,0x00,255);
    } else {
        gDPSetEnvColor(POLY_XLU_DISP++,255,255,255,255);
    }

   gSPDisplayList(POLY_XLU_DISP++, gEnDummy_DlGAttackCone);
}

void UpdateDamage(EnDummy* this, PlayState* play){

    // Check for normal damage immunity
    if ((this->actor.colorFilterTimer > 0 && this->actor.colorFilterParams == 0x4000)){
        return;
    }

    //If hit check for damage effects
    if (this->bodycollider.base.acFlags & AC_HIT){
        this->bodycollider.base.acFlags &= ~AC_HIT;

        if (this->movementState == Stunned && this->actor.colChkInfo.damageEffect == Dummy_DMGEFF_STUN) {
            return;
        }

        EnDummy_NumberParticle(this, play);
        EnDummy_Particle(this, play);

        if (this->movementState == Stunned) { // If already stunned, bypass stun effects
            goto apply_normal_damage;
        }

        if (this->actor.colChkInfo.damageEffect == Dummy_DMGEFF_STUN) {
            this->movementState = Stunned;
            this->stunTimer = stunTimerMAX;
            Actor_SetColorFilter(&this->actor, 0, 0x78, 0, stunTimerMAX);
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
            return;
        }

        if (this->actor.colChkInfo.damageEffect == Dummy_DMGEFF_ICE) {
            this->movementState = Stunned;
            this->iceTimer = iceTimerMAX;
            Actor_SetColorFilter(&this->actor, 0, 250, 0, iceTimerMAX);
            Actor_ApplyDamage(&this->actor);
            return;
        }

        apply_normal_damage:

        if (this->actor.colChkInfo.damageEffect == Dummy_DMGEFF_PANCAKE) {
            this->movementState = None;
            Actor_SetColorFilter(&this->actor, 0x4000, 0xFF, 0, 25);
            this->actor.scale.y = DUMMY_SCALE / 10.0f;
            
            if (Actor_ApplyDamage(&this->actor) == 0) { 
                this->actor.health = maxHealth;
            }

            return;
        }

        Animation_ChangeByInfo(&this->skelAnime,sDummyAnims, AnimRecoil);
        EnDummy_SetAction(this, EnDummy_Recoil);

        this->stunTimer = 0;
        this->iceTimer = 0;

        if (this->actor.colChkInfo.damageEffect == Dummy_DMGEFF_NONE) {
            this->movementState = None;
            Actor_SetColorFilter(&this->actor, 0x4000, 0xFF, 0, 8);
            
            if (Actor_ApplyDamage(&this->actor) == 0) { 
                this->actor.health = maxHealth;
            }

            return;
        }

        if (this->actor.colChkInfo.damageEffect == Dummy_DMGEFF_FIRE) {
            this->movementState = None;
            this->fireTimer = fireTimerMAX;
            Actor_SetColorFilter(&this->actor, 0x4000, 0xFF, 0, fireTimerMAX);
            
            if (Actor_ApplyDamage(&this->actor) == 0) { 
                this->actor.health = maxHealth;
            }

            return;
        }
    }
}

void EnDummy_Init(EnDummy* this, PlayState* play) {

    //Variable 000F - Type
    this->type = this->actor.params & 0xF;
    Actor_SetScale(&this->actor, DUMMY_SCALE);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 55.0f);

    this->actor.naviEnemyId = 0xA; //NAVI_ENEMY_DEKU_SCRUB 231 hint scrubs
    this->actor.targetMode = 2;

    this->actor.colChkInfo.cylRadius = 40;
    this->actor.colChkInfo.cylHeight = 70;

    this->bodycollider = (ColliderCylinder) {
        .base.shape            = COLSHAPE_CYLINDER,
        .base.actor            = &this->actor,
        .base.colType          = COLTYPE_TREE,
        .base.atFlags          = AT_TYPE_ENEMY | AT_ON,
        .base.ocFlags1         = OC1_ON | OC1_TYPE_ALL,
        .base.ocFlags2         = OC2_TYPE_1,
        .base.acFlags          = AC_ON | AC_TYPE_PLAYER,
        .info.ocElemFlags      = OCELEM_ON,
        .info.bumper.dmgFlags  = DMG_DEFAULT,
        .info.bumperFlags      = BUMP_ON | BUMP_HOOKABLE,
        .dim.radius            = 16,
        .dim.height            = 70,
    };

    this->actor.colChkInfo.health = maxHealth; //Assign Health values for actor
    this->actor.colChkInfo.damageTable = &sDamageTable; //Assign Damage Table for actor

    SKELANIME_INIT(gEnDummy_SkelBody, gEnDummy_AnimIdle, BODY_LIMB_MAX);

    EnDummy_SetAction(this,EnDummy_Idle);
}

void EnDummy_Destroy(EnDummy* this, PlayState* play) {
}

void EnDummy_Update(EnDummy* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    this->func(this, play);

    if (this->movementState != Stunned){
        SkelAnime_Update(&this->skelAnime); // Play animations if not stunned

        if (this->actor.isTargeted){
            Math_ApproachS(&this->actor.world.rot.y, Math_Vec3f_Yaw(&this->actor.pos, &player->actor.world.pos), 2, 500); // rotate to face player when being targeted
            this->actor.shape.rot.y = this->actor.world.rot.y;
        }
    }

    Actor_UpdateBgCheckInfo(play, &this->actor, 0.0f, 0.0f, 0.0f, UPDBGCHECKINFO_FLAG_2);

    UpdateDamage(this,play);

    Collider_UpdateCylinder(&this->actor, &this->bodycollider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->bodycollider.base);
    CollisionCheck_SetAT(play, &play->colChkCtx, &this->bodycollider.base);

    if (this->actor.colorFilterTimer == 0 || (this->fireTimer > 0 && this->fireTimer < fireTimerMAX - 4)){
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->bodycollider.base);
        Math_ApproachF(&this->actor.scale.y,DUMMY_SCALE,1.0f,DUMMY_SCALE/10.0f); // Reset from Pancake flattening
    }

    if (this->movementState == Stunned){
        DECR(this->iceTimer);
        DECR(this->stunTimer);

        if (this->stunTimer < stunTimerMAX - 4){
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->bodycollider.base);
        }

        if (this->iceTimer == 0 && this->stunTimer == 0){
            this->movementState = None; // Reset stun state
        }
    }

    if (CanSee(&this->actor,&player->actor,play,DEG_TO_BINANG(targetAngle),targetRange,targetHeight)){
        this->canSee = true;
    } else {
        this->canSee = false;
    }
}

s32 EnDummy_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {

    EnDummy* this = (EnDummy*)thisx;
    Player* player = GET_PLAYER(play);

    s16 targetYaw;
    s16 targetPitch;

    if (this->canSee){
        targetYaw = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
        targetPitch = Math_Vec3f_Pitch(&this->actor.pos,&player->actor.pos);
    } else {
        targetYaw = 0;
        targetPitch = 0;
    }

    if (this->movementState != Stunned){
        Math_SmoothStepToS(&this->currentHeadYaw,targetYaw,1.0,0x050,0x010);
        Math_SmoothStepToS(&this->currentHeadPitch,targetPitch,1.0,0x050,0x010);
    }

    if (limbIndex == BODY_LIMB_HEAD) {
        Matrix_Translate(pos->x, pos->y, pos->z, MTXMODE_APPLY);    // translate to root so rotations are centered
        Matrix_RotateY_s(this->currentHeadYaw, MTXMODE_APPLY);      // turn head left/right
        Matrix_RotateX_s(this->currentHeadPitch, MTXMODE_APPLY);    // tilt head up/down
        Matrix_Translate(-pos->x, -pos->y, -pos->z, MTXMODE_APPLY); // translate back to correct position
    }


    return false;
}

void EnDummy_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    EnDummy* this = (void*)thisx;

    Vec3f ModelOffset = {0.0f, 0.0f, 0.0f}; //targeting offset
    s32 bodyPartIndex = -1;

    if (limbIndex == BODY_LIMB_HEAD){
        Matrix_MultVec3f(&ModelOffset, &this->actor.focus.pos); // Align focus position to head bone position
    }

    if ((this->fireTimer != 0) || ((this->actor.colorFilterTimer != 0) && (this->actor.colorFilterParams & 0x4000))) {
        switch (limbIndex) {
            case BODY_LIMB_HEAD:
                bodyPartIndex = 0;
                break;
            case BODY_LIMB_HANDLE:
                bodyPartIndex = 1;
                break;
            case BODY_LIMB_CHEST:
                bodyPartIndex = 2;
                break;
            case BODY_LIMB_HAND_R:
                bodyPartIndex = 3;
                break;
            case BODY_LIMB_HAND_L:
                bodyPartIndex = 4;
                break;
        }
        
        if (bodyPartIndex >= 0) {
            Vec3f bodyPartPos;
            
            Matrix_MultVec3f(&ModelOffset, &bodyPartPos);

            this->bodyPartsPos[bodyPartIndex].x = bodyPartPos.x;
            this->bodyPartsPos[bodyPartIndex].y = bodyPartPos.y;
            this->bodyPartsPos[bodyPartIndex].z = bodyPartPos.z;
        }
    }
}

void EnDummy_Draw(EnDummy* this, PlayState* play) {

    SKELANIME_DRAW_OPA(EnDummy_OverrideLimbDraw, EnDummy_PostLimbDraw);

    Gfx_SetupDL_25Xlu(__gfxCtx);
    EnDummy_ParticleDraw(this,play);
    EnDummy_NumberParticleDraw(this,play);

    if (this->type == Debug){
        DrawDebugCone(&this->actor,targetRange,DEG_TO_BINANG(targetAngle),__gfxCtx,this->canSee);
    }

    // Spawn some ice        
    if (this->iceTimer == iceTimerMAX - 1) {
        Vec3f icePos = this->actor.world.pos;

        icePos.y += 50.0f;

        EffectSsEnIce_SpawnFlyingVec3f(play, &this->actor, &icePos, 150, 150, 150, 250, 235, 245, 255, 1.8f);
        icePos.x += 10.0f;
        icePos.y -= 8.0f;
        icePos.z += 10.0f;
        EffectSsEnIce_SpawnFlyingVec3f(play, &this->actor, &icePos, 150, 150, 150, 250, 235, 245, 255, 1.4f);
        icePos.x -= 20.0f;
        icePos.z -= 15.0f;
        EffectSsEnIce_SpawnFlyingVec3f(play, &this->actor, &icePos, 150, 150, 150, 250, 235, 245, 255, 1.7f);
        icePos.y -= 40.0f;
        icePos.z += 10.0f;
        EffectSsEnIce_SpawnFlyingVec3f(play, &this->actor, &icePos, 150, 150, 150, 250, 235, 245, 255, 2.2f);
        icePos.x += 10.0f;
        EffectSsEnIce_SpawnFlyingVec3f(play, &this->actor, &icePos, 150, 150, 150, 250, 235, 245, 255, 2.4f);
        icePos.x -= 15.0f;
        icePos.z -= 15.0f;
        EffectSsEnIce_SpawnFlyingVec3f(play, &this->actor, &icePos, 150, 150, 150, 250, 235, 245, 255, 2.2f);
    }
    

    if (this->fireTimer != 0) {
        this->actor.colorFilterTimer++; // increment filter timer to keep actor colored

        if (this->fireTimer >= 24 && this->fireTimer % 4 == 0) { // Spawn flames at 40,36,32,28,24 frames
            int index = (40 - this->fireTimer) / 4;              // 0,1,2,3,4 index
            EffectSsEnFire_SpawnVec3s(play, &this->actor,  &this->bodyPartsPos[index], 70, 0, 0,index);
        }

        this->fireTimer--;
    }
}