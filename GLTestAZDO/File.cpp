/*
 *  File.cpp
 *  RTRacing
 *
 *  Created by Claire Rogers on 22/07/2006.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include "File.h"

#ifdef __APPLE__
#include "libgen.h"
#include "unistd.h"
#elif WIN32
#include "direct.h"
#elif PSVITA

#else
#include "unistd.h"
#endif

namespace ion
{
	void SetCwd( const char* filename )
	{
#if defined(__APPLE__)
		chdir(filename);
#elif WIN32
		_chdir(filename);
#else

#endif		
	}
	
	char* FileRead(const char* filename, const char* filetype) 
	{
		FILE *fp;
		char *content = NULL;
		size_t count = 0;
			
		if(filename != NULL) 
		{
			fp = fopen(filename, filetype);
			
			if(fp != NULL) 
			{
				fseek(fp, 0, SEEK_END);
				count = ftell(fp);
				rewind(fp);
					
				if(count > 0) 
				{
					content = new char[count+1];
					count = fread(content,sizeof(char),count,fp);
					content[count] = '\0';
				}
				fclose(fp);
			}
		}
		return content;
	}
}; //namespace ion
