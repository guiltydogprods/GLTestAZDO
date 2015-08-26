//
//  Frustum.cpp
//  ion
//
//  Created by Claire Rogers on 22/10/2012.
//  Copyright (c) 2013 Guilty Dog Productions Ltd. All rights reserved.
//

#include "stdafx.h"
#include "Frustum.h"
#include "Camera.h"

#include "Math/FMath.h"

	Frustum::Frustum(float fieldOfView, float width, float height, float nearZ, float farZ)
	{
		m_fieldOfView = fieldOfView;
		m_aspectRatio = height / width;
		m_nearZ = nearZ;
		m_farZ = farZ;
	}

	Frustum::Frustum(float fieldOfView, float aspectRatio, float nearZ, float farZ)
	{
		m_fieldOfView = fieldOfView;
		m_aspectRatio = aspectRatio;
		m_nearZ = nearZ;
		m_farZ = farZ;
	}
	
	void Frustum::CalcPoints(Camera* pCamera)
	{
		Matrix44& camWorld = pCamera->GetWorldMatrix();
		
		Vector4 right = camWorld.GetAxisX();
		Vector4 up = camWorld.GetAxisY();
		Vector4 fwd = camWorld.GetAxisZ() * -1.0f;
		Vector4 camPos = camWorld.GetAxisW();
		
		Vector4 fc = camPos + fwd * m_farZ;
		Vector4 nc = camPos + fwd * m_nearZ;
				
		// these heights and widths are half the heights and widths of
		// the near and far plane rectangles
		float nearHeight = tanf(m_fieldOfView/2.0f) * m_nearZ;
		float nearWidth = nearHeight * 1.0f / m_aspectRatio;
		float farHeight = tanf(m_fieldOfView/2.0f) * m_farZ;
		float farWidth = farHeight * 1.0f / m_aspectRatio;
		
		m_points[0] = nc - up*nearHeight - right*nearWidth;
		m_points[1] = nc + up*nearHeight - right*nearWidth;
		m_points[2] = nc + up*nearHeight + right*nearWidth;
		m_points[3] = nc - up*nearHeight + right*nearWidth;
		
		m_points[4] = fc - up*farHeight - right*farWidth;
		m_points[5] = fc + up*farHeight - right*farWidth;
		m_points[6] = fc + up*farHeight + right*farWidth;
		m_points[7] = fc - up*farHeight + right*farWidth;
	}
	
	void Frustum::Render()
	{
/*
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3fv((float*)&m_points[0]);
		glVertex3fv((float*)&m_points[1]);
		glVertex3fv((float*)&m_points[1]);
		glVertex3fv((float*)&m_points[2]);
		glVertex3fv((float*)&m_points[2]);
		glVertex3fv((float*)&m_points[3]);
		glVertex3fv((float*)&m_points[3]);
		glVertex3fv((float*)&m_points[0]);

		glVertex3fv((float*)&m_points[4]);
		glVertex3fv((float*)&m_points[5]);
		glVertex3fv((float*)&m_points[5]);
		glVertex3fv((float*)&m_points[6]);
		glVertex3fv((float*)&m_points[6]);
		glVertex3fv((float*)&m_points[7]);
		glVertex3fv((float*)&m_points[7]);
		glVertex3fv((float*)&m_points[4]);

		glVertex3fv((float*)&m_points[0]);
		glVertex3fv((float*)&m_points[4]);
		glVertex3fv((float*)&m_points[1]);
		glVertex3fv((float*)&m_points[5]);
		glVertex3fv((float*)&m_points[2]);
		glVertex3fv((float*)&m_points[6]);
		glVertex3fv((float*)&m_points[3]);
		glVertex3fv((float*)&m_points[7]);
		
		glEnd();
*/ 
	}
