/*
 *  Quat.h
 *  ion::Engine
 *
 *  Created by Claire Rogers on 27/02/2006.
 *  Copyright (c) 2013 Guilty Dog Productions Ltd. All rights reserved.
 *
 */

#pragma once

	class Matrix33;
	class Matrix44;
	class Vector;

	class Quat
	{
	public:
		Quat();
		Quat(float x, float y, float z, float w);
		Quat(float angle, Vector axis);
		
		void SetIdentity();
		void SetRotation(float angle, Vector axis);
		//	Mat33 GetMat33();
		Matrix44 GetMatrix44();
		void  GetAngleAxis(float& angle, Vector& axis);
		
		float Length();
		float Norm();
		void Normalise();
		Quat Conjugate();
		Quat Conjugate(Quat right);
		Quat Inverse();
		void ToEuler(Vector& euler);
		void FromEuler(Vector& euler);
		
		Quat operator+(Quat right);
		void operator+=(Quat right);
		Quat operator-(Quat right);
		void operator-=(Quat right);
		Quat operator*(float right);
		Quat operator/(float right);
		Quat operator*(Quat right);
		
	private:
		float m_x, m_y, m_z, m_w;
	}
#ifdef WIN32
	;
#else
	__attribute__((aligned (16)));
#endif
	
	Quat operator*(float left, Quat right);
