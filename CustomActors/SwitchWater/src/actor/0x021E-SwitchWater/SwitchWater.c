/**
 * by Nokaubure
 *
 * Variable
 * 
 * & 0xFF00 = WaterboxID
 * & 0x003F = SwitchFlag
 * 
 * Xrot = speed, higher the faster it raises/lowers
 * Zrot = target Y position
 **/


#define AVAL(base,type,offset)  (*(type*)((u8*)(base)+(offset)))

#include "z64math.h"

#include "ultra64.h"
#include "global.h"

#include "macros.h"

#define ACT_ID 0x021E
#define OBJ_ID 0x021E
#define OBJ_H "object/0x021E-SwitchWater.h"

#include OBJ_H


typedef struct {
	Actor actor;
	u8 waterboxID;
	u8 SwitchFlag;
	s16 Ytarget;
	f32 Xscale;
	f32 Zscale;
	f32 Speed;
	WaterBox* waterbox;
} SwitchWater;


static void draw(SwitchWater* this, PlayState* play);
static void update(SwitchWater* this, PlayState* play);


static void init(SwitchWater *this, PlayState *play) {

	this->waterboxID = (this->actor.params & 0xFF00) >> 8;
	this->SwitchFlag = (this->actor.params & 0x00FF);
	this->Ytarget = this->actor.home.rot.z;
	this->Speed = ABS(this->actor.home.rot.x) + 1.0f;
	//this->actor.home.rot.z = this->actor.home.rot.x = 0;
	this->actor.world.rot.z = this->actor.world.rot.x = 0;
    this->actor.shape.rot.z = this->actor.shape.rot.x = 0;

	if (this->waterboxID > play->colCtx.colHeader->numWaterBoxes-1)
	{
		Actor_Kill(&this->actor);
	}

	

	this->waterbox = &play->colCtx.colHeader->waterBoxes[this->waterboxID];

	this->actor.home.pos.y = this->waterbox->ySurface;

	if (Flags_GetSwitch(play, this->SwitchFlag))
	{
		this->actor.world.pos.y = this->Ytarget;
		this->waterbox->ySurface = this->Ytarget;
	}
	else
	{
		this->actor.world.pos.y = this->waterbox->ySurface;
	}

	
	this->Xscale = this->waterbox->xLength / 500.0f;
	this->Zscale = this->waterbox->zLength / 500.0f;
	this->actor.world.pos.x = this->waterbox->xMin + (this->waterbox->xLength/2.0f);
	this->actor.world.pos.z = this->waterbox->zMin + (this->waterbox->zLength/2.0f);

	this->actor.scale.x = this->Xscale;
	this->actor.scale.y = 1;
	this->actor.scale.z = this->Zscale;

	this->actor.shape.yOffset = -1.0f;


}

static void update(SwitchWater *this, PlayState *play) {

	if (Flags_GetSwitch(play, this->SwitchFlag))
	{
		if (this->actor.world.pos.y != this->Ytarget)
		{

			Math_ApproachF(&this->actor.world.pos.y, this->Ytarget, 1.0f, this->Speed);
			this->waterbox->ySurface = this->actor.world.pos.y;

	        Audio_PlaySfxGeneral(NA_SE_EV_WATER_LEVEL_DOWN - SFX_FLAG, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                             &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
		}
	}
	else if (this->actor.world.pos.y != this->actor.home.pos.y)
	{
			Math_ApproachF(&this->actor.world.pos.y, this->actor.home.pos.y, 1.0f, this->Speed);
			this->waterbox->ySurface = this->actor.world.pos.y;

	        Audio_PlaySfxGeneral(NA_SE_EV_WATER_LEVEL_DOWN - SFX_FLAG, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                             &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
	}


}



static void dest(SwitchWater *this, PlayState *play) {


	
}

Gfx* Gfx_TexScale(GraphicsContext* gfxCtx, u32 width, u32 height, u32 gameplayFrames) {
    Gfx* displayList = Graph_Alloc(gfxCtx, 6 * sizeof(Gfx));

    u32 x1 = 127 - (gameplayFrames % 128);
    u32 x2 = (gameplayFrames % 128);
    u32 y1 = 64 + (gameplayFrames % 128);
    u32 y2 = (gameplayFrames % 128);
    u32 width1 = 32;
    u32 width2 = 32;
    u32 height1 = 32;
    u32 height2 = 32;

    x1 %= 512 << 2;
    y1 %= 512 << 2;
    x2 %= 512 << 2;
    y2 %= 512 << 2;


    gDPTileSync(displayList);
    gSPTexture(displayList + 1, width, height, 0, G_TX_RENDERTILE, G_ON);
    gDPSetTileSize(displayList + 2, 0, x1, y1, x1 + ((width1 - 1) << 2), y1 + ((height1 - 1) << 2));
    gDPTileSync(displayList + 3);
    gSPTexture(displayList + 1, width, height, 1, G_TX_RENDERTILE, G_ON);
    gDPSetTileSize(displayList + 4, 1, x2, y2, x2 + ((width2 - 1) << 2), y2 + ((height2 - 1) << 2));
    gSPEndDisplayList(displayList + 5);

    return displayList;
}

static void draw(SwitchWater *this, PlayState *play) {

		u32 gameplayFrames;


    	OPEN_DISPS(play->state.gfxCtx, "", 228);

    	gameplayFrames = play->state.frames;


		Gfx_SetupDL_25Xlu(play->state.gfxCtx);

		gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, 0);

		gSPSegment(POLY_XLU_DISP++, 0x08, Gfx_TexScale(play->state.gfxCtx, 0xFFFF * (this->Zscale / 10.0f), 0xFFFF * (this->Xscale / 10.0f), gameplayFrames));


		Gfx_DrawDListXlu(play,(Gfx*)gSwitchwater_DlWaterplane);
		

		CLOSE_DISPS(play->state.gfxCtx, "", 244);


}

/* .data */
const ActorInit init_vars = {
	ACT_ID, 
	ACTORCAT_BG, 
	0x00000030,
	OBJ_ID,
 	sizeof(SwitchWater),
	(ActorFunc)init,
	(ActorFunc)dest,
	(ActorFunc)update,
	(ActorFunc)draw
};


