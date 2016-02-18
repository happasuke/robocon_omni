/*
 * omni.hpp
 *
 *  Created on: 2015/05/29
 *      Author: 葉介
 */

//dual shock3からの信号をmbedのusbで受け取る

#ifndef OMNI_HPP_
#define OMNI_HPP_

#define use_cat

#include "mbed.h"
#include "AutoControll.hpp"
#include "util.h"
#include "alias.h"

PwmOut pwm1(p26);//dig 2[4]
PwmOut pwm2(p25);//dig 2[5]
PwmOut pwm3(p24);//dig 2[6]
PwmOut pwm4(p23);//dig 2[7]

#ifdef use_cat
DigitalOut roll(p22);
#endif

class Omni{

	float output[4];

public:
	Omni(){
		pwm1.period_us(400);
		pwm2.period_us(400);
		pwm3.period_us(400);
		pwm4.period_us(400);

		LPC_PINCON->PINSEL4 &= ~(3<<8);	//P2[4]
		LPC_GPIO2->FIODIR |= 1<<4;

		LPC_PINCON->PINSEL4 &= ~(3<<10);	//P2[5]
		LPC_GPIO2->FIODIR |= 1<<5;

		LPC_PINCON->PINSEL4 &= ~(3<<12);	//P2[6]
		LPC_GPIO2->FIODIR |= 1<<6;

		LPC_PINCON->PINSEL4 &= ~(3<<14);	//P2[7]
		LPC_GPIO2->FIODIR |= 1<<7;

		pwm1 = pwm2 = pwm3 = pwm4 = 0;

		clr1Dig();
		clr2Dig();
		clr3Dig();
		clr4Dig();
	}

	void set1Dig(){
		LPC_GPIO2->FIOSET |= 1<<4;
	}

	void clr1Dig(){
		LPC_GPIO2->FIOCLR |= 1<<4;
	}

	void set2Dig(){
		LPC_GPIO2->FIOSET |= 1<<5;
	}

	void clr2Dig(){
		LPC_GPIO2->FIOCLR |= 1<<5;
	}

	void set3Dig(){
		LPC_GPIO2->FIOSET |= 1<<6;
	}

	void clr3Dig(){
		LPC_GPIO2->FIOCLR |= 1<<6;
	}

	void set4Dig(){
		LPC_GPIO2->FIOSET |= 1<<7;
	}

	void clr4Dig(){
		LPC_GPIO2->FIOCLR |= 1<<7;
	}

	inline int AutoControl(RecievedData rec){
		Point goal = detectFixedPosition(rec.urg1, rec.urg2);
		Point center = getCenter(rec.urg1, rec.urg2);
		float power = calcPower(goal);
		float katamuki = goal.x/goal.y;
		float akatamuki = goal.y/goal.x;

		//reach goal
		if(goal.x>0 && goal.y>0){
			pwm1 = (1-power)*katamuki;set1Dig();
			pwm2 = (1-power)*akatamuki;set2Dig();
			pwm3 = power*katamuki;clr3Dig();
			pwm4 = power*akatamuki;clr4Dig();
		}else if(goal.x>0 && goal.y<0){
			pwm1 = (1-power)*katamuki;set1Dig();
			pwm2 = power*akatamuki;clr2Dig();
			pwm3 = (1-power)*katamuki;set3Dig();
			pwm4 = power*akatamuki;clr4Dig();
		}else if(goal.x<0 && goal.y>0){
			pwm1 = power*katamuki;clr1Dig();
			pwm2 = (1-power)*akatamuki;set2Dig();
			pwm3 = power*katamuki;clr3Dig();
			pwm4 = (1-power)*akatamuki;set4Dig();
		}else if(goal.x<0 && goal.y<0){
			pwm1 = power*katamuki;clr1Dig();
			pwm2 = power*akatamuki;clr2Dig();
			pwm3 = (1-power)*katamuki;set3Dig();
			pwm4 = (1-power)*akatamuki;set4Dig();
		}else{
			//rotation
			if(abs(center.x)<5){
				pwm1 = pwm2 = pwm3 = pwm4 = 0;
				clr1Dig();
				clr2Dig();
				clr3Dig();
				clr4Dig();

				return AUTO_MODE_FINISHED;

			}else if(center.x>4){
				power = 0.01;
				pwm1 = 1-power;set1Dig();
				pwm2 = 1-power;set2Dig();
				pwm3 = 1-power;set3Dig();
				pwm4 = 1-power;set4Dig();
			}else if(center.x<4){
				power = 0.01;
				pwm1 = power;clr1Dig();
				pwm2 = power;clr2Dig();
				pwm3 = power;clr3Dig();
				pwm4 = power;clr4Dig();
			}
		}



		return 0;
	}

// 2 roller 3
//	+-----+
//	|     |
//	|     |
//	+-----+
// 1  air   4

	inline void ManualControl(RecievedData rec, RecievedData prec){
		if(rec.rud>0){
			if(rec.rlr>=0){
				pwm1 = ((1-rec.rlr)+(1-prec.rlr))/2;set1Dig();
				pwm2 = ((1-rec.rud)+(1-prec.rud))/2;set2Dig();
				pwm3 = ((rec.rlr)+(prec.rlr))/2;clr3Dig();
				pwm4 = ((rec.rud)+(prec.rud))/2;clr4Dig();
			}else if(rec.rlr<0){
				pwm1 = (abs(rec.rlr)+abs(prec.rlr))/2;clr1Dig();
				pwm2 = ((1-rec.rud)+(1-prec.rud))/2;  set2Dig();
				pwm3 = (1-abs(rec.rlr)+1-abs(prec.rlr))/2;set3Dig();
				pwm4 = ((rec.rud)+(prec.rud))/2;  clr4Dig();
			}
		}else if(rec.rud<0){
			if(rec.rlr>=0){
				pwm1 = ((1-rec.rlr)+(1-prec.rlr))/2;  set1Dig();
				pwm2 = (abs(rec.rud)+abs(prec.rud))/2;clr2Dig();
				pwm3 = ((rec.rlr)+(prec.rlr))/2;  clr3Dig();
				pwm4 = (1-abs(rec.rud)+1-abs(prec.rud))/2;set4Dig();
			}else if(rec.rlr<0){
				pwm1 = (abs(rec.rlr)+abs(prec.rlr))/2;clr1Dig();
				pwm2 = (abs(rec.rud)+abs(prec.rud))/2;clr2Dig();
				pwm3 = (1-abs(rec.rlr)+1-abs(prec.rlr))/2;set3Dig();
				pwm4 = (1-abs(rec.rud)+1-abs(prec.rud))/2;set4Dig();
			}
		}else if(rec.rud==0&&rec.rlr!=0){
			if(rec.rlr>0){
				pwm1 = ((1-rec.rlr)+(1-prec.rlr))/2;set1Dig();
				pwm2 = 1;		 					set2Dig();
				pwm3 = ((rec.rlr)+(prec.rlr))/2;clr3Dig();
				pwm4 = 1;		 					set4Dig();
			}else{
				pwm1 = (abs(rec.rlr)+abs(prec.rlr))/2;clr1Dig();
				pwm2 = 1;			  				  set2Dig();
				pwm3 = (1-abs(rec.rlr)+1-abs(prec.rlr))/2;set3Dig();
				pwm4 = 1;  			  				  set4Dig();
			}
		}else if(rec.llr>0){
			pwm1 = abs(rec.llr/2);clr1Dig();
			pwm2 = abs(rec.llr/2);clr2Dig();
			pwm3 = abs(rec.llr/2);clr3Dig();
			pwm4 = abs(rec.llr/2);clr4Dig();
		}else if(rec.llr<0){
			pwm1 = 1-abs(rec.llr/2);set1Dig();
			pwm2 = 1-abs(rec.llr/2);set2Dig();
			pwm3 = 1-abs(rec.llr/2);set3Dig();
			pwm4 = 1-abs(rec.llr/2);set4Dig();
		}else{
			pwm1 = pwm2 = pwm3 = pwm4 = 0;
			clr1Dig();
			clr2Dig();
			clr3Dig();
			clr4Dig();
		}
	}


	int update(RecievedData rec){

		static RecievedData prec;
		int flg;

//		if(prec.automode){
//			flg = AutoControl(prec);
//			if(flg==AUTO_MODE_FINISHED){
//				rec.automode = prec.automode = false;
//				return AUTO_MODE_FINISHED;
//			}
//		}else{
			ManualControl(rec, prec);
//		}

		prec = rec;

		return 0;
	}

};

#endif /* OMNI_HPP_ */


