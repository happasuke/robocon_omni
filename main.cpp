//can id table:
//  0series main board(recieave controller, control omni)
//100series slave

//can id is 1

#include "mbed.h"
#include "CAN.h"
#include "omni.hpp"
#include "util.h"
#include "alias.h"

#define use_controller

Serial pc(p9,p10);
Serial controller(p28,p27);

CAN can(p30,p29);

Omni omni;

const int myid = 100;

inline void init(){
	controller.baud(9600);
	pc.baud(57600);
	can.reset();
	can.frequency(10000);

	pc.printf("\n\nrun lpcxpresso no.1\n");
}


RecievedData checkButton(RecievedData r){

	if(r.cross){
		r.rlr = 0;
		r.rud = 0;
		r.llr = 0;
		r.lud = 0;
	}

	//オートモード
	if(r.triangle){
		if(r.automode==false){
//			urg.putc(10);
			r.automode = true;
			pc.printf("auto mode\n");
		}else{
			r.automode = false;
			pc.printf("stop auto mode\n");
		}

		r.triangle = false;
	}

	return r;
}


int main() {

	//Using "rec" like udp protocol.
	RecievedData rec,prec;
	CANMessage can_rec;

#ifdef use_controller
	const int serialrec_size = 24;
	char serialrec[serialrec_size];
#endif

	init();
	rec.automode = prec.automode = false;

	while(1) {

//		pc.printf("atmode:%d ",prec.automode?1:0);

		if(can.read(can_rec)){
			if(can_rec.id==myid){
				rec.urg1.y = ((int)(can_rec.data[0]|can_rec.data[1]<<5));
				rec.urg1.x = ((int)(can_rec.data[2]|can_rec.data[3]<<5)-600);
				rec.urg2.y = ((int)(can_rec.data[4]|can_rec.data[5]<<5));
				rec.urg2.x = ((int)(can_rec.data[6]|can_rec.data[7]<<5)-600);
				prec.urgenable = true;
				pc.printf("urg:%d %d %d %d\n",rec.urg1.x,rec.urg1.y,rec.urg2.x,rec.urg2.y);
			}else{
				prec.urgenable = false;
			}
		}else{
			prec.urgenable= false;
		}


#ifdef use_controller
		if(controller.readable()){

			controller.gets(serialrec,serialrec_size);

			serialrec[0] = (serialrec[1]-48)*10+(serialrec[2]-48)-28;
			serialrec[1] = (serialrec[4]-48)*10+(serialrec[5]-48)-28;
			serialrec[2] = (serialrec[8]-48);
			serialrec[3] = (serialrec[10]-48)*10+(serialrec[11]-48)-29;
			serialrec[4] = (serialrec[13]-48)*10+(serialrec[14]-48)-29;
			serialrec[5] = (serialrec[16]-48)*10+(serialrec[17]-48)-29;
			serialrec[6] = (serialrec[19]-48)*10+(serialrec[20]-48)-29;

			int flg = CheckRecieveError(serialrec);

			if(flg==1){

				rec = parseControllerData(serialrec);
				rec = correct(rec);
				rec = checkButton(rec);

				CanData data = makeCanMessage(rec,101,0,0);
				char str[3] = {data.data1,data.data2,data.data3};
				pc.printf("101:%x %x %x ",data.data1,data.data2,data.data3);
				can.write(CANMessage(101,str,CAN_MESSAGE_LENGTH));
				data = makeCanMessage(rec,102,serialrec[4],0);
				str[0] = data.data1;str[1] = data.data2;str[2] = data.data3;
				pc.printf("102:%x %x %x ",data.data1,data.data2,data.data3);
				can.write(CANMessage(102,str,CAN_MESSAGE_LENGTH));
				data = makeCanMessage(rec,103,serialrec[5],0);
				str[0] = data.data1;str[1] = data.data2;str[2] = data.data3;
				pc.printf("103:%x %x %x ",data.data1,data.data2,data.data3);
				can.write(CANMessage(103,str,CAN_MESSAGE_LENGTH));

				if(!rec.r1 & !rec.l1 & !rec.right & !rec.left & !rec.top & !rec.down){
					omni.update(rec);
				}
				pc.printf("\n");
			}
		}else if(prec.urgenable){
			pc.printf("urg unebled : automode:%d\n",prec.automode?1:0);
			int flg = omni.update(prec);
			if(flg==AUTO_MODE_FINISHED){
				prec.automode = false;
				pc.printf("\n\n auto mode finished\n\n");
			}
			pc.printf("\n");
		}

		prec = rec;
#endif
	}

	while(1){pc.printf("please reboot\n");wait(1);}
}

