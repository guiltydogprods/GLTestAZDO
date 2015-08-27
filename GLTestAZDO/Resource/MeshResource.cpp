//
//  MeshResource.cpp
//  ion
//
//  Created by Claire Rogers on 28/04/2013.
//  Copyright (c) 2013 Spinning Head Software Ltd. All rights reserved.
//

#include "stdafx.h"
#include "MeshResource.h"
#include "Renderer/Mesh.h"
#include "System/System.h"

	MeshResource::MeshResource(const std::string filename)
	{
		std::string fullpath = /*ion::System::GetResourcePath() +*/ filename;

		FILE *fptr =  fopen(fullpath.c_str(), "rb");
		
		fseek(fptr, 0L, SEEK_END);
		uint32_t len = (uint32_t)ftell(fptr);
		fseek(fptr, 0L, SEEK_SET);
		//	uint8_t *buffer = (uint8_t*)malloc(len);
		uint8_t *buffer = new uint8_t [len];
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
		
	MeshResource::~MeshResource()
	{
		
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

	void MeshResource::LoadMeshChunk(ChunkId *chunk)
	{
		MeshChunk *meshChunk = (MeshChunk *)chunk;
		uint8_t *ptr = (uint8_t *)chunk + sizeof(MeshChunk);
		
		for (uint32_t i = 0; i < meshChunk->numMeshes; ++i) {
			Model* pModel = nullptr;	// new Model();
			ptr = LoadMeshChunkRecursive(ptr, pModel);
//			m_models.push_back(pModel);
		}
	}
	
	uint8_t* MeshResource::LoadMeshChunkRecursive(uint8_t* ptr, Model* pModel)
	{
		MeshInfo *info = (MeshInfo *)ptr;
		RenderableInfo* rendInfo = (RenderableInfo*)(ptr + sizeof(MeshInfo));
		
		uint32_t numRenderables = info->numRenderables;
//		Renderable* renderables = new Renderable [numRenderables];
		
		uint8_t* meshData = (uint8_t*)((uint8_t*)rendInfo + sizeof(RenderableInfo) * numRenderables);
		for (uint32_t rend = 0; rend < numRenderables; ++rend)
		{
//			CRVertexBuffer* srcVertexBuffer = (ion::CRVertexBuffer*)meshData;
			uint32_t numVertices = rendInfo[rend].numVertices;
			uint32_t verticesSize = sizeof(CRVertexBuffer) + srcVertexBuffer->m_streams[0].m_stride * numVertices;
			uint32_t numIndices = rendInfo[rend].numIndices;
			uint32_t indicesSize = sizeof(uint32_t) * numIndices;
			
			Renderable* pRenderable = &renderables[rend];
			MaterialPtr pMaterial = MaterialManager::Get()->Find(rendInfo[rend].materialHash);
			pRenderable->SetMaterial(pMaterial);
			pRenderable->SetNumVertices(numVertices);
			VertexBuffer* vertexBuffer = pRenderable->GetVertexBuffer();
			uint32_t numStreams = srcVertexBuffer->m_numStreams;
			vertexBuffer->SetNumStreams(numStreams);
			for (int i = 0; i < numStreams; ++i) {
				uint32_t numElements = (uint32_t)srcVertexBuffer->m_streams[i].m_numElements;
				for (uint32_t j = 0; j < numElements; ++j) {
					VertexElement newElem;
					//CRVertexElement (from data file) has an 8 bit offset , VertexElement has a 32bit offset, due to PS3 using offset into Video memory
					newElem.m_index = srcVertexBuffer->m_streams[i].m_elements[j].m_index;
					newElem.m_size = srcVertexBuffer->m_streams[i].m_elements[j].m_size;
					newElem.m_type = srcVertexBuffer->m_streams[i].m_elements[j].m_type;
					newElem.m_normalized = srcVertexBuffer->m_streams[i].m_elements[j].m_normalized;
					newElem.m_offset = (uint32_t)srcVertexBuffer->m_streams[i].m_elements[j].m_offset;
					vertexBuffer->AddElement(i, newElem);
				}
			}
			vertexBuffer->SetNumVertices(numVertices);
			for (int i = 0; i < numStreams; ++i) {
				uint8_t* vertexData = (uint8_t*)srcVertexBuffer + sizeof(CRVertexBuffer);	//(uint8_t*)info + srcVertexBuffer->m_streams[i].m_dataOffset;
				uint32_t size = (numVertices * vertexBuffer->GetVertexStreams()[i].m_stride) / 4;
				(uint32_t*)vertexData, size;
				vertexBuffer->SetData(i, vertexData);
			}
			vertexBuffer->Write();
			
			uint32_t* indices = (uint32_t*)((uint8_t*)srcVertexBuffer + verticesSize);
			IndexBuffer* indexBuffer = IndexBuffer::Create(numIndices, k32Bit);
			indices, numIndices;
			indexBuffer->WriteIndices(indices);
			indexBuffer->Upload();
//			pRenderable->SetNumIndices(rendInfo[rend].numIndices);
//			pRenderable->WriteIndices(indices, rendInfo[rend].numIndices * sizeof(uint32_t));
			pRenderable->SetIndexBuffer(indexBuffer);
			meshData = meshData + verticesSize + indicesSize;
		}
		ptr = (uint8_t*)meshData;
		(uint32_t*)&info->worldMatrix, 16;
		pModel->SetWorldMatrix(info->worldMatrix);
		pModel->AddRenderables(numRenderables, renderables);
		uint32_t numChildren = info->numChildren;
		pModel->AllocateChildren(numChildren);
		for (uint32_t i = 0; i < numChildren; ++i) {
			Model* pChild = nullptr;	// pModel->GetChild(i);
			ptr = LoadMeshChunkRecursive(ptr, pChild);
		}
		return ptr;
	}
	/*
	void MeshResource::GetModels(std::vector<ion::Model*>& scene)
	{
		for (int i = 0; i < m_models.size(); ++i) {
			scene.push_back(m_models[i]);
		}
	}
	*.
