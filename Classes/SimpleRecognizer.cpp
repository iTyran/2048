//
//  SimpleRecognizer.cpp
//  2048
//
//  Created by ZeroYang on 5/7/14.
//
//

#include "SimpleRecognizer.h"

USING_NS_CC;

#define MAX_DOUBLE std::numeric_limits<double>::max();

// class define
SimpleRecognizer::SimpleRecognizer()
{
	this->result = SimpleGesturesError;
}

// be called in onTouchBegan
void SimpleRecognizer::beginPoint(double x, double y)
{
	this->result = SimpleGesturesError;
    points.clear();
	points.push_back(Point(x,y));
}

void SimpleRecognizer::movePoint(double x, double y)
{
	points.push_back(Point(x, y));
    
	if (result == SimpleGesturesNotSupport) {
		return;
	}
    
	SimpleGestures newRtn = SimpleGesturesError;
	int len = this->points.size();
	//每当触点移动时,在当前触点和之前触点之间计算不同的x坐标和y坐标
	double dx = this->points[len - 1].x - this->points[len - 2].x;
	double dy = this->points[len - 1].y - this->points[len - 2].y;
    
	if (abs(dx) > abs(dy)) {
		//在这种情况下,运动趋势的触点在x轴方向
		if (dx > 0) {
			newRtn = SimpleGesturesRight;
		} else {
			newRtn = SimpleGesturesLeft;
		}
	} else {
		//在这种情况下,运动趋势的触点在y轴方向
		if (dy > 0) {
			newRtn = SimpleGesturesUp;
		} else {
			newRtn = SimpleGesturesDown;
		}
	}
    
	// first set result
	if (result == SimpleGesturesError) {
		result = newRtn;
		return;
	}
    
	// if diretcory change, not support Recongnizer
	if (result != newRtn) {
		result = SimpleGesturesNotSupport;
	}
}

SimpleGestures SimpleRecognizer::endPoint()
{
	if (this->points.size() < 1) {
		return SimpleGesturesError;
	}
	return result;
}

std::vector<Point>& SimpleRecognizer::getPoints()
{
	return points;
}