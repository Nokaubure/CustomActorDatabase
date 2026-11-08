#ifndef __EN_JACKED_BOX_H__
#define __EN_JACKED_BOX_H__

#include <uLib.h>
#include <ulib_math.h>

#define OBJ_H "object/0x024A-EnJackedBox.h"
#include OBJ_H

//Extra Stuff if not available
/*static u32 rmask(u32 value, u32 mask) {
    u32 shift = __builtin_ctz(mask);
    
    value >>= shift;
    mask >>= shift;
    
    return value & mask;
}*/

#define ACTORFLAG_CAN_TARGET    (1)
#define ACTORFLAG_ENEMY         (1 << 2)
#define ACTORFLAG_FRIENDLY      (1 << 3)
#define ACTORFLAG_ALWAYS_UPDATE (1 << 4)
#define ACTORFLAG_ALWAYS_DRAW  (1 << 5)
#define ACTORFLAG_UNCULLED     (1 << 6)
#define ACTORFLAG_INVISIBLE    (1 << 7)
#define ACTORFLAG_TALK_REQUEST (1 << 8)
#define ACTORFLAG_HOOKSHOT_PULL   (1 << 9)
#define ACTORFLAG_HOOKSHOT_FLY_TO (1 << 10)
#define ACTORFLAG_ENKUSA_CUT      (1 << 11)
#define ACTORFLAG_NO_QUAKE        (1 << 12)
#define ACTORFLAG_HOOKSHOT_ATTACHED (1 << 13)
#define ACTORFLAG_ARROW_CAN_CARRY   (1 << 14)
#define ACTORFLAG_ARROW_ATTACHED    (1 << 15)
#define ACTORFLAG_FORCE_TALK        (1 << 16)
#define ACTORFLAG_HEAVY_OBJECT (1 << 17)
#define ACTORFLAG_CHECKSPOT    (1 << 18)
#define ACTORFLAG_SFX_AT_POS   (1 << 19)
#define ACTORFLAG_SFX_CENTER_2 (1 << 20)
#define ACTORFLAG_SFX_CENTER    (1 << 21)
#define ACTORFLAG_NO_POINTLIGHT (1 << 22)
#define ACTORFLAG_ALWAYS_THROW  (1 << 23)
#define ACTORFLAG_BODY_HIT_SFX  (1 << 24)
#define ACTORFLAG_OCARINA_UPDATE (1 << 25)
#define ACTORFLAG_PRESS_SWITCH   (1 << 26)
#define ACTORFLAG_NO_TARGET_LOCK (1 << 27)
#define ACTORFLAG_SFX_TIMER      (1 << 28)


#define BOX_COLLECTIBLE           rmask(this->actor.params, 0x000F)    // 0000 0000 0000 1111
#define BOX_COLLECTIBLE_FLAG      rmask(this->actor.params, 0x3F00)    // 0011 1111 0000 0000 
#define BOX_COLLECTIBLE_FLAG_RAW       (this->actor.params & 0x3F00)   // 0011 1111 0000 0000 
#define BOX_BEHAVIOR              rmask(this->actor.params, 0xC000)    // 1100 0000 0000 0000

#define BOX_PATH_BEHAVIOR         rmask(this->actor.rot.z, 0x00F0)     // 0000 0000 1111 0000
#define BOX_PATH_ID               rmask(this->actor.rot.z, 0x000F)     // 0000 0000 0000 1111

typedef enum {
    BOX_COLOR_PURPLE,
    BOX_COLOR_BLUE,
    BOX_COLOR_RED,
    BOX_COLOR_GREEN,
    BOX_COLOR_SILVER,
    BOX_COLOR_GREY,
} BoxColorIndex;

typedef enum {
    BOX_COLLECTIBLE_RUPEES = BOX_COLOR_PURPLE,
    BOX_COLLECTIBLE_AMMO   = BOX_COLOR_BLUE,
    BOX_COLLECTIBLE_HEALTH = BOX_COLOR_RED,
    BOX_COLLECTIBLE_MAGIC  = BOX_COLOR_GREEN,
    BOX_COLLECTIBLE_SMALL_KEY = BOX_COLOR_SILVER,
    BOX_COLLECTIBLE_NONE   = BOX_COLOR_GREY,
    BOX_COLLECTIBLE_SMART,
} BoxCollectible;

typedef enum {
    ACTION_DORMANT,
    ACTION_PATROL,
    ACTION_CLOSE_LID,
    ACTION_JUMP_AND_TURN,
    ACTION_JUMP,
    ACTION_TRACK,
    ACTION_MENACE,
    ACTION_ATTACK,
    ACTION_DEATH,
    ACTION_LOOT_BOX,
    ACTION_HELD,
    ACTION_THROWN,
} BoxActionId;

typedef enum {
    BOX_TYPE_AXE,
    BOX_TYPE_KNIFE,
    BOX_TYPE_TORCH,
    BOX_TYPE_BOXING,
    BOX_TYPE_RAPIER,
    BOX_TYPE_MAX,
} BoxHandType;

typedef enum {
    BOX_BEHAVIOR_DORMANT,
    BOX_BEHAVIOR_PATROL,
    BOX_BEHAVIOR_HARMLESS,
} BoxBehavior;

typedef enum {
    PATH_LOOP,
    PATH_REVERSE,
    PATH_RANDOMIZE,
} PathBehavior;

#define STUN_TIMER 88
#define NAVI_ENEMY_JACKED_BOX 0xA

#define VERTICAL_OFFSET_RAISED  20.0f
#define VERTICAL_OFFSET_LOWERED  6.0f

#define SPRING_VERTICAL_SCALE 0.05f    // impulse strength
#define SPRING_K_VERTICAL 0.2f         // stiffness
#define SPRING_K 0.08f
#define SPRING_DAMPING 0.80f
#define SPRING_LEAN_FULL_AT_DELTA 12.0f
#define SPRING_MAX_LEAN DEG_TO_BINANG(60)

#define FINALIZE_MATRIX() Matrix_NewMtx(__gfxCtx, __FILE__, __LINE__)

static int ExcludeRandom(int lastchosen, int base, int range){
    //Prevent Rerolls to same position
    int ExcludeRandom = Rand_S16Offset(base, range);
    
    while (ExcludeRandom == lastchosen){
        ExcludeRandom = Rand_S16Offset(base, range);
    }
    return ExcludeRandom;
}

struct EnJackedBox;

typedef struct {
    Vec3f pos;       // lagged position of box+hand assembly
    Vec3f posVel;    // spring-damper velocity for topPos
    f32   posImpulse;
    s16   rotX;      // lean of that assembly
    s16   rotXVel;   // spring-damper velocity for topRot
} Box;

typedef void (* EnJackedBoxFunc)(struct EnJackedBox*, PlayState*);

typedef struct EnJackedBox {
    Actor actor;
    EnJackedBoxFunc func;
    SkelAnime skelAnime;
    Vec3s jointTable[BODY_LIMB_MAX];
    Vec3s morphTable[BODY_LIMB_MAX];

    ColliderCylinder collider;
    ColliderJntSph colliderSpheres;
    ColliderJntSphElement colliderSpheresElements[3];

    LightNode* lightNode;
    LightInfo lightInfo;

    Box box;
    f32 verticalOffset;
    f32 scaleVelocity;
    f32 prevVelY;
    f32 effectScale;
    s32 effectIndex;

    s16 flameTextureScroll;
    s16 lidRot;

    s8 colorIndex; // maps drop contents to color. BOX_COLLECTIBLE_SMART will color boxes to the most likely needed item based on a generated weighted list 

    s8 pathIndex;
    s8 pathBehavior;
    s8 pathReverse; // flag for reversing direction

    s8 actionId;  // current action id
    s8 actionVar; // multiple meanings depending on context of action. i.e. waypoint during patrol, boredom timer etc
    s8 stunTimer;
    
    s8 attackOn;
    u8 handType;
    
} EnJackedBox;

#endif // __EN_JACKED_BOX_H__