/*
 *  Vector4.h
 *  ionEngine
 *
 *  Created by Claire Rogers on 22/10/08.
 *  Copyright (c) 2013 Guilty Dog Productions Ltd. All rights reserved.
 *
 */
#pragma once

	class Vector;
	
	class Vector4
	{
	public:
		// standard constructors
		Vector4();
		Vector4( float x, float y, float z, float w );
		
		Vector4 operator + (Vector4 right) const;
		Vector4 operator - (Vector4 right) const;
		Vector4 operator * (Vector4 right) const;
		Vector4 operator + (float right) const;
		Vector4 operator * (float right) const;
		Vector4 operator / (float right) const;
		
		void operator += (Vector4 right);
		void operator -= (Vector4 right);
		void operator *= (float right);
		bool operator == (Vector4 right);
		
		void  SetX( const float val );
		void  SetY( const float val );
		void  SetZ( const float val );
		void  SetW( const float val );
		
		float X() const;
		float Y() const;
		float Z() const;
		float W() const;
		
		Vector4	GetVector4() const;
		
		// non-member functions, which need to be friends
		friend	float	Length(Vector4 v);
		friend	Vector4	Normalize(Vector4 v);
		friend	Vector	Cross3(Vector4 v1, Vector4 v2);
		friend	float	Dot(Vector4 v1, Vector4 v2);
		friend	Vector4	Min(Vector4 v1, Vector4 v2);
		friend	Vector4	Max(Vector4 v1, Vector4 v2);
		friend	Vector4	Homogenize(Vector4 v);
		friend	void	PrintVector(const char* text, Vector4 vec);
		
	private:
		float m_x, m_y, m_z, m_w;
	};

