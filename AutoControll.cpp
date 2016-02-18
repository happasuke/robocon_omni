/*
 * AutoControll.cpp
 *
 *  Created on: 2015/10/21
 *      Author: 葉介
 */

#include "AutoControll.hpp"

Point detectFixedPosition(Point p1, Point p2){
	Point center={p1.x+iabs(p1.x-p2.x)/2, p1.y+iabs(p1.y-p2.y)/2};
	float katamuki = (float)abs(p1.y-p2.y)/(float)abs(p1.x-p2.x);
	float gapy = (katamuki*(float)gap) * (p1.y<p2.y)?1:-1;
	float gapx = (1.0-katamuki) * (float)gap;
	Point goal={center.x+(int)gapx, center.y-(int)gapy};

	return goal;
}

Point getCenter(Point p1, Point p2){
	Point center={p1.x+iabs(p1.x-p2.x)/2, p1.y-iabs(p1.y-p2.y)/2};
	return center;
}

float calcPower(Point p){
	float sqdistance = p.x*p.x+p.y*p.y;
	if(sqdistance<4){//2cm
		return 0;
	}else if(sqdistance<3600){//60cm
		return sqdistance/3600.0;
	}else{
		return 1;
	}
}


