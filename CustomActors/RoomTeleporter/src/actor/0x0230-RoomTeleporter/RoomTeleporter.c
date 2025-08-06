/**
 * by Nokaubure

Variable

X rotation: the amount of X units that will warp the player to, or single point pathway to warp to
Z rotation
& 0x0F00 : 0: uses X units, 1: uses single point pathway
& 0x00FF : the room to teleport to

Variable
& 0x1000 : Disabled, needs switch flag
& 0x2000 : Short cutscene between teleports
& 0xC000 : 0: teleports only Link, 1: Teleports all actors minus teleporters, 2: Teleports pushboxes and small crates (configurable)
& 0x0F00 : 0 red, 1 blue, 2 green, 3 yellow, 4 purple, 5 orange, rest of the colors are unused
& 0x003F : switch flag

 **/

#include <uLib.h>

#define FLAGS (ACTOR_FLAG_4 | ACTOR_FLAG_22)

#define OBJ_ID         0x0200  
#define ACT_ID         0x0230
#define OBJ_H "object/0x0200-RoomTeleporter.h"
#include OBJ_H


const u32 Colors[] = 
{
    0xFF2B00FF, // Red
    0x6A00FFFF, // Blue
    0x00FF26FF, // Green
    0xFFEC00FF, // Yellow
    0xAA329BFF, // Purple
    0xFF973CFF, // Orange
    0xFFFFFFFF, // Empty
    0xFFFFFFFF, // Empty
    0xFFFFFFFF, // Empty
    0xFFFFFFFF, // Empty
    0xFFFFFFFF, // Empty
    0xFFFFFFFF, // Empty
    0xFFFFFFFF, // Empty
    0xFFFFFFFF, // Empty
    0xFFFFFFFF, // Empty
    0xFFFFFFFF, // Empty
};

#define TELEPORT_LINK 0
#define TELEPORT_ALLACTORS 1
#define TELEPORT_SELECTEDACTORS 2

#define WARPMODE_RELATIVEX 0
#define WARPMODE_PATHWAY 1

const u16 ActorsToTeleport[] =
{
    0x0000, //Link
    0x0018, //Navi
    0x00FF, //pushblock
    0x0054, //armos
    0x0110, //small crate
};

typedef struct {
    DynaPolyActor dyna;
    u8 switchflag;
    u8 enabled;
    u8 triggered;
    u8 canwarp;
    u8 playCutscene;
    u8 teleportType;
    u8 color;
    s16 warpX;
    u8 room;
    u8 timer;
    u8 warpmode;
    Vec3f warpamount;
} RoomTeleporter;

static void RoomTeleporter_Update(RoomTeleporter *this, PlayState *play);

void GetPathpointPos(Path* pathList, Vec3f* pos, s32 pathId, s32 pointId) {
    Path* path = pathList;
    Vec3s* point;

    path += pathId;
    point = &((Vec3s*)SEGMENTED_TO_VIRTUAL(path->points))[pointId];

    pos->x = point->x;
    pos->y = point->y;
    pos->z = point->z;

}

static void RoomTeleporter_Init(RoomTeleporter *this, PlayState *play) {


    Actor_SetScale(&this->dyna.actor, 0.1f);
    this->dyna.actor.uncullZoneDownward = 5000.0f;
    this->dyna.actor.uncullZoneForward = 5000.0f;

    CollisionHeader* colHeader = NULL;
    DynaPolyActor_Init(&this->dyna, 0);
    CollisionHeader_GetVirtual((void*)gRoomTeleporter_CollRoomTeleporterCol,&colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna,&this->dyna.actor, colHeader);

    this->enabled = ((this->dyna.actor.params & 0x1000) >> 12) == 0 ? 1 : 0;
    this->playCutscene = (this->dyna.actor.params & 0x2000) >> 13;
    this->teleportType = (this->dyna.actor.params & 0xC000) >> 14;
    this->color = (this->dyna.actor.params & 0x0F00) >> 8;
    this->switchflag = (this->dyna.actor.params & 0x003F);
    this->warpX = this->dyna.actor.home.rot.x;
    this->room = this->dyna.actor.home.rot.z & 0x00FF;
    this->warpmode = (this->dyna.actor.home.rot.z & 0x0F00) >> 8;
    this->canwarp = 0;

    this->dyna.actor.world.rot.z= 0;
    this->dyna.actor.world.rot.x= 0;
    this->dyna.actor.shape.rot.z= 0;
    this->dyna.actor.shape.rot.x= 0;

    if (!this->enabled && Flags_GetSwitch(play, this->switchflag)) {
        this->enabled = 1;
    }

    if (this->warpmode == WARPMODE_RELATIVEX)
    {
        this->warpamount.x = this->warpX;
        this->warpamount.y = 0;
        this->warpamount.z = 0;
    }
    else //WARPMODE_PATHWAY
    {
        Vec3f temp;
        GetPathpointPos(play->setupPathList, &temp, this->warpX, 0);
        this->warpamount.x = temp.x - this->dyna.actor.world.pos.x;
        this->warpamount.y = temp.y - this->dyna.actor.world.pos.y;
        this->warpamount.z = temp.z - this->dyna.actor.world.pos.z;
    }

}

static void BeginTeleport(RoomTeleporter *this, PlayState *play) {
    u8 category = 0;
    Actor* pointer;
    u8 roomchange = (this->room != this->dyna.actor.room);
    u8 prevroom = this->room;

    //this handles warping other actors aside of Link
    while(category != 0xC)
    {
        if (category != ACTORCAT_DOOR)
        {
        pointer = play->actorCtx.actorLists[category].head;
        while (pointer != 0)
        {
            u8 target = 0;
            switch(this->teleportType)
            {
            case TELEPORT_LINK:
                {
                    if (pointer->id == ActorsToTeleport[0] || pointer->id == ActorsToTeleport[1]) target = 1;
                    break;
                }
            case TELEPORT_ALLACTORS:
                {
                    if (pointer->id != ACT_ID) target = 1;
                    break;
                }
            case TELEPORT_SELECTEDACTORS:
                {
                    for(u8 i = 0; i < ARRAY_COUNT(ActorsToTeleport); i++)
                    {
                        if (pointer->id == ActorsToTeleport[i]) {target = 1; break;}
                    }
                    break;
                }
            }
            if (target == 1)
            {
                Math_Vec3f_Sum(&pointer->world.pos, &this->warpamount, &pointer->world.pos);
                Math_Vec3f_Sum(&pointer->prevPos, &this->warpamount, &pointer->prevPos);
                Math_Vec3f_Sum(&pointer->home.pos, &this->warpamount, &pointer->home.pos);

            if (roomchange && pointer->room >= 0) pointer->room = -57;
            }
            
            pointer = pointer->next;
        }
        }
        category++;
    }
    Math_Vec3f_Sum(&play->mainCamera.eye, &this->warpamount, &play->mainCamera.eye);
    Math_Vec3f_Sum(&play->mainCamera.eyeNext, &this->warpamount, &play->mainCamera.eyeNext);
    Math_Vec3f_Sum(&play->mainCamera.at, &this->warpamount, &play->mainCamera.at);

    if (roomchange)
    {
        RoomContext* roomCtx = &play->roomCtx;
        roomCtx->curRoom.num = this->room;

        if (this->playCutscene) this->dyna.actor.room = -1;

        roomCtx->prevRoom.num = -1; //required to despawn actors
        roomCtx->prevRoom.segment = NULL; //hides previous room mesh, has to be set here
        func_80031B14(play, &play->actorCtx); //despawns actors
        func_8009728C(play, &play->roomCtx, this->room); //loads room file, spawns actors and transitions
        func_80097534(play, &play->roomCtx);

        //the following makes so actors can despawn if we use a door later on
        u8 category = 0;
        Actor* pointer;
        while(category != 0xC)
        {
        pointer = play->actorCtx.actorLists[category].head;
        while (pointer != 0)
        {
            
            if (pointer->room == -57)
            {
                pointer->room = this->room;
            }
            
            pointer = pointer->next;
        }

        category++;
        }

    }
    else
    {
        this->triggered = 0;
        this->canwarp = 0;
    }


}
static void RoomTeleporter_Teleporting(RoomTeleporter *this, PlayState *play) {
    this->timer++;
    if (this->timer <= 5) play->envCtx.screenFillColor[3] = 255.0f / CLAMP_MIN((5-this->timer),1);
    else if (this->timer >= 6) play->envCtx.screenFillColor[3] = 255.0f / (this->timer-5);

    if (this->timer == 5)
    {
        BeginTeleport(this,play);
    }
    else if (this->timer == 10)
    {
        func_80064534(play, &play->csCtx);
       // func_8002DF54(play,&this->dyna.actor,0x7);
        if(this->dyna.actor.room == -1) Actor_Kill(&this->dyna.actor);
        else this->dyna.actor.update = (ActorFunc)RoomTeleporter_Update;
        play->envCtx.screenFillColor[3] = 0;
        play->envCtx.fillScreen = false;
    }
}


static void RoomTeleporter_Update(RoomTeleporter *this, PlayState *play) {
    Actor Link = GET_PLAYER(play)->actor;
    if (this->enabled == 0 && Flags_GetSwitch(play, this->switchflag))
       this->enabled = 1;

    if (this->enabled == 0) return;
        
    f32 distance_to_link = Math_Vec3f_DistXZ(&this->dyna.actor.world.pos, &Link.world.pos);
    if (!(this->dyna.interactFlags & DYNA_INTERACT_PLAYER_ON_TOP)) 
            this->canwarp = 1;
        
    if ((this->canwarp != 0) && (this->dyna.interactFlags & DYNA_INTERACT_PLAYER_ON_TOP) && (this->triggered == 0)) {
        this->triggered = 1;
        func_80078884(NA_SE_SY_WHITE_OUT_T); //sound
        if (this->playCutscene) 
        {
            this->dyna.actor.update = (ActorFunc)RoomTeleporter_Teleporting;
            func_80064520(play, &play->csCtx);
            play->envCtx.screenFillColor[0] = 160;
            play->envCtx.screenFillColor[1] = 160;
            play->envCtx.screenFillColor[2] = 160;
            play->envCtx.screenFillColor[3] = 0;
            play->envCtx.fillScreen = true;
        }
        else BeginTeleport(this,play);
    }
}


static void RoomTeleporter_Destroy(RoomTeleporter *this, PlayState *play) {

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);

}


static void RoomTeleporter_Draw(RoomTeleporter *this, PlayState *play) {

        u32 gameplayFrames = play->gameplayFrames;

        Gfx_SetupDL_25Opa(play->state.gfxCtx);

        gDPSetEnvColor(POLY_OPA_DISP++, 0,0,0,0);

        if (this->enabled == 1)
        {

        gDPSetPrimColor(POLY_OPA_DISP++, 0,0, (Colors[this->color] & 0xFF000000) >> 24, (Colors[this->color] & 0x00FF0000) >> 16, (Colors[this->color] & 0x0000FF00)>> 8, 0xFF);

        gSPSegment(POLY_OPA_DISP++, 0x08, Gfx_TexScroll(play->state.gfxCtx, (gameplayFrames * 2) % 128, 0, 32, 32));

        }
        else
        {
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF); // white

            gSPSegment(POLY_OPA_DISP++, 0x08, Gfx_TexScroll(play->state.gfxCtx, 0, 0, 32, 32));
        }


        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        gSPDisplayList(POLY_OPA_DISP++, gRoomTeleporter_DlRoomTeleporter);


}





/* .data */
const ActorInit init_vars = {
	ACT_ID, 
	0x06, 
	FLAGS,
	OBJ_ID,
	sizeof(RoomTeleporter),
	(ActorFunc)RoomTeleporter_Init,
	(ActorFunc)RoomTeleporter_Destroy,
	(ActorFunc)RoomTeleporter_Update,
	(ActorFunc)RoomTeleporter_Draw
};







