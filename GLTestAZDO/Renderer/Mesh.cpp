/*
 *  Mesh.cpp
 *  ionEngine
 *
 *  Created by Claire Rogers on 21/07/2007.
 *  Copyright 2007 Digital Frelp. All rights reserved.
 *
 */
#include "stdafx.h"
#include "Mesh.h"

	CRVertexStream::CRVertexStream()
	{
		m_numElements = 0;
		m_stride = 0;
	}
	
	void CRVertexStream::AddElement(uint8_t index, int8_t size, uint16_t type, uint8_t normalized, uint8_t offset)
	{
		uint32_t elem = m_numElements++;
		m_elements[elem].m_index = index;
		m_elements[elem].m_size = size;
		m_elements[elem].m_type = type;
		m_elements[elem].m_normalized = normalized;
		m_elements[elem].m_offset = offset;
		/*
		switch (type)
		{
			case kFloat:
				m_stride += sizeof(float) * size;
				break;
			default:
				assert(0);
		}
		*/
	}
	
	void CRVertexBuffer::Init(uint32_t numStreams)
	{
		this->m_numStreams = numStreams;
		for (uint32_t i = 0; i < numStreams; ++i)
		{
			this->m_streams[i].m_numElements = 0;
			this->m_streams[i].m_stride = 0;
		}
	}
	
		
	Renderable::Renderable()
	{
		//		m_flags = 0;
		m_vertexBuffer = VertexBuffer::Create();
		m_indexBuffer = 0;
	}
	
	Renderable::~Renderable()
	{
		//CLR: We should probably delete our vertex buffer here too.
		if (m_indexBuffer) {
			m_indexBuffer->Destroy();
		}
	}
	
	Model::Model()
	{
		m_numChildren = 0;
		m_children = 0;
	}
	
	Model::~Model()
	{
		
	}
	
	void Model::AllocateChildren(uint32_t childCount)
	{
		m_numChildren = childCount;
		if (childCount > 0) {
			m_children = new ion::Model [childCount];			
		}
	}
	
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
	
	void Model::Render(Matrix44* pWorldMatrix)
	{
		/*
		Matrix44 worldMatrix;
		//Set matrix
		if (pWorldMatrix == NULL)
		{
			worldMatrix = m_worldMatrix;
		}
		else
		{
			worldMatrix =  *pWorldMatrix * m_worldMatrix;
		}
		ion::GfxDevice::Get()->SetWorldMatrix(worldMatrix);
		
		//Iterate over renderables
		uint32_t i = m_numRenderables;
		ion::Renderable* pRenderable = m_renderables;
		while(i--)
		{
			ion::Material* pMaterial = pRenderable->GetMaterial();
			GfxDevice::Get()->SetMaterial(pMaterial);

			VertexBuffer* pVertexBuffer = pRenderable->GetVertexBuffer();
			IndexBuffer* pIndexBuffer = pRenderable->GetIndexBuffer();
			pVertexBuffer->Bind();
			pIndexBuffer->Bind();
			ion::GfxDevice::Get()->DrawElements(kPrimTriangles, pIndexBuffer->GetNumIndices(), pIndexBuffer->GetType(), 0);

			++pRenderable;
		}
		
		for (uint32_t i = 0; i < m_numChildren; ++i)
		{
			m_children[i].Render(&worldMatrix);
		}
		*/
	}
