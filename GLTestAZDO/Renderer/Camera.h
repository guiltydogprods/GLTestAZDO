//
//  Camera.h
//  ion
//
//  Created by Claire Rogers on 22/10/2012.
//  Copyright (c) 2013 Guilty Dog Productions Ltd. All rights reserved.
//

#pragma once 

#include "Frustum.h"

#include "Math/Vector4.h"
#include "Math/Point.h"
#include "Math/Matrix44.h"

	class Camera
	{
	public:
		Camera(float fieldOfView, float width, float height, float zNear, float zFar);

		void Update();

		void LookAt(Point position, Point at, Vector up);
		void SetWorldMatrix(Matrix44& viewMatrix);
		
		void SetPosition( float x, float y,  float z );
		
		inline void SetPosition(Point position)						{	m_worldMatrix.SetAxisW(position.GetVector4());			}

		inline Frustum& GetFrustum()								{	return m_frustum;							}
		inline Vector	GetRightVector()							{	return Vector(m_worldMatrix.GetAxisX());	}
		inline Vector	GetUpVector()								{	return Vector(m_worldMatrix.GetAxisY());	}
		inline Vector	GetForwardVector()							{	return Vector(m_worldMatrix.GetAxisZ());	}
		inline Point	GetPosition()								{	return Point(m_worldMatrix.GetAxisW());		}
		inline Matrix44& GetViewMatrix()							{	return m_viewMatrix;						}
		inline Matrix44& GetWorldMatrix()							{	return m_worldMatrix;						}
		inline Matrix44& GetProjectionMatrix()						{	return m_projectionMatrix;					}

	protected:
		Frustum		m_frustum;
		Matrix44	m_worldMatrix;
		Matrix44	m_viewMatrix;
		Matrix44	m_projectionMatrix;		
	};

