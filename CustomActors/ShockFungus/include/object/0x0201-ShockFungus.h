#ifndef __SHOCKFUNGUS_H__
#define __SHOCKFUNGUS_H__

extern u16 gShockfungus_TexMushroomStem2[];
extern u16 gShockfungus_TexMushroom[];
extern u16 gShockfungus_TexEye[];
extern u16 gShockfungus_TexEye0[];
extern u16 gShockfungus_TexEye1[];
extern u16 gShockfungus_TexEye2[];
extern u16 gShockfungus_TexStem[];
extern u16 gShockfungus_TexTransition[];
extern Gfx gShockfungus_MtlF3dliteMaterial[];
extern Gfx gShockfungus_MtlF3dliteMaterial001[];
extern Gfx gShockfungus_MtlF3dliteMaterial004[];
extern Gfx gShockfungus_MtlF3dliteMaterial005[];
extern Gfx gShockfungus_MtlF3dliteMaterial006[];
extern Gfx gShockfungus_DlCubeBody[];
extern Gfx gShockfungus_DlCubeHead[];
extern Gfx gShockfungus_DlCubeShroom[];
extern Gfx gShockfungus_DlCubeHipL[];
extern Gfx gShockfungus_DlCubeFootL[];
extern Gfx gShockfungus_DlCubeHipR[];
extern Gfx gShockfungus_DlCubeFootR[];
extern FlexSkeletonHeader gShockfungus_SkelCube[];

typedef enum {
    CUBE_LIMB_ROOT_TRANSLATE,
    CUBE_LIMB_ROOT,
    CUBE_LIMB_BODY,
    CUBE_LIMB_HEAD,
    CUBE_LIMB_SHROOM,
    CUBE_LIMB_HIP_L,
    CUBE_LIMB_FOOT_L,
    CUBE_LIMB_HIP_R,
    CUBE_LIMB_FOOT_R,
    CUBE_LIMB_MAX,
} SkelCubeLimbs;

extern AnimationHeader gShockfungus_Anim00[];
extern AnimationHeader gShockfungus_AnimDying[];
extern AnimationHeader gShockfungus_AnimDying001[];
extern AnimationHeader gShockfungus_AnimIdle[];
extern AnimationHeader gShockfungus_AnimNotToUse[];
extern AnimationHeader gShockfungus_AnimRoll[];
extern AnimationHeader gShockfungus_AnimRunning[];
extern AnimationHeader gShockfungus_AnimSpores[];

#endif /* __SHOCKFUNGUS_H__ */
