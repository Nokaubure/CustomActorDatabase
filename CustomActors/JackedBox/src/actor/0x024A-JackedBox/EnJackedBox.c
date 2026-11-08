#include "EnJackedBox.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "assets/objects/gameplay_dangeon_keep/gameplay_dangeon_keep.h"
#include "functions.h"
#include "overlays/effects/ovl_Effect_Ss_Kakera/z_eff_ss_kakera.h"
#include "uLib_vector.h"

#define ACT_ID 0x02A4
#define OBJ_ID 0x02A4

#define OBJ_H "object/0x024A-EnJackedBox.h"
#include OBJ_H

void EnJackedBox_Init(EnJackedBox* this, PlayState* play);
void EnJackedBox_Destroy(EnJackedBox* this, PlayState* play);
void EnJackedBox_Update(EnJackedBox* this, PlayState* play);
void EnJackedBox_Draw(EnJackedBox* this, PlayState* play);

static void SetAction(EnJackedBox* this, BoxActionId id, PlayState* play);

extern void Knockback_Player(PlayState* play, Actor* actor, f32 speed, s16 dir, f32 gravity, u32 damage);
asm("Knockback_Player = func_8002F6D4");

#define FLAGS (ACTORFLAG_PRESS_SWITCH | ACTORFLAG_ALWAYS_UPDATE)

const ActorInit sEnJackedBox_InitVars = {
    .id           = ACT_ID,
    .category     = ACTORCAT_ENEMY,
    .flags        = FLAGS,
    .objectId     = OBJ_ID,
    .instanceSize = sizeof(EnJackedBox),
    .init         = (ActorFunc)EnJackedBox_Init,
    .destroy      = (ActorFunc)EnJackedBox_Destroy,
    .update       = (ActorFunc)EnJackedBox_Update,
    .draw         = (ActorFunc)EnJackedBox_Draw
};

typedef enum {
    /* 0x0 */ DMGEFF_NONE,
    /* 0x1 */ DMGEFF_STUN,
} DamageEffect;

static DamageTable sDamageTable = { // Basic damage table with effects
    /* Deku nut      */ DMG_ENTRY(0, DMGEFF_STUN),
    /* Deku stick    */ DMG_ENTRY(0, 0x0),
    /* Slingshot     */ DMG_ENTRY(2, 0x0),
    /* Explosive     */ DMG_ENTRY(8, 0x0),
    /* Boomerang     */ DMG_ENTRY(2, 0x0),
    /* Normal arrow  */ DMG_ENTRY(2, 0x0),
    /* Hammer swing  */ DMG_ENTRY(8, 0x0),
    /* Hookshot      */ DMG_ENTRY(0, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(2, 0x0),
    /* Master sword  */ DMG_ENTRY(4, 0x0),
    /* Giant's Knife */ DMG_ENTRY(8, 0x0),
    /* Fire arrow    */ DMG_ENTRY(4, 0x0),
    /* Ice arrow     */ DMG_ENTRY(4, 0x0),
    /* Light arrow   */ DMG_ENTRY(4, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(4, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(4, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(4, 0x0),
    /* Fire magic    */ DMG_ENTRY(4, 0x0),
    /* Ice magic     */ DMG_ENTRY(4, 0x0),
    /* Light magic   */ DMG_ENTRY(4, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(4, 0x0),
    /* Giant spin    */ DMG_ENTRY(8, 0x0),
    /* Master spin   */ DMG_ENTRY(4, 0x0),
    /* Kokiri jump   */ DMG_ENTRY(4, 0x0),
    /* Giant jump    */ DMG_ENTRY(8, 0x0),
    /* Master jump   */ DMG_ENTRY(8, 0x0),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(8, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static ColliderJntSphElementInit sJntSphItemsInit[3] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x00, 0x00 }, // Damage
            { DMG_DEFAULT, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { BODY_LIMB_ARM_R, { { 0, 0, 0 }, 15 }, 150 },
    },

    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x00, 0x00 },
            { DMG_DEFAULT, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { BODY_LIMB_FOREARM_R, { { 0, 0, 0 }, 15 }, 125 }, // Limb Index, offsets, size
    },

    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x00, 0x10 }, // Toucher: dmgFlags / effect / damage
            { DMG_DEFAULT, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_HARD,
            BUMP_NONE,
            OCELEM_ON,
        },
        { BODY_LIMB_HAND_R, { { 0, 500, -1500 }, 15 }, 100 },  // left, forward, up
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_HIT7,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON  | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphItemsInit),
    sJntSphItemsInit,
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum {
    JACK_ANIM_MENACE,
    JACK_ANIM_MENACE_OVERHEAD,
    JACK_ANIM_MENACE_TORCH,
    JACK_ANIM_MENACE_THRUST,
    JACK_ANIM_STAB,
    JACK_ANIM_SLASH,
    JACK_ANIM_THRUST,
} EnJackedBoxAnimationIndex;

static AnimationInfo sEnJackedBoxAnims[] = {
    [JACK_ANIM_MENACE]          = {gEnJackedBox_AnimMenace,  1, 0, -1, ANIMMODE_LOOP , 5},
    [JACK_ANIM_MENACE_OVERHEAD] = {gEnJackedBox_AnimMenaceOverhead,  1, 0, -1, ANIMMODE_LOOP , 5},
    [JACK_ANIM_MENACE_TORCH]    = {gEnJackedBox_AnimMenaceTorch,  1, 0, -1, ANIMMODE_LOOP , 5},
    [JACK_ANIM_MENACE_THRUST]   = {gEnJackedBox_AnimMenaceThrust,  1, 0, -1, ANIMMODE_LOOP , 5},
    [JACK_ANIM_STAB]            = {gEnJackedBox_AnimStab,  1, 0, -1, ANIMMODE_ONCE , 2},
    [JACK_ANIM_SLASH]           = {gEnJackedBox_AnimSlash,  1, 0, -1, ANIMMODE_ONCE , 4},
    [JACK_ANIM_THRUST]          = {gEnJackedBox_AnimThrust,  1, 0, -1, ANIMMODE_ONCE , 3},
};

static u32 rmask(u32 value, u32 mask) {
    u32 shift = __builtin_ctz(mask);
    
    value >>= shift;
    mask >>= shift;
    
    return value & mask;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void FollowPlayer(EnJackedBox* this, PlayState* play) {
    s32 playerNearby = (this->actor.xzDistToPlayer < 80.0f);
    s16 turnRate = playerNearby? 0x800: 0x300;

    if (Math_SmoothStepToF(&this->actor.speedXZ, 0.0f, 1, 0.25f, 0.05f) < 0.1f){
        if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND){
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_WOLFOS_WALK);
            this->actor.velocity.y = 6.0f;

            f32 speed = 4.0f;

            if (playerNearby){
                speed *= this->actor.xzDistToPlayer/ 80.0f;
            }
            this->actor.speedXZ = speed;
        }
    }

    Actor_MoveForward(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 50.0f, 0.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);
    Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1, turnRate, 0x100);
    this->actor.shape.rot.y = this->actor.world.rot.y;
}

static void FollowPath(EnJackedBox* this, PlayState* play) {
    Path* path = &play->setupPathList[this->pathIndex];

    if (Math_SmoothStepToF(&this->actor.speedXZ, 0.0f, 1, 0.25f, 0.05f) < 0.1f){
        if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND){
            Audio_PlayActorSfx2(&this->actor, NA_SE_EN_WOLFOS_WALK);
            this->actor.velocity.y = 6.0f;
            this->actor.speedXZ = 3.5f;
        }
    }

    s16 yaw;
    f32 distSq = Path_OrientAndGetDistSq(&this->actor, path, this->actionVar, &yaw);

    if ((distSq > 0.0f) && (distSq < 1000.0f)) {
        switch (this->pathBehavior) {
            case PATH_LOOP: 
                this->actionVar++;
                if (this->actionVar > (path->count - 1)) {
                    this->actionVar = 0;
                }
                break;

            case PATH_REVERSE:
                if (!this->pathReverse) {
                    this->actionVar++;
                    if (this->actionVar > (path->count - 1)) {
                        this->pathReverse = true;
                        this->actionVar = path->count - 2;
                    }
                } else {
                    this->actionVar--;
                    if (this->actionVar < 0) {
                        this->pathReverse = false;
                        this->actionVar = 1;
                    }
                }
                break;

            case PATH_RANDOMIZE:
                if (path->count > 1) {
                    this->actionVar = ExcludeRandom(this->actionVar, 0, path->count);
                }
                break;

            default:
                break;
        }
    }

    Actor_MoveForward(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 50.0f, 0.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);
    Math_SmoothStepToS(&this->actor.world.rot.y, yaw, 1, 0x300, 0x100);
    this->actor.shape.rot.y = this->actor.world.rot.y;

}

static s32 BounceScale(EnJackedBox* this, PlayState* play){
    static const f32 springStrength = 0.15f; 
    static const f32 damping = 0.82f;           
    static const f32 scaleTarget = 0.01f;        

    f32 offset = scaleTarget - this->actor.scale.y;

    this->scaleVelocity += offset * springStrength;
    this->scaleVelocity *= damping;

    this->actor.scale.y += this->scaleVelocity;
    this->actor.scale.x = this->actor.scale.z = this->actor.scale.y;

    if (fabsf(this->scaleVelocity) < 0.00007f && fabsf(offset) < 0.00007f){
        return true;
    }
    
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void MakeChangeList(s16* list, s16 amount, s16* count) {
    s16 i = 0;

    while (amount >= 50) {
        list[i++] = ITEM00_RUPEE_PURPLE;
        amount -= 50;
    }

    while (amount >= 20) {
        list[i++] = ITEM00_RUPEE_RED;
        amount -= 20;
    }

    while (amount >= 5) {
        list[i++] = ITEM00_RUPEE_BLUE;
        amount -= 5;
    }

    while (amount >= 1) {
        list[i++] = ITEM00_RUPEE_GREEN;
        amount -= 1;
    }

    *count = i;
}

static s16 GetHealthDrop(void) {
    if (gSaveContext.health <= 0x30) {
        return ITEM00_FLEXIBLE; //fairy
    }

    return ITEM00_RECOVERY_HEART;
}

static s16 GetMagicDrop(void) {
    if (gSaveContext.magicLevel == 0) {
        return ITEM00_NONE;
    }

    if (gSaveContext.magic == 0) {
        return ITEM00_MAGIC_LARGE;
    }

    return ITEM00_MAGIC_SMALL;
}

typedef struct {
    s16 item;
    s16 weight;
} ResourceDrop;

static s16 GetAmmoDrop(void) {
    ResourceDrop drops[4];
    s16 count = 0;
    s16 totalWeight = 0;
    s16 roll;

    if (AMMO(ITEM_SLINGSHOT) < CUR_CAPACITY(UPG_BULLET_BAG)) {
        drops[count++] = (ResourceDrop){ ITEM00_SEEDS, 25 };
    }

    if (AMMO(ITEM_BOW) < CUR_CAPACITY(UPG_QUIVER)) {
        drops[count++] = (ResourceDrop){ ITEM00_ARROWS_SMALL, 25 };
    }

    if (AMMO(ITEM_BOMB) < CUR_CAPACITY(UPG_BOMB_BAG)) {
        drops[count++] = (ResourceDrop){ ITEM00_BOMBS_A, 25 };
    }

    if (AMMO(ITEM_BOMBCHU) < 25) {
        drops[count++] = (ResourceDrop){ ITEM00_BOMBS_B, 25 };
    }

    if (count == 0) {
        return ITEM00_NONE;
    }

    for (s16 i = 0; i < count; i++) {
        totalWeight += drops[i].weight;
    }

    roll = Rand_S16Offset(0, totalWeight);

    for (s16 i = 0; i < count; i++) {
        if (roll < drops[i].weight) {
            return drops[i].item;
        }

        roll -= drops[i].weight;
    }

    return ITEM00_NONE;
}

static s16 GetResourceDrop(void) {
    ResourceDrop drops[3];
    s16 count = 0;
    s16 totalWeight = 0;
    s16 roll;

    s16 healthDrop = GetHealthDrop();
    s16 magicDrop = GetMagicDrop();
    s16 ammoDrop = GetAmmoDrop();

    if (healthDrop != ITEM00_NONE) {
        drops[count++] = (ResourceDrop){ healthDrop, 20 };
    }

    if (magicDrop != ITEM00_NONE) {
        drops[count++] = (ResourceDrop){ magicDrop, 20 };
    }

    if (ammoDrop != ITEM00_NONE) {
        drops[count++] = (ResourceDrop){ ammoDrop, 20 };
    }

    if (count == 0) {
        return ITEM00_NONE;
    }

    for (s16 i = 0; i < count; i++) {
        totalWeight += drops[i].weight;
    }

    roll = Rand_S16Offset(0, totalWeight);

    for (s16 i = 0; i < count; i++) {
        if (roll < drops[i].weight) {
            return drops[i].item;
        }

        roll -= drops[i].weight;
    }

    return ITEM00_NONE;
}

static void SpawnMoney(EnJackedBox* this, PlayState* play, s16 base, s16 range) {
    s16 money[8];
    s16 count;
    MakeChangeList(money, Rand_S16Offset(base, range), &count);

    for (s16 i = 0; i < count; i++) {
        Item_DropCollectible(play,&this->actor.world.pos, money[i] | BOX_COLLECTIBLE_FLAG_RAW);
    }
}
static void SpawnCollectible(EnJackedBox* this, PlayState* play) {
    s16 boxFlag = BOX_COLLECTIBLE_FLAG;
    s16 item = ITEM00_NONE;

    if (!Flags_GetCollectible(play, boxFlag) && boxFlag != 0 && this->colorIndex != BOX_COLOR_SILVER) {
        SpawnMoney(this,play,15,36);
    } else {
        switch(this->colorIndex){
            case BOX_COLOR_PURPLE : SpawnMoney(this,play,3,8); return;
            case BOX_COLOR_BLUE   : item = GetAmmoDrop();    break;
            case BOX_COLOR_RED    : item = GetHealthDrop();  break;
            case BOX_COLOR_GREEN  : item = GetMagicDrop();   break;
            case BOX_COLOR_SILVER : item = ITEM00_SMALL_KEY; break;
            case BOX_COLOR_GREY   : break;
        }
    }

    if (item != ITEM00_NONE){
        if (item == ITEM00_SMALL_KEY){
            Item_DropCollectible(play,&this->actor.world.pos, item | BOX_COLLECTIBLE_FLAG_RAW);
        } else if (item == ITEM00_FLEXIBLE){
            Item_DropCollectible(play,&this->actor.world.pos, item);
        } else {
            s16 count = Rand_S16Offset(2, 2);
            for (s16 i = 0; i < count; i++) {
                Item_DropCollectible(play,&this->actor.world.pos, item);
            }
        }
    }
}

static void BoxKill(EnJackedBox* this, PlayState* play) {
    SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_WOODBOX_BREAK);
    SpawnCollectible(this, play);
    Actor_Kill(&this->actor);
}

static void AirBreak(EnJackedBox* this, PlayState* play) {
    s16 angle;
    s32 i;
    Vec3f* breakPos = &this->actor.world.pos;
    Vec3f pos;
    Vec3f velocity;

    for (i = 0, angle = 0; i < 12; i++, angle += 0x4E20) {
        f32 sn = Math_SinS(angle);
        f32 cs = Math_CosS(angle);
        f32 temp_rand;
        s16 phi_s0;

        pos.x = sn * 16.0f;
        pos.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        pos.z = cs * 16.0f;
        velocity.x = pos.x * 0.2f;
        velocity.y = (Rand_ZeroOne() * 6.0f) + 2.0f;
        velocity.z = pos.z * 0.2f;
        pos.x += breakPos->x;
        pos.y += breakPos->y;
        pos.z += breakPos->z;
        temp_rand = Rand_ZeroOne();
        if (temp_rand < 0.1f) {
            phi_s0 = 0x60;
        } else if (temp_rand < 0.7f) {
            phi_s0 = 0x40;
        } else {
            phi_s0 = 0x20;
        }
        EffectSsKakera_Spawn(play, &pos, &velocity, breakPos, -200, phi_s0, 10, 10, 0, (Rand_ZeroOne() * 30.0f) + 10.0f,
                             0, 32, 60, KAKERA_COLOR_NONE, OBJECT_GAMEPLAY_DANGEON_KEEP, gSmallWoodenBoxFragmentDL);
    }
    func_80033480(play, &this->actor.world.pos, 40.0f, 3, 50, 140, 1);
    BoxKill(this,play);
}

static void WaterBreak(EnJackedBox* this, PlayState* play) {
    s16 angle;
    s32 i;
    Vec3f* breakPos = &this->actor.world.pos;
    Vec3f pos;
    Vec3f velocity;

    pos = *breakPos;
    pos.y += this->actor.yDistToWater;
    EffectSsGSplash_Spawn(play, &pos, NULL, NULL, 0, 500);

    for (i = 0, angle = 0; i < 12; i++, angle += 0x4E20) {
        f32 sn = Math_SinS(angle);
        f32 cs = Math_CosS(angle);
        f32 temp_rand;
        s16 phi_s0;

        pos.x = sn * 16.0f;
        pos.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        pos.z = cs * 16.0f;
        velocity.x = pos.x * 0.18f;
        velocity.y = (Rand_ZeroOne() * 4.0f) + 2.0f;
        velocity.z = pos.z * 0.18f;
        pos.x += breakPos->x;
        pos.y += breakPos->y;
        pos.z += breakPos->z;
        temp_rand = Rand_ZeroOne();
        phi_s0 = (temp_rand < 0.2f) ? 0x40 : 0x20;
        EffectSsKakera_Spawn(play, &pos, &velocity, breakPos, -180, phi_s0, 30, 30, 0, (Rand_ZeroOne() * 30.0f) + 10.0f,
                             0, 32, 70, KAKERA_COLOR_NONE, OBJECT_GAMEPLAY_DANGEON_KEEP, gSmallWoodenBoxFragmentDL);
    }
    BoxKill(this,play);
}

static void ApplyGravity(EnJackedBox* this) {
    this->actor.velocity.y += this->actor.gravity;
    if (this->actor.velocity.y < this->actor.minVelocityY) {
        this->actor.velocity.y = this->actor.minVelocityY;
    }
}

typedef struct {
    Gfx* dl;
    u32 sfxID;
    f32 sfxFrame;
    f32 attackInactiveFrame;
    f32 colliderScale;
    f32 range;
    f32 posImpulse;
    s16 rotXVel;
    s16 angle;
    s16 colliderOffsetZ;
    s16 menaceAnim;
    s16 attackAnim;
    u8  damage;     // toucher.damage
    u8  effect;     // toucher.effect
} AttackData;

// damageEffect against player before remap
typedef enum {
    PLAYER_DMG_EFFECT_NONE = 0,
    PLAYER_DMG_EFFECT_FIRE = 1,
    PLAYER_DMG_EFFECT_ICE = 2,
    PLAYER_DMG_EFFECT_ELECTRIC = 3,
    PLAYER_DMG_EFFECT_KNOCKBACK = 4,
} Player_Dmg_Effect;

static AttackData sAttackData[] = {
    [BOX_TYPE_AXE] = {
        .dl                   = gEnJackedBox_DlAxe,
        .sfxFrame             = 2.0f,
        .attackInactiveFrame  = 10.0f,
        .sfxID                = NA_SE_IT_SWORD_SWING_HARD,
        .menaceAnim           = JACK_ANIM_MENACE_OVERHEAD,
        .range                = 60.0f,
        .angle                = DEG_TO_BINANG(50),
        .colliderScale        = 1.0f,
        .colliderOffsetZ      = -1500,
        .effect               = PLAYER_DMG_EFFECT_NONE,
        .damage               = 0x10,
        .attackAnim           = JACK_ANIM_SLASH,
        .posImpulse           = 15.0f,
        .rotXVel              = DEG_TO_BINANG(20),
    },

    [BOX_TYPE_KNIFE] = {
        .dl                   = gEnJackedBox_DlKnife,
        .sfxFrame             = 2.0f,
        .attackInactiveFrame  = 10.0f,
        .sfxID                = NA_SE_IT_SWORD_SWING_HARD,
        .menaceAnim           = JACK_ANIM_MENACE,
        .range                = 60.0f,
        .angle                = DEG_TO_BINANG(30),
        .colliderScale        = 1.0f,
        .colliderOffsetZ      = 1500,
        .effect               = PLAYER_DMG_EFFECT_NONE,
        .damage               = 0x10,
        .attackAnim           = JACK_ANIM_STAB,
        .posImpulse           = -20.0f,
        .rotXVel              = DEG_TO_BINANG(-45),
    },

    [BOX_TYPE_TORCH] = {
        .dl                   = gEnJackedBox_DlTorch,
        .sfxFrame             = 3.0f,
        .attackInactiveFrame  = 7.0f,
        .sfxID                = NA_SE_IT_SWORD_SWING,
        .menaceAnim           = JACK_ANIM_MENACE_TORCH,
        .range                = 60.0f,
        .angle                = DEG_TO_BINANG(50),
        .colliderScale        = 1.0f,
        .colliderOffsetZ      = -1500,
        .effect               = PLAYER_DMG_EFFECT_FIRE,
        .damage               = 0x04,
        .attackAnim           = JACK_ANIM_SLASH,
        .posImpulse           = 15.0f,
        .rotXVel              = DEG_TO_BINANG(20),
    },

    [BOX_TYPE_BOXING] = {
        .dl                   = gEnJackedBox_DlBoxingGlove,
        .sfxFrame             = 2.0f,
        .attackInactiveFrame  = 6.0f,
        .sfxID                = SOUND_CARTOON_PUNCH,
        .menaceAnim           = JACK_ANIM_MENACE_THRUST,
        .range                = 120.0f,
        .angle                = DEG_TO_BINANG(30),
        .colliderScale        = 1.5f,
        .colliderOffsetZ      = 0,
        .effect               = PLAYER_DMG_EFFECT_NONE,
        .damage               = 0x04,
        .attackAnim           = JACK_ANIM_THRUST,
        .posImpulse           = 25.0f,
        .rotXVel              = DEG_TO_BINANG(20),
    },

    [BOX_TYPE_RAPIER] = {
        .dl                   = gEnJackedBox_DlRapier,
        .sfxFrame             = 3.0f,
        .attackInactiveFrame  = 7.0f,
        .sfxID                = NA_SE_IT_SWORD_SWING,
        .menaceAnim           = JACK_ANIM_MENACE_THRUST,
        .range                = 120.0f,
        .angle                = DEG_TO_BINANG(20),        
        .colliderScale        = 1.0f,
        .colliderOffsetZ      = -1800,
        .effect               = PLAYER_DMG_EFFECT_NONE,
        .damage               = 0x20,
        .attackAnim           = JACK_ANIM_THRUST,
        .posImpulse           = 20.0f,
        .rotXVel              = DEG_TO_BINANG(15),
    },
};

#define VISION_ANGLE  DEG_TO_BINANG(60)
#define VISION_RANGE  200.0f
#define VISION_HEIGHT 100.0f
#define HEARING_RANGE 60.0f

typedef enum {
    REACT_NONE,
    REACT_MEASURED,
    REACT_STARTLED,
} BoxReaction;

static s32 CanSee(EnJackedBox* this, PlayState* play) {
    CollisionPoly* colPoly;
    s32 bgId;
    Vec3f colPoint;
    Player* player = GET_PLAYER(play);

    if (fabsf(this->actor.yDistToPlayer) > VISION_HEIGHT) {
        return REACT_NONE;
    }

    if (this->actor.xzDistToPlayer < HEARING_RANGE) {
        return REACT_STARTLED;
    }

    if (this->actor.xzDistToPlayer > VISION_RANGE) {
        return REACT_NONE;
    }

    if (ABS(this->actor.yawTowardsPlayer - this->actor.shape.rot.y) > VISION_ANGLE) {
        return REACT_NONE;
    }

    if (BgCheck_EntityLineTest1(&play->colCtx, &this->actor.focus.pos, &player->actor.focus.pos, &colPoint, &colPoly, true,
                                false, false, true, &bgId)) {
        return REACT_NONE;
    }

    return REACT_MEASURED;
}

static void CheckReturnToIdle(EnJackedBox* this, PlayState* play) {
    if (this->actor.xzDistToPlayer > 250.0f){
        if (this->actionVar++ > 80){
            switch (BOX_BEHAVIOR){
                case BOX_BEHAVIOR_DORMANT:
                    if (Math_StepToF(&this->verticalOffset, VERTICAL_OFFSET_LOWERED, 0.5f)){
                        SetAction(this,ACTION_CLOSE_LID,play);
                    }
                    break;
                case BOX_BEHAVIOR_PATROL:
                    SetAction(this,ACTION_CLOSE_LID,play);
                    break;
            }
        }
    } else if (this->actor.xzDistToPlayer < 100.0f){
        this->actionVar = 0;
    }
}

static void RemoveTorchLight(EnJackedBox* this, PlayState* play) {
    if (this->lightNode != NULL) {
        LightContext_RemoveLight(play, &play->lightCtx, this->lightNode);
        this->lightNode = NULL;
    }
}

static void InsertTorchLight(EnJackedBox* this, PlayState* play) {
    if (this->lightNode == NULL) {
        this->lightNode = LightContext_InsertLight(play, &play->lightCtx, &this->lightInfo);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Setup_Dormant(EnJackedBox* this, PlayState* play) {
    this->actor.speedXZ = 0.0f;
}

static void Action_Dormant(EnJackedBox* this, PlayState* play) {
    s32 canSee = CanSee(this,play);

    switch (canSee){
        case REACT_NONE:
            Math_StepToF(&this->verticalOffset, VERTICAL_OFFSET_LOWERED, 0.5f);
            break;
        case REACT_MEASURED:
            if (Math_StepToF(&this->verticalOffset, VERTICAL_OFFSET_RAISED, 1.5f)){
                SetAction(this,ACTION_JUMP_AND_TURN,play);
            }
            break;        
        case REACT_STARTLED:
            SetAction(this,ACTION_JUMP_AND_TURN,play);
            break;    
    }
}

static void Setup_JumpAndTurn(EnJackedBox* this, PlayState* play) {
    this->actionVar = 0;
    this->actor.flags |= (ACTORFLAG_CAN_TARGET | ACTORFLAG_ENEMY);
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    this->actor.speedXZ = -14.0f;
    this->actor.velocity.y = 5.0f;
    this->verticalOffset = VERTICAL_OFFSET_RAISED;
}

static void Action_JumpAndTurn(EnJackedBox* this, PlayState* play) {
    Math_SmoothStepToF(&this->actor.speedXZ,0.0f, 1.0f, 0.025f, 0.005f);
    Actor_MoveForward(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 50.0f, 0.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);

    if (Math_StepToS(&this->actor.shape.rot.y, this->actor.world.rot.y , 0x800)){
        this->actor.speedXZ = 0.0f;
        SetAction(this,ACTION_MENACE,play); 
    }
}

static void Setup_Jump(EnJackedBox* this, PlayState* play) {
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    this->actor.speedXZ = -12.0f;
    this->actor.velocity.y = 4.0f;
    this->actionVar = false;
}

//Close lid, reposition and track player
static void Action_Jump(EnJackedBox* this, PlayState* play) {
    Math_SmoothStepToF(&this->actor.speedXZ,0.0f, 1.0f, 0.025f, 0.005f);
    Actor_MoveForward(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 50.0f, 0.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);

    Math_StepToS(&this->actor.shape.rot.y, this->actor.world.rot.y , 0x800);

    if (Math_StepToS(&this->lidRot,0,DEG_TO_BINANG(15))){
        if (!this->actionVar){
            this->actionVar = true;
            Audio_PlayActorSfx2(&this->actor, NA_SE_EV_PUT_DOWN_WOODBOX);
        }
    }

    if (Math_StepToF(&this->actor.scale.y, 0.0f, 0.001f)){
        RemoveTorchLight(this, play);
        this->actor.speedXZ = 0.0f;
        SetAction(this,ACTION_TRACK,play); 
    }

    this->actor.scale.x = this->actor.scale.z = this->actor.scale.y;
}

static void Setup_Patrol(EnJackedBox* this, PlayState* play) {
    this->actionVar = 0;
}

static void Action_Patrol(EnJackedBox* this, PlayState* play) {
    FollowPath(this,play);

    if (CanSee(this,play)){
        this->actor.flags |= (ACTORFLAG_CAN_TARGET | ACTORFLAG_ENEMY);
        SetAction(this,ACTION_MENACE,play);
    }
}

static void Action_Track(EnJackedBox* this, PlayState* play) {
    FollowPlayer(this,play);
    CheckReturnToIdle(this,play);

    if (this->actor.xzDistToPlayer < 250.0f){
        SetAction(this,ACTION_MENACE,play);
    }
}

static void Setup_CloseLid(EnJackedBox* this, PlayState* play) {
    this->actionVar = 0;
}

static void Action_CloseLid(EnJackedBox* this, PlayState* play) {

    Math_SmoothStepToF(&this->actor.speedXZ,0.0f, 1.0f, 0.025f, 0.005f);
    Actor_MoveForward(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 50.0f, 0.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);

    if (Math_StepToS(&this->lidRot,0,DEG_TO_BINANG(15))){
        if (!this->actionVar){
            this->actionVar = true;
            Audio_PlayActorSfx2(&this->actor, NA_SE_EV_PUT_DOWN_WOODBOX);
        }
    }

    if (Math_StepToF(&this->actor.scale.y, 0.0f, 0.001f)){
        RemoveTorchLight(this, play);
        this->actor.speedXZ = 0.0f;

        switch (BOX_BEHAVIOR){
            case BOX_BEHAVIOR_DORMANT:
                SetAction(this,ACTION_DORMANT,play);
                break;
            case BOX_BEHAVIOR_PATROL:
                SetAction(this,ACTION_PATROL,play);
                break;
        }
    }

    this->actor.scale.x = this->actor.scale.z = this->actor.scale.y;
}

// Setup attack data and brandish weapon 
static void Setup_Menace(EnJackedBox* this, PlayState* play) {
    this->actionVar = 0;
    this->handType  = ExcludeRandom(this->handType,0,BOX_TYPE_MAX);
    AttackData* a = &sAttackData[this->handType];

    this->colliderSpheresElements[2].dim.modelSphere.center.z = a->colliderOffsetZ;
    this->colliderSpheresElements[2].info.toucher.effect = a->effect;
    this->colliderSpheresElements[2].info.toucher.damage = a->damage;
    this->colliderSpheresElements[2].dim.scale = a->colliderScale;
    this->colliderSpheresElements[2].info.toucherFlags &= ~DMG_UNBLOCKABLE;

    if (this->handType == BOX_TYPE_RAPIER){
        this->colliderSpheresElements[2].info.toucherFlags |= DMG_UNBLOCKABLE;
    }

    Animation_ChangeByInfo(&this->skelAnime, sEnJackedBoxAnims, a->menaceAnim);

    if (this->handType == BOX_TYPE_TORCH){
        InsertTorchLight(this,play);
        Lights_PointGlowSetInfo(&this->lightInfo, UnfoldVec3f(this->colliderSpheresElements[2].dim.worldSphere.center), 0, 0, 0, 0);
        Audio_PlayActorSfx2(&this->actor, NA_SE_EV_FLAME_IGNITION); 
    } else {
        Audio_PlayActorSfx2(&this->actor, NA_SE_IT_SWORD_PICKOUT); 
    }
}


static void Action_Menace(EnJackedBox* this, PlayState* play) {
    AttackData* a = &sAttackData[this->handType];
    SkelAnime_Update(&this->skelAnime);
    FollowPlayer(this,play);
    CheckReturnToIdle(this,play);
    Math_StepToS(&this->lidRot,DEG_TO_BINANG(125),DEG_TO_BINANG(25));

    if (BounceScale(this,play)){
        if (this->actor.xzDistToPlayer < a->range){
            if (ABS(this->actor.yawTowardsPlayer - this->actor.shape.rot.y) < a->angle) {
                SetAction(this,ACTION_ATTACK,play);
            }
        }
    }
}

static void Setup_Attack(EnJackedBox* this, PlayState* play) {
    AttackData* a = &sAttackData[this->handType];
    Box* b = &this->box;
    this->effectScale = 1.0f;

    Animation_ChangeByInfo(&this->skelAnime, sEnJackedBoxAnims, a->attackAnim);
    b->posImpulse = a->posImpulse;
    b->rotXVel = a->rotXVel;

    if (this->handType == BOX_TYPE_BOXING){
        this->actor.speedXZ = 0.0f;
    }
}

static void Action_Attack(EnJackedBox* this, PlayState* play) {
    AttackData* a = &sAttackData[this->handType];

    Math_SmoothStepToF(&this->actor.speedXZ,0.0f, 1.0f, 0.05f, 0.025f);
    Actor_MoveForward(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 50.0f, 0.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);

    if (this->skelAnime.curFrame == a->sfxFrame){
        if (this->handType == BOX_TYPE_BOXING) {
            SoundEffect_PlayDefaultOneshot(a->sfxID, &this->actor.pos, this);
        } else {
            Audio_PlayActorSfx2(&this->actor, a->sfxID);
        }
        this->attackOn = true;
    }

    if (this->skelAnime.curFrame == a->attackInactiveFrame){
        this->attackOn = false;
    }

    if (this->attackOn != true) {
        EffectBlure_AddSpace(Effect_GetByIndex(this->effectIndex));
    }

    if (this->handType == BOX_TYPE_BOXING){
        if (this->colliderSpheres.base.atFlags & AT_HIT) {
            this->colliderSpheres.base.atFlags &= ~AT_HIT;
            Knockback_Player(play, &this->actor, 6.0f, this->actor.yawTowardsPlayer, 2.0f, 0x10);
        }

        if (this->attackOn == true){
            Math_StepToF(&this->effectScale, 1.5f, 0.15f);
        } else {
            Math_StepToF(&this->effectScale, 1.0f, 0.25f);
        }
    }

    if (SkelAnime_Update(&this->skelAnime)){
        SetAction(this,ACTION_JUMP,play);
    }
}

static void Setup_Death(EnJackedBox* this, PlayState* play) {
    this->actor.flags &= ~ACTORFLAG_CAN_TARGET;
    Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_PROP);
    Enemy_StartFinishingBlow(play, &this->actor);
    Animation_ChangeByInfo(&this->skelAnime, sEnJackedBoxAnims, JACK_ANIM_MENACE);
    this->attackOn = false;
    this->actionVar = false;
    RemoveTorchLight(this, play);
}

static void Action_Death(EnJackedBox* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    Math_SmoothStepToF(&this->actor.speedXZ, 0.0f, 1, 0.5f, 0.05f);
    Actor_MoveForward(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 50.0f, 0.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);


    if (Math_StepToS(&this->lidRot,0,DEG_TO_BINANG(6))){
        if (!this->actionVar){
            this->actionVar = true;
            Audio_PlayActorSfx2(&this->actor, NA_SE_EV_PUT_DOWN_WOODBOX);
        }
    }

    Math_StepToF(&this->actor.scale.y, 0.0f, 0.0005f);
    this->actor.scale.x = this->actor.scale.z = this->actor.scale.y;

    if (Math_StepToF(&this->verticalOffset, VERTICAL_OFFSET_LOWERED, 0.5f)){
        SetAction(this,ACTION_LOOT_BOX,play);
    }
}

static void Setup_LootBox(EnJackedBox* this, PlayState* play) {
    this->collider.base.colType = COLTYPE_WOOD;
    this->actor.naviEnemyId = NAVI_ENEMY_NONE;
    this->actor.minVelocityY = -13.0f;
    this->actor.colChkInfo.mass = MASS_HEAVY;
    this->actor.shape.yOffset = 0.0f;
    this->collider.dim.height = 35;
    this->collider.base.acFlags &= ~AC_HIT;
}

static void Action_LootBox(EnJackedBox* this, PlayState* play) {
    Math_StepToF(&this->verticalOffset, VERTICAL_OFFSET_LOWERED, 0.5f);

    if (Actor_HasParent(&this->actor, play)) {
        SetAction(this,ACTION_HELD,play);
    } else if ((this->actor.bgCheckFlags & BGCHECKFLAG_WATER) && (this->actor.yDistToWater > 19.0f)) {
        WaterBreak(this, play);
    } else if (this->collider.base.acFlags & AC_HIT) {
        AirBreak(this, play);
    } else {
        Actor_MoveForward(&this->actor);
        Actor_UpdateBgCheckInfo(play, &this->actor, 19.0f, 20.0f, 0.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);
        if (!(this->collider.base.ocFlags1 & OC1_TYPE_PLAYER) && (this->actor.xzDistToPlayer > 28.0f)) {
            this->collider.base.ocFlags1 |= OC1_TYPE_PLAYER;
        }
        if (this->actor.xzDistToPlayer < 600.0f) {
            Collider_UpdateCylinder(&this->actor, &this->collider);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
            if (this->actor.xzDistToPlayer < 180.0f) {
                CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
            }
        }
        if (this->actor.xzDistToPlayer < 100.0f) {
            func_8002F580(&this->actor, play);
        }
    }
}

static void Setup_Held(EnJackedBox* this, PlayState* play) {
    this->actor.room = -1;
    func_8002F7DC(&this->actor, NA_SE_PL_PULL_UP_WOODBOX);
    this->actor.shape.yOffset = 3.0f;
}

static void Action_Held(EnJackedBox* this, PlayState* play) {
    Math_StepToF(&this->verticalOffset, VERTICAL_OFFSET_RAISED, 0.5f);

    if (Actor_HasNoParent(&this->actor, play)) {
        this->actor.room = play->roomCtx.curRoom.num;
        if (fabsf(this->actor.speedXZ) < 0.1f) {
            Audio_PlayActorSfx2(&this->actor, NA_SE_EV_PUT_DOWN_WOODBOX);
            SetAction(this,ACTION_LOOT_BOX,play);
            this->collider.base.ocFlags1 &= ~OC1_TYPE_PLAYER;
        } else {
            SetAction(this,ACTION_THROWN,play);
            ApplyGravity(this);
            func_8002D7EC(&this->actor);
        }
        Actor_UpdateBgCheckInfo(play, &this->actor, 19.0f, 20.0f, 0.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);
    }
}

static void Setup_Thrown(EnJackedBox* this, PlayState* play) {
    this->actor.velocity.x = Math_SinS(this->actor.world.rot.y) * this->actor.speedXZ;
    this->actor.velocity.z = Math_CosS(this->actor.world.rot.y) * this->actor.speedXZ;
    this->actor.colChkInfo.mass = 240;
}

static void Action_Thrown(EnJackedBox* this, PlayState* play) {
     if ((this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH | BGCHECKFLAG_WALL)) ||
        (this->collider.base.atFlags & AT_HIT)) {
        AirBreak(this, play);
    } else if (this->actor.bgCheckFlags & BGCHECKFLAG_WATER_TOUCH) {
        WaterBreak(this, play);
    } else {
        ApplyGravity(this);
        func_8002D7EC(&this->actor);
        Actor_UpdateBgCheckInfo(play, &this->actor, 19.0f, 20.0f, 0.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);
        Collider_UpdateCylinder(&this->actor, &this->collider);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    }
}

typedef struct {
    EnJackedBoxFunc update;
    EnJackedBoxFunc setup;
} ActionDef;

static ActionDef sActionTable[] = {
    [ACTION_DORMANT]            = { Action_Dormant           , Setup_Dormant},
    [ACTION_JUMP_AND_TURN]      = { Action_JumpAndTurn       , Setup_JumpAndTurn},
    [ACTION_JUMP]               = { Action_Jump              , Setup_Jump},
    [ACTION_PATROL]             = { Action_Patrol            , Setup_Patrol},
    [ACTION_CLOSE_LID]          = { Action_CloseLid          , Setup_CloseLid},
    [ACTION_TRACK]              = { Action_Track             , NULL},
    [ACTION_MENACE]             = { Action_Menace            , Setup_Menace},
    [ACTION_ATTACK]             = { Action_Attack            , Setup_Attack},
    [ACTION_DEATH]              = { Action_Death             , Setup_Death},
    [ACTION_LOOT_BOX]           = { Action_LootBox           , Setup_LootBox},
    [ACTION_HELD]               = { Action_Held              , Setup_Held},
    [ACTION_THROWN]             = { Action_Thrown            , Setup_Thrown},
};

static void SetAction(EnJackedBox* this, BoxActionId id, PlayState* play) {
    this->actionId = id;
    this->func = sActionTable[id].update;

    if (sActionTable[id].setup != NULL) {
        sActionTable[id].setup(this, play);
    }
}

static void ReceiveStandardDamage(EnJackedBox* this, PlayState* play) {
    Actor* attacker = this->colliderSpheres.base.ac;
    Vec3f hitDir;

    if (attacker != NULL){
        Math_Vec3f_Diff(&attacker->pos, &this->actor.pos, &hitDir);
        hitDir = Vec3f_Normalize(hitDir);
        hitDir.y = 0.0f;
        hitDir.x *= 25.0f;
        hitDir.z *= 25.0f;
    } else {
        hitDir.x = Math_SinS(this->actor.yawTowardsPlayer) * 25.0f;
        hitDir.y = 0.0f;
        hitDir.z = Math_CosS(this->actor.yawTowardsPlayer) * 25.0f;
    }

    this->stunTimer = 0;
    Actor_SetColorFilter(&this->actor, 0x4000, 0xFF, 0, 8);

    this->box.posVel.x -= hitDir.x;
    this->box.posVel.y += 5.0f;
    this->box.posVel.z -= hitDir.z;

    if (Actor_ApplyDamage(&this->actor) == 0) { 
        SetAction(this, ACTION_DEATH, play);
    } else {
        if (this->actionId >= ACTION_DORMANT && this->actionId <= ACTION_CLOSE_LID){
            SetAction(this, ACTION_JUMP_AND_TURN, play);
        }
    }
}

static void UpdateDamage(EnJackedBox* this, PlayState* play) {

    if (this->actor.colorFilterTimer == 0 || this->stunTimer > 0){
        if (this->colliderSpheres.base.acFlags & AC_HIT ) {
            this->colliderSpheres.base.acFlags &= ~AC_HIT;

            if (this->actor.colChkInfo.damageEffect == DMGEFF_STUN) {
                
                this->stunTimer = STUN_TIMER;
                Actor_SetColorFilter(&this->actor, 0, 0x78, 0, STUN_TIMER);
                Audio_PlayActorSfx2(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
                return;
            }

            ReceiveStandardDamage(this,play); 
        } 
    }
}

static void RegisterColliders(EnJackedBox* this, PlayState* play) {
    Collider_UpdateCylinder(&this->actor, &this->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);

    if (this->attackOn == true){
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->colliderSpheres.base);
    }

    if (this->actor.colorFilterTimer == 0 || this->stunTimer > 0){
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderSpheres.base);
    }
}

static void InitColliders(EnJackedBox* this, PlayState* play) {
    this->collider = (ColliderCylinder) {
        .base.shape            = COLSHAPE_CYLINDER,
        .base.actor            = &this->actor,
        
        // (OC)Object Collider Properties
        .base.ocFlags1         = OC1_ON | OC1_TYPE_ALL,
        .base.ocFlags2         = OC2_TYPE_2,
        .info.ocElemFlags      = OCELEM_ON,

        // (AC)Attack Collider Properties
        .base.colType          = COLTYPE_METAL,
        .base.acFlags          = AC_ON | AC_HARD | AC_TYPE_PLAYER,
        .info.bumper.dmgFlags  = DMG_DEFAULT,
        .info.bumperFlags      = BUMP_ON,

        // Dimensions of Collider
        .dim.radius            = 15,
        .dim.height            = 15,
    };

    Collider_InitJntSph(play, &this->colliderSpheres);
    Collider_SetJntSph(play, &this->colliderSpheres, &this->actor, &sJntSphInit, this->colliderSpheresElements);
}

static void InitPathwayInfo(EnJackedBox* this) {
    this->pathIndex = BOX_PATH_ID;
    this->pathBehavior = BOX_PATH_BEHAVIOR;
    this->actor.rot.z = 0;
}

static void InitBlure(EnJackedBox* this, PlayState* play) {
    EffectBlureInit1 slashBlure;

    slashBlure.p1StartColor[0] = slashBlure.p1StartColor[1] = slashBlure.p1StartColor[2] = slashBlure.p1StartColor[3] =
    slashBlure.p2StartColor[0] = slashBlure.p2StartColor[1] = slashBlure.p2StartColor[2] =
        slashBlure.p1EndColor[0] = slashBlure.p1EndColor[1] = slashBlure.p1EndColor[2] = slashBlure.p2EndColor[0] =
            slashBlure.p2EndColor[1] = slashBlure.p2EndColor[2] = 255;

    slashBlure.p1EndColor[3] = 0;
    slashBlure.p2EndColor[3] = 0;
    slashBlure.p2StartColor[3] = 64;

    slashBlure.elemDuration = 4;
    slashBlure.unkFlag = 0;
    slashBlure.calcMode = 2;

    Effect_Add(play, &this->effectIndex, EFFECT_BLURE1, 0, 0, &slashBlure);
}

typedef struct {
    Item00Type type;
    s16  index;
} ContentColorInfo;

static ContentColorInfo sContentColors[] = {
    {ITEM00_ARROWS_SMALL,   BOX_COLOR_BLUE},
    {ITEM00_SEEDS,          BOX_COLOR_BLUE},
    {ITEM00_BOMBS_A,        BOX_COLOR_BLUE}, 
    {ITEM00_BOMBS_B,        BOX_COLOR_BLUE},  // blue item drops
    {ITEM00_RECOVERY_HEART, BOX_COLOR_RED},   // red
    {ITEM00_FLEXIBLE,       BOX_COLOR_RED},   // red fairy
    {ITEM00_MAGIC_SMALL,    BOX_COLOR_GREEN}, // green
    {ITEM00_MAGIC_LARGE,    BOX_COLOR_GREEN}, // green
};

static s16 GetColorIndexFromItem(Item00Type item) {
    for (s16 i = 0; i < ARRAY_COUNT(sContentColors); i++) {
        if (sContentColors[i].type == item) {
            return sContentColors[i].index;
        }
    }
    return 0;
}

// If no flag set give specified contents
// otherwise give rupee payout, unless specifically a small key
// color actor by contents
static void AssignColorIndex(EnJackedBox* this, PlayState* play) {
    BoxCollectible collectible = BOX_COLLECTIBLE;
    s16 boxFlag = BOX_COLLECTIBLE_FLAG;

    if (!Flags_GetCollectible(play, boxFlag) && boxFlag != 0){ // if dont have the flag and flag is not 0
        if (collectible == BOX_COLLECTIBLE_SMALL_KEY) {
            this->colorIndex = BOX_COLOR_SILVER;
        } else {
            this->colorIndex = BOX_COLOR_PURPLE; // give rupees first
        }
    } else {
        if (collectible >= BOX_COLLECTIBLE_SMART) {
            this->colorIndex = GetColorIndexFromItem(GetResourceDrop()); // fallback to smart drop if out of range
        } else if (collectible == BOX_COLLECTIBLE_SMALL_KEY) {
            this->colorIndex = BOX_COLOR_GREY; // key has been picked up
        } else {
            this->colorIndex = BOX_COLLECTIBLE;
        }
    }
}

void EnJackedBox_Init(EnJackedBox* this, PlayState* play) {
    this->box.pos = this->actor.pos;
    this->actor.gravity = -1.0f;
    Actor_SetScale(&this->actor, 0.0001f);

    this->actor.naviEnemyId = NAVI_ENEMY_JACKED_BOX;
    this->actor.health = 4; //Assign Health values for actor
    this->actor.colChkInfo.damageTable = &sDamageTable; //Assign Damage Table for actor

    this->flameTextureScroll = (s16)(Rand_ZeroOne() * 20.0f);
    this->effectScale = 1.0f;
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 20.0f);

    InitColliders(this,play);
    InitBlure(this,play);
    AssignColorIndex(this,play);
    SkelAnime_InitFlex(play, &this->skelAnime, gEnJackedBox_SkelBody, gEnJackedBox_AnimMenace, this->jointTable, this->morphTable, BODY_LIMB_MAX);

    switch (BOX_BEHAVIOR){
        case BOX_BEHAVIOR_DORMANT:
            this->verticalOffset = VERTICAL_OFFSET_LOWERED;
            SetAction(this,ACTION_DORMANT,play);
            break;
        case BOX_BEHAVIOR_PATROL:
            InitPathwayInfo(this);
            this->verticalOffset = VERTICAL_OFFSET_RAISED;
            this->actor.speedXZ = Rand_ZeroOne() * 2.0f;
            this->actor.pos.y += (Rand_ZeroOne() * 3.0f);
            this->actor.flags |= (ACTORFLAG_CAN_TARGET | ACTORFLAG_ENEMY);
            SetAction(this,ACTION_PATROL,play);
            break;
        case BOX_BEHAVIOR_HARMLESS:
            Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_PROP);
            this->verticalOffset = VERTICAL_OFFSET_LOWERED;
            SetAction(this,ACTION_LOOT_BOX,play);
            break;
    }
}

void EnJackedBox_Destroy(EnJackedBox* this, PlayState* play) {
    RemoveTorchLight(this, play);
}

static void Update_SpringRider(EnJackedBox* this, PlayState* play) {
    Box* b = &this->box;

    Vec3f target = this->actor.world.pos;
    f32 velDeltaY = this->actor.velocity.y - this->prevVelY;
    this->prevVelY = this->actor.velocity.y;

    f32 deltaX = this->actor.world.pos.x - this->actor.prevPos.x;
    f32 deltaZ = this->actor.world.pos.z - this->actor.prevPos.z;
    f32 sinYaw =  Math_SinS(this->actor.world.rot.y);
    f32 cosYaw =  Math_CosS(this->actor.world.rot.y);
    f32 fwdDelta = deltaX * sinYaw + deltaZ * cosYaw;

    b->posVel.y -= velDeltaY * SPRING_VERTICAL_SCALE;

    if (b->posImpulse != 0.0f){
        b->posVel.x += sinYaw * b->posImpulse; 
        b->posVel.z += cosYaw * b->posImpulse; 
        b->posImpulse = 0.0f;
    }

    b->posVel.x += (target.x - b->pos.x) * SPRING_K;
    b->posVel.y += (target.y - b->pos.y) * SPRING_K_VERTICAL;
    b->posVel.z += (target.z - b->pos.z) * SPRING_K;

    b->posVel.x *= SPRING_DAMPING;
    b->posVel.y *= SPRING_DAMPING;
    b->posVel.z *= SPRING_DAMPING;

    b->pos.x += b->posVel.x;
    b->pos.y += b->posVel.y;
    b->pos.z += b->posVel.z;

    s16 targetRotX = (s16)CLAMP((-fwdDelta / SPRING_LEAN_FULL_AT_DELTA * SPRING_MAX_LEAN), -SPRING_MAX_LEAN, SPRING_MAX_LEAN);
    s16 errX = targetRotX - b->rotX;

    b->rotXVel += (s16)(errX * SPRING_K);
    b->rotXVel = (s16)(b->rotXVel * SPRING_DAMPING);
    b->rotX += b->rotXVel; // pitch
}

void EnJackedBox_Update(EnJackedBox* this, PlayState* play) {

    if (this->stunTimer > 0) {
        DECR(this->stunTimer); 
    } else {
        this->func(this, play);
        Update_SpringRider(this,play);
    }

    if (this->actor.category != ACTORCAT_PROP){
        UpdateDamage(this,play);
        RegisterColliders(this,play);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static u64 SpringTlut[] = {
    0xDEF4EF79DEF5CE71, 0xC62FB5AB9463841F, 0x83DF739BA4E6A4E7, 0x41CB398929492107
};

static Gfx springDl[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetRenderMode(GBL_c1(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_IN, G_BL_1MA), GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM) | AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP | ZMODE_OPA | CVG_X_ALPHA | ALPHA_CVG_SEL),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetCombineLERP(TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, COMBINED, 0, SHADE, 0, 0, 0, 0, COMBINED),
    gsDPSetPrimColor(0x00, 0x00, 255, 255, 255, 255),
    gsSPGeometryMode(G_TEXTURE_ENABLE | G_CULL_FRONT | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR, G_TEXTURE_ENABLE | G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsDPLoadTextureBlock_4b(gEnJackedBox_TexSpring, G_IM_FMT_CI, 32, 128, 15, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 7, 0, 0),
    gsDPLoadTLUT(16, 0x1F0, SpringTlut),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, (31<<2), (127<<2)),
    gsSPEndDisplayList(),
};

static void DrawSpring(Vtx* vtx, Vec3f from, Vec3f to, s16 yaw) {
    Vec3f right;
    const f32 SPRING_HALF_WIDTH = 5.0f;
    const s16 w = 32;
    const s16 h = 128;

    right.x = Math_CosS(yaw) * SPRING_HALF_WIDTH;
    right.z = -Math_SinS(yaw) * SPRING_HALF_WIDTH;

    vtx[0] = gdSPDefVtxC(from.x - right.x, from.y, from.z - right.z, 0, 0, 255,255,255,255);
    vtx[1] = gdSPDefVtxC(from.x + right.x, from.y, from.z + right.z, w, 0, 255,255,255,255);
    vtx[2] = gdSPDefVtxC(to.x   - right.x, to.y,   to.z   - right.z, 0, h, 255,255,255,255);
    vtx[3] = gdSPDefVtxC(to.x   + right.x, to.y,   to.z   + right.z, w, h, 255,255,255,255);
}

void EnJackedBox_DrawSpring(EnJackedBox* this, PlayState* play, Vec3f to) {
    s16 angle = Math_Vec3f_Yaw(&this->actor.pos, &play->view.eye);
    Vtx* vtx = Graph_Alloc(play->state.gfxCtx, sizeof(Vtx) * 4);

    Vec3f from = {
        this->actor.pos.x,
        this->actor.pos.y + 2.0f + this->actor.shape.yOffset,
        this->actor.pos.z,
    };
    
    DrawSpring(vtx, from, to, angle);

    gSPMatrix(POLY_OPA_DISP++, &gMtxClear,G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPVertex(POLY_OPA_DISP++, vtx, 4, 0);
    gSPDisplayList(POLY_OPA_DISP++, springDl);
    gSP2Triangles(POLY_OPA_DISP++, 0, 1, 2, 0, 1, 3, 2, 0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static Color_RGBA8 primColor = { 255, 200, 0, 255 };
static Color_RGBA8 envColor = { 255, 0, 0, 255 };

void EnJackedBox_DrawTorchFlame(EnJackedBox* this, PlayState* play) {
    if (this->handType != BOX_TYPE_TORCH) return;

    f32 lightBrightness;
    u8 red,green,blue;

    lightBrightness = (0.3f * Rand_ZeroOne()) + 0.7f;

    red = (u8)(primColor.r * lightBrightness);
    green = (u8)(primColor.g * lightBrightness);
    blue = (u8)(primColor.b * lightBrightness);

    Lights_PointGlowSetInfo(&this->lightInfo,UnfoldVec3f(this->colliderSpheresElements[2].dim.worldSphere.center), red, green, blue, 200);

    Gfx_SetupDL_25Xlu(play->state.gfxCtx);
    Matrix_Push();
    gSPSegment(POLY_XLU_DISP++, 0x08,Gfx_TwoTexScroll(play->state.gfxCtx, G_TX_RENDERTILE, 0, 0, 32, 64, 1, 0,
                                    (++this->flameTextureScroll * -20) & 0x1FF, 32, 128));

    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, primColor.r, primColor.g, primColor.b, 255);
    gDPSetEnvColor(POLY_XLU_DISP++, envColor.r, envColor.g, envColor.b, 255);

    Matrix_Translate(UnfoldVec3f(this->colliderSpheresElements[2].dim.worldSphere.center), MTXMODE_NEW);
    Matrix_ReplaceRotation(&play->billboardMtxF);
    Matrix_Scale(0.22f, 0.22f, 0.22f, MTXMODE_APPLY);
    Matrix_Scale(UnfoldVec3f(this->actor.scale), MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, FINALIZE_MATRIX(), G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);
    Matrix_Pop();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
s32 EnJackedBox_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnJackedBox* this = (void*)thisx;
    AttackData* a = &sAttackData[this->handType];

    if (limbIndex == BODY_LIMB_HAND_R){
        Matrix_Scale(this->effectScale, this->effectScale, this->effectScale, MTXMODE_APPLY);
        *dl = a->dl;
        return 0;
    }

    return 0;
}

void EnJackedBox_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dl, Vec3s* rot, void* thisx) {
    EnJackedBox* this = (void*)thisx;
    static Vec3f ModelOffset = {0.0f, 100.0f, 0.0f};
    static Vec3f zeroVec = {0.0f, 0.0f, 0.0f};

    //blure positions
    static Vec3f AxeSwingPoint1 = { 0.0f, 1200.0f, -1500.0f }; 
    static Vec3f AxeSwingPoint2 = { 0.0f, 1200.0f, -3600.0f };
    static Vec3f SwingPoint1    = { 0.0f, 280.0f, -1000.0f }; 
    static Vec3f SwingPoint2    = { 0.0f, 280.0f, -2500.0f };
    static Vec3f StabPoint1     = { 0.0f, 100.0f, 500.0f }; 
    static Vec3f StabPoint2     = { 0.0f, 100.0f, 3900.0f };

    Collider_UpdateSpheres(limbIndex, &this->colliderSpheres);

    if (limbIndex == BODY_LIMB_ROOT){
        Vec3f boxVisualPos;
        Matrix_MultVec3f(&zeroVec, &boxVisualPos);
        EnJackedBox_DrawSpring(this, play,boxVisualPos);
    }

    if (limbIndex == BODY_LIMB_HAND_R){
        Matrix_MultVec3f(&ModelOffset, &this->actor.focus.pos);

        if (this->attackOn == true) {
            Vec3f vector1;
            Vec3f vector2;
            switch(this->handType){
                case BOX_TYPE_AXE:
                case BOX_TYPE_RAPIER:
                    Matrix_MultVec3f(&AxeSwingPoint1, &vector1);
                    Matrix_MultVec3f(&AxeSwingPoint2, &vector2);
                    break;
                case BOX_TYPE_TORCH:
                    Matrix_MultVec3f(&SwingPoint1, &vector1);
                    Matrix_MultVec3f(&SwingPoint2, &vector2);
                    break;
                case BOX_TYPE_KNIFE:
                    Matrix_MultVec3f(&StabPoint1, &vector1);
                    Matrix_MultVec3f(&StabPoint2, &vector2);
                    break;
                case BOX_TYPE_BOXING:
                    Matrix_MultVec3f(&zeroVec,     &vector1);
                    Matrix_MultVec3f(&ModelOffset, &vector2);
                    break;
            }

            EffectBlure_AddVertex(Effect_GetByIndex(this->effectIndex), &vector1, &vector2);
        }
    }
}

static Color_RGB8 BoxColor[] ={
    [BOX_COLOR_PURPLE] = {0x9F,0x61,0xFF},
    [BOX_COLOR_BLUE]   = {0x59,0x7C,0xFF},
    [BOX_COLOR_RED]    = {0xE9,0x3E,0x41},
    [BOX_COLOR_GREEN]  = {0x53,0xC0,0x6F},
    [BOX_COLOR_SILVER] = {0xEF,0xEF,0xFF},
    [BOX_COLOR_GREY]   = {0x5C,0x5C,0x61},
};

void EnJackedBox_Draw(EnJackedBox* this, PlayState* play) {
    Box* b = &this->box;
    Color_RGB8* color = &BoxColor[this->colorIndex];

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    Matrix_Translate(UnfoldVec3f(this->actor.pos), MTXMODE_NEW);
    Matrix_Translate(0.0f,this->actor.shape.yOffset,0.0f, MTXMODE_APPLY);
    Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);
    Matrix_RotateZYX(UnfoldVec3f(this->actor.shape.rot), MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, FINALIZE_MATRIX(), G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gEnJackedBox_DlSpringBase);

    Matrix_Translate(UnfoldVec3f(b->pos), MTXMODE_NEW);
    Matrix_Translate(0.0f, this->verticalOffset, 0.0f, MTXMODE_APPLY);
    Matrix_RotateY_s(this->actor.shape.rot.y, MTXMODE_APPLY);
    Matrix_RotateX_s(b->rotX, MTXMODE_APPLY);

    Matrix_Push();
    gDPSetPrimColor(POLY_OPA_DISP++, 0x80, 0x80, color->r, color->g , color->b ,255);
    Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, FINALIZE_MATRIX(), G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gEnJackedBox_DlCube);
    Matrix_Translate(0.0f, 2000.0f, -1000.0f, MTXMODE_APPLY);
    Matrix_RotateX_s(-this->lidRot, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, FINALIZE_MATRIX(), G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gEnJackedBox_DlCubeLid);
    Matrix_Pop();

    Matrix_Scale(UnfoldVec3f(this->actor.scale), MTXMODE_APPLY);
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton,  this->jointTable, this->skelAnime.dListCount, EnJackedBox_OverrideLimbDraw, EnJackedBox_PostLimbDraw, &this->actor);
    EnJackedBox_DrawTorchFlame(this,play);
}
