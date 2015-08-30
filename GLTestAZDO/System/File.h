/*
 *  File.h
 *  RTRacing
 *
 *  Created by Claire Rogers on 22/07/2006.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "ScopeStackAllocator.h"

char* FileRead(const char* filename, const char* filetype);

class File
{
public:
	File(const char *filename, const char *mode, ScopeStack &scopeStack)
		: m_buffer(nullptr)
		, m_fptr(fopen(filename, mode))
	{
		if (m_fptr != NULL)
		{
			fseek(m_fptr, 0, SEEK_END);
			m_size = ftell(m_fptr);
			rewind(m_fptr);
			m_buffer = static_cast<char *>(scopeStack.alloc(m_size+1));
			memset(m_buffer, 0, m_size);
			size_t numBytesRead = 0;
			numBytesRead = fread(m_buffer, 1, m_size, m_fptr);
			m_buffer[numBytesRead] = '\0';
		}
	}

	~File()
	{
		printf("File dtor\n");
		fclose(m_fptr);
	}

	char *getData()
	{
		return m_buffer;
	}

private:
	FILE	*m_fptr;
	size_t	m_size;
	char	*m_buffer;
};