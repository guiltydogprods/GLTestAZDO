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
		~Camera();

		void update();

		void lookAt(const Point& position, const Point& at, const Vector& up);
		void setWorldMatrix(const Matrix44& viewMatrix);
		
		void setPosition( float x, float y,  float z );
		


	protected:
		Frustum		m_frustum;
		Matrix44	m_worldMatrix;
		Matrix44	m_viewMatrix;
		Matrix44	m_projectionMatrix;		
	};

