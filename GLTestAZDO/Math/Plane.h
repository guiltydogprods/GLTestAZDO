/*
 *  Plane.h
 *  AirHockey
 *
 *  Created by Claire Rogers on 31/01/2009.
 *  Copyright 2009 Spinning Head Software. All rights reserved.
 *
 */

#pragma once

#include "Vector.h"

namespace ion
{
	class Plane
	{
	public:
		void Init(Vector& p0, Vector& p1, Vector& p2);
		float DistanceToPoint(Vector& point);
		
		//private:
		Vector m_normal;
		float  m_d;
	} __attribute__((aligned (16)));
	
	//void PlaneInit(Plane *plane, Vector p0, Vector p1, Vector p2);
	//float PlaneDistanceToPoint(Plane *plane, Vector point);
	
	struct Ray
	{
		Vector p1;
		Vector p2;					//Maybe best to store in parametric form.
	} __attribute__((aligned (16)));
	
	float RayIntersectsPlane(Vector& xPos, Plane *plane, Ray *ray);	
}


