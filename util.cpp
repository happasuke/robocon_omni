/*
 * util.cpp
 *
 *  Created on: 2015/04/04
 *      Author: 葉介
 */

#include "math.h"
#include "string.h"
#include "util.h"
#include "Instruct.hpp"

float sq(float n){
	return n*n;
}

Point rotatePoint(float x,float y){
	Point p;
	p.x = x*cos(PI3p4)-y*sin(PI3p4);
	p.y = x*sin(PI3p4)+y*cos(PI3p4);
	return p;
}

float powerErrorControl(float source){
	if(source>1){
		source = 1;
	}else if(source<0){
		source = 0;
	}
	return source;
}


RecievedData parseControllerData(char* rec){

	//	protocol
	//
	//	+----+----+
	//	|head|data|
	//	+----+----|
	//	|1bit|7bit|
	//	+----+----+
	//
	//	+---------+
	//	| |×|■|△|○|
	//	+---------+
	//	| |↓|←|↑|→|
	//	+---------+
	//	| |l2|l1|r2|r1|
	//	+---------+
	//	|   RX    |
	//	+---------+
	//	|   RY    |
	//	+---------+
	//	|   LX    |
	//	+---------+
	//	|   LY    |
	//	+---------+

	static RecievedData rd;

	rd.triangle = rd.circle = rd.cross = rd.square = false;
	rd.top = rd.down = rd.left = rd.right = false;
	rd.l1 = rd.r1 = rd.isPower = false;

	if((rec[0] & 0x08)==0x08)rd.cross = true;
	if((rec[0] & 0x04)==0x04)rd.square = true;
	if((rec[0] & 0x02)==0x02)rd.triangle = true;
	if((rec[0] & 0x01)==0x01)rd.circle = true;

	if((rec[1] & 0x08)==0x08)rd.down  = true;
	if((rec[1] & 0x04)==0x04)rd.left  = true;
	if((rec[1] & 0x02)==0x02)rd.top   = true;
	if((rec[1] & 0x01)==0x01)rd.right = true;

	if((rec[2] & 0x04)==0x04)rd.l1 = true;
	if((rec[2] & 0x01)==0x01)rd.r1 = true;

	rd.rlr = (rec[3]-14);
	rd.rud = rec[4]-14;
	rd.llr = (rec[5]-14);
	rd.lud = rec[6]-14;

	return rd;
}


RecievedData correct(RecievedData r){
	static int th=4;

	//cut threshold
	if(r.llr>-th&&r.llr<th && r.lud>-th&&r.lud<th){
		r.llr = r.lud = 0;
	}

	if(r.rlr>-th&&r.rlr<th && r.rud>-th&&r.rud<th){
		r.rlr = r.rud = 0;
	}

//rotate input
	Point p = rotatePoint(r.rlr, r.rud);
	r.rlr = p.x;
	r.rud = p.y;

	r.rud = -r.rud/14.0;
	r.rlr = r.rlr/14.0;
	r.lud = -r.lud/14.0;
	r.llr = r.llr/14.0;
	return r;
}


CanData makeCanMessage(RecievedData r,int id,char arg1,char arg2){
	CanData data;
	data.data1 = data.data2 = data.data3 = 0;
//id:101
//data[0] : |↓|←|↑|→|×|○|l1|r1| 	//what key
//data[1] : |0						//what stick input
//data[2] : |^						//what instruct
//id:102
//data[0] : |↓|← |↑|0             | //what key
//data[1] : |→|l1|□|right y stick |	//what stick input
//data[2] : |^						//what instruct
//id:103
//data[0] : |l1|□|        		  |	//what key
//data[1] : |0 |0|0|right x stick |	//what stick input
//data[2] : |^			 		  |	//what instruct

	if(id==101){
		if(r.r1){
			data.data1 |= 0x01;
		}
		if(r.l1){
			data.data1 |= 0x02;
		}
		if(r.circle){
			data.data1 |= 0x04;
		}
		if(r.cross){
			data.data1 |= 0x08;
		}
		if(r.right){
			data.data1 |= 0x10;
		}
		if(r.top){
			data.data1 |= 0x20;
		}
		if(r.left){
			data.data1 |= 0x40;
		}
		if(r.down){
			data.data1 |= 0x80;
		}

		if(r.r1&&r.l1){
			data.data3 = SWITCH_ROLLER_ROTATION;
		}else if(!r.r1&!r.l1&(r.right | r.top | r.left | r.down)){
			data.data3 = RING_CHARGE;
		}

	}else if(id==102){
		if(r.r1){
			data.data2 |= 0x20;
		}
		if(r.square){
			data.data2 |= 0x40;
		}
		if(r.right){
			data.data2 |= 0x80;
		}
		if(r.top){
			data.data1 |= 0x20;
		}
		if(r.left){
			data.data1 |= 0x40;
		}
		if(r.down){
			data.data1 |= 0x80;
		}

		data.data1 |= arg1;

		if(!r.r1&r.l1&(r.right|r.left|r.top)&((arg1-14)!=0)){
			data.data3 |= CHANGE_HEIGHT;
		}
		if(r.square){
			data.data3 |= USE_AIRCYLINDER;
		}

	}else if(id==103){
		if(r.l1){
			data.data1 |= 0x80;
		}

		data.data2 |= arg1;

		if(r.l1 && arg1-14){
			data.data3 |= CHANGE_WIDTH;
		}

		if(r.l1 && r.square){
			data.data1 |= 0x40;
			data.data3 |= SWITCH_LASER;
		}
	}

	return data;
}

int CheckRecieveError(char *r){
	static char pr[3]={""};
	static int c=0,roopc=0;

	roopc++;
	if(0==strcmp(pr,r)){
		c++;
		if(c==3){
			roopc = c = 0;
			return 1;
		}
	}else if(roopc>3){
		strncpy(pr,r,3);
		roopc = c = 0;
	}
	return 0;
}

float abs(float a){
	if(a>=0){
		return a;
	}else{
		return -a;
	}
}

int iabs(int n){
	if(n>=0)return n;else return -n;
}




