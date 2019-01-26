/*
 *  Matrix44.cpp
 *  ion::Engine
 *
 *  Created by Claire Rogers on 10/09/2005.
 *  Copyright (c) 2013 Guilty Dog Productions Ltd. All rights reserved.
 *
 */
 
#include "stdafx.h"
#include <stdio.h>
//#include <memory.h>
#include "Matrix44.h"
#include "Vector4.h"
#include "Quat.h"
#include "Math.h"

	Matrix44::Matrix44()
	{
	}
	
	Matrix44::Matrix44(Vector4 v1, Vector4 v2, Vector4 v3, Vector4 v4)
	{
		
	}
	
	Vector4 identityAxisX = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
	Vector4 identityAxisY = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	Vector4 identityAxisZ = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	Vector4 identityTranslation = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	
	Matrix44::Matrix44(IdentityTag)
	{
		m_xAxis = identityAxisX;
		m_yAxis = identityAxisY;
		m_zAxis = identityAxisZ;
		m_wAxis = identityTranslation;
	}

	Matrix44::Matrix44(Point& position, Quat& rotation)
	{
		Matrix44 rotationMatrix = rotation.GetMatrix44();
		m_xAxis = rotationMatrix.GetAxisX();
		m_yAxis = rotationMatrix.GetAxisX();
		m_yAxis = rotationMatrix.GetAxisX();
		m_wAxis = position.GetVector4();
	}
	
	void SetIdentity(Matrix44& mat)
	{
		//	memset(&mat, 0, sizeof(Matrix44));
		//	mat.m[0][0] = mat.m[1][1] = mat.m[2][2] = mat.m[3][3] = 1.0f;
		mat.m_xAxis = identityAxisX;
		mat.m_yAxis = identityAxisY;
		mat.m_zAxis = identityAxisZ;
		mat.m_wAxis = identityTranslation;
	}
	
	void Matrix44::SetAxisX(Vector4 axis)
	{
		//	m[0][0] = axis.m_x;
		//	m[0][1] = axis.m_y;
		//	m[0][2] = axis.m_z;
		//	m[0][3] = 0.0f;
		m_xAxis = axis;
	}
	
	void Matrix44::SetAxisY(Vector4 axis)
	{
		//	m[1][0] = axis.m_x;
		//	m[1][1] = axis.m_y;
		//	m[1][2] = axis.m_z;
		//	m[1][3] = 0.0f;
		m_yAxis = axis;
	}
	
	void Matrix44::SetAxisZ(Vector4 axis)
	{
		//	m[2][0] = axis.m_x;
		//	m[2][1] = axis.m_y;
		//	m[2][2] = axis.m_z;
		//	m[2][3] = 0.0f;
		m_zAxis = axis;
	}
	
	void Matrix44::SetAxisW(Vector4 axis)
	{
		m_wAxis = axis;
	}
	
	void Matrix44::SetIdentity()
	{
		m_xAxis = identityAxisX;
		m_yAxis = identityAxisY;
		m_zAxis = identityAxisZ;
		m_wAxis = identityTranslation;
	}

	void Matrix44::SetRotation(float angle, Vector axis)
	{
		float ax = axis.X();
		float ay = axis.Y();
		float az = axis.Z();
		float xx = ax * ax;
		float yy = ay * ay;
		float zz = az * az;
		
		float sina = sinf(angle);
		float cosa = cosf(angle);
		
		float oneMinusC = 1.0f - cosa;
		
		float m00 = cosa + xx * oneMinusC;
		float m01 = ax * ay * oneMinusC + az * sina;
		float m02 = ax * az * oneMinusC - ay * sina;
		
		float m10 = ax * ay * oneMinusC - az * sina;
		float m11 = cosa + yy * oneMinusC;
		float m12 = ay * az * oneMinusC + ax * sina;
		
		float m20 = ax * az * oneMinusC + ay * sina;
		float m21 = ay * az * oneMinusC - ax * sina;
		float m22 = cosa + zz * oneMinusC;
		
		m_xAxis = Vector4( m00, m01, m02, 0.0f );
		m_yAxis = Vector4( m10, m11, m12, 0.0f );
		m_zAxis = Vector4( m20, m21, m22, 0.0f );
		m_wAxis = Vector4( 0.0f, 0.0f, 0.0f, 1.0f );
	}
	
	//void Matrix44::SetTranslate(Vec4 translation)
	void Matrix44::SetTranslate(Point translation)
	{
		//	m[3][0] = translation.m_x;
		//	m[3][1] = translation.m_y;
		//	m[3][2] = translation.m_z;
		//	m[3][3] = translation.m_w;
		m_wAxis = translation.GetVector4();
	}

	void Matrix44::SetTranslate(Vector translation)
	{
		//	m[3][0] = translation.m_x;
		//	m[3][1] = translation.m_y;
		//	m[3][2] = translation.m_z;
		//	m[3][3] = translation.m_w;
		m_wAxis = Vector4(translation.X(), translation.Y(), translation.Z(), 1.0f);
	}
	
	void Matrix44::SetScale(float scale)
	{
		m_xAxis = Vector4( scale, 0.0f, 0.0f, 0.0f );
		m_yAxis = Vector4( 0.0f, scale, 0.0f, 0.0f );
		m_zAxis = Vector4( 0.0f, 0.0f, scale, 0.0f );
		m_wAxis = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	void Matrix44::SetFromPositionAndRotation(Point& position, Quat& rotation)
	{
		*((Matrix44*)&m_xAxis) = rotation.GetMatrix44();
		m_wAxis = position.GetVector4();
/*
		Matrix44 rot = rotation.GetMatrix44();
		ion::Matrix44 trans(ion::kIdentity);
		trans.SetTranslate(position);
		Matrix44 res2 = trans * rot;
		m_xAxis = res2.GetAxisX();
		m_yAxis = res2.GetAxisY();
		m_zAxis = res2.GetAxisZ();
		m_wAxis = res2.GetAxisW();
*/
	}

	void Matrix44::SetFromPositionAndRotation(Point& position, Matrix44& rotation)
	{
		m_xAxis = rotation.GetAxisX();
		m_xAxis = rotation.GetAxisY();
		m_xAxis = rotation.GetAxisZ();
		m_wAxis = position.GetVector4();
/*
		ion::Matrix44 trans(ion::kIdentity);
		trans.SetTranslate(position);
		Matrix44 res = trans * rotation;
		m_xAxis = res.GetAxisX();
		m_yAxis = res.GetAxisY();
		m_zAxis = res.GetAxisZ();
		m_wAxis = res.GetAxisW();
*/
	}

	Matrix44 Matrix44::operator+(Matrix44& right)
	{
		Matrix44 res;
		res.m_xAxis = m_xAxis + right.m_xAxis;
		res.m_yAxis = m_yAxis + right.m_yAxis;
		res.m_zAxis = m_zAxis + right.m_zAxis;
		res.m_wAxis = m_wAxis + right.m_wAxis;
		return res;
	}
	
	Vector4 Matrix44::operator *(Vector4 right) const
	{
		Vector4 res, acc;
		
		acc = m_wAxis * right.W();
		acc = acc + m_zAxis * right.Z();
		acc = acc + m_yAxis * right.Y();
		res = acc + m_xAxis * right.X();
		
		return res;
	}

	Point Matrix44::operator *(Point right) const
	{
		Point res, acc;
		
		acc = m_wAxis;
		acc = acc + m_zAxis * right.Z();
		acc = acc + m_yAxis * right.Y();
		res = acc + m_xAxis * right.X();
		
		return res;
	}

	Matrix44 Matrix44::operator*(float right)
	{
		Matrix44 res;
		res.m_xAxis = m_xAxis * right;
		res.m_yAxis = m_yAxis * right;
		res.m_zAxis = m_zAxis * right;
		res.m_wAxis = m_wAxis * right;
		return res;
	}
	
	Matrix44 Matrix44::operator*(const Matrix44& right) const
	{
		Matrix44 res;
		
		res.m_xAxis = *this * right.m_xAxis;
		res.m_yAxis = *this * right.m_yAxis;
		res.m_zAxis = *this * right.m_zAxis;
		res.m_wAxis = *this * right.m_wAxis;
		
		return res;
	}
	
	Matrix44 Transpose(Matrix44& mat)
	{
		Matrix44 result;
		
		result.m_xAxis = Vector4(mat.m_xAxis.X(), mat.m_yAxis.X(), mat.m_zAxis.X(), mat.m_wAxis.X());
		result.m_yAxis = Vector4(mat.m_xAxis.Y(), mat.m_yAxis.Y(), mat.m_zAxis.Y(), mat.m_wAxis.Y());
		result.m_zAxis = Vector4(mat.m_xAxis.Z(), mat.m_yAxis.Z(), mat.m_zAxis.Z(), mat.m_wAxis.Z());
		result.m_wAxis = Vector4(mat.m_xAxis.W(), mat.m_yAxis.W(), mat.m_zAxis.W(), mat.m_wAxis.W());
		
		return result;
	}
	
	Matrix44 Transpose3x3(const Matrix44& mat)
	{
		Matrix44 res;
		
		SetIdentity(res);
		res.m_xAxis = Vector4(mat.m_xAxis.X(), mat.m_yAxis.X(), mat.m_zAxis.X(), 0.0f);
		res.m_yAxis = Vector4(mat.m_xAxis.Y(), mat.m_yAxis.Y(), mat.m_zAxis.Y(), 0.0f);
		res.m_zAxis = Vector4(mat.m_xAxis.Z(), mat.m_yAxis.Z(), mat.m_zAxis.Z(), 0.0f);
		return res;
	}
	
	Matrix44 OrthoInverse(const Matrix44& mat)
	{
		Matrix44 out;
		Matrix44 invRot, invTrans;
		
		SetIdentity(invRot);
		invRot = Transpose3x3(mat);
		SetIdentity(invTrans);
		//	invTrans.SetTranslate(Vec4(-mat.m_translation.GetX(), -mat.m_translation.GetY(), -mat.m_translation.GetZ(), 1.0f));
		invTrans.SetTranslate(Point(-mat.m_wAxis.X(), -mat.m_wAxis.Y(), -mat.m_wAxis.Z()));
		
		out = invRot * invTrans;
		//	MatMul( *(Matrix*)&out, *(Matrix*)&invRot, *(Matrix*)&invTrans );
		return out;
	}
	
	Matrix44 Inverse(Matrix44& mat)
	{
		Matrix44 res = kIdentity;
		Matrix44 copy = mat;
		float*	data = (float*)&copy;
		float*	res_data = (float*)&res;
		
		int i, j, k;
		
		for( i = 0; i < 4; i++ )
		{
			float tmp = 1.0f / data[i*4+i];
			for( j = 3; j >= 0; j-- )
			{
				res_data[i*4+j] *= tmp;
				data[i*4+j] *= tmp;
			}
			for( j = 0; j < 4; j++ )
			{
				if( j != i )
				{
					tmp = data[j*4+i];
					for( k = 3; k >= 0; k-- )
					{
						res_data[j*4+k] -= res_data[i*4+k] * tmp;
						data[j*4+k] -= data[i*4+k] * tmp;
					}
				}
			}
		}
		return res;
	}
	
	void Matrix44::Ortho(Matrix44& result, float left, float right, float bottom, float top, float zNear, float zFar)
	{
		float tx = -(right + left) / (right - left);
		float ty = -(top + bottom) / (top - bottom);
		float tz = -(zFar + zNear) / (zFar - zNear);
		
		float* __restrict xAxis = (float*)&result.m_xAxis;
		float* __restrict yAxis = (float*)&result.m_yAxis;
		float* __restrict zAxis = (float*)&result.m_zAxis;
		float* __restrict wAxis = (float*)&result.m_wAxis;
		
		xAxis[0] = 2.0f / (right - left);
		xAxis[1] = 0.0f;
		xAxis[2] = 0.0f;
		xAxis[3] = 0.0f;
		
		yAxis[0] = 0.0f;
		yAxis[1] = 2.0f / (top - bottom);
		yAxis[2] = 0.0f;
		yAxis[3] = 0.0f;
		
		zAxis[0] = 0.0f;
		zAxis[1] = 0.0f;
		zAxis[2] = -2.0f / (zFar - zNear);
		zAxis[3] = 0.0f;
		
		wAxis[0] = tx;
		wAxis[1] = ty;
		wAxis[2] = tz;
		wAxis[3] = 1.0;
	}
	
	void Matrix44::Frustum(Matrix44& result, float left, float right, float bottom, float top, float nearZ, float farZ)
	{
		float A = (right + left) / (right - left);
		float B = (top + bottom) / (top - bottom);
		float C = -(farZ + nearZ) / (farZ - nearZ);
		float D = -(2.0f * farZ * nearZ) / (farZ - nearZ);
		
		float* __restrict xAxis = (float*)&result.m_xAxis;
		float* __restrict yAxis = (float*)&result.m_yAxis;
		float* __restrict zAxis = (float*)&result.m_zAxis;
		float* __restrict wAxis = (float*)&result.m_wAxis;
		
		xAxis[0] = 2.0f * nearZ / (right - left);
		xAxis[1] = 0.0f;
		xAxis[2] = 0.0f;
		xAxis[3] = 0.0f;
		
		yAxis[0] = 0.0f;
		yAxis[1] = 2.0f * nearZ / (top - bottom);
		yAxis[2] = 0.0f;
		yAxis[3] = 0.0f;
		
		zAxis[0] = A;
		zAxis[1] = B;
		zAxis[2] = C;
		zAxis[3] = -1.0f;
		
		wAxis[0] = 0.0f;
		wAxis[1] = 0.0f;
		wAxis[2] = D;
		wAxis[3] = 0.0;
	}
	
	void PrintMatrix(const char* text, Matrix44& mat)
	{
		printf("%s:\n", text);
		PrintVector("xAxis", mat.m_xAxis);
		PrintVector("yAxis", mat.m_yAxis);
		PrintVector("zAxis", mat.m_zAxis);
		PrintVector("wAxis", mat.m_wAxis);
		//	printf("\t%9.3f, %9.3f, %9.3f, %9.3f\n", mat.m_xAxis.m_x, mat.m_xAxis.m_y, mat.m_xAxis.m_z, mat.m_xAxis.m_w);
		//	printf("\t%9.3f, %9.3f, %9.3f, %9.3f\n", mat.m_yAxis.m_x, mat.m_yAxis.m_y, mat.m_yAxis.m_z, mat.m_yAxis.m_w);
		//	printf("\t%9.3f, %9.3f, %9.3f, %9.3f\n", mat.m_zAxis.m_x, mat.m_zAxis.m_y, mat.m_zAxis.m_z, mat.m_zAxis.m_w);
		//	printf("\t%9.3f, %9.3f, %9.3f, %9.3f\n", mat.m_translation.m_x, mat.m_translation.m_y, mat.m_translation.m_z, mat.m_translation.m_w);
	}	
