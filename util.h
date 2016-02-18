/*
 * util.h
 *
 *  Created on: 2015/04/04
 *      Author: 葉介
 */

#ifndef UTIL_H_
#define UTIL_H_

#define CAN_MESSAGE_LENGTH 3

typedef struct _Point{
	int x;
	int y;
}Point;

const float PI = 3.141592;
const float PI4 = PI/4;
const float PI3p4 = PI/4*3;

const float COS_PI_4 = 0.7071067;
const float SIN_PI_4 = 0.7071067;

Point rotatePoint(float x,float y);

float powerErrorControl(float source);

//構造体とかクラスを見ていると幸せなってくる
typedef struct _CanData{
	char data1;
	char data2;
	char data3;
}CanData;

typedef struct _RecievedData{

	bool triangle;
	bool circle;
	bool cross;
	bool square;

	bool top;
	bool down;
	bool right;
	bool left;

	bool l1;
	bool r1;

	float rud;//-1<rud<1
	float rlr;
	float lud;
	float llr;

	bool isPower;

	Point urg1;
	Point urg2;

	int urgangle;
	bool urgenable;
	bool parsefinished;

	bool automode;

}RecievedData;

RecievedData parseControllerData(char *rec);

RecievedData correct(RecievedData r);

CanData makeCanMessage(RecievedData r, int id,char arg1,char arg2);

int CheckRecieveError(char *r);

float abs(float a);
int iabs(int n);

#endif /* UTIL_H_ */
