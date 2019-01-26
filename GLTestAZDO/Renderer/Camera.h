/*
 *  Camera.h
 *  ion::Engine
 *
 *  Created by Claire Rogers on 01/07/2013.
 *  Copyright 2013 Guilty Dog Productions Ltd. All rights reserved.
 *
 */

#pragma once 

#include "Frustum.h"

#include "Math/Vector4.h"
#include "Math/Point.h"
#include "Math/Matrix44.h"

	class Camera
	{
	public:
		Camera(float fieldOfView, float width, float height, float zNear, float zFar);
		~Camera();

		void update();

		void lookAt(const Point& position, const Point& at, const Vector& up);
		void setWorldMatrix(const Matrix44& viewMatrix);
		
		void setPosition( float x, float y,  float z );
		
		inline void setPosition(const Point& position)				{	m_worldMatrix.SetAxisW(position.GetVector4());	}

		inline const Frustum& getFrustum() const					{	return m_frustum;								}
		inline Vector	getRightVector() 							{	return Vector(m_worldMatrix.GetAxisX());		}
		inline Vector	getUpVector()								{	return Vector(m_worldMatrix.GetAxisY());		}
		inline Vector	getForwardVector()							{	return Vector(m_worldMatrix.GetAxisZ());		}
		inline Point	getPosition() 								{	return Point(m_worldMatrix.GetAxisW());			}
		inline Matrix44 getViewMatrix()								{	return m_viewMatrix;							}
		inline Matrix44 getWorldMatrix()							{	return m_worldMatrix;							}
		inline Matrix44 getProjectionMatrix()						{	return m_projectionMatrix;						}

	protected:
		Frustum		m_frustum;
		Matrix44	m_worldMatrix;
		Matrix44	m_viewMatrix;
		Matrix44	m_projectionMatrix;		
	};

