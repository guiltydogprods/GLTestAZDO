/*
 *  Point.cpp
 *  ionEngine
 *
 *  Created by Claire Rogers on 22/10/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include "stdafx.h"
#include "Point.h"
#include "Vector4.h"
#include "Vector.h"

	Point::Point()
	{
	}
	
	Point::Point(float x, float y, float z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
		m_w = 1.0f;
	}
	
	Point::Point(Vector4 val)
	{
		m_x = val.X();
		m_y = val.Y();
		m_z = val.Z();
		m_w = 1.0f;
	}
	
	Vector Point::operator-(Point right) const
	{
		Vector res;
		
		float x = m_x - right.m_x;
		float y = m_y - right.m_y;
		float z = m_z - right.m_z;
		
		return Vector(x, y, z);
	}

	Point Point::operator+(Vector right) const
	{
		Vector res;
		
		float x = m_x + right.X();
		float y = m_y + right.Y();
		float z = m_z + right.Z();
		
		return Point(x, y, z);
	}

	Point Point::operator-(Vector right) const
	{
		Point res;

		float x = m_x - right.X();
		float y = m_y - right.Y();
		float z = m_z - right.Z();

		return Point(x, y, z);
	}

	Point Point::operator*(float right) const
	{
		Point res;

		float x = m_x * right;
		float y = m_y * right;
		float z = m_z * right;

		return Point(x, y, z);
	}

	void Point::operator+=(Vector right)
	{
		(*this) = (*this) + right;
	}

	void Point::operator-=(Vector right)
	{
		(*this) = (*this) - right;
	}

	void Point::SetX(float val)
	{
		m_x = val;
	}
	
	void Point::SetY(float val)
	{
		m_y = val;
	}
	
	void Point::SetZ(float val)
	{
		m_z = val;
	}
	
	
	float Point::X() const
	{
		return m_x;
	}
	
	float Point::Y() const
	{
		return m_y;
	}
	
	float Point::Z() const
	{
		return m_z;
	}
	
	Vector4 Point::GetVector4() const
	{
		return Vector4(m_x, m_y, m_z, m_w);
	}

	float Distance(Point& v1, Point& v2)
	{
		float length = Length(v1 - v2);
		return length;
	}

	float SquaredDistance(Point& v1, Point& v2)
	{
		Vector delta = v1 - v2;
		return Dot(delta, delta);
	}

	Point Min(Point& v1, Point& v2)
	{
		float x = v1.X() < v2.X() ? v1.X() : v2.X();
		float y = v1.Y() < v2.Y() ? v1.Y() : v2.Y();
		float z = v1.Z() < v2.Z() ? v1.Z() : v2.Z();
		
		return Point(x, y, z);
	}
	
	Point Max(Point& v1, Point& v2)
	{
		float x = v1.X() > v2.X() ? v1.X() : v2.X();
		float y = v1.Y() > v2.Y() ? v1.Y() : v2.Y();
		float z = v1.Z() > v2.Z() ? v1.Z() : v2.Z();
		
		return Point(x, y, z);
	}
	
	void Min(Point& res, Point& v1, Point& v2)
	{
		float x = v1.X() < v2.X() ? v1.X() : v2.X();
		float y = v1.Y() < v2.Y() ? v1.Y() : v2.Y();
		float z = v1.Z() < v2.Z() ? v1.Z() : v2.Z();
		
		res = Point(x, y, z);
	}
	
	void Max(Point& res, Point& v1, Point& v2)
	{
		float x = v1.X() > v2.X() ? v1.X() : v2.X();
		float y = v1.Y() > v2.Y() ? v1.Y() : v2.Y();
		float z = v1.Z() > v2.Z() ? v1.Z() : v2.Z();

		res = Point(x, y, z);
	}

