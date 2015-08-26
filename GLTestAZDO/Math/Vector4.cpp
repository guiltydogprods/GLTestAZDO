/*
 *  Vector.cpp
 *  ionEngine
 *
 *  Created by Claire Rogers on 22/10/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include "Math.h"
#include "Vector4.h"
#include "Vector.h"
#include "Math.h"

#include <stdio.h>

	Vector4::Vector4()
	{
		m_x = m_y = m_z = m_w = 0.0f;
	}
	
	Vector4::Vector4( float x, float y, float z, float w)
	{
		m_x = x;
		m_y = y;
		m_z = z;
		m_w = w;
	}
	
	Vector4 Vector4::operator+(Vector4 right) const
	{
		Vector4 res;
		
		res.m_x = m_x + right.m_x;
		res.m_y = m_y + right.m_y;
		res.m_z = m_z + right.m_z;
		res.m_w = m_w + right.m_w;
		
		return res;
	}
	
	void Vector4::operator+=(Vector4 right)
	{
		(*this) = (*this) + right;
	}
	
	Vector4 Vector4::operator-(Vector4 right) const
	{
		Vector4 res;
		
		res.m_x = m_x - right.m_x;
		res.m_y = m_y - right.m_y;
		res.m_z = m_z - right.m_z;
		res.m_w = m_w - right.m_w;
		return res;
	}
	
	void Vector4::operator-=(Vector4 right)
	{
		(*this) = (*this) - right;
	}
	
	Vector4 Vector4::operator*(float right) const
	{
		Vector4 res;
		
		res.m_x = m_x * right;
		res.m_y = m_y * right;
		res.m_z = m_z * right;
		res.m_w = m_w * right;
		
		return res;
	}
	
	void Vector4::operator*=(float right)
	{
		(*this) = (*this) * right;
	}
	
	Vector4 operator*(float left, Vector4 right)
	{
		return right * left;
	}
	
	bool Vector4::operator==(Vector4 right)
	{
//		return ((fabsf(m_x - right.m_x) < 0.00001f) &
//			(fabsf(m_y - right.m_y) < 0.00001f) &
//			(fabsf(m_z - right.m_z) < 0.00001f) &
//			(fabsf(m_w - right.m_w) < 0.00001f));
		return ((m_x == right.m_x) &
				(m_y == right.m_y) &
				(m_z == right.m_z) &
				(m_w == right.m_w));
	}

	float Vector4::X() const
	{
		return m_x;
	}
	
	float Vector4::Y() const
	{
		return m_y;
	}
	
	float Vector4::Z() const
	{
		return m_z;
	}
	
	float Vector4::W() const
	{
		return m_w;
	}
	
	float Length(Vector4 v)
	{
		return sqrtf( Dot( v, v ) );
	}
	
	Vector4 Normalize(Vector4 v)
	{
		Vector4 res;
		
		float len = Length(v);
		if (len != 0.0)
		{
			len = 1.0f / len;
			float x = v.X() * len;
			float y = v.Y() * len;
			float z = v.Z() * len;
			float w = v.W() * len;
			res = Vector4(x, y, z, w);
		}
		
		return res;
	}
	
	Vector Cross3(Vector4 v1, Vector4 v2)
	{
		float x = (v1.Y() * v2.Z()) - (v1.Z() * v2.Y());
		float y = (v1.Z() * v2.X()) - (v1.X() * v2.Z());
		float z = (v1.X() * v2.Y()) - (v1.Y() * v2.X());
		return Vector(x, y, z);
	}
	
	float Dot(Vector4 v1, Vector4 v2)
	{
		return(v1.X() * v2.X() + v1.Y() * v2.Y() + v1.Z() * v2.Z() + v1.W() * v2.W());
	}
	
	Vector4 Min(Vector4 v1, Vector4 v2)
	{
		Vector4 res;
		float x, y, z, w;
		
		x = v1.X() < v2.X() ? v1.X() : v2.X();
		y = v1.Y() < v2.Y() ? v1.Y() : v2.Y();
		z = v1.Z() < v2.Z() ? v1.Z() : v2.Z();
		w = v1.W() < v2.W() ? v1.W() : v2.W();
		res = Vector4(x, y, z, w);
		
		return res;
	}
	
	Vector4 Max(Vector4 v1, Vector4 v2)
	{
		float x, y, z, w;
		
		x = v1.X() > v2.X() ? v1.X() : v2.X();
		y = v1.Y() > v2.Y() ? v1.Y() : v2.Y();
		z = v1.Z() > v2.Z() ? v1.Z() : v2.Z();
		w = v1.W() > v2.W() ? v1.W() : v2.W();
		
		Vector4 res(x, y, z, w);
		return res;
	}
	
	Vector4 Homogenize(Vector4 v)
	{
		float x, y, z, w;
		x = v.X();
		y = v.Y();
		z = v.Z();
		w = v.W();
		
		Vector4 res(x/w, y/w, z/w, w/w);
		return res;
	}
	void	PrintVector(const char* text, Vector4 vec)
	{
		printf("%.3f, %.3f, %.3f, %.3f\n", vec.m_x, vec.m_y, vec.m_z, vec.m_w);
	}	

