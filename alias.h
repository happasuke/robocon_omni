#ifndef __ALIAS__
#define __ALIAS__

enum{

//prompt
	STOP = 0,
	GO = 1,

	NONE,
	CHANGE_DIRECTION,
	CONTROL_POWER,
	CHANNEL,

	AUTO_MODE_FINISHED

};


float upper = 0.5;
const float gain = 0.01;
float revi = 0.0006;//補正値

#endif
