/*
 *  Plane.c
 *  AirHockey
 *
 *  Created by Claire Rogers on 31/01/2009.
 *  Copyright 2009 Spinning Head Software. All rights reserved.
 *
 */

#include "Plane.h"

namespace ion
{
	void Plane::Init(Vector& p0, Vector& p1, Vector& p2)
	{
		//	PLANE( const VECTOR& p0, const VECTOR& p1,
		//		  const VECTOR& p2 ):	N((p1-p0).cross(p2-p0).unit()),
		//	D(-N.dot(p0))
		//	{}
		Vector p1p0 = p1 - p0;
		Vector p2p0 = p2 - p0;
		Vector vec = Cross(p1p0, p2p0);
		m_normal = Normalize(vec);
		m_d = -Dot(m_normal, p0);
	}
	
	float Plane::DistanceToPoint(Vector& point)
	{
		return Dot(m_normal, point) + m_d;
	}
/*
	float RayIntersectsPlane(Vector& xPos, Plane *plane, Ray *ray)
	{
		float p1DotN = Dot(ray->p1, plane->m_normal);
		float numer = plane->m_d - p1DotN;
		Vector vec = ray->p2 - ray->p1;
		float denom = Dot(vec, plane->m_normal);
		float t = numer / denom;
		//	Vector xPos;
		VectorInterpolate(xPos, ray->p1, ray->p2, t);
		
		return t;
	}
*/
}
