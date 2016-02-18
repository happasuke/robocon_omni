/*
 * AutoControll.hpp
 *
 *  Created on: 2015/10/21
 *      Author: 葉介
 */

#ifndef AUTOCONTROLL_HPP_
#define AUTOCONTROLL_HPP_

#include "util.h"

const int gap = 150;

Point detectFixedPosition(Point p1, Point p2);

Point getCenter(Point p1, Point p2);

float calcPower(Point p);

#endif /* AUTOCONTROLL_HPP_ */
