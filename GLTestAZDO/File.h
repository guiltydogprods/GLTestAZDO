/*
 *  File.h
 *  RTRacing
 *
 *  Created by Claire Rogers on 22/07/2006.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ION_FILE_H
#define _ION_FILE_H

#pragma once
#include "stdafx.h"
#include "ScopeStackAllocator.h" 

class File
{
public:
	File(ScopeStack& scope, const char *filename, const char *mode)
		: m_buffer(nullptr)
		, m_fptr(fopen(filename, mode))
	{
		if (m_fptr != NULL)
		{
			fseek(m_fptr, 0, SEEK_END);
			m_size = ftell(m_fptr);
			rewind(m_fptr);
			m_buffer = static_cast<uint8_t *>(scope.alloc(m_size));
		}
		ms_count++;
	}

	~File()
	{
		printf("File dtor %d\n", ms_count);
		fclose(m_fptr);
	}

private:
	uint8_t  *m_buffer;
	FILE	 *m_fptr;
	uint32_t m_size;
	static	 uint32_t ms_count;
};

#endif //_ION_FILE_H
