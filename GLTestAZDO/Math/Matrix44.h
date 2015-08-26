//
//	Matrix44.h
//	ion
//
//	Created by Claire Rogers on 14/10/2012.
//	Copyright (c) 2013 Guilty Dog Productions Ltd. All rights reserved.
//

#pragma once

#include "Vector4.h"
#include "Vector.h"
#include "Point.h"

	class Quat;

	enum	IdentityTag	{kIdentity};
	
	class Matrix44
	{
	public:
		Vector4	m_xAxis;
		Vector4	m_yAxis;
		Vector4	m_zAxis;
		Vector4	m_wAxis;
		Matrix44();
		Matrix44(Vector4 v1, Vector4 v2, Vector4 v3, Vector4 v4);
		Matrix44(IdentityTag);
		Matrix44(Point& position, Quat& rotation);

		void SetAxisX(Vector4 axis);
		void SetAxisY(Vector4 axis);
		void SetAxisZ(Vector4 axis);
		void SetAxisW(Vector4 axis);
		
		inline Vector4 GetAxisX()					{	return m_xAxis;	}
		inline Vector4 GetAxisY()					{	return m_yAxis;	}
		inline Vector4 GetAxisZ()					{	return m_zAxis;	}
		inline Vector4 GetAxisW()					{	return m_wAxis;	}
		
		void SetIdentity();
		void SetRotation(float angle, Vector axis);
		void SetTranslate(Point translation);
		void SetTranslate(Vector translation);
		void SetScale(float scale);
		void SetFromPositionAndRotation(Point& position, Matrix44& rotation);
		void SetFromPositionAndRotation(Point& position, Quat& rotation);
		
		Vector4 operator*(Vector4 right) const;
		Point operator*(Point right) const;
		//	Vector4 operator *(Vector4 right) const;
		Matrix44 operator*(float right);
		Matrix44 operator+(Matrix44& right);
		Matrix44 operator*(const Matrix44& right) const;

		static void Ortho(Matrix44& result, float left, float right, float bottom, float top, float zNear, float zFar);
		static void Frustum(Matrix44& result, float left, float right, float bottom, float top, float nearZ, float farZ);
	};
	
	// non-member functions, which need to be friends	
	void SetIdentity(Matrix44& mat);
	Matrix44 Transpose(Matrix44& mat);
	Matrix44 OrthoInverse(const Matrix44& mat);
	Matrix44 Inverse(Matrix44& mat);
	void PrintMatrix(const char* text, Matrix44& mat);
