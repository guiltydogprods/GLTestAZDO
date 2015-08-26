//
//	Matrix33.h
//	ion
//
//	Created by Claire Rogers on 14/10/2012.
//	Copyright (c) 2013 Guilty Dog Productions Ltd. All rights reserved.
//

#ifndef __ion__Matrix33__
#define __ion__Matrix33__

#include "Vector4.h"

namespace ion
{
	class Matrix33
	{
	public:
		Matrix33();
		Matrix33(Vector4& v1, Vector4& v2, Vector4& v3);
		
		void SetAxisX(Vector4 axis);
		void SetAxisY(Vector4 axis);
		void SetAxisZ(Vector4 axis);
		
		void SetRotation(float angle, Vector4 axis);
		
		Vector4 operator*(Vector4 right);
		Matrix33 operator*(float right);
		Matrix33 operator+(Matrix33 right);
		Matrix33 operator*(Matrix33 right);

	private:
		Vector4	m_xAxis;
		Vector4	m_yAxis;
		Vector4	m_zAxis;		
	};
	
	void SetIdentity(Matrix33& mat);
	Matrix33 Transpose(Matrix33& mat);
	void PrintMatrix(const char* text, Matrix33& mat);
}

#endif //__ion__Matrix33__

