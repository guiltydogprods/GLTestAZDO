/*
 *  Shader.cpp
 */
#include "stdafx.h"
#include "Shader.h"
#include "System/File.h"

void CheckGLError();

	void Shader::CheckForCompileErrors(GLuint shader, GLint shaderType)
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
				else if (shaderType == GL_FRAGMENT_SHADER)
				{
					printf("Error: compiling fragmentShader\n%s\n", infoLog);
				}
				else if (shaderType == GL_COMPUTE_SHADER)
				{
					printf("Error: compiling computeShader\n%s\n", infoLog);
				}
				else
				{
					printf("Error: unknown shader type\n%s\n", infoLog);
				}
				free(infoLog);
			}
		}
	}

	Shader::Shader(const char* vertexShaderName, const char* fragmentShaderName, LinearAllocator& allocator)
		: m_glslProgram(0)
	{
		ScopeStack tempStack(allocator);

		const char *compileStrings[2] = { nullptr, nullptr };
		
		m_glslProgram = glCreateProgram();
		CheckGLError();
		if (vertexShaderName)
		{
			uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
			CheckGLError();
			File *pFile = tempStack.newObject<File>(vertexShaderName, "rt", tempStack);
			char* vs = (char *)pFile->getData();
			compileStrings[0] = vs;
			glShaderSource(vertexShader, 1, compileStrings, nullptr);
			CheckGLError();
			glCompileShader(vertexShader);
			CheckGLError();
			CheckForCompileErrors(vertexShader, GL_VERTEX_SHADER);
			glAttachShader(m_glslProgram, vertexShader);
			CheckGLError();
			glDeleteShader(vertexShader);
			CheckGLError();
		}
		
		if (fragmentShaderName)
		{
			uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			File *pFile = tempStack.newObject<File>(fragmentShaderName, "rt", tempStack);
			char* fs = (char *)pFile->getData();
			compileStrings[0] = fs;
			glShaderSource(fragmentShader, 1, compileStrings, nullptr);
			CheckGLError();
			glCompileShader(fragmentShader);
			CheckGLError();
			CheckForCompileErrors(fragmentShader, GL_FRAGMENT_SHADER);
			glAttachShader(m_glslProgram, fragmentShader);
			CheckGLError();
			glDeleteShader(fragmentShader);
			CheckGLError();
		}
		glLinkProgram(m_glslProgram);
		CheckGLError();
	}

	Shader::Shader(const char* computeShaderName, LinearAllocator& allocator)
		: m_glslProgram(0)
	{
		ScopeStack tempStack(allocator);

		const char *compileString = nullptr;

		m_glslProgram = glCreateProgram();
		CheckGLError();
		if (computeShaderName)
		{
			uint32_t computeShader = glCreateShader(GL_COMPUTE_SHADER);
			CheckGLError();
			File *pFile = tempStack.newObject<File>(computeShaderName, "rt", tempStack);
			char* vs = (char *)pFile->getData();
			compileString = vs;
			glShaderSource(computeShader, 1, &compileString, nullptr);
			CheckGLError();
			glCompileShader(computeShader);
			CheckGLError();
			CheckForCompileErrors(computeShader, GL_COMPUTE_SHADER);
			glAttachShader(m_glslProgram, computeShader);
			CheckGLError();
			glDeleteShader(computeShader);
			CheckGLError();
		}
		glLinkProgram(m_glslProgram);
		CheckGLError();
	}

	Shader::~Shader()
	{
		fprintf(stdout, "Shader dtor\n");
		glDeleteProgram(m_glslProgram);
		CheckGLError();
	}
