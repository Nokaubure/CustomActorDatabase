/*
description
 */

#include <uLib.h>
#include "ShockFungus.h"

#define ACT_ID 0x0201
#define OBJ_ID 0x0201
#define OBJ_H "object/0x0201-ShockFungus.h"
#include OBJ_H

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_2 | ACTOR_FLAG_4 | ACTOR_FLAG_10)

void ShockFungus_Init(ShockFungus* this, PlayState* play);
void ShockFungus_Destroy(ShockFungus* thisx, PlayState* play);
void ShockFungus_Update(ShockFungus* thisx, PlayState* play);
void ShockFungus_Draw(ShockFungus* thisx, PlayState* play);

void ShockFungus_SetupDead(ShockFungus* this, PlayState* play);
void ShockFungus_SetupIdle(ShockFungus* this, PlayState* play);
void ShockFungus_SetupRunning(ShockFungus* this, PlayState* play, f32 speed);
void ShockFungus_SetupStun(ShockFungus* this, PlayState* play);
void ShockFungus_SetupAttack(ShockFungus* this, PlayState* play);
void ShockFungus_SetupSpore(ShockFungus* this, PlayState* play);

void ShockFungus_Dead(ShockFungus* this, PlayState* play);
void ShockFungus_Idle(ShockFungus* this, PlayState* play);
void ShockFungus_Running(ShockFungus* this, PlayState* play);
void ShockFungus_Stun(ShockFungus* this, PlayState* play);
void ShockFungus_Attack(ShockFungus* this, PlayState* play);
void ShockFungus_Spore(ShockFungus* this, PlayState* play);
void ShockFungus_Roll(ShockFungus* this, PlayState* play);

#define Audio_PlayActorSound func_8002F7DC
static void* eyeTextures[] = { gShockfungus_TexEye0, gShockfungus_TexEye0, gShockfungus_TexEye1, gShockfungus_TexEye1, gShockfungus_TexEye2, gShockfungus_TexEye2, gShockfungus_TexEye1, gShockfungus_TexEye1,  };

static u8 Health[] =  {20,20,40};
static u8 Phase2Health[] =  {10,10,20};

Vec3f MoveDistByAngle(float distance, u16 angle){

    Vec3f result;

    angle += 0x3FFF;
    float distanceb = distance;

    float f0 = Math_CosS(angle);
    distance = distance * f0;
    result.x = distance;

    f0 = Math_SinS(angle);
    distanceb = distanceb * f0;
    result.z = distanceb;

    return result;

}

static ColliderCylinderInit HitboxStruct = {
    {
        COLTYPE_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x04, 0x10 },
        { 0xFFDFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { 65, 70, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit HurtboxStruct = {
    {
        COLTYPE_HIT3,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER | AC_TYPE_ALL,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x08 },
        { 0xFFDFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 30, 60, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit SporeStruct = {
    {
        COLTYPE_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x20000000, 0x03, 0x10 },
        { 0xFFDFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { 200, 20, 0, { 0, 0, 0 } },
};

#define DMGEFF_STUN 1
#define DMGEFF_FIRE 2
#define DMGEFF_FREEZE 3

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, DMGEFF_STUN),
    /* Deku stick    */ DMG_ENTRY(2, 0xF),
    /* Slingshot     */ DMG_ENTRY(0, 0x0),
    /* Explosive     */ DMG_ENTRY(2, 0xF),
    /* Boomerang     */ DMG_ENTRY(0, 0x0),
    /* Normal arrow  */ DMG_ENTRY(0, 0x0),
    /* Hammer swing  */ DMG_ENTRY(2, 0xF),
    /* Hookshot      */ DMG_ENTRY(0, DMGEFF_STUN),
    /* Kokiri sword  */ DMG_ENTRY(1, 0xF),
    /* Master sword  */ DMG_ENTRY(2, 0xF),
    /* Giant's Knife */ DMG_ENTRY(4, 0xF),
    /* Fire arrow    */ DMG_ENTRY(4, DMGEFF_FIRE),
    /* Ice arrow     */ DMG_ENTRY(2, DMGEFF_FREEZE),
    /* Light arrow   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(0, 0x0),
    /* Fire magic    */ DMG_ENTRY(4, DMGEFF_FIRE),
    /* Ice magic     */ DMG_ENTRY(2, DMGEFF_FREEZE),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(1, 0xF),
    /* Giant spin    */ DMG_ENTRY(4, 0xF),
    /* Master spin   */ DMG_ENTRY(2, 0xF),
    /* Kokiri jump   */ DMG_ENTRY(2, 0xF),
    /* Giant jump    */ DMG_ENTRY(8, 0xF),
    /* Master jump   */ DMG_ENTRY(4, 0xF),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(4, 0xF),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};



const ActorInit ShockFungus_InitVars = {
    ACT_ID, 
    ACTORCAT_ENEMY,
    FLAGS,
    OBJ_ID,
    sizeof(ShockFungus),
    (ActorFunc)ShockFungus_Init,
    (ActorFunc)ShockFungus_Destroy,
    (ActorFunc)ShockFungus_Update,
    (ActorFunc)ShockFungus_Draw,
};

Vec3s Math_Vec3f_ToVec3s(Vec3f* src)
{
    Vec3s ret;
    ret.x = src->x;
    ret.y = src->y;
    ret.z = src->z;

    return ret;
}

void ShockFungus_Init(ShockFungus* this, PlayState* play) {



    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 2.0f);
    Actor_SetScale(&this->actor,0.3f);
    SkelAnime_InitFlex(play, &this->skelAnime, gShockfungus_SkelCube, gShockfungus_AnimIdle, this->jointTable, this->morphTable, CUBE_LIMB_MAX);

    //this->actor.naviEnemyId = 0; //Navi Text ID
    
    this->type = this->actor.params & 0x000F;
    this->actor.targetMode = 3;
    this->actor.health = Health[this->type];
    this->actor.mass = MASS_IMMOVABLE;
    this->actor.gravity = -0.5f;
    this->rollcd = 20;

    

    Collider_InitCylinder(play, &this->Hitbox);
    Collider_SetCylinder(play, &this->Hitbox, &this->actor, &HitboxStruct);
    Collider_InitCylinder(play, &this->Hurtbox);
    Collider_SetCylinder(play, &this->Hurtbox, &this->actor, &HurtboxStruct);
    Collider_InitCylinder(play, &this->Sporehitbox);
    Collider_SetCylinder(play, &this->Sporehitbox, &this->actor, &SporeStruct);

    this->actor.colChkInfo.damageTable = &sDamageTable;
    ShockFungus_SetupIdle(this,play);

}

void ShockFungus_Destroy(ShockFungus* this, PlayState* play) {
    Collider_DestroyCylinder(play, &this->Hitbox);
    Collider_DestroyCylinder(play, &this->Hurtbox);
    Collider_DestroyCylinder(play, &this->Sporehitbox);
    if (this->type > 0)
    {
        func_800F5B58(); //reset music
    }
}

void ShockFungus_SetupDead(ShockFungus* this, PlayState* play) {
    Animation_Change(&this->skelAnime, gShockfungus_AnimDying, 1.0f, 0.0f,
                     Animation_GetLastFrame(&gShockfungus_AnimDying), ANIMMODE_ONCE, -2.0f);
    this->actionFunc = ShockFungus_Dead;
    this->state = STATE_DEAD;
    this->actor.speedXZ = 0.0f;
}

static Vec3f sDeadEffectVel = { 0.0f, 0.0f, 0.0f };
void ShockFungus_Dead(ShockFungus* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);

    if (this->skelAnime.curFrame >= Animation_GetLastFrame(&gShockfungus_AnimDying)) {
        if (this->skelAnime.playSpeed != 0.0f)
        {
            this->skelAnime.playSpeed = 0.0f;
            this->deadtimer = 10;
            this->actor.flags &= ~ACTOR_FLAG_0;

            Vec3f accel;
            Vec3f pos;
            Vec3f dest;
            pos.x = this->actor.world.pos.x;
            pos.y = (this->actor.world.pos.y + 5.0f) - 10.0f;
            pos.z = this->actor.world.pos.z;
            Vec3f dist = MoveDistByAngle(-30.0f, -this->actor.world.rot.y);
            Math_Vec3f_Sum(&pos, &dist, &dest);

            accel = sDeadEffectVel;
            accel.y = 0.03f;
            EffectSsKFire_Spawn(play, &dest, &sDeadEffectVel, &accel, 60, 0);
        }
        else
        {
            this->deadtimer--;
            if (this->deadtimer == 0)
            {
                Actor_Kill(&this->actor);
                Audio_PlaySfxGeneral(NA_SE_EN_EXTINCT, &this->actor.projectedPos, 4, &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
            }
        }
        this->actor.scale.y = 0.3f * (this->deadtimer / 10.0f);

        
        
    }

}

void ShockFungus_SetupIdle(ShockFungus* this, PlayState* play) {
    Animation_Change(&this->skelAnime, gShockfungus_AnimIdle, 1.0f, 0.0f,
                     Animation_GetLastFrame(&gShockfungus_AnimIdle), ANIMMODE_LOOP, -2.0f);
    this->actionFunc = ShockFungus_Idle;
    this->state = STATE_STAND;
    this->Timer = Rand_S16Offset(50, 50);
    this->actor.speedXZ = 0.0f;
}

void ShockFungus_Idle(ShockFungus* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    this->Timer--;
    if (this->Timer <= 0 && this->aggro == 0)
    {
        ShockFungus_SetupRunning(this,play,1.0f);
        this->actor.world.rot.y = this->actor.shape.rot.y = Rand_S16Offset(0, 0x7FFF) *2;
        this->Timer = Rand_S16Offset(50, 50);
    }
}

void ShockFungus_SetupRunning(ShockFungus* this, PlayState* play, f32 speed) {
    Animation_Change(&this->skelAnime, gShockfungus_AnimRunning, speed, 0.0f,
                     Animation_GetLastFrame(&gShockfungus_AnimRunning), ANIMMODE_LOOP, -2.0f);
    this->actionFunc = ShockFungus_Running;
    this->state = STATE_WALK;
    this->actor.speedXZ = speed;

}

void ShockFungus_Running(ShockFungus* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    this->Timer--;
    if (this->aggro > 0)
    {

        Math_ApproachS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 3, 1500);
        this->actor.shape.rot.y = this->actor.world.rot.y;
        this->Timer = 1;
        if (this->actor.xzDistToPlayer < 220.0f && (this->actor.bgCheckFlags & 0x80) == 0x80)
        {
            if (this->SporeTimer <= 0 && this->actor.xzDistToPlayer < 120.0f && (this->actor.health <= Phase2Health[this->type] || (this->actor.health > Phase2Health[this->type] && Rand_ZeroOne() < 0.4f)))
            {
                ShockFungus_SetupSpore(this,play);
            }
            else if (Actor_IsFacingPlayer(&this->actor, 0x21C7) && this->invincibilityTimer < 2)
            {
                this->actor.world.rot.y = this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
                ShockFungus_SetupAttack(this,play);
            }
            
        }
    }

    if (this->skelAnime.curFrame == 18.0f || this->skelAnime.curFrame == 48.0f)
    {
        
       func_80033480(play,&this->actor.pos,70.0f, 0x6, 0x99, 0x5A, 0x1); // dust particle

       Audio_PlayActorSound(&this->actor,NA_SE_EN_DODO_J_WALK);
    }
    if (this->Timer <= 0)
    {
        ShockFungus_SetupIdle(this,play);
    }
}

void ShockFungus_SetupStun(ShockFungus* this, PlayState* play) {
    Animation_Change(&this->skelAnime, gShockfungus_AnimIdle, 0.0f, 0.0f,
                     Animation_GetLastFrame(&gShockfungus_AnimRunning), ANIMMODE_ONCE, -2.0f);
    this->actionFunc = ShockFungus_Stun;
    this->state = STATE_STUN;
    this->Timer = 60;
    this->actor.speedXZ = 0.0f;

}

void ShockFungus_Stun(ShockFungus* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    Actor_SetColorFilter(&this->actor, 0, 180, 0, 2);
    this->Timer--;
    if (this->Timer <= 0)
    {
        ShockFungus_SetupIdle(this,play);
    }
}

void ShockFungus_SetupAttack(ShockFungus* this, PlayState* play) {
    Animation_Change(&this->skelAnime, gShockfungus_AnimNotToUse, 1.0f, 0.0f,
                     Animation_GetLastFrame(&gShockfungus_AnimNotToUse), ANIMMODE_ONCE, -2.0f);

    this->actionFunc = ShockFungus_Attack;
    this->state = STATE_ATTACK;
    this->skelAnime.curFrame = this->actor.health <= Phase2Health[this->type] ? 8.0f : 3.0f;
    this->actor.speedXZ = 0.0f;


}

void ShockFungus_SetupRoll(ShockFungus* this, PlayState* play) {
    Animation_Change(&this->skelAnime, gShockfungus_AnimRoll, 2.0f, 0.0f,
                     Animation_GetLastFrame(&gShockfungus_AnimNotToUse), ANIMMODE_ONCE, -2.0f);

    this->actionFunc = ShockFungus_Roll;
    this->state = STATE_ROLL;
}

void ShockFungus_Roll(ShockFungus* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    this->actor.speedXZ *= 0.95f;
    if (this->skelAnime.curFrame < 9.0f)
    {
        Math_ApproachS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 3, 3000);
        this->actor.shape.rot.y = this->actor.world.rot.y;
    }
    if (this->skelAnime.curFrame > 4.0f && this->skelAnime.curFrame < 27.0f)
    {
        func_80033480(play,&this->actor.pos,70.0f, 0x6, 0xB4, 0x5A, 0x1); // dust particle

    }
    if (this->skelAnime.curFrame >= 4.0f && this->skelAnime.curFrame <= 7.0f)
    {
    this->actor.speedXZ = 10.0f;
    Audio_PlayActorSound(&this->actor,NA_SE_PL_ROLL_DUST);
    Audio_PlayActorSound(&this->actor,NA_SE_PL_ROLL_DUST);
    }
    if (this->skelAnime.curFrame >= Animation_GetLastFrame(&gShockfungus_AnimRoll))
    {
        ShockFungus_SetupIdle(this,play);
        this->actor.speedXZ = 0.0f;
    }
}

void ShockFungus_Attack(ShockFungus* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    if (this->skelAnime.curFrame == 13.0f)
    {
        this->actor.speedXZ = 2.0f * (1.0f + this->actor.xzDistToPlayer / 70.0f);
        this->actor.velocity.y = 3.0f * (CLAMP_MIN(this->actor.xzDistToPlayer / 200.0f, 1.0f));
        Audio_PlayActorSound(&this->actor,NA_SE_EN_STAL_JUMP);
    }
    else if (this->skelAnime.curFrame > 13.0f)
    {
        if (this->actor.velocity.y > 0.0f && this->skelAnime.curFrame >= 17.0f)
        {
            this->skelAnime.playSpeed = 0.0f;
        }
        else if (this->actor.velocity.y <= 0.0f && !(this->actor.bgCheckFlags & BGCHECKFLAG_GROUND))
        {
            this->skelAnime.playSpeed = this->skelAnime.curFrame <= 26.0f ? 1.0f : 0.0f;
            if (this->skelAnime.curFrame < 18.0f)
                this->skelAnime.curFrame = 18.0f;
        }
        else if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND))
        {
            if (this->skelAnime.curFrame > 26.0f)
            {
                f32 tmp = this->skelAnime.playSpeed;
                this->skelAnime.playSpeed = this->actor.health <= Phase2Health[this->type] ? 2.5f : 2.0f;
                if (tmp != this->skelAnime.playSpeed && this->actor.health <= Phase2Health[this->type] && this->rollcd == 0)
                {
                    
                    Math_ApproachS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 3, 1500);
                    this->actor.shape.rot.y = this->actor.world.rot.y;
                    ShockFungus_SetupRoll(this,play);
                }
            }

        }
    }
    if (this->skelAnime.curFrame >= Animation_GetLastFrame(&gShockfungus_AnimNotToUse))
    {
        ShockFungus_SetupIdle(this,play);
    }
}

void ShockFungus_SetupSpore(ShockFungus* this, PlayState* play) {
    Animation_Change(&this->skelAnime, gShockfungus_AnimSpores, 1.0f, 0.0f,
                     Animation_GetLastFrame(&gShockfungus_AnimSpores), ANIMMODE_ONCE, -2.0f);
    this->actionFunc = ShockFungus_Spore;
    this->state = STATE_SPORE;
    this->actor.speedXZ = 0.0f;

    


}

void ShockFungus_Spore(ShockFungus* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    if (this->skelAnime.curFrame >= 15.0f && this->SporeTimer <= 0)
    {
        Audio_PlayActorSound(&this->actor,NA_SE_EN_FANTOM_THUNDER);
        this->SporeTimer = 51;
        Vec3s tmp = Math_Vec3f_ToVec3s(&this->actor.world.pos);
        Collider_SetCylinderPosition(&this->Sporehitbox, &tmp);
        

    }
    if (this->skelAnime.curFrame >= Animation_GetLastFrame(&gShockfungus_AnimSpores))
    {
        ShockFungus_SetupIdle(this,play);
    }
}


void ShockFungus_Update(ShockFungus* this, PlayState* play) {


    if (this->invincibilityTimer != 0) {
        this->invincibilityTimer--;
    }

    if (this->SporeTimer != 0) {
        this->SporeTimer--;
    }

    Actor_SetFocus( &this->actor, 60.0f);

    
    Actor_ApplyDamage(&this->actor);

    if (this->type > 0 && !this->musicOn && this->aggro > 0)
    {
        func_800F5ACC(NA_BGM_MINI_BOSS); //begin midboss music on aggro depending on params
    }

    if (this->actor.health <= Phase2Health[this->type] && this->rollcd > 0)
    {
        this->rollcd--;
    }

    if (this->actor.health == 0 && this->state != STATE_DEAD)
    {
        Actor_SetColorFilter(&this->actor, 0x4000, 255, 0, 5);
        ShockFungus_SetupDead(this,play);
        Audio_PlayActorSound(&this->actor,NA_SE_EN_TEKU_DEAD);
        Item_DropCollectibleRandom(play, NULL, &this->actor.world.pos, 0x30);
    }
    else if (this->actor.damageEffect == 0x0F || this->actor.damageEffect == DMGEFF_FIRE)
    {
        Actor_SetColorFilter(&this->actor, 0x4000, 255, 0, 5);
        this->invincibilityTimer = 13;
        this->aggro += 120;
        Audio_PlayActorSound(&this->actor,NA_SE_EN_TEKU_DAMAGE);
        if (this->state == STATE_STUN) ShockFungus_SetupIdle(this,play);
        if (this->actor.damageEffect == DMGEFF_FIRE)
        {
            Vec3f pos = this->actor.world.pos;
            pos.y += 30.f;
            EffectSsEnFire_SpawnVec3f(play, &this->actor, &pos, 250, 0, 0, -1);
        }
        
    }
    else if ((this->actor.damageEffect == DMGEFF_STUN || this->actor.damageEffect == DMGEFF_FREEZE) && this->state != STATE_STUN)
    {
        //stun
        ShockFungus_SetupStun(this,play);
        Audio_PlayActorSound(&this->actor,0x389E);
        this->aggro += 200;
        
        if (this->actor.damageEffect == DMGEFF_FREEZE)
        {
        Vec3s pos = Math_Vec3f_ToVec3s(&this->actor.world.pos);
        this->Timer *= 2;

        pos.y += 20.f;
        EffectSsEnIce_SpawnFlyingVec3s(play, &this->actor, &pos, 150, 150, 150, 250, 235, 245,255, 2.5f);
        pos.y += 40.f;
        EffectSsEnIce_SpawnFlyingVec3s(play, &this->actor, &pos, 150, 150, 150, 250, 235, 245,255, 3.5f);
        }


    }


    if (this->state != STATE_DEAD)
    {
    //whenever to keep aggro or not
        if (this->aggro > 0 && this->actor.xzDistToPlayer > (this->actor.health <= Phase2Health[this->type] ? 500.0f : 300.0f))
        {
            this->aggro--;
            if (this->aggro == 0)
                ShockFungus_SetupIdle(this,play);
        }
        else if (!this->aggro && this->actor.xzDistToPlayer < (this->actor.health <= Phase2Health[this->type] ? 400.0f : 250.0f))
        {
            this->aggro = 20;
            
        }
        if (this->aggro > 0 && (this->state == STATE_STAND || 
            (this->state == STATE_WALK && this->actor.health <= Phase2Health[this->type] && this->actor.speedXZ < 3.0f) ||
            (this->state == STATE_WALK && this->actor.health > Phase2Health[this->type] && this->actor.speedXZ < 2.0f) ))
        {
            ShockFungus_SetupRunning(this,play,this->actor.health > Phase2Health[this->type] ? 2.0f : 3.0f);
        }


    //enable electric spore hitbox
       if (this->SporeTimer > 0)
        {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->Sporehitbox.base);
            Vec3f tmp;
            Math_Vec3s_ToVec3f(&tmp,&this->Sporehitbox.dim.pos);
            func_8002F8F0(&this->actor, NA_SE_PL_SPARK - SFX_FLAG);
            this->Sporehitbox.dim.radius = 200 - this->SporeTimer*2;
            for (u8 i = 0; i < 10; i++)
            {
            Vec3f dist = MoveDistByAngle(Rand_ZeroOne()*(200.0f - this->SporeTimer*2.0f), Rand_S16Offset(0, 0x7FFF)*2);
            Vec3f dest;
            Math_Vec3f_Sum(&tmp, &dist, &dest);
            EffectSsFhgFlash_SpawnShock(play, &this->actor, &dest, 100, 0);
            }
        }

    }

    this->actionFunc(this, play);

    Actor_MoveForward(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 50.0f, 50.0f, 100.0f,
                                UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);

    Collider_UpdateCylinder(&this->actor, &this->Hurtbox);

    CollisionCheck_SetOC(play, &play->colChkCtx, &this->Hurtbox.base);
    if (this->state != STATE_DEAD && this->invincibilityTimer <= 0) CollisionCheck_SetAC(play, &play->colChkCtx, &this->Hurtbox.base);

    if ((this->state == STATE_ATTACK && this->actor.velocity.y != 0 && this->skelAnime.curFrame >= 13.0f && this->skelAnime.curFrame <= 26.0f) ||
    (this->state == STATE_ROLL && this->skelAnime.curFrame >= 4.0f && this->skelAnime.curFrame < 27.0f) )
    {
    Collider_UpdateCylinder(&this->actor, &this->Hitbox);
    CollisionCheck_SetAT(play, &play->colChkCtx, &this->Hitbox.base);
    }

    if(((this->actor.bgCheckFlags & 0x80) == 0x80 && this->actor.velocity.y != 0) || this->actor.bgCheckFlags == 0x83)
    {

        if (this->state == STATE_ATTACK) this->actor.speedXZ = 0.0f;

       func_80033480(play,&this->actor.pos,70.0f, 0x6, 0xB4, 0x5A, 0x1); // dust particle

       Audio_PlayActorSound(&this->actor,0x387B);
    }

    // eye index
    if (this->state != STATE_STUN)
    {
    if (this->state != STATE_DEAD)
    {
        if (this->blinkTimer != 0) {
            this->blinkTimer--;
        } else {
            this->eyeTexIndex++;

            if (this->eyeTexIndex >= 8) {
                this->eyeTexIndex = 0;
                this->blinkTimer = Rand_ZeroFloat(60.0f) + 20.0f;
            }
        }
    }
    else
    {
        this->eyeTexIndex = 5;
    }
}

}

s32 ShockFungus_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {

    return false;

}

void ShockFungus_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {

    static Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };


    Actor* this = (Actor*)thisx;

    if (limbIndex == CUBE_LIMB_HEAD) {
        Matrix_MultVec3f(&zeroVec, &this->focus.pos);
    }


}

void ShockFungus_Draw(ShockFungus* this, PlayState* play) {
    
    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[this->eyeTexIndex]));
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount, ShockFungus_OverrideLimbDraw,
                      ShockFungus_PostLimbDraw, this);

}