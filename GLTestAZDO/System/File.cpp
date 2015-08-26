/*
 *  File.cpp
 *  RTRacing
 *
 *  Created by Claire Rogers on 22/07/2006.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */
#include "stdafx.h"
#include "File.h"

#include "direct.h"
	
	char* FileRead(const char* filename, const char* filetype) 
	{
		FILE *fp;
		char *content = NULL;
		size_t count = 0;
			
		if (filename != NULL) 
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
