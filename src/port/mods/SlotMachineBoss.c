#include "SlotMachineBoss.h"

#include "port/hooks/Events.h"
#include "assets/ast_blue_marine.h"
#include "hud.h"

void SlotMachine_Init(Boss* boss){
    gBossActive = true;
    gBossFrameCount = 0;

    boss->drawShadow = true;
    boss->timer_050 = 354;
    boss->health = 601;
    boss->fwork[18] = -gArwingSpeed - 10.0f;
    boss->obj.pos.z = gPlayer[0].trueZpos + 2000.0f;
}

void SlotMachine_Update(Boss* boss){
    Vec3f src;
    Vec3f dest;
    gBossFrameCount++;

    boss->info.drawType = 1;

    src.x = 0.0f;
    src.y = 0.0f;
    src.z = 60.0f;

    Matrix_MultVec3f(gCalcMatrix, &src, &dest);

    boss->vel.x = dest.x;
    boss->vel.y = dest.y;
    boss->vel.z = dest.z - gPathVelZ;

    boss->fwork[16] = 4.0f;
    boss->vel.z = boss->fwork[18];
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