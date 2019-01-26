/*
 *  System.cpp
 *  ion::Engine
 *
 *  Created by Claire Rogers on 01/07/2013.
 *  Copyright 2013 Guilty Dog Productions Ltd. All rights reserved.
 *
 */

#include "stdafx.h"
#include "System.h"
#include <stdarg.h>

	void AssertFunc(const char *func, const char *file, int line, const char *e)
	{
		fprintf(stderr, "Assertion Failed: (%s),  function %s, file %s, line %d.\n", e, func, file, line);
		while (1) {}
		/*
#if defined(TARGET_CPU_X86) || defined(TARGET_CPU_X86_64)
		__asm("int3");
#endif
#if TARGET_CPU_ARM
		__asm volatile("bkpt 0");
#endif
		*/
	}
	
	void AssertMsgFunc(const char *func, const char *file, int line, const char *fmt, ...)
	{
		char debugString[255];
		
		va_list args;
		va_start(args, fmt);
		vsprintf(debugString, fmt, args);
		va_end(args);
		
		fprintf(stderr, "Assertion Failed: %s\n  function %s, file %s, line %d.\n", debugString, func, file, line);
		while(1) {}
		/*
#if defined(TARGET_CPU_X86) || defined(TARGET_CPU_X86_64)
		__asm("int3");
#endif
#if TARGET_CPU_ARM
		__asm volatile("bkpt 0");
#endif
		*/
	}
