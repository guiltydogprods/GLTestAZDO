/*
 *  Point.h
 *  ionEngine
 *
 *  Created by Claire Rogers on 22/10/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

	class Vector;
	class Vector4;
	
	class Point
	{
	public:
		Point();
		Point(float x, float y, float z);
		Point(Vector4 val);
		
		Vector operator - (Point right) const;
		Point operator - (Vector right) const;
		Point operator + (Vector right) const;
		Point operator * (float right) const;
		void operator += (Vector right);
		void operator -= (Vector right);

		
		void  SetX( const float val );
		void  SetY( const float val );
		void  SetZ( const float val );

		float X() const;
		float Y() const;
		float Z() const;
		
		Vector4	GetVector4() const;
		
		// non-member functions, which need to be friends
		friend  float	Distance(Point& v1, Point& v2);
		friend  float	SquaredDistance(Point& v1, Point& v2);
		friend	Point	Min(Point& v1, Point& v2);
		friend	Point	Max(Point& v1, Point& v2);
		friend	void	Min(Point& res, Point& v1, Point& v2);
		friend	void	Max(Point& res, Point& v1, Point& v2);
		
	private:
		float m_x, m_y, m_z, m_w;
	};	
