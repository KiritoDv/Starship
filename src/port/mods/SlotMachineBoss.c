#include "SlotMachineBoss.h"

#include "port/hooks/Events.h"
#include "assets/ast_blue_marine.h"
#include "hud.h"

#define BOSS_STATE boss->state
#define BOSS_SCALE boss->fwork[0]
#define BOSS_TARGET_XPOS boss->fwork[1]
#define BOSS_TARGET_YPOS boss->fwork[2]
#define BOSS_DISTANCE boss->fwork[3]

void SlotMachine_Init(Boss* boss){
    BOSS_DISTANCE = 1500.0f;
    BOSS_SCALE = 2.0f;
    BOSS_STATE = 0;

    gBossActive = true;
    gBossFrameCount = 0;
    boss->drawShadow = true;
    boss->timer_050 = 354;
    boss->health = 2000;
    boss->obj.pos.x = gPlayer[0].pos.x;
    boss->obj.pos.y = gPlayer[0].pos.y;
    boss->obj.pos.z = gPlayer[0].trueZpos - BOSS_DISTANCE;
    boss->vel.z = gPlayer[0].vel.z;
    boss->info.drawType = 1;
}

void SlotMachine_Update(Boss* boss) {
    Vec3f src;
    Vec3f dest;
    gBossFrameCount++;


    // Generic update
    boss->vel.x = dest.x;
    boss->vel.y = dest.y;
    boss->vel.z = dest.z - gPathVelZ;
    boss->obj.pos.z = gPlayer[0].trueZpos - BOSS_DISTANCE + ((gPlayer[0].boostSpeed + gPlayer->baseSpeed) * 10); 
}

void SlotMachine_Draw(Boss* boss){
    RCP_SetupDL(&gMasterDisp, SETUPDL_29_POINT);
    // Matrix_Push(&gGfxMatrix);
    // Matrix_Translate(gGfxMatrix, boss->obj.pos.x, boss->obj.pos.y, boss->obj.pos.z, MTXF_APPLY);
    // Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, aNewBomb);
    // Matrix_Pop(&gGfxMatrix);
}

void OnSpyborgArmsInit(ObjectInitEvent* event) {
    if (event->type != OBJECT_TYPE_BOSS) {
        return;
    }

    Boss* boss = (Boss*) event->object;

    switch (boss->obj.id) {
        case OBJ_BOSS_SX_SPYBORG_LEFT_ARM:
        case OBJ_BOSS_SX_SPYBORG_RIGHT_ARM:
            boss->obj.status = OBJ_FREE;
            event->event.cancelled = true;
            break;
        case OBJ_BOSS_SX_SPYBORG:
            SlotMachine_Init(boss);
            break;
        default:
            break;
    }
}
void OnSpyborgDraw(ObjectDrawPostSetupEvent* event) {
    if (event->type != OBJECT_TYPE_BOSS) {
        return;
    }

    Boss* boss = (Boss*) event->object;
    if (boss->obj.id != OBJ_BOSS_SX_SPYBORG) {
        return;
    }

    event->event.cancelled = true;
    SlotMachine_Draw(boss);
}
void OnSpyborgUpdate(ObjectUpdateEvent* event){
    if (event->type != OBJECT_TYPE_BOSS) {
        return;
    }

    Boss* boss = (Boss*) event->object;
    if (boss->obj.id != OBJ_BOSS_SX_SPYBORG) {
        return;
    }

    event->event.cancelled = true;
    SlotMachine_Update(boss);
}

void SlotMachine_Register() {
    REGISTER_LISTENER(ObjectInitEvent, OnSpyborgArmsInit, EVENT_PRIORITY_NORMAL);
    REGISTER_LISTENER(ObjectUpdateEvent, OnSpyborgUpdate, EVENT_PRIORITY_NORMAL);
    REGISTER_LISTENER(ObjectDrawPostSetupEvent, OnSpyborgDraw, EVENT_PRIORITY_NORMAL);
}