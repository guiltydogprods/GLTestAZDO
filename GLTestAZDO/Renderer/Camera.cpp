/*
 *  Camera.cpp
 *  ion::Engine
 *
 *  Created by Claire Rogers on 01/07/2013.
 *  Copyright 2013 Guilty Dog Productions Ltd. All rights reserved.
 *
 */

#include "stdafx.h"

#include "Camera.h"
#include "Math/FMath.h"

	Camera::Camera(float fieldOfView, float width, float height, float nearZ, float farZ)
	:
	m_frustum(fieldOfView, width, height, nearZ, farZ)
	{
	}

	Camera::~Camera()
	{
		fprintf(stdout, "Camera dtor\n");
	}

	void Camera::lookAt(const Point& position, const Point& at, const Vector& up)
	{
		Vector zAxis = Normalize(position - at);
		Vector xAxis = Normalize(Cross(up, zAxis));
		Vector yAxis = Cross(zAxis, xAxis);
		
		m_worldMatrix.SetAxisX(xAxis.GetVector4());
		m_worldMatrix.SetAxisY(yAxis.GetVector4());
		m_worldMatrix.SetAxisZ(zAxis.GetVector4());
		m_worldMatrix.SetAxisW(position.GetVector4());
		m_viewMatrix = OrthoInverse(m_worldMatrix);
	}
	
	void Camera::setWorldMatrix(const Matrix44& viewMatrix)
	{
		m_worldMatrix = viewMatrix;
		m_viewMatrix = OrthoInverse(m_worldMatrix);
	}

	void Camera::update()
	{
		float fov = Deg2Rad(m_frustum.m_fieldOfView);
		float aspectRatio = m_frustum.m_aspectRatio;
		float nearZ = m_frustum.m_nearZ;
		float farZ = m_frustum.m_farZ;
		float focalLength = 1.0f / tanf(fov * 0.5f);

//		float fovy = Rad2Deg(2.0f * (float)atan( aspectRatio / focalLength ));
//		gluPerspective(fovy, aspectRatio, nearZ, farZ);
//The following code is the same as calling gluPerspective above.
		float left = -nearZ / focalLength;
		float right = nearZ / focalLength;
		float bottom = -aspectRatio * nearZ / focalLength;
		float top = aspectRatio * nearZ / focalLength;
		
		Matrix44::Frustum(m_projectionMatrix, left, right, bottom, top, nearZ, farZ);
	}

