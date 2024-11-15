#ifndef __ENDUMMY_H__
#define __ENDUMMY_H__

extern u16 gEnDummy_TexMetal[];
extern u16 gEnDummy_TexWood[];
extern u16 gEnDummy_TexBarrelLid[];
extern u16 gEnDummy_TexBarrel[];
extern u16 gEnDummy_TexCloth[];
extern u16 gEnDummy_TexFace[];
extern u16 gEnDummy_TexTargetMirrorXMirrorY[];
extern u16 gEnDummy_TexBelt[];
extern u16 gEnDummy_TexSplat[];
extern u16 gEnDummy_TexAttackCone[];
extern u16 gEnDummy_TexHay[];
extern u16 gEnDummy_TexNumber[];
extern u16 gEnDummy_TexNumber0[];
extern u16 gEnDummy_TexNumber1[];
extern u16 gEnDummy_TexNumber2[];
extern u16 gEnDummy_TexNumber3[];
extern u16 gEnDummy_TexNumber4[];
extern u16 gEnDummy_TexNumber5[];
extern u16 gEnDummy_TexNumber6[];
extern u16 gEnDummy_TexNumber7[];
extern u16 gEnDummy_TexNumber8[];
extern u16 gEnDummy_TexNumber9[];
extern Gfx gEnDummy_MtlMetal[];
extern Gfx gEnDummy_MtlWood[];
extern Gfx gEnDummy_MtlBucketTop[];
extern Gfx gEnDummy_MtlBucket[];
extern Gfx gEnDummy_MtlCloth[];
extern Gfx gEnDummy_MtlFace[];
extern Gfx gEnDummy_MtlTarget[];
extern Gfx gEnDummy_MtlBelt[];
extern Gfx gEnDummy_MtlFacePaint[];
extern Gfx gEnDummy_MtlParticleEffect1[];
extern Gfx gEnDummy_MtlParticleEffect2[];
extern Gfx gEnDummy_MtlAttackCone[];
extern Gfx gEnDummy_DlBodyRoot[];
extern Gfx gEnDummy_DlBodyChest[];
extern Gfx gEnDummy_DlBodyHead[];
extern Gfx gEnDummy_DlBodyHandle[];
extern Gfx gEnDummy_DlBodyArmL[];
extern Gfx gEnDummy_DlBodyHandL[];
extern Gfx gEnDummy_DlBodyArmR[];
extern Gfx gEnDummy_DlBodyHandR[];
extern FlexSkeletonHeader gEnDummy_SkelBody[];

typedef enum {
    BODY_LIMB_ROOT_TRANSLATE,
    BODY_LIMB_ROOT,
    BODY_LIMB_CHEST,
    BODY_LIMB_HEAD,
    BODY_LIMB_HANDLE,
    BODY_LIMB_ARM_L,
    BODY_LIMB_HAND_L,
    BODY_LIMB_ARM_R,
    BODY_LIMB_HAND_R,
    BODY_LIMB_MAX,
} SkelBodyLimbs;

extern Gfx gEnDummy_DlGAttackCone[];
extern Gfx gEnDummy_DlParticleEffect1[];
extern Gfx gEnDummy_DlParticleEffect2[];
extern AnimationHeader gEnDummy_AnimIdle[];
extern AnimationHeader gEnDummy_AnimRecoil[];

#endif /* __ENDUMMY_H__ */
