/*
 *  Shader.h
 *  ion::Engine
 *
 *  Created by Claire Rogers on 01/07/2013.
 *  Copyright 2013 Guilty Dog Productions Ltd. All rights reserved.
 *
 */

#pragma once

#ifdef __cplusplus
	class Shader
	{
	public:		
		Shader(const char* vertexShader, const char* fragmentShader, LinearAllocator& allocator);
		Shader(const char* computeShader, LinearAllocator& allocator);
		~Shader();

		inline uint32_t GetProgram()				{ return m_glslProgram; }

		void CheckForCompileErrors(GLuint shader, GLint shaderType);

		uint32_t		m_glslProgram;
	};

#endif //__cplusplus