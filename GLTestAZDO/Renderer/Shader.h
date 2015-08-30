//
//  Shader.h
//	ion
//
//  Created by Claire Rogers on 14/10/2012.
//  Copyright (c) 2013 Guilty Dog Productions Ltd. All rights reserved.
//
#pragma once
#include "ScopeStackAllocator.h"

#ifdef __cplusplus
	class Shader
	{
	public:		
		Shader(uint32_t hash, const char* vertexShader, const char* fragmentShader, LinearAllocator& allocator);
		~Shader();

		inline uint32_t GetProgram()				{ return m_glslProgram; }
		inline uint32_t GetHash()					{ return m_hash; }
		inline uint32_t* GetParameters()			{ return m_parameters; }

		void CheckForError(GLuint shader, GLint shaderType);

		uint32_t		m_hash;
		uint32_t		m_glslProgram;
		uint32_t		m_vertexShader;
		uint32_t		m_fragmentShader;
		
		uint32_t		m_parameters[7];
		//		uint32_t m_texturedShaderModelViewProjPos;
		//		uint32_t m_texturedShaderModelViewPos;
		//		uint32_t m_texturedShaderNormalMatrixPos;
		//		uint32_t m_texturedShaderLight0PosPos;
		//		uint32_t m_texturedShaderMaterialDiffusePos;
		//		uint32_t m_texturedShaderTexPos;
		
	};

#endif //__cplusplus