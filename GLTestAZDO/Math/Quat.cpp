/*
 *  Quat.cpp
 *  ion::Engine
 *
 *  Created by Claire Rogers on 27/02/2006.
 *  Copyright (c) 2013 Guilty Dog Productions Ltd. All rights reserved.
 *
 */
#include "stdafx.h"
#include "Quat.h"
#include "Matrix33.h"
#include "Matrix44.h"
#include "Vector.h"
#include "FMath.h"

	Quat::Quat()
	{
		// Do nothing, we want speed.
	}
	
	Quat::Quat( float w, float x, float y, float z )
	{
		//	SetRotation( angle, Vec4( x, y, z, 0.0f ) );
		//	SetRotation( float w, float x, float y, float z )
		m_w = w;
		m_x = x;
		m_y = y;
		m_z = z;
	}
	
	Quat::Quat(float angle, Vector axis)
	{
		SetRotation( angle, axis );
	}
	
	void Quat::SetIdentity()
	{
		m_w = 1.0f;
		m_x = m_y = m_z = 0.0f;
	}
	
	void Quat::SetRotation( float angle, Vector axis )
	{
		float halfTheta = angle * 0.5f;
		
		float c = cosf(halfTheta);
		float s = sinf(halfTheta);
		
		m_x = axis.X() * s;
		m_y = axis.Y() * s;
		m_z = axis.Z() * s;
		m_w = c;
	}
	/*
	 Mat33 Quat::GetMat33()
	 {
	 Mat33 mat;
	 float x = m_x;
	 float y = m_y;
	 float z = m_z;
	 float w = m_w;
	 float xx = x * x * 2.0f;
	 float yy = y * y * 2.0f;
	 float zz = z * z * 2.0f;
	 float xy = x * y * 2.0f;
	 float xz = x * z * 2.0f;
	 float yz = y * z * 2.0f;
	 float wx = w * x * 2.0f;
	 float wy = w * y * 2.0f;
	 float wz = w * z * 2.0f;
	 
	 float m00 = 1.0f - yy - zz;
	 float m01 = xy + wz;
	 float m02 = xz - wy;
	 float m10 = xy - wz;
	 float m11 = 1.0f - xx - zz;
	 float m12 = yz + wx;
	 float m20 = xz + wy;
	 float m21 = yz - wx;
	 float m22 = 1.0f - xx - yy;
	 
	 mat.m_xAxis = Vector4( m00, m01, m02, 0.0f );
	 mat.m_yAxis = Vector4( m10, m11, m12, 0.0f );
	 mat.m_zAxis = Vector4( m20, m21, m22, 0.0f );
	 
	 return mat;
	 }
	 */
	
	Matrix44 Quat::GetMatrix44()
	{
		Matrix44 mat;
		float x = m_x;
		float y = m_y;
		float z = m_z;
		float w = m_w;
		float xx = x * x * 2.0f;
		float yy = y * y * 2.0f;
		float zz = z * z * 2.0f;
		float xy = x * y * 2.0f;
		float xz = x * z * 2.0f;
		float yz = y * z * 2.0f;
		float wx = w * x * 2.0f;
		float wy = w * y * 2.0f;
		float wz = w * z * 2.0f;
		
		float m00 = 1.0f - yy - zz;
		float m01 = xy + wz;
		float m02 = xz - wy;
		float m10 = xy - wz;
		float m11 = 1.0f - xx - zz;
		float m12 = yz + wx;
		float m20 = xz + wy;
		float m21 = yz - wx;
		float m22 = 1.0f - xx - yy;

		mat.m_xAxis = Vector4( m00, m01, m02, 0.0f );
		mat.m_yAxis = Vector4( m10, m11, m12, 0.0f );
		mat.m_zAxis = Vector4( m20, m21, m22, 0.0f );
//		mat.m_xAxis = Vector4( m00, m10, m20, 0.0f );
//		mat.m_yAxis = Vector4( m01, m11, m21, 0.0f );
//		mat.m_zAxis = Vector4( m02, m12, m22, 0.0f );
		mat.m_wAxis = Vector4( 0.0f, 0.0f, 0.0f, 1.0f );
		
		return mat;
	}
	
	const float epsilon = 0.00001f;
	const float epsilonSquared = epsilon * epsilon;
	
	void  Quat::GetAngleAxis( float& angle, Vector& axis )
	{
		const float squareLength = m_x * m_x + m_y * m_y + m_z * m_z;
		
		if( squareLength > epsilonSquared )
		{
			angle = 2.0f * (float)acosf(m_w);
			const float inverseLength = 1.0f / (float)pow(squareLength, 0.5f);
			axis = Vector( m_x, m_y, m_z ) * inverseLength;
		}
		else
		{
			angle = 0.0f;
			axis = Vector(1.0f, 0.0f, 0.0f);
		}
	}
	
	float Quat::Length()
	{
		return sqrtf(m_w*m_w + m_x*m_x + m_y*m_y + m_z*m_z);
	}
	
	float Quat::Norm()
	{
		return m_w*m_w + m_x*m_x + m_y*m_y + m_z*m_z;
	}

	void Quat::Normalise()
	{
		float length = this->Length();
		
		if(length == 0)
		{
			m_w = 1;
			m_x = 0;
			m_y = 0;
			m_z = 0;
		}
		else
		{
			float inv = 1.0f / length;
			m_x = m_x * inv;
			m_y = m_y * inv;
			m_z = m_z * inv;
			m_w = m_w * inv;
		}
		
	}
	
	Quat Quat::Conjugate()
	{
		Quat res;
		res.m_w = m_w;
		res.m_x = -m_x;
		res.m_y = -m_y;
		res.m_z = -m_z;

		return res;
	}

	Quat Quat::Conjugate(Quat right)
	{
		Quat res;
		res.m_w = right.m_w;
		res.m_x = -right.m_x;
		res.m_y = -right.m_y;
		res.m_z = -right.m_z;
		
		return res;
	}

	Quat Quat::Inverse()
	{
		Quat conj = Conjugate();
		float norm = Norm();

		return conj / (norm * norm);
	}

	///////////////////////////////////////////////////////////////////////////////
	// Function:	QuatToEuler
	// Purpose:		Convert a Quaternion back to Euler Angles
	// Arguments:	Quaternions and target Euler vector
	// Notes:		The method is to convert Quaternion to a 3x3 matrix and
	//				decompose the matrix.  This is subject to the
	//				ambiguities of square roots and problems with inverse trig.
	//				Matrix to Euler conversion is really very ill-defined but works
	//				for my purposes.
	///////////////////////////////////////////////////////////////////////////////
	void Quat::ToEuler(Vector& euler)
	{
		/// Local Variables ///////////////////////////////////////////////////////////
		float matrix[3][3];
		float cx,sx;
		float cy,sy,yr;
		float cz,sz;
		///////////////////////////////////////////////////////////////////////////////
		// CONVERT QUATERNION TO MATRIX - I DON'T REALLY NEED ALL OF IT
		matrix[0][0] = 1.0f - (2.0f * m_y * m_y) - (2.0f * m_z * m_z);
		//	matrix[0][1] = (2.0f * quat->x * quat->y) - (2.0f * quat->w * quat->z);
		//	matrix[0][2] = (2.0f * quat->x * quat->z) + (2.0f * quat->w * quat->y);
		matrix[1][0] = (2.0f * m_x * m_y) + (2.0f * m_w * m_z);
		//	matrix[1][1] = 1.0f - (2.0f * quat->x * quat->x) - (2.0f * quat->z * quat->z);
		//	matrix[1][2] = (2.0f * quat->y * quat->z) - (2.0f * quat->w * quat->x);
		matrix[2][0] = (2.0f * m_x * m_z) - (2.0f * m_w * m_y);
		matrix[2][1] = (2.0f * m_y * m_z) + (2.0f * m_w * m_x);
		matrix[2][2] = 1.0f - (2.0f * m_x * m_x) - (2.0f * m_y * m_y);
		
		sy = -matrix[2][0];
		cy = sqrtf(1.0f - (sy * sy));
		yr = (float)atan2f(sy,cy);
		euler.SetY(yr);
		
		// AVOID DIVIDE BY ZERO ERROR ONLY WHERE Y= +-90 or +-270
		// NOT CHECKING cy BECAUSE OF PRECISION ERRORS
		if (sy != 1.0f && sy != -1.0f)
		{
			cx = matrix[2][2] / cy;
			sx = matrix[2][1] / cy;
			euler.SetX(atan2f(sx,cx));
			
			cz = matrix[0][0] / cy;
			sz = matrix[1][0] / cy;
			euler.SetZ(atan2f(sz,cz));
		}
		else
		{
			// SINCE Cos(Y) IS 0, I AM SCREWED.  ADOPT THE STANDARD Z = 0
			// I THINK THERE IS A WAY TO FIX THIS BUT I AM NOT SURE.  EULERS SUCK
			// NEED SOME MORE OF THE MATRIX TERMS NOW
			matrix[1][1] = 1.0f - (2.0f * m_x * m_x) - (2.0f * m_z * m_z);
			matrix[1][2] = (2.0f * m_y * m_z) - (2.0f * m_w * m_x);
			cx = matrix[1][1];
			sx = -matrix[1][2];
			euler.SetX(atan2f(sx,cx));
			
			cz = 1.0f;
			sz = 0.0f;
			euler.SetZ(atan2f(sz,cz));
		}
	}
	// QuatToEuler  ///////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////////
	// Function:	EulerToQuaternion
	// Purpose:		Convert a set of Euler angles to a Quaternion
	// Arguments:	A rotation set of 3 angles, a quaternion to set
	// Discussion:  As the order of rotations is important I am
	//				using the Quantum Mechanics convention of (X,Y,Z)
	//				a Yaw-Pitch-Roll (Y,X,Z) system would have to be
	//				adjusted.  It is more efficient this way though.
	///////////////////////////////////////////////////////////////////////////////
	void Quat::FromEuler(Vector& euler)
	{
		/// Local Variables ///////////////////////////////////////////////////////////
		float rx, ry, rz, tx, ty, tz, cx, cy, cz, sx, sy, sz, cc, cs, sc, ss;
		///////////////////////////////////////////////////////////////////////////////
		rx =  euler.X();
		ry =  euler.Y();
		rz =  euler.Z();

		tx = rx * (float)0.5f;
		ty = ry * (float)0.5f;
		tz = rz * (float)0.5f;

		cx = (float)cosf(tx);
		cy = (float)cosf(ty);
		cz = (float)cosf(tz);
		sx = (float)sinf(tx);
		sy = (float)sinf(ty);
		sz = (float)sinf(tz);
		
		cc = cx * cz;
		cs = cx * sz;
		sc = sx * cz;
		ss = sx * sz;
		
		m_x = (cy * sc) - (sy * cs);
		m_y = (cy * ss) + (sy * cc);
		m_z = (cy * cs) - (sy * sc);
		m_w = (cy * cc) + (sy * ss);
		
		// INSURE THE QUATERNION IS NORMALIZED
		// PROBABLY NOT NECESSARY IN MOST CASES
//		Normalise();
	}
	
	Quat Quat::operator+(Quat right)
	{
		Quat res;
		
		res.m_x = m_x + right.m_x;
		res.m_y = m_y + right.m_y;
		res.m_z = m_z + right.m_z;
		res.m_w = m_w + right.m_w;
		
		return res;
	}
	
	void Quat::operator+=(Quat right)
	{
		*this = *this + right;
	}
	
	
	Quat Quat::operator*(Quat right)
	{
		Quat res;
		res.m_x =	m_x * right.m_w + m_w * right.m_x + m_z * right.m_y - m_y * right.m_z;
		res.m_y =	m_y * right.m_w + m_w * right.m_y + m_x * right.m_z - m_z * right.m_x;
		res.m_z =	m_z * right.m_w + m_w * right.m_z + m_y * right.m_x - m_x * right.m_y;
		res.m_w =	m_w * right.m_w - m_x * right.m_x - m_y * right.m_y - m_z * right.m_z;
		return res;
	}
	
	Quat Quat::operator*(float right)
	{
		Quat res;
		res.m_x = m_x * right;
		res.m_y = m_y * right;
		res.m_z = m_z * right;
		res.m_w = m_w * right;
		
		return res;
	}
	
	Quat operator*(float left, Quat right)
	{
		return right * left;
	}

	Quat Quat::operator/(float right)
	{
		Quat res;

		res.m_x = m_x / right;
		res.m_y = m_y / right;
		res.m_z = m_z / right;
		res.m_w = m_w / right;

		return res;
	}

