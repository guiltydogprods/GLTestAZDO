//
//  MeshResource.cpp
//  ion
//
//  Created by Claire Rogers on 28/04/2013.
//  Copyright (c) 2013 Spinning Head Software Ltd. All rights reserved.
//

#include "stdafx.h"
#include "Mesh.h"
#include "Renderer/Mesh.h"
#include "System/System.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void CheckGLError();


	Mesh::Mesh(const std::string filename, LinearAllocator& allocator)
	{
		ScopeStack tempStack(allocator);

		std::string fullpath = /*ion::System::GetResourcePath() +*/ filename;

		FILE *fptr =  fopen(fullpath.c_str(), "rb");
		
		fseek(fptr, 0L, SEEK_END);
		uint64_t len = (uint64_t)ftell(fptr);
		fseek(fptr, 0L, SEEK_SET);
		uint8_t *buffer = static_cast<uint8_t *>(tempStack.alloc(len));
		fread(buffer, len, 1, fptr);
		fclose(fptr);
		
		bool bLoaded = false;
		uint8_t *ptr = (uint8_t *)(buffer + sizeof(ModelHeader));
		while (!bLoaded)
		{
			ChunkId *chunk = (ChunkId *)ptr;
			if (!strncmp(chunk->tag, "TEXT", 4))
			{
//				LoadTextureChunk(chunk);
			}
			else if (!strncmp(chunk->tag, "MATL", 4))
			{
//				LoadMaterialChunk(chunk);
			}
			else if (!strncmp(chunk->tag, "MESH", 4))
			{
				LoadMeshChunk(chunk);
				bLoaded = true;
			}
			else
			{
				AssertMsg(0, "Invalid Chunk");
			}
			
			ptr = ptr + chunk->size;
		}
	}
		
	Mesh::~Mesh()
	{
		fprintf(stdout, "Mesh dtor\n", glGetString(GL_VENDOR));
		glDeleteBuffers(1, &m_indexBuffer);
		glDeleteVertexArrays(1, &m_vertexArrayObject);
		glDeleteBuffers(1, &m_vertexBuffer);
	}
	
	/*
	void MeshResource::LoadTextureChunk(ChunkId *chunk)
	{
		TextureChunk *textureChunk = (TextureChunk *)chunk;
		uint32_t numTextures = LE2NE(textureChunk->numTextures);
		uint8_t* ptr = (uint8_t*)chunk + sizeof(TextureChunk);
		for (uint32_t i = 0; i < numTextures; ++i)
		{
			uint32_t len = LE2NE(*(uint32_t*)ptr);
			ptr += sizeof(uint32_t);
			char strBuffer[1024];
			strncpy(strBuffer, (char*)ptr, len);
			strBuffer[len] = '\0';
			std::string filename(strBuffer);
			ion::TextureManager::Get()->Find(filename);
			uint32_t alignedLen = len + 4 & ~(4-1);						//4 bytes length + string length + padding to 4 bytes
			ptr += alignedLen;
		}
	}
	
	void MeshResource::LoadMaterialChunk(ChunkId *chunk)
	{		
		MaterialChunk *materialChunk = (MaterialChunk *)chunk;
		uint32_t numMaterials = LE2NE(materialChunk->numMaterials);
		uint8_t* ptr = (uint8_t*)chunk + sizeof(MaterialChunk);		
		for (uint32_t i = 0; i < numMaterials; ++i)
		{
			uint32_t len = LE2NE(*(uint32_t*)ptr);
			ptr += sizeof(uint32_t);
			char strBuffer[1024];
			strncpy(strBuffer, (char*)ptr, len);
			strBuffer[len] = '\0';
			std::string materialName(strBuffer);
			uint32_t alignedLen = len + 4 & ~(4-1);						//4 bytes length + string length + padding to 4 bytes
			ptr += alignedLen;
			ion::MaterialPtr pMaterial = ion::MaterialManager::Get()->Find(materialName);
			if (!pMaterial)
			{
				MaterialInfo *info = (MaterialInfo *)ptr;
				pMaterial = new Material();
				pMaterial->SetName(materialName);
				pMaterial->m_diffuse[0] = LE2NE(info->diffuse[0]);
				pMaterial->m_diffuse[1] = LE2NE(info->diffuse[1]);
				pMaterial->m_diffuse[2] = LE2NE(info->diffuse[2]);
				pMaterial->m_ambient[0] = LE2NE(info->ambient[0]);
				pMaterial->m_ambient[1] = LE2NE(info->ambient[1]);
				pMaterial->m_ambient[2] = LE2NE(info->ambient[2]);
				pMaterial->m_specular[0] = LE2NE(info->specular[0]);
				pMaterial->m_specular[1] = LE2NE(info->specular[1]);
				pMaterial->m_specular[2] = LE2NE(info->specular[2]);
				pMaterial->m_specular[3] = LE2NE(info->specular[3]);
				pMaterial->m_alpha = LE2NE(info->alpha);
				pMaterial->m_flags = LE2NE(info->flags);
				pMaterial->m_reflectVal = LE2NE(info->reflectVal);
				if (pMaterial->m_flags & 0x1) {
					pMaterial->m_pDiffuseMap = ion::TextureManager::Get()->Find(LE2NE(info->diffuseMapHash));
				}
				pMaterial->BindToGL();
				ion::MaterialManager::Get()->Add(pMaterial);				
			}
			ptr += sizeof(MaterialInfo);
		}
	}
	*/

	void Mesh::LoadMeshChunk(ChunkId *chunk)
	{
		MeshChunk *meshChunk = (MeshChunk *)chunk;
		uint8_t *ptr = (uint8_t *)chunk + sizeof(MeshChunk);
		
		for (uint32_t i = 0; i < meshChunk->numMeshes; ++i) 
		{
			ptr = LoadMeshChunkRecursive(ptr);
		}
	}
	
	uint8_t* Mesh::LoadMeshChunkRecursive(uint8_t* ptr)
	{
		MeshInfo *info = (MeshInfo *)ptr;
		RenderableInfo* rendInfo = (RenderableInfo*)(ptr + sizeof(MeshInfo));
		
		uint32_t numRenderables = info->numRenderables;
		
		uint8_t* meshData = (uint8_t*)((uint8_t*)rendInfo + sizeof(RenderableInfo) * numRenderables);
		for (uint32_t rend = 0; rend < numRenderables; ++rend)
		{
			VertexBuffer* srcVertexBuffer = (VertexBuffer*)meshData;
			uint32_t numVertices = rendInfo[rend].numVertices;
			uint32_t verticesSize = sizeof(VertexBuffer) + srcVertexBuffer->m_streams[0].m_stride * numVertices;
			uint32_t numIndices = m_numIndices = rendInfo[rend].numIndices;
			uint32_t indicesSize = sizeof(uint32_t) * numIndices;
			
			m_aabbMin[0] = rendInfo->aabbMinX;
			m_aabbMin[1] = rendInfo->aabbMinY;
			m_aabbMin[2] = rendInfo->aabbMinZ;
			m_aabbMax[0] = rendInfo->aabbMaxX;
			m_aabbMax[1] = rendInfo->aabbMaxY;
			m_aabbMax[2] = rendInfo->aabbMaxZ;
//			MaterialPtr pMaterial = MaterialManager::Get()->Find(rendInfo[rend].materialHash);
			uint32_t numStreams = srcVertexBuffer->m_numStreams;
			for (uint32_t i = 0; i < numStreams; ++i) 
			{
				uint8_t* vertexData = (uint8_t*)srcVertexBuffer + sizeof(VertexBuffer);
				uint16_t stride = srcVertexBuffer->m_streams[0].m_stride;
				glGenBuffers(1, &m_vertexBuffer);
				CheckGLError();
				glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
				CheckGLError();
				glBufferData(GL_ARRAY_BUFFER, numVertices * stride, vertexData, GL_STATIC_DRAW);
				CheckGLError();
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				CheckGLError();

				glGenVertexArrays(1, &m_vertexArrayObject);
				CheckGLError();
				glBindVertexArray(m_vertexArrayObject);
				CheckGLError();
				glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
				CheckGLError();
				uint32_t numElements = (uint32_t)srcVertexBuffer->m_streams[i].m_numElements;
				for (uint32_t j = 0; j < numElements; ++j) 
				{
					//CRVertexElement (from data file) has an 8 bit offset , VertexElement has a 32bit offset, due to PS3 using offset into Video memory
					uint8_t index = srcVertexBuffer->m_streams[i].m_elements[j].m_index;
					int8_t size = srcVertexBuffer->m_streams[i].m_elements[j].m_size;
					uint16_t type = srcVertexBuffer->m_streams[i].m_elements[j].m_type;
					uint8_t normalized = srcVertexBuffer->m_streams[i].m_elements[j].m_normalized;
					uint8_t offset = srcVertexBuffer->m_streams[i].m_elements[j].m_offset;
					glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(offset));
					CheckGLError();
					glEnableVertexAttribArray(index);
					CheckGLError();
				}
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				CheckGLError();
			}
			uint32_t* indices = (uint32_t*)((uint8_t*)srcVertexBuffer + verticesSize);
			glGenBuffers(1, &m_indexBuffer);
			CheckGLError();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
			CheckGLError();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32_t), indices, GL_STATIC_DRAW);
			CheckGLError();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			CheckGLError();
			meshData = meshData + verticesSize + indicesSize;
		}
		ptr = (uint8_t*)meshData;
		(uint32_t*)&info->worldMatrix, 16;
//		pModel->SetWorldMatrix(info->worldMatrix);
		uint32_t numChildren = info->numChildren;
		for (uint32_t i = 0; i < numChildren; ++i) {
			ptr = LoadMeshChunkRecursive(ptr);
		}
		return ptr;
	}

	void Mesh::getAABB(float(&aabbMin)[3], float(&aabbMax)[3])
	{
		for (uint32_t i = 0; i < 3; ++i)
		{
			aabbMin[i] = m_aabbMin[i];
			aabbMax[i] = m_aabbMax[i];
		}
	}


	