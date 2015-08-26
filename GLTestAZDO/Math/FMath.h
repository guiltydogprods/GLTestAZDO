//
//  FMath.h
//  ion
//
//  Created by Claire Rogers on 22/10/2012.
//  Copyright (c) 2013 Guilty Dog Productions Ltd. All rights reserved.
//

#ifndef ion_FMath_h
#define ion_FMath_h

#pragma once

#include <math.h>
#include <stdint.h>

	const float kPi = 3.14159265359f;
	const float kTwoPi = 2.0f * kPi;
	const float kHalfPi = kPi * 0.5f;
	const float kInvTwoPi = 1.0f / kTwoPi;
	const float kPiOver180 = kPi / 180.0f;
	const float kRPiOver180 = 1.0f / kPiOver180;
	const float kFourOverPi = 4.0f/kPi;
	const float kMinusFourOverPiSquared = -4.0f/(kPi*kPi);

	inline float Deg2Rad(float x)
	{
		return x * kPiOver180;
	}
	
	inline float Rad2Deg(float x)
	{
		return x * kRPiOver180;
	}
	
	inline float Min(float a, float b)
	{
		return a < b ? a : b;
	}
	
	inline float Max(float a, float b)
	{
		return a > b ? a : b;
	}
	
	inline float Abs(float f)
	{
		int32_t	i = ((*(int32_t*)&f) & 0x7fffffff);
		return (*(float*)&i);
	}
	
	inline float Sin(float x)
	{
		
		//		float twoPi = 2.0f * kPi;
		//		float invTwoPi = 1.0f / twoPi;
/*
		if (x < -kPi || x > kPi)
		{
			int k = (int)(x * kInvTwoPi);
			x = x - (float)k * kTwoPi;
			
			//followed by
			//		float x = ...;
			x = Min(x, kPi - x);
			x = Max(x, -kPi - x);
			x = Min(x, kPi - x);
		}
*/ 
		//		float y = B * x + C * x * fabs(x);
		float y = kFourOverPi * x + kMinusFourOverPiSquared * x * Abs(x);
		
#if 1	//def EXTRA_PRECISION
		//  const float Q = 0.775;
        const float P = 0.225f;
		
        y = P * (y * Abs(y) - y) + y;   // Q * y + P * y * abs(y)
#endif
		return y;
	}
	
	inline float Cos(float x)
	{
		x = x < kHalfPi ? x + kHalfPi : x - (kPi+kHalfPi);
		return Sin(x);
	}
	
	inline float FastSinApprox(float x)
	{
		float   Result;
		/*
		 int k = (int)(x * kInvTwoPi);
		 x = x - (float)k * kTwoPi;
		 //followed by
		 //		float x = ...;
		 x = Min(x, kPi - x);
		 x = Max(x, -kPi - x);
		 x = Min(x, kPi - x);
		 */
		if(x < -3.14159265f)
		{
			x += 6.28318531f;
		}
		else
		{
			if (x >  3.14159265f)
			{
				x -= 6.28318531f;
			}
		}
		
		if(x < 0.0f)
		{
			Result = 1.27323954f * x + 0.405284735f * x * x;
			if (Result < 0.0f)
			{
				Result = 0.225f * (Result *-Result - Result) + Result;
			}
			else
			{
				Result = 0.225f * (Result * Result - Result) + Result;
			}
		}
		else
		{
			Result = 1.27323954f * x - 0.405284735f * x * x;
			
			if (Result < 0.0f)
			{
				Result = 0.225f * (Result * -Result - Result) + Result;
			}
			else
			{
				Result = 0.225f * (Result * Result - Result) + Result;
			}
		}
		
		return	Result;
	}

#endif
