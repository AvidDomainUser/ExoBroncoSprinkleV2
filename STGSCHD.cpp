#include "STGSCHD.h"

//TODO: Abort Schedule/Sequence?
struct Stage schedule[MAX_STAGES];
unsigned int stage_count = 0; //amount of stages scheduled
unsigned int stage_index = 0; //current stage

typedef enum Stage_Action Action;
typedef enum Stage_Trigger Trigger;

void add_stage(Trigger it = ST_NONE, float ic = 0, Trigger at = ST_NONE, float ac = 0, Action act = SA_NONE_OPER) {
	if (stage_count > MAX_STAGES) Serial.println("ERROR: COULD NOT FULLY ALLOCATE SCHEDULER!!!"), return;

	schedule[stage_count++] = {
		.i_trig = it,
		.i_cond = ic,

		.a_trig = at,
		.a_cond = ac,

		.action = act,
	}
}

bool test_trigger(DataPacket *pData, Trigger tt, float cond) {
	switch (tt) {
		case ST_NONE:
			return true;
		case ST_MANU:
			Serial.println("TODO! ST_MANU test_trigger");
			//idk how to check the correct pins :sob:
			return false;
		case ST_CDWN:
			Serial.println("TODO! ST_CDWN test_trigger");
			//idk how to do this bc timers are weird
			//also we might have leds to flash?
			return false;
		case ST_ALTI:
			return pData->altitude >= cond;
		case ST_AVEL:
			return pData->speed >= cond;
		case ST_AACC:
			float accel_avg[3];
			accel_avg[0] = pData->lsm_accel[0] + pData->icm_accl[0];
			accel_avg[1] = pData->lsm_accel[1] + pData->icm_accl[1];
			accel_avg[2] = pData->lsm_accel[2] + pData->icm_accl[2];

			float total_avg = sqrtf(powf(accel_avg[0], 2) + powf(accel_avg[1], 2) + powf(accel_avg[2], 2));
			return total_avg >= cond;
		case ST_ROTH: //TODO: make this a better horizontal indicator
			float gyro_avg[3];
			gyro_avg[0] = pData->lsm_gyro[0] + pData->icm_gyro[0];
			gyro_avg[1] = pData->lsm_gyro[1] + pData->icm_gyro[1];
			gyro_avg[2] = pData->lsm_gyro[2] + pData->icm_gyro[2];

			return gyro_avg[0] >= cond && gyro_avg[2] >= cond; //NOTE: Is y vertical rotation or is z?
		default:
			Serial.println("ERROR: Unexpected stage?");
			return false;
	}
	return false;
}

bool complete_action(Action act) {
	switch (act) {
		case SA_NONE_OPER:
			return true;
		case SA_BSTR_FIRE:
			Serial.println("TODO! SA_BSTR_FIRE complete_action");
			//idk how to do this bc idk how to access pins
			return false;
		case SA_SSTR_FIRE:
			Serial.println("TODO! SA_SSTR_FIRE complete_action");
			//ditto
			return false;
		case SA_STAG_SEPR:
			Serial.println("TODO! SA_STAG_SEPR complete_action");
			//ditto
			return false;
		case SA_PCHTB_RLSE:
			Serial.println("TODO! SA_PCHTB_RLSE complete_action");
			//ditto
			return false;
		case SA_PCHTS_RLSE:
			Serial.println("TODO! SA_PCHTS_RLSE complete_action");
			//ditto
			return false;
		case SA_DROGS_RLSE:
			Serial.println("TODO! SA_DROGS_RLSE complete_action");
			//ditto
			return false;
		default:
			Serial.println("ERROR: Unexpected action?");
			return false;
	}

	return false;
}

//Assuming that there will be one board for each stage
void setupScheduler(bool booster_board) {
	Serial.println("Creating initial stage schedule!");

	//Copied directly from Launch Parameters document (currently handling things in meters and degrees)
	if (booster_board) {
		add_stage(ST_MANU);
		add_stage(ST_CDWN);
		add_stage(ST_NONE, 0, ST_ROTH, 10, SA_BSTR_FIRE); //Expected burnout at 422m, optional to add an empty stage for an abort condition?
		add_stage(ST_ALTI, 511, ST_NONE, 0, SA_STAG_SEPR); //May need to confirm that the acceleration is dropping
		add_stage(ST_ALTI, 1118, ST_NONE, 0, SA_PCHTB_RLSE);
	} else {
		add_stage(ST_ALTI, 512, ST_ROTH, 60, SA_SSTR_FIRE); //Not sure about this
		add_stage(ST_ALTI, 3243, ST_AACC, 10, SA_DROGS_RLSE);
		add_stage(ST_ALTI, 1000, ST_NONE, 0, SA_PCHTS_RLSE);
	}
}

void stepScheduler(DataPacket *pData) {
	if (stage_index >= stage_count) return; //Nothing left to do
	Stage current = schedule[stage_index];
	
	//Do we abort?
	if (!test_trigger(pData, current.a_trig, current.a_cond)) {
		Serial.println("ABORT ABORT ABORT!!!");
		stage_index = stage_count + 1;
		//TODO: Abort Sequence
		return;
	}
	
	//Do we start this sequence?
	if (test_trigger(pData, current.i_trig, current.i_cond)) {
		Serial.println("Initiating a stage action!");
		bool succ = complete_action(current.action);

		if (!succ) {
			Serial.println("Failed to initiate stage action. Aborting...");
			stage_index = stage_count + 1;
			//TODO: Abort Sequence
			return;
		}

		stage_index++;
	}
}