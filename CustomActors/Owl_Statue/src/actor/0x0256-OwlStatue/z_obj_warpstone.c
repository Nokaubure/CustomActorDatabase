/*
 * File: z_obj_warpstone.c
 * Overlay: ovl_Obj_Warpstone
 * Description: Owl Statue
 */

#include <uLib.h>
#include "global.h"
#include "vt.h"
#include "src/actor/0x0256-Owl_Statue/z_obj_warpstone.h"
//#include "objects/gameplay_keep/gameplay_keep.h"

#define ACTOR_OBJ_WARPSTONE 0x0256
#define OBJECT_SEK 0x0256

#define FADE_DURATION (1 * 25) // 3 seconds

#define FLAGS (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_FRIENDLY)
//DEFINE_ACTOR(Obj_Warpstone, ACTOR_OBJ_WARPSTONE, ALLOCTYPE_NORMAL)
#define ACTOR_FLAG_TARGETABLE    (1 << 0)
#define ACTOR_FLAG_FRIENDLY      (1 << 3)
#define THIS ((ObjWarpstone*)thisx)
#define actorCsId CsId
#define NA_SE_EV_OWL_WARP_SWITCH_ON	NA_SE_EV_FLYING_AIR
#define	Actor_PlaySfx	Audio_PlayActorSfx2
//#define unk120D4
//#define unk120D6
//#define "gSaveContext.save.saveInfo.playerData.owlActivationFlags" 

typedef enum {
    /* 0x0 */ OWL_WARP_GREAT_BAY_COAST,
    /* 0x1 */ OWL_WARP_ZORA_CAPE,
    /* 0x2 */ OWL_WARP_SNOWHEAD,
    /* 0x3 */ OWL_WARP_MOUNTAIN_VILLAGE,
    /* 0x4 */ OWL_WARP_CLOCK_TOWN,
    /* 0x5 */ OWL_WARP_MILK_ROAD,
    /* 0x6 */ OWL_WARP_WOODFALL,
    /* 0x7 */ OWL_WARP_SOUTHERN_SWAMP,
    /* 0x8 */ OWL_WARP_IKANA_CANYON,
    /* 0x9 */ OWL_WARP_STONE_TOWER,
    /* 0xA */ OWL_WARP_ENTRANCE, // Special index for warping to the entrance of a scene
    /* 0xB */ OWL_WARP_MAX
} OwlWarpId;

typedef enum TargetMode {
    /*  0 */ TARGET_MODE_0,
    /*  1 */ TARGET_MODE_1,
    /*  2 */ TARGET_MODE_2,
    /*  3 */ TARGET_MODE_3,
    /*  4 */ TARGET_MODE_4,
    /*  5 */ TARGET_MODE_5,
    /*  6 */ TARGET_MODE_6,
    /*  7 */ TARGET_MODE_7,
    /*  8 */ TARGET_MODE_8,
    /*  9 */ TARGET_MODE_9,
    /* 10 */ TARGET_MODE_10,
    /* 11 */ TARGET_MODE_MAX
} TargetMode;

void ObjWarpstone_Init(Actor* thisx, PlayState* play);
void ObjWarpstone_Destroy(Actor* thisx, PlayState* play);
void ObjWarpstone_Update(Actor* thisx, PlayState* play);
void ObjWarpstone_Draw(Actor* thisx, PlayState* play2);
void ObjWarpstone_SetupAction(ObjWarpstone* this, ObjWarpstoneActionFunc actionFunc);
s32 ObjWarpstone_ClosedIdle(ObjWarpstone* this, PlayState* play);
s32 ObjWarpstone_BeginOpeningCutscene(ObjWarpstone* this, PlayState* play);
s32 ObjWarpstone_PlayOpeningCutscene(ObjWarpstone* this, PlayState* play);
s32 ObjWarpstone_OpenedIdle(ObjWarpstone* this, PlayState* play);


ActorInit Obj_Warpstone_InitVars = {
    ACTOR_OBJ_WARPSTONE,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_SEK,
    sizeof(ObjWarpstone),
    (ActorFunc)ObjWarpstone_Init,
    (ActorFunc)ObjWarpstone_Destroy,
    (ActorFunc)ObjWarpstone_Update,
    (ActorFunc)ObjWarpstone_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00000000, 0x00, 0x00 },
        { 0x4FC1FFFE, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 20, 60, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, TARGET_MODE_1, ICHAIN_STOP),
};

static Gfx* sOwlStatueDLs[] = { (Gfx*)gOwlStatueClosedDL, (Gfx*)gOwlStatueOpenedDL };

void ObjWarpstone_SetupAction(ObjWarpstone* this, ObjWarpstoneActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void ObjWarpstone_Init(Actor* thisx, PlayState* play) {
    ObjWarpstone* this = THIS;

    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    Collider_InitCylinder(play, &this->collider);
	Collider_SetCylinder(play, &this->collider, &this->dyna.actor, &sCylinderInit);
    Actor_SetFocus(&this->dyna.actor, 40.0f);
	
	//ObjWarpstone_SetupAction(this, ObjWarpstone_OpenedIdle);
	//this->modelIndex = SEK_MODEL_OPENED;

    //if (!OBJ_WARPSTONE_IS_ACTIVATED(OBJ_WARPSTONE_GET_ID(&this->dyna.actor))) {
    if (this->modelIndex != SEK_MODEL_CLOSED) {
        ObjWarpstone_SetupAction(this, ObjWarpstone_ClosedIdle);
    } else {
        ObjWarpstone_SetupAction(this, ObjWarpstone_OpenedIdle);
        this->modelIndex = SEK_MODEL_OPENED;
    }
}

void ObjWarpstone_Destroy(Actor* thisx, PlayState* play) {
    ObjWarpstone* this = THIS;

    Collider_DestroyCylinder(play, &this->collider);
}

s32 ObjWarpstone_ClosedIdle(ObjWarpstone* this, PlayState* play) {
    if (this->collider.base.acFlags & AC_HIT) {
        ObjWarpstone_SetupAction(this, ObjWarpstone_BeginOpeningCutscene);
        return true;
    } else {
    //    /*Ye who hold the sacred sword, leave proof of our encounter.*/
        this->dyna.actor.textId = 0xC00;
        return false;
    }
}

s32 ObjWarpstone_BeginOpeningCutscene(ObjWarpstone* this, PlayState* play) {
    if ((this->dyna.actor.id <= CAM_ID_NONE)) {
        //CutsceneManager_StartWithPlayerCs(this->dyna.actor.id, &this->dyna.actor);
        ObjWarpstone_SetupAction(this, ObjWarpstone_PlayOpeningCutscene);
        //Actor_PlaySfx(&this->dyna.actor, NA_SE_EV_OWL_WARP_SWITCH_ON);
    //} else {
    //    CutsceneManager_Queue(this->dyna.actor.id);
    }
    return true;
}

s32 ObjWarpstone_PlayOpeningCutscene(ObjWarpstone* this, PlayState* play) {
    if (this->openingCSTimer++ >= OBJ_WARPSTONE_TIMER_ACTIVATE_THRESHOLD) {
        //CutsceneManager_Stop(this->dyna.actor.id);
        //Sram_ActivateOwl(OBJ_WARPSTONE_GET_ID(&this->dyna.actor));
        ObjWarpstone_SetupAction(this, ObjWarpstone_OpenedIdle);
    } else if (this->openingCSTimer < OBJ_WARPSTONE_TIMER_OPEN_THRESHOLD) {
        Math_StepToF(&this->dyna.actor.velocity.x, 0.01f, 0.001f);
       Math_StepToS(&this->dyna.actor.home.rot.x, 0xFF, 0x12);
    } else {
        Math_StepToF(&this->dyna.actor.velocity.x, 20.0f, 0.01f);
        if (this->dyna.actor.velocity.x > 0.2f) {
            this->modelIndex = SEK_MODEL_OPENED;
            Math_StepToS(&this->dyna.actor.home.rot.x, 0, 0x14);
        }
    }
    return true;
}

s32 ObjWarpstone_OpenedIdle(ObjWarpstone* this, PlayState* play) {
    /*You can save your progress and quit here.*/
    this->dyna.actor.textId = 0x71B4;
    return false;
}

asm ("TitleSetup_Init = 0x80803CAC");

void ObjWarpstone_ReturnToTitle(PlayState* play) {
    gSaveContext.entranceIndex = 0;
    play->state.running = false;
    SET_NEXT_GAMESTATE(&play->state, TitleSetup_Init, TitleSetupState);
}

void ObjWarpstone_Update(Actor* thisx, PlayState* play) {
    ObjWarpstone* this = THIS;
    if (this->waitReset)
    {
        this->waitReset -= 1;
        if (this->waitReset == 0) {
			gSaveContext.gameMode = GAMEMODE_TITLE_SCREEN;
			play->transitionTrigger = TRANS_TRIGGER_START;
			play->transitionType = TRANS_TYPE_FADE_BLACK;
            ObjWarpstone_ReturnToTitle(play);
		}
		
        //func_8002F2F4(&this->dyna.actor, play);
        this->screenOpa = (1.0f - (((float)this->waitReset) / FADE_DURATION)) * 0xff;
        return;
    }

    if (this->isTalking) {
        if (Actor_TextboxIsClosing(&this->dyna.actor, play)) {
            this->isTalking = false;
        } else if ((Message_GetState(&play->msgCtx) == TEXT_STATE_CHOICE) && Message_ShouldAdvance(play)) {
			// Save and Continue
            if (play->msgCtx.choiceIndex == 0) {
				Play_SaveSceneFlags(play);
				gSaveContext.savedSceneId = play->sceneId;
				Sram_WriteSave(&play->sramCtx);
			// Save and Quit
            } else if (play->msgCtx.choiceIndex == 1) {
                 Audio_PlaySfxGeneral(NA_SE_SY_PIECE_OF_HEART, &gSfxDefaultPos, 4,
                 &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                 &gSfxDefaultReverb);
					play->msgCtx.msgMode = MSGMODE_PAUSED;
					play->state.unk_A0 = 0;
				    Play_SaveSceneFlags(play);
				    gSaveContext.savedSceneId = play->sceneId;
				    Sram_WriteSave(&play->sramCtx);
					gSaveContext.gameMode = GAMEMODE_TITLE_SCREEN;
					if (play->state.frames >= 30) {
						this->waitReset = FADE_DURATION;
					}
			// Cancel
            } else if (play->msgCtx.choiceIndex == 2) {
                 Audio_PlaySfxGeneral(NA_SE_SY_CANCEL, &gSfxDefaultPos, 4,
                 &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                 &gSfxDefaultReverb);
				Message_ContinueTextbox(play, 0x71B5);
            } else {
                Message_CloseTextbox(play);
            }
        }
    } else if (Actor_ProcessTalkRequest(&this->dyna.actor, play)) {
        this->isTalking = true;
    } else if (!this->actionFunc(this, play)) {
        func_8002F2F4(&this->dyna.actor, play);
    }

    Collider_ResetCylinderAC(play, &this->collider.base);
    Collider_UpdateCylinder(&this->dyna.actor, &this->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
}

void ObjWarpstone_Draw(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    ObjWarpstone* this = THIS;

    if (this->screenOpa)
        Environment_FillScreen(play->state.gfxCtx, 0, 0, 0, this->screenOpa, FILL_SCREEN_OPA | FILL_SCREEN_XLU);

    Gfx_DrawDListOpa(play, sOwlStatueDLs[this->modelIndex]);
    if (this->dyna.actor.home.rot.x != 0) {
        OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
		
		Environment_FillScreen(play->state.gfxCtx, 0, 0, 0, 0, FILL_SCREEN_XLU);

        Gfx_SetupDL_25Xlu(play->state.gfxCtx);
        Matrix_Translate(this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y + 34.0f,
                         this->dyna.actor.world.pos.z, MTXMODE_NEW);
        Matrix_Mult(&play->billboardMtxF, MTXMODE_APPLY);
        Matrix_Translate(0.0f, 0.0f, 30.0f, MTXMODE_APPLY);
        Matrix_Scale(this->dyna.actor.velocity.x, this->dyna.actor.velocity.x, this->dyna.actor.velocity.x,
                     MTXMODE_APPLY);
        Matrix_Push();
        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetPrimColor(POLY_XLU_DISP++, 128, 128, 255, 255, 200, this->dyna.actor.home.rot.x);
        gDPSetEnvColor(POLY_XLU_DISP++, 100, 200, 0, 255);
        Matrix_RotateZ(BINANG_TO_RAD_ALT2((play->gameplayFrames * 1500) & 0xFFFF), MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gEffFlash1DL);
        Matrix_Pop();
        Matrix_RotateZ(BINANG_TO_RAD_ALT2(~((play->gameplayFrames * 1200) & 0xFFFF)), MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gEffFlash1DL);

        CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
    }
}
