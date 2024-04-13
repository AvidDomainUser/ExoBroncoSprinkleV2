#ifndef STGSCHD_H
#define STGSCHD_H
#include <Arduino.h>
#include "DataPacket.h"

#define MAX_STAGES 15 //reduce to reduce memory usage

typedef enum {
	TRIG_NO_TRIG, //triggers no matter what
	TRIG_MANUAL,
	TRIG_COUNTDOWN,
	TRIG_ALTITUDE,
	TRIG_ABS_VELOCITY,
	TRIG_ABS_ACCELERATION,
	TRIG_NOT_VERTICAL, //Rocket horizontal?
	TRIG_COUNT, //For Loops: (int)ST_COUNT
} StageTrigger;

typedef enum {
	ACTION_NO_ACTION,
	ACTION_IGNITE,
	ACTION_STAGE_SEP,
	ACTION_DROGUE_RELEASE, 
	ACTION_CHUTE_RELEASE, //Release Parachute on Booster
	ACTION_COUNT, //For Loops: (int)SA_COUNT
} StageAction;

typedef enum {
	BOOSTER,
	SUSTAINER
} BoardType;
struct Stage {
	//TODO: vector
	StageTrigger trigger;
	float trigger_condition; //this number depends on the begin type;

	//TODO: make this a vector for multi abort checks?
	enum Stage_Trigger abort_trig; //Abort Trigger
	float abort_condition;

	enum Stage_Action action;
};

void setupScheduler(BoardType initialization);
void stepScheduler(DataPacket *pData);

#endif