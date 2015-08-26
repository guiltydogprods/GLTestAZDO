//============================================================================
//	FastSinApprox
//
//	Fast approximation to sin using a quadratic curve
//============================================================================

float FastSinApprox(float x)
{
	float   Result;

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
