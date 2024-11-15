#ifndef __EN_DUMMY_H__
#define __EN_DUMMY_H__

#include <uLib.h>
#include "object/0x0220-EnDummy.h"
#include <ulib_math.h>
#include "uLib_vector.h"

#define SKELANIME_MEMBER(limb_max) \
    SkelAnime skelAnime; \
    Vec3s jointTbl[limb_max]; \
    Vec3s morphTbl[limb_max]
#define SKELANIME_INIT(skeleton, animation, maxLimb) \
    SkelAnime_InitFlex(play, &this->skelAnime, skeleton, animation, this->jointTbl, this->morphTbl, maxLimb);
#define SKELANIME_DRAW_OPA(override, post) \
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->jointTbl, this->skelAnime.dListCount, override, post, this);

typedef struct {
    Vec3f pos;
    Vec3f vel;
    Vec3s rot;
    u8    timer;
} EnDummyParticle;

typedef struct {
    Vec3f pos;
    Vec3f vel;
    u8    timer;
    u8    damage;
    u8    damageEffect;
} EnDummyNumberParticle;

struct EnDummy;

typedef void (* EnDummyFunc)(struct EnDummy*, PlayState*);

#define DUMMY_SCALE  0.0035f
#define ParticleMax  10
#define particleGravity  1.2f

#define NumberParticleMax  5
#define NumberParticleGravity 0.05f

#define stunTimerMAX 40
#define iceTimerMAX 60
#define fireTimerMAX 40

#define maxHealth 16

#define targetRange 250.0f
#define targetAngle 70
#define targetHeight 150.0f

typedef enum {
    Default,
    Debug,
} type;

#define NEW_MATRIX() Matrix_NewMtx(__gfxCtx, __FILE__, __LINE__)

typedef struct EnDummy {
    Actor actor;
    Vec3s bodyPartsPos[5];
    
    EnDummyFunc func;

    SKELANIME_MEMBER(BODY_LIMB_MAX);
    ColliderCylinder bodycollider;

    u8  type;
    s8  movementState;
    s16 stunTimer;
    s16 iceTimer;
    s16 fireTimer;
    s16 currentHeadYaw;
    s16 currentHeadPitch;
    u8 canSee;

    EnDummyParticle particles[ParticleMax];
    EnDummyNumberParticle numberParticles[NumberParticleMax];

    u8 numparticles;
    u8 numberNumparticles;

} EnDummy;

#endif // __EN_DUMMY_H__