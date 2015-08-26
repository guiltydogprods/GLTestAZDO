/*
 *  Vector.h
 *  ionEngine
 *
 *  Created by Claire Rogers on 22/10/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

	class Vector4;
	
	class Vector
	{
	public:
		// standard constructors
		Vector();
		Vector( float x, float y, float z );
		Vector(Vector4 val);
		
		Vector operator + (Vector right) const;
		Vector operator - (Vector right) const;
		Vector operator * (Vector right) const;
		Vector operator + (float right) const;
		Vector operator * (float right) const;
		Vector operator / (float right) const;
		bool   operator == (Vector right) const;
		
		void operator += (Vector right);
		void operator -= (Vector right);
		void operator *= (float right);
		
		void  SetX( const float val );
		void  SetY( const float val );
		void  SetZ( const float val );
		
		float X() const;
		float Y() const;
		float Z() const;
		
		Vector4	GetVector4() const;
		
		// non-member functions, which need to be friends
		friend	float	Length(Vector v);
		friend  float	SquaredDistance(Vector v1, Vector v2);
		friend	Vector	Normalize(Vector v);
		friend	Vector	Cross(Vector v1, Vector v2);
		friend	float	Dot(Vector v1, Vector v2);
		friend	Vector	Min(Vector v1, Vector v2);
		friend	Vector	Max(Vector v1, Vector v2);
		friend	Vector	Interpolate(Vector v1, Vector v2, float t);
		friend	Vector	Homogenize(Vector v);
		friend	void	PrintVector(const char* text, Vector vec);
		
	private:
		float m_x, m_y, m_z, m_w;
	};

