/*
 * File: z_en_egblock.c
 * Overlay: ovl_En_Egblock
 * Description: Unused pillar. Eyegore can destroy it.
 */

#include "z_en_egblock.h"
//#include "objects/object_eg/object_eg.h"

#define ACT_ID 0x0207
#define OBJ_ID 0x0201

#define FLAGS (ACTOR_FLAG_27)


void Matrix_RotateXS(s16 x, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempY;
    f32 tempZ;

    if (mode == MTXMODE_APPLY) {
        if (x != 0) {
            cmf = sCurrentMatrix;

            sin = Math_SinS(x);
            cos = Math_CosS(x);

            tempY = cmf->xy;
            tempZ = cmf->xz;
            cmf->xy = tempY * cos + tempZ * sin;
            cmf->xz = tempZ * cos - tempY * sin;

            tempY = cmf->yy;
            tempZ = cmf->yz;
            cmf->yy = tempY * cos + tempZ * sin;
            cmf->yz = tempZ * cos - tempY * sin;

            tempY = cmf->zy;
            tempZ = cmf->zz;
            cmf->zy = tempY * cos + tempZ * sin;
            cmf->zz = tempZ * cos - tempY * sin;

            tempY = cmf->wy;
            tempZ = cmf->wz;
            cmf->wy = tempY * cos + tempZ * sin;
            cmf->wz = tempZ * cos - tempY * sin;
        }
    } else {
        cmf = sCurrentMatrix;

        if (x != 0) {
            sin = Math_SinS(x);
            cos = Math_CosS(x);
        } else {
            sin = 0.0f;
            cos = 1.0f;
        }

        cmf->yx = 0.0f;
        cmf->zx = 0.0f;
        cmf->wx = 0.0f;
        cmf->xy = 0.0f;
        cmf->wy = 0.0f;
        cmf->xz = 0.0f;
        cmf->wz = 0.0f;
        cmf->xw = 0.0f;
        cmf->yw = 0.0f;
        cmf->zw = 0.0f;
        cmf->xx = 1.0f;
        cmf->ww = 1.0f;
        cmf->yy = cos;
        cmf->zz = cos;
        cmf->zy = sin;
        cmf->yz = -sin;
    }
}
void Matrix_RotateYS(s16 y, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempX;
    f32 tempZ;

    if (mode == MTXMODE_APPLY) {
        if (y != 0) {
            cmf = sCurrentMatrix;

            sin = Math_SinS(y);
            cos = Math_CosS(y);

            tempX = cmf->xx;
            tempZ = cmf->xz;
            cmf->xx = tempX * cos - tempZ * sin;
            cmf->xz = tempX * sin + tempZ * cos;

            tempX = cmf->yx;
            tempZ = cmf->yz;
            cmf->yx = tempX * cos - tempZ * sin;
            cmf->yz = tempX * sin + tempZ * cos;

            tempX = cmf->zx;
            tempZ = cmf->zz;
            cmf->zx = tempX * cos - tempZ * sin;
            cmf->zz = tempX * sin + tempZ * cos;

            tempX = cmf->wx;
            tempZ = cmf->wz;
            cmf->wx = tempX * cos - tempZ * sin;
            cmf->wz = tempX * sin + tempZ * cos;
        }
    } else {
        cmf = sCurrentMatrix;

        if (y != 0) {
            sin = Math_SinS(y);
            cos = Math_CosS(y);
        } else {
            sin = 0.0f;
            cos = 1.0f;
        }

        cmf->yx = 0.0f;
        cmf->wx = 0.0f;
        cmf->xy = 0.0f;
        cmf->zy = 0.0f;
        cmf->wy = 0.0f;
        cmf->yz = 0.0f;
        cmf->wz = 0.0f;
        cmf->xw = 0.0f;
        cmf->yw = 0.0f;
        cmf->zw = 0.0f;
        cmf->yy = 1.0f;
        cmf->ww = 1.0f;
        cmf->xx = cos;
        cmf->zz = cos;
        cmf->zx = -sin;
        cmf->xz = sin;
    }
}
void Matrix_RotateZS(s16 z, MatrixMode mode) {
    MtxF* cmf;
    f32 sin;
    f32 cos;
    f32 tempX;
    f32 tempY;
    f32 zero = 0.0;
    f32 one = 1.0;

    if (mode == MTXMODE_APPLY) {
        if (z != 0) {
            cmf = sCurrentMatrix;

            sin = Math_SinS(z);
            cos = Math_CosS(z);

            tempX = cmf->xx;
            tempY = cmf->xy;
            cmf->xx = tempX * cos + tempY * sin;
            cmf->xy = tempY * cos - tempX * sin;

            tempX = cmf->yx;
            tempY = cmf->yy;
            cmf->yx = tempX * cos + tempY * sin;
            cmf->yy = tempY * cos - tempX * sin;

            tempX = cmf->zx;
            tempY = cmf->zy;
            cmf->zx = tempX * cos + tempY * sin;
            cmf->zy = tempY * cos - tempX * sin;

            tempX = cmf->wx;
            tempY = cmf->wy;
            cmf->wx = tempX * cos + tempY * sin;
            cmf->wy = tempY * cos - tempX * sin;
        }
    } else {
        cmf = sCurrentMatrix;

        if (z != 0) {
            sin = Math_SinS(z);
            cos = Math_CosS(z);
        } else {
            sin = zero;
            cos = one;
        }

        cmf->zx = zero;
        cmf->wx = zero;
        cmf->zy = zero;
        cmf->wy = zero;
        cmf->xz = zero;
        cmf->yz = zero;
        cmf->wz = zero;
        cmf->xw = zero;
        cmf->yw = zero;
        cmf->zw = zero;
        cmf->zz = one;
        cmf->ww = one;
        cmf->xx = cos;
        cmf->yy = cos;
        cmf->yx = sin;
        cmf->xy = -sin;
    }
}



#define THIS ((EnEgblock*)thisx)

typedef enum {
    /* 0 */ EGBLOCK_EFFECT_DEBRIS_SOLID,
    /* 1 */ EGBLOCK_EFFECT_DEBRIS_FLAT
} EnEgblockEffectType;

void EnEgblock_Init(Actor* thisx, PlayState* play);
void EnEgblock_Destroy(Actor* thisx, PlayState* play);
void EnEgblock_Update(Actor* thisx, PlayState* play);
void EnEgblock_Draw(Actor* thisx, PlayState* play2);

void EnEgblock_Active(EnEgblock* this, PlayState* play);
void EnEgblock_DoNothing(EnEgblock* this, PlayState* play);
void EnEgblock_Inactive(EnEgblock* this, PlayState* play);

void EnEgblock_SpawnEffect(EnEgblock* this, Vec3f* pos, s16 lifetime, s16 arg3);
void EnEgblock_UpdateEffects(EnEgblock* this, PlayState* play);
void EnEgblock_DrawEffects(EnEgblock* this, PlayState* play);
void EnEgblock_InitDynaPoly(EnEgblock* this, PlayState* play, CollisionHeader* collision, s32 flags);

ActorInit En_Egblock_InitVars = {
    ACT_ID,
    ACTORCAT_PROP,
    FLAGS,
    OBJ_ID,
    sizeof(EnEgblock),
    (ActorFunc)EnEgblock_Init,
    (ActorFunc)EnEgblock_Destroy,
    (ActorFunc)EnEgblock_Update,
    (ActorFunc)EnEgblock_Draw,
};

void EnEgblock_Init(Actor* thisx, PlayState* play) {
    EnEgblock* this = THIS;
    CollisionHeader* colHeader = NULL;
    s32 pad;

    EnEgblock_InitDynaPoly(this, play, (void*)0x06001820, 0);
    //this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);
    this->dyna.actor.colChkInfo.mass = MASS_IMMOVABLE;

    this->inactive = false;
    Actor_SetScale(&this->dyna.actor, 0.5f);
    this->type = EGBLOCK_GET_TYPE(&this->dyna.actor);
    this->paramF80 = EGBLOCK_GET_PARAM_F80(&this->dyna.actor);
    this->param7F = EGBLOCK_GET_PARAM_7F(&this->dyna.actor);
    if (this->type == EGBLOCK_TYPE_ACTIVE) {
        this->dyna.actor.colChkInfo.health = 1;
        this->actionFunc = EnEgblock_Active;
    } else {
        this->actionFunc = EnEgblock_DoNothing;
    }
}

void EnEgblock_InitDynaPoly(EnEgblock* this, PlayState* play, CollisionHeader* collision, s32 flags) {
    s32 pad;
    CollisionHeader* colHeader = NULL;
    s32 pad2;

    DynaPolyActor_Init(&this->dyna, flags);
    CollisionHeader_GetVirtual(collision, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);


}

void EnEgblock_Destroy(Actor* thisx, PlayState* play) {
    EnEgblock* this = THIS;

    if (this->dyna.actor.colChkInfo.health == 1) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
    }
}

void EnEgblock_Active(EnEgblock* this, PlayState* play) {
    if (this->dyna.actor.colChkInfo.health <= 0) {
        s32 i;

        //! @bug Egblock doesn't have an effect with index 2, so half of these spawns do nothing.
        for (i = 0; i < ARRAY_COUNT(this->effects); i++) {
            EnEgblock_SpawnEffect(this, &this->dyna.actor.world.pos, 30, (i & 1) + EGBLOCK_EFFECT_DEBRIS_FLAT);
        }
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
        Actor_SpawnFloorDustRing(play, &this->dyna.actor, &this->dyna.actor.world.pos, 30.0f, 30, 10.0f, 100, 30, true);
        this->inactive = true;
        this->timer = 50;
        this->actionFunc = EnEgblock_Inactive;
    }
}

void EnEgblock_Inactive(EnEgblock* this, PlayState* play) {
    if (this->timer == 0) {
        Actor_Kill(&this->dyna.actor);
    }
}

void EnEgblock_DoNothing(EnEgblock* this, PlayState* play) {
}

void EnEgblock_Update(Actor* thisx, PlayState* play) {
   
    EnEgblock* this = THIS;

    this->actionFunc(this, play);

    DECR(this->timer);

    EnEgblock_UpdateEffects(this, play);
}

void EnEgblock_Draw(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnEgblock* this = THIS;

    Gfx_SetupDL_25Opa(play->state.gfxCtx);

    if (this->inactive != true) {
        OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetPrimColor(POLY_OPA_DISP++, 0x00, 0x80, 255, 255, 255, 255);
        gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 255);
        gSPDisplayList(POLY_OPA_DISP++, 0x06001698);

        CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
    }

    EnEgblock_DrawEffects(this, play);
}

void EnEgblock_SpawnEffect(EnEgblock* this, Vec3f* pos, s16 lifetime, s16 arg3) {
    EnEgblockEffect* effect = this->effects;
    s16 i;

    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (!effect->isActive) {
            effect->isActive = true;
            effect->pos = *pos;
            effect->pos.x += Rand_CenteredFloat(50.0f);
            effect->pos.z += Rand_CenteredFloat(50.0f);
            effect->timer = lifetime;
            effect->alpha = 255;
            effect->type = arg3;
            effect->accel.x = Rand_ZeroOne() - 0.5f;
            effect->accel.y = -1.0f;
            effect->accel.z = Rand_ZeroOne() - 0.5f;
            effect->velocity.x = 2.0f * (Rand_ZeroOne() - 0.5f);
            effect->velocity.y = 5.0f + (10.0f * Rand_ZeroOne());
            effect->velocity.z = 2.0f * (Rand_ZeroOne() - 0.5f);
            effect->scale = 0.5f + (0.2f * Rand_ZeroFloat(1.0f));
            effect->rot.x = Rand_CenteredFloat(0x7530);
            effect->rot.y = Rand_CenteredFloat(0x7530);
            effect->rot.z = Rand_CenteredFloat(0x7530);
            break;
        }
    }
}

void EnEgblock_UpdateEffects(EnEgblock* this, PlayState* play) {
    EnEgblockEffect* effect = this->effects;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (effect->isActive) {
            if (effect->type != EGBLOCK_EFFECT_DEBRIS_SOLID) {
                effect->rot.x -= 0x64;
                effect->rot.y -= 0x64;
                effect->rot.z -= 0x64;
            }

            effect->pos.x += effect->velocity.x;
            effect->pos.y += effect->velocity.y;
            effect->pos.z += effect->velocity.z;

            effect->velocity.x += effect->accel.x;
            effect->velocity.y += effect->accel.y;
            effect->velocity.z += effect->accel.z;

            if (effect->timer) {
                effect->timer--;
            } else {
                effect->alpha -= 10;
                if (effect->alpha < 10) {
                    effect->isActive = false;
                }
            }
        }
    }
}

void EnEgblock_DrawEffects(EnEgblock* this, PlayState* play) {
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    EnEgblockEffect* effect = this->effects;
    s16 i;

    OPEN_DISPS(gfxCtx, __FILE__, __LINE__);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);

    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (effect->isActive) {
            switch (effect->type) {
                case EGBLOCK_EFFECT_DEBRIS_SOLID:
                case EGBLOCK_EFFECT_DEBRIS_FLAT:
                    Matrix_Push();

                    Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
                    Matrix_Scale(effect->scale, effect->scale, effect->scale, MTXMODE_APPLY);
                    Matrix_RotateYS(effect->rot.y, MTXMODE_APPLY);
                    Matrix_RotateXS(effect->rot.x, MTXMODE_APPLY);
                    Matrix_RotateZS(effect->rot.z, MTXMODE_APPLY);

                    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gfxCtx, __FILE__, __LINE__), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                    gDPSetPrimColor(POLY_OPA_DISP++, 0x00, 0x80, 255, 255, 255, 255);
                    gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, effect->alpha);

                    if (effect->type == EGBLOCK_EFFECT_DEBRIS_SOLID) {
                        gSPDisplayList(POLY_OPA_DISP++, 0x06001918);
                    } else {
                        gSPDisplayList(POLY_OPA_DISP++, 0x06001bf8);
                    }

                    Matrix_Pop();
                    break;

                default:
                    break;
            }
        }
    }

    CLOSE_DISPS(gfxCtx, __FILE__, __LINE__);
}
