#ifndef STGSCHD_H
#define STGSCHD_H
#include <Arduino.h>
#include "DataPacket.h"

#define MAX_STAGES 15 //reduce to reduce memory usage

enum Stage_Trigger {
	ST_NONE, //Will trigger on next tick no matter what
	ST_MANU, //Manual Trigger (waiting for clearance)
	ST_CDWN, //Countdown Trigger (before initial ignition)
	ST_ALTI, //Altitude
	ST_AVEL, //Average Velocity over all axes
	ST_AACC, //Average Acceleration over all axes
	ST_ROTH, //Rocket horizontal?
	ST_COUNT, //For Loops: (int)ST_COUNT
};

enum Stage_Action {
	SA_NONE_OPER, //Do nothing
	SA_BSTR_FIRE, //Begin Booster Ignition
	SA_SSTR_FIRE, //Begin Sustainer Ignition
	SA_STAG_SEPR, //Separate Booster and Sustainer Stages
	SA_PCHTB_RLSE, //Release Parachute on Booster
	SA_PCHTS_RLSE, //Release Parachute on Sustainer
	SA_DROGS_RLSE, //Release Drogue Chute (only on sustainer?)
	SA_COUNT, //For Loops: (int)SA_COUNT
};

struct Stage {
	enum Stage_Trigger i_trig; //Initial Trigger
	float i_cond; //this number depends on the begin type;

	//TODO: make this a vector for multi abort checks?
	enum Stage_Trigger a_trig; //Abort Trigger
	float a_cond;

	enum Stage_Action action;
};

void setupScheduler(bool booster_board);
void stepScheduler(DataPacket *pData);

#endif