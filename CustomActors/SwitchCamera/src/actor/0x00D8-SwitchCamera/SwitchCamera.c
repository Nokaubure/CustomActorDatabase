/**
 * by Nokaubure

Variable

& 0x0F00 : camera ID
& 0x1000 : 1 discovery sound
& 0x2000 : 2 use camera ID
& 0x003F : switch flag

Zrot = time in seconds / 10 (1 unit = 0.1 seconds), defaults at 1 second

 **/


#define AVAL(base,type,offset)  (*(type*)((u8*)(base)+(offset)))

#include "z64math.h"

#include "ultra64.h"
#include "global.h"

#include "macros.h"

#define OBJ_ID  0x01      
#define ACT_ID 0x0005

#define SMOOTH 1



typedef struct {
    Actor actor;
    u16 switchflag;
    s16 camera;
    u16 timer;
    u8 useCamID;
    u8 camID;
    u8 enabled;
    u8 discoverysound;
    Vec3f targetpos;
    Vec3f currentpos;
    Vec3f currenttarget;
    Vec3f posincr;
    Vec3f targetincr;

} entity_t;


Vec3f move_distance_by_XYrot(float distance, Vec3s angle){

    Vec3f result;


    angle.x = -angle.x;

    //pitch yaw
    f32 sp24 = Math_CosS(angle.x) * distance;
    result.x = Math_SinS(angle.y) * sp24;
    result.y = Math_SinS(angle.x) * distance;
    result.z = Math_CosS(angle.y) * sp24;

    return result;

}



void init(entity_t *en, PlayState *global) {

    en->camID = (en->actor.params & 0x0F00) >> 8;
    en->switchflag = (en->actor.params & 0x003F);
    en->discoverysound = ((en->actor.params & 0x1000) != 0);
    en->useCamID = ((en->actor.params & 0x2000) != 0);
    en->timer = en->actor.world.rot.z * 2;
    if (en->timer == 0) en->timer = 10;
    
    if (Flags_GetSwitch(global, en->switchflag) != 0)
    {
        Actor_Kill(&en->actor);
    }

}



void update(entity_t *en, PlayState *global) {


    if (en->enabled == 0 && Flags_GetSwitch(global, en->switchflag) != 0)
    {
        en->enabled = 1;

        if (en->discoverysound)
        {
            func_80078884((u16)NA_SE_SY_CORRECT_CHIME);
        }
        if (en->useCamID == 0)
        {
        en->camera = Play_CreateSubCamera(global);
        Play_ChangeCameraStatus(global, CAM_ID_MAIN, CAM_STAT_WAIT);
        Play_ChangeCameraStatus(global, en->camera, CAM_STAT_ACTIVE);
        en->targetpos = move_distance_by_XYrot(50.0f, en->actor.rot);
        en->targetpos.x += en->actor.world.pos.x;
        en->targetpos.y += en->actor.world.pos.y;
        en->targetpos.z += en->actor.world.pos.z;

        Play_CameraSetAtEye(global, en->camera, &en->targetpos, &en->actor.pos);
        }
        else
        {

            Camera_ChangeBgCamIndex(&global->mainCamera, en->camID);


            //Play_CameraSetAtEye(global, en->camera, &en->currenttarget, &en->currentpos);

            //func_800C0808(global, en->camera, GET_PLAYER(global), CAM_SET_FREE0);

        }
        func_80064520(global, &global->csCtx);


    }
    else if (en->enabled == 1)
    {
        en->timer--;
        if (en->useCamID != 0)
        {
            Camera_ChangeBgCamIndex(&global->mainCamera, en->camID);
        }
        if (en->timer == 0)
        {
            func_80064534(global, &global->csCtx); 
            if (!en->useCamID) func_800C08AC(global, en->camera,0);
            Actor_Kill(&en->actor);
            
        }

    }


}

static void dest(entity_t *en, PlayState *global) {

}


static void draw(entity_t *en, PlayState *global) {




    
}





/* .data */
const ActorInit init_vars = {
    ACT_ID, 
    ACTORCAT_BG, 
    0x00000010,
    OBJ_ID,
    sizeof(entity_t),
    (ActorFunc)init,
    (ActorFunc)dest,
    (ActorFunc)update,
    (ActorFunc)draw
};







