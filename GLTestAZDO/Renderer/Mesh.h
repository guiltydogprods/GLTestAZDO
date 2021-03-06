//
/*
 *  Mesh.h
 *  ion::Engine
 *
 *  Created by Claire Rogers on 01/07/2013.
 *  Copyright 2013 Guilty Dog Productions Ltd. All rights reserved.
 *
 */

#pragma once

#include "Math/Matrix44.h"
#include "VertexBuffer.h"

	const int kVersionMajor = 0;
	const int kVersionMinor = 1;
	
	struct ChunkId
	{
		char	tag[4];
		uint32_t size;
	};
	
	struct ModelHeader
	{
		ChunkId		chunkId;
		uint32_t	versionMajor;
		uint32_t	versionMinor;
	};
	
	struct RenderableInfo
	{
		uint32_t	numVertices;
		uint32_t	numIndices;
		uint32_t	materialHash;
		float		aabbMinX;
		float		aabbMinY;
		float		aabbMinZ;
		float		aabbMaxX;
		float		aabbMaxY;
		float		aabbMaxZ;
		uint32_t	verticesOffset;
		uint32_t	indicesOffset;
	};
	
	struct MeshInfo
	{
		Matrix44	worldMatrix;
		float		aabbMinX;
		float		aabbMinY;
		float		aabbMinZ;
		float		aabbMaxX;
		float		aabbMaxY;
		float		aabbMaxZ;
		uint32_t	numRenderables;
		uint32_t	numChildren;
	};
	
	struct MeshChunk
	{
		ChunkId		chunkId;
		uint32_t	numMeshes;
	};
	
	struct TextureInfo
	{
		uint32_t width;
		uint32_t height;
		uint32_t textureOffset;
	};
	
	struct TextureChunk
	{
		ChunkId chunkId;
		uint32_t numTextures;
	};
	
	struct MaterialInfo
	{
		float		diffuse[3];
		float		ambient[3];
		float		specular[4];
		float		alpha;
		uint32_t	flags;
		float		reflectVal;
		uint32_t	diffuseMapHash;
		uint32_t	normalMapHash;
	};
	
	struct MaterialChunk
	{
		ChunkId chunkId;
		uint32_t numMaterials;
	};
	
	class Mesh
	{
	public:
		Mesh(const std::string filename, LinearAllocator& allocator);
		~Mesh();

		GLuint getVertexArrayObject()	{	return m_vertexArrayObject; }
		GLuint getIndexBuffer()			{	return m_indexBuffer;		}
		GLsizei getNumIndices()			{ return m_numIndices; }
		void getAABB(float(&aabbMin)[3], float(&aabbMax)[3]);

	private:
		void LoadTextureChunk(ChunkId* chunk);
		void LoadMaterialChunk(ChunkId *chunk);
		void LoadMeshChunk(ChunkId *chunk);
		uint8_t* LoadMeshChunkRecursive(uint8_t* ptr);

		GLuint	m_vertexArrayObject;
		GLuint	m_vertexBuffer;
		GLuint  m_indexBuffer;
		GLsizei m_numIndices;
		float	m_aabbMin[3];
		float	m_aabbMax[3];
	};
