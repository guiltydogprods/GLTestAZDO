/*
 *  Frustum.h
 *  ion::Engine
 *
 *  Created by Claire Rogers on 01/07/2013.
 *  Copyright 2013 Guilty Dog Productions Ltd. All rights reserved.
 *
 */

#pragma once

#include "Math/Vector4.h"

	class Camera;
	
	class Frustum
	{
	public:
		float	m_fieldOfView;
		float	m_aspectRatio;
		float	m_nearZ;
		float	m_farZ;
		Vector4	m_points[8];
		
		Frustum()	{}
		Frustum(float fieldOfView, float width, float height, float nearZ, float farZ);
		Frustum(float fieldOfView, float aspectRatio, float nearZ, float farZ);
		
		inline void SetAspectRatio(float width, float height) { m_aspectRatio = height / width; }
		inline void SetAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; }
		
		void CalcPoints(Camera* pCamera);
	
		void Render();
		
	};


