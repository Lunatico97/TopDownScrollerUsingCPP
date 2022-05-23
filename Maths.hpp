
#pragma once

#include <iostream>
#include <cmath>

struct Vector2f
{
	float x, y ;
	
	Vector2f() : x(0.0f), y(0.0f) {}
	Vector2f(float x1, float y1) : x(x1), y(y1) {}
	
	float getDistance(Vector2f v){
		return sqrt(pow(v.x-x,2) + pow(v.y-y,2)) ;
	}
	
	void logs(){std::cout << x << ", " << y << std::endl ; }
} ;

