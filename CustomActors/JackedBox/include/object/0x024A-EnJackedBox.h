#ifndef __ENJACKEDBOX_H__
#define __ENJACKEDBOX_H__

extern u16 gEnJackedBox_TexRampTex[];
extern u16 gEnJackedBox_TexCube[];
extern u16 gEnJackedBox_TexCubeTop[];
extern u16 gEnJackedBox_TexCubeInside[];
extern u16 gEnJackedBox_TexMetalCrate[];
extern u16 gEnJackedBox_TexTorchHandle[];
extern u16 gEnJackedBox_TexMetalRusted[];
extern u16 gEnJackedBox_TexSpring[];
extern u16 gEnJackedBox_TexHilt[];
extern u16 gEnJackedBox_TexTorchTop[];
extern u16 gEnJackedBox_TexRampTex2[];
extern Gfx gEnJackedBox_MtlArmGen[];
extern Gfx gEnJackedBox_MtlEmpty[];
extern Gfx gEnJackedBox_MtlCube[];
extern Gfx gEnJackedBox_MtlCubeBottom[];
extern Gfx gEnJackedBox_MtlCubeInside[];
extern Gfx gEnJackedBox_MtlCubeTop[];
extern Gfx gEnJackedBox_MtlMetalBase[];
extern Gfx gEnJackedBox_MtlMetalBaseDarken[];
extern Gfx gEnJackedBox_MtlTorchHandle[];
extern Gfx gEnJackedBox_MtlMetal[];
extern Gfx gEnJackedBox_MtlSpring[];
extern Gfx gEnJackedBox_MtlMetalHandle[];
extern Gfx gEnJackedBox_MtlMetalBackface[];
extern Gfx gEnJackedBox_MtlTorchTop[];
extern Gfx gEnJackedBox_MtlBoxingGlove[];
extern Gfx gEnJackedBox_DlBodyRoot[];
extern Gfx gEnJackedBox_DlBodyArmR[];
extern Gfx gEnJackedBox_DlBodyForearmR[];
extern Gfx gEnJackedBox_DlBodyHandR[];
extern FlexSkeletonHeader gEnJackedBox_SkelBody[];

typedef enum {
    BODY_LIMB_ROOT_TRANSLATE,
    BODY_LIMB_ROOT,
    BODY_LIMB_ARM_R,
    BODY_LIMB_FOREARM_R,
    BODY_LIMB_HAND_R,
    BODY_LIMB_MAX,
} SkelBodyLimbs;

extern Gfx gEnJackedBox_DlCube[];
extern Gfx gEnJackedBox_DlCubeLid[];
extern Gfx gEnJackedBox_DlSpringBase[];
extern Gfx gEnJackedBox_DlAxe[];
extern Gfx gEnJackedBox_DlSpring[];
extern Gfx gEnJackedBox_DlKnife[];
extern Gfx gEnJackedBox_DlTorch[];
extern Gfx gEnJackedBox_DlBoxingGlove[];
extern Gfx gEnJackedBox_DlRapier[];
extern AnimationHeader gEnJackedBox_AnimMenace[];
extern AnimationHeader gEnJackedBox_AnimMenaceOverhead[];
extern AnimationHeader gEnJackedBox_AnimMenaceThrust[];
extern AnimationHeader gEnJackedBox_AnimMenaceTorch[];
extern AnimationHeader gEnJackedBox_AnimSlash[];
extern AnimationHeader gEnJackedBox_AnimStab[];
extern AnimationHeader gEnJackedBox_AnimThrust[];

#endif /* __ENJACKEDBOX_H__ */
