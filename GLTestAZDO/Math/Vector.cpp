/*
 *  Vector.cpp
 *  ionEngine
 *
 *  Created by Claire Rogers on 22/10/08.
 *  Copyright (c) 2013 Guilty Dog Productions Ltd. All rights reserved.
 *
 */
#include "stdafx.h"
#include "Math.h"
#include "Vector.h"
#include "Vector4.h"
#include "System/System.h"

	Vector::Vector()
	{
		m_x = m_y = m_z = 0.0f;
		m_w = 0.0f;
	}
	
	Vector::Vector( float x, float y, float z )
	{
		m_x = x;
		m_y = y;
		m_z = z;
		m_w = 0.0f;
	}
	
	Vector::Vector(Vector4 val)
	{
		m_x = val.X();
		m_y = val.Y();
		m_z = val.Z();
		m_w = 0.0f;
	}
	
	Vector Vector::operator+(Vector right) const
	{
		Vector res;
		
		res.m_x = m_x + right.m_x;
		res.m_y = m_y + right.m_y;
		res.m_z = m_z + right.m_z;
		
		return res;
	}
	
	void Vector::operator+=(Vector right)
	{
		(*this) = (*this) + right;
	}
	
	Vector Vector::operator-(Vector right) const
	{
		Vector res;
		
		res.m_x = m_x - right.m_x;
		res.m_y = m_y - right.m_y;
		res.m_z = m_z - right.m_z;
		
		return res;
	}
	
	void Vector::operator-=(Vector right)
	{
		(*this) = (*this) - right;
	}
	
	Vector Vector::operator*(float right) const
	{
		Vector res;
		
		res.m_x = m_x * right;
		res.m_y = m_y * right;
		res.m_z = m_z * right;
		
		return res;
	}
	
	void Vector::operator*=(float right)
	{
		(*this) = (*this) * right;
	}
	
	Vector operator*(float left, Vector right)
	{
		return right * left;
	}
	
	bool Vector::operator==(Vector right) const 
	{
		if (m_x == right.X() && m_y == right.Y() && m_z == right.Z()) {
			return true;
		} else {
			return false;
		}
	}
	
	void Vector::SetX(float val)
	{
		m_x = val;
	}

	void Vector::SetY(float val)
	{
		m_y = val;
	}
	
	void Vector::SetZ(float val)
	{
		m_z = val;
	}
	
	float Vector::X() const
	{
		return m_x;
	}
	
	float Vector::Y() const
	{
		return m_y;
	}
	
	float Vector::Z() const
	{
		return m_z;
	}
	
	Vector4 Vector::GetVector4() const
	{
		return Vector4(m_x, m_y, m_z, m_w);
	}
	
	float Length(Vector v)
	{
		return sqrtf(Dot(v, v));
	}

	float SquaredDistance(Vector v1, Vector v2)
	{
		return ((v1.X() - v2.X()) * (v1.Z() - v2.Z())) + 
			   ((v1.Y() - v2.Y()) * (v1.Y() - v2.Y())) + 	
			   ((v1.Z() - v2.Z()) * (v1.Z() - v2.Z()));
	}

	Vector Normalize(Vector v)
	{
		Vector res;
		
		float len = Length(v);
//		if (len != 0.0)
		AssertMsg(len != 0.0f, "Vector is zero length\n");
		{
			len = 1.0f / len;
			float x = v.X() * len;
			float y = v.Y() * len;
			float z = v.Z() * len;
			res = Vector(x, y, z);
		}
		
		return res;
	}
	
	Vector Cross(Vector v1, Vector v2)
	{
		float x = (v1.Y() * v2.Z()) - (v1.Z() * v2.Y());
		float y = (v1.Z() * v2.X()) - (v1.X() * v2.Z());
		float z = (v1.X() * v2.Y()) - (v1.Y() * v2.X());
		return Vector(x, y, z);
	}
	
	float Dot(Vector v1, Vector v2)
	{
		return(v1.X() * v2.X() + v1.Y() * v2.Y() + v1.Z() * v2.Z());
	}
	
	Vector Min(Vector v1, Vector v2)
	{
		Vector res;
		float x, y, z;	//, w;
		
		x = v1.X() < v2.X() ? v1.X() : v2.X();
		y = v1.Y() < v2.Y() ? v1.Y() : v2.Y();
		z = v1.Z() < v2.Z() ? v1.Z() : v2.Z();
		res = Vector(x, y, z);
		
		return res;
	}
	
	Vector Max(Vector v1, Vector v2)
	{
		float x, y, z;	//, w;
		
		x = v1.X() > v2.X() ? v1.X() : v2.X();
		y = v1.Y() > v2.Y() ? v1.Y() : v2.Y();
		z = v1.Z() > v2.Z() ? v1.Z() : v2.Z();
		
		Vector res(x, y, z);
		return res;
	}
	
	Vector Interpolate(Vector v1, Vector v2, float t)
	{
		float x = v1.X() + (v2.X() - v1.X()) * t;
		float y = v1.Y() + (v2.Y() - v1.Y()) * t;
		float z = v1.Z() + (v2.Z() - v1.Z()) * t;
		
		return Vector(x, y, z);
	}
	
