#pragma once
#include "Camera.h"

class Object
{
public:
	virtual void Draw(const Camera& camera) = 0;
};