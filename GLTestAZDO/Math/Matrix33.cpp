/*
 *  Matrix33.cpp
 *  ion::Engine
 *
 *  Created by Claire Rogers on 10/09/2005.
 *  Copyright 2005 Digital Frelp. All rights reserved.
 *
 */
 
#include <stdio.h>
#include <memory.h>
#include "Matrix33.h"
#include "Vec4.h"

Matrix33::Matrix33()
{
}

Matrix33::Matrix33(Vec4& v1, Vec4& v2, Vec4& v3)
{
	
}

#ifdef __APPLE__
Matrix33::Matrix33(v128 v1, v128 v2, v128 v3)
{
	m_xAxis = v1;
	m_yAxis = v2;
	m_zAxis = v3;
}
#endif

extern Vec4 identityAxisX;
extern Vec4 identityAxisY;
extern Vec4 identityAxisZ;

void SetIdentity(Matrix33& mat)
{
//	memset(&mat, 0, sizeof(Matrix33));
//	mat.m[0][0] = mat.m[1][1] = mat.m[2][2] = mat.m[3][3] = 1.0f;
	mat.m_xAxis = identityAxisX;
	mat.m_yAxis = identityAxisY;
	mat.m_zAxis = identityAxisZ;
}

void Matrix33::SetAxisX(Vec4 axis)
{
	m_xAxis = axis;
}

void Matrix33::SetAxisY(Vec4 axis)
{
	m_yAxis = axis;
}

void Matrix33::SetAxisZ(Vec4 axis)
{
	m_zAxis = axis;
}

void Matrix33::SetRotation(float angle, Vec4 axis)
{
	float ax = axis.GetX();
	float ay = axis.GetY();
	float az = axis.GetZ();
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
	
	m_xAxis = Vec4( m00, m01, m02, 0.0f );
	m_yAxis = Vec4( m10, m11, m12, 0.0f );
	m_zAxis = Vec4( m20, m21, m22, 0.0f );
}

Matrix33 Matrix33::operator+(Matrix33 right)
{
	Matrix33 res;
	res.m_xAxis = m_xAxis + right.m_xAxis;
	res.m_yAxis = m_yAxis + right.m_yAxis;
	res.m_zAxis = m_zAxis + right.m_zAxis;
	return res;
}

Vec4 Matrix33::operator *(Vec4 right)
{
#if USE_ALTIVEC	
	v128 zero = (v128)vec_splat_u32(0);
	v128 vec = v128(right);
	v128* m = (v128*)this;
	v128 acc =   vec_madd(m[0], vec_splat(vec, 0), zero);
	acc		 =   vec_madd(m[1], vec_splat(vec, 1), acc);
	return		 vec_madd(m[2], vec_splat(vec, 2), acc);
#else
	Vec4 res, acc;
	
	acc = m_zAxis * right.GetZ();
	acc = acc + m_yAxis * right.GetY();
	res = acc + m_xAxis * right.GetX();
	
	return res;
#endif
}

Matrix33 Matrix33::operator*(float right)
{
	Matrix33 res;
	res.m_xAxis = m_xAxis * right;
	res.m_yAxis = m_yAxis * right;
	res.m_zAxis = m_zAxis * right;
	return res;
}

Matrix33 Matrix33::operator*(Matrix33 right)
{
	Matrix33 res;
	
	res.m_xAxis = *this * right.m_xAxis;
	res.m_yAxis = *this * right.m_yAxis;
	res.m_zAxis = *this * right.m_zAxis;
	
	return res;
}

Matrix33 Transpose(Matrix33& mat)
{
	Matrix33 result;

#if USE_ALTIVEC
	v128 row0 = mat.m_xAxis;
	v128 row1 = mat.m_yAxis;
	v128 row2 = mat.m_zAxis;
	v128 row3 = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	v128 temp0 = vec_mergeh( row0, row2 );
	v128 temp1 = vec_mergeh( row1, row3 );
	v128 temp2 = vec_mergel( row0, row2 );
	v128 temp3 = vec_mergel( row1, row3 );
	result.m_xAxis = vec_mergeh( temp0, temp1 );
	result.m_yAxis = vec_mergel( temp0, temp1 );
	result.m_zAxis = vec_mergeh( temp2, temp3 );
#else
	result.m_xAxis = Vec4(mat.m_xAxis.GetX(), mat.m_yAxis.GetX(), mat.m_zAxis.GetX(), 0.0f);
	result.m_yAxis = Vec4(mat.m_xAxis.GetY(), mat.m_yAxis.GetY(), mat.m_zAxis.GetY(), 0.0f);
	result.m_zAxis = Vec4(mat.m_xAxis.GetZ(), mat.m_yAxis.GetZ(), mat.m_zAxis.GetZ(), 0.0f);
#endif	
	
	return result;
}

void PrintMatrix(const char* text, Matrix33& mat)
{
	printf("%s:\n", text);
	PrintVector("xAxis", mat.m_xAxis);
	PrintVector("yAxis", mat.m_yAxis);
	PrintVector("zAxis", mat.m_zAxis);
}
