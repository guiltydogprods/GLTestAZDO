/*
 *  Shader.cpp
 *  ion::Engine
 *
 *  Created by Claire Rogers on 22/09/2005.
 *  Copyright 2005 Digital Frelp. All rights reserved.
 *
 */
#include "stdafx.h"
#include "Shader.h"
#include "System/File.h"
#include "ScopeStackAllocator.h"

void CheckGLError();

	void Shader::CheckForError(GLuint shader, GLint shaderType)
	{
		int32_t error = 0;

		glGetShaderiv(shader, GL_COMPILE_STATUS, &error);
		if (!error)
		{
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen > 1)
			{
				char* infoLog = (char*)malloc(sizeof(char) * infoLen);
				glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
				GLint shaderType = 0;
				glGetShaderiv(shader, GL_SHADER_TYPE, &shaderType);
				if (shaderType == GL_VERTEX_SHADER)
				{
					printf("Error: compiling vertexShader\n%s\n", infoLog);
				}
				else
				{
					printf("Error: compiling fragmentShader\n%s\n", infoLog);
				}
				free(infoLog);
			}
		}
	}

	Shader::Shader(uint32_t hash, const char* vertexShader, const char* fragmentShader, LinearAllocator& allocator)
		: m_glslProgram(0)
		, m_vertexShader(0)
		, m_fragmentShader(0)
	{
		ScopeStack tempStack(allocator);

		const char *compileStrings[2] = { nullptr, nullptr };
		char preprocString[512];
		
		m_glslProgram = glCreateProgram();
		CheckGLError();
		if( vertexShader )
		{
			m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
			CheckGLError();
			File *pFile = tempStack.newObject<File>(vertexShader, "rt", tempStack);
			char* vs = (char *)pFile->getData();
			compileStrings[0] = vs;
			glShaderSource(m_vertexShader, 1, compileStrings, nullptr);
			CheckGLError();
			glCompileShader(m_vertexShader);
			CheckGLError();
			CheckForError(m_vertexShader, GL_VERTEX_SHADER);
			glAttachShader(m_glslProgram, m_vertexShader);
			CheckGLError();
		}
		
		if( fragmentShader )
		{
			m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			File *pFile = tempStack.newObject<File>(fragmentShader, "rt", tempStack);
			char* fs = (char *)pFile->getData();
			compileStrings[0] = fs;
			glShaderSource(m_fragmentShader, 1, compileStrings, nullptr);
			CheckGLError();
			glCompileShader(m_fragmentShader);
			CheckGLError();
			CheckForError(m_fragmentShader, GL_FRAGMENT_SHADER);
			glAttachShader(m_glslProgram, m_fragmentShader);
			CheckGLError();
		}
		glLinkProgram(m_glslProgram);
		CheckGLError();
	}

	Shader::~Shader()
	{
		fprintf(stdout, "Shader dtor\n");
	}
