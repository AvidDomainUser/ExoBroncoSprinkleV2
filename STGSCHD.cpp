#include "STGSCHD.h"

//TODO: Abort Schedule/Sequence?
struct Stage schedule[MAX_STAGES];
BoardType attached_to = BOOSTER;
unsigned int stage_count = 0; //amount of stages scheduled
unsigned int stage_index = 0; //current stage

void add_stage(StageTrigger initial = ST_NONE, float initcond = 0, StageTrigger abort = ST_NONE, float abortcond = 0, Action act = SA_NONE_OPER) {
	if (stage_count > MAX_STAGES) Serial.println("ERROR: COULD NOT FULLY ALLOCATE SCHEDULER!!!"), return;

	schedule[stage_count++] = {
		.trigger = initial,
		.trigger_condition = initcond,

		.abort_trigger = abort,
		.abort_condition = abortcond,

		.action = act,
	}
}

bool test_trigger(DataPacket *pData, StageTrigger tt, float cond) {
	switch (tt) {
		case TRIG_NO_TRIG:
			return true;
		case TRIG_MANUAL:
			Serial.println("TODO! ST_MANU test_trigger");
			//idk how to check the correct pins :sob:
			return false;
		case TRIG_COUNTDOWN:
			Serial.println("TODO! ST_CDWN test_trigger");
			//idk how to do this bc timers are weird
			//also we might have leds to flash?
			return false;
		case TRIG_ALTITUDE:
			return pData->altitude >= cond;
		case TRIG_ABS_VELOCITY:
			return pData->speed >= cond;
		case TRIG_ABS_ACCELERATION:
			float accel_avg[3];
			accel_avg[0] = pData->lsm_accel[0] + pData->icm_accl[0];
			accel_avg[1] = pData->lsm_accel[1] + pData->icm_accl[1];
			accel_avg[2] = pData->lsm_accel[2] + pData->icm_accl[2];

			float total_avg = sqrtf(powf(accel_avg[0], 2) + powf(accel_avg[1], 2) + powf(accel_avg[2], 2));
			return total_avg >= cond;
		case TRIG_NOT_VERTICAL: //TODO: make this a better horizontal indicator
			float gyro_avg[3];
			gyro_avg[0] = pData->lsm_gyro[0] + pData->icm_gyro[0]; //TODO: what is vertical?
			gyro_avg[1] = pData->lsm_gyro[1] + pData->icm_gyro[1];
			gyro_avg[2] = pData->lsm_gyro[2] + pData->icm_gyro[2];

			return gyro_avg[0] >= cond && gyro_avg[2] >= cond; //NOTE: Is y vertical rotation or is z?
		default:
			Serial.println("ERROR: Unexpected stage?");
			return false;
	}
	return false;
}

bool complete_action(StageAction act) {
	//TODO: Please take not of the attached_to var for which pin to activate
	switch (act) {
		case ACTION_NO_ACTION:
			return true;
		case ACTION_IGNITE:
			Serial.println("TODO! SA_BSTR_FIRE complete_action");
			//idk how to do this bc idk how to access pins
			return false;
		case ACTION_STAGE_SEP:
			Serial.println("TODO! SA_STAG_SEPR complete_action");
			//ditto
			return false;
		case ACTION_DROGUE_RELEASE:
			Serial.println("TODO! SA_DROGS_RLSE complete_action");
			//ditto
			return false;
		case ACTION_CHUTE_RELEASE:
			Serial.println("TODO! SA_PCHTS_RLSE complete_action");
			//ditto
			return false;
		default:
			Serial.println("ERROR: Unexpected action?");
			return false;
	}

	return false;
}

//Assuming that there will be one board for each stage
void setupScheduler(BoardType initialization) {
	Serial.println("Creating initial stage schedule!");
	attached_to = initialization;

	//Copied directly from Launch Parameters document (currently handling things in meters and degrees)
	//TODO: Please redo this to be more accurate to state machine
	if (initialization == BOOSTER) {
		add_stage(TRIG_MANUAL);
		add_stage(TRIG_COUNTDOWN);
		add_stage(TRIG_NO_TRIG, 0, TRIG_NOT_VERTICAL, 30, ACTION_IGNITE); //Expected burnout at 422m, optional to add an empty stage for an abort condition?
		add_stage(TRIG_ALTITUDE, 511, TRIG_NO_TRIG, 0, ACTION_STAGE_SEP); //May need to confirm that the acceleration is dropping
		add_stage(TRIG_ABS_ACCELERATION, 100, TRIG_NO_TRIG, 0, ACTION_DROGUE_RELEASE);
		add_stage(TRIG_ALTITUDE, 1118, TRIG_NO_TRIG, 0, ACTION_CHUTE_RELEASE);
	} else { //sustainer
		add_stage(TRIG_ALTITUDE, 512, TRIG_NOT_VERTICAL, 30, ACTION_IGNITE); //Not sure about this
		add_stage(TRIG_ALTITUDE, 3243, TRIG_NO_TRIG, 0, ACTION_DROGUE_RELEASE);
		add_stage(TRIG_ALTITUDE, 1000, TRIG_NO_TRIG, 0, ACTION_CHUTE_RELEASE);
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