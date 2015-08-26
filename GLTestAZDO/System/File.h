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

char* FileRead(const char* filename, const char* filetype);

#endif //_ION_FILE_H

/*
class NewFile
{
public:
NewFile(ScopeStack& scope, const char *filename, const char *mode)
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

~NewFile()
{
printf("File dtor %d\n", ms_count);
fclose(m_fptr);
}
*/