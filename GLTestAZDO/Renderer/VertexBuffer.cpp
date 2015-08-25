/*
 *  VertexBuffer.c
 *  ionEngine
 *
 *  Created by Claire Rogers on 01/07/2013.
 *  Copyright 2013 Guilty Dog Productions Ltd. All rights reserved.
 *
 */
#include "stdafx.h"
#include "VertexBuffer.h"

VertexStream::VertexStream()
:
m_numElements(0),
m_stride(0)
{
}
	
VertexStream::~VertexStream()
{
}
	
		
VertexBuffer::VertexBuffer()
:
m_numVertices(0), m_numStreams(0)
{
}
	
VertexBuffer::~VertexBuffer()
{
		
}

void VertexBuffer::AddElement(uint32_t stream, VertexElement& elem)
{
	VertexStream* buffer = &m_vertexStreams[stream];
	uint32_t index = buffer->m_numElements++;
	buffer->m_elements[index].m_index = elem.m_index;
	buffer->m_elements[index].m_size = elem.m_size;
	buffer->m_elements[index].m_type = elem.m_type;
	buffer->m_elements[index].m_normalized = elem.m_normalized;
	buffer->m_elements[index].m_offset = elem.m_offset;
	buffer->m_stride += (sizeof(float) * elem.m_size);	//CLR: Todo - Do a proper size calculation based on type.
}

void VertexBuffer::SetNumStreams(uint32_t numStreams)
{
	m_numStreams = numStreams;
	glGenBuffers(m_numStreams, m_glVertexBuffers);
	for (uint32_t i = 0; i < m_numStreams; ++i)
	{
		m_vertexStreams[i].m_numElements = 0;
		m_vertexStreams[i].m_stride = 0;
	}
}

void VertexBuffer::SetNumVertices(uint32_t numVertices)
{
	m_numVertices = numVertices;
	for (uint32_t i = 0; i < m_numStreams; ++i)
	{
		VertexStream* buffer = &m_vertexStreams[i];
		buffer->m_data = new uint8_t[buffer->m_stride*numVertices];
	}
}

void VertexBuffer::SetData(uint32_t stream, VertexElement& elem, const void* data, uint32_t srcStride)
{
	VertexStream* buffer = &m_vertexStreams[stream];
	uint32_t index = elem.m_index;

		//		printf("WriteData: element %d offset %d\n", index, buffer->m_elements[index].m_offset);
	uint8_t* srcPtr = (uint8_t*)data;
	uint8_t* dstPtr = (uint8_t*)buffer->m_data + buffer->m_elements[index].m_offset;
	uint32_t dstStride = buffer->m_stride;
	for (uint32_t i = 0; i < m_numVertices; ++i)
	{
		memcpy(dstPtr, srcPtr, srcStride);
		srcPtr += srcStride;
		dstPtr += dstStride;
	}
}

void VertexBuffer::SetData(uint32_t stream, void* data)
{
	VertexStream* buffer = &m_vertexStreams[stream];
	memcpy(buffer->m_data, data, m_numVertices*buffer->m_stride);
}

void VertexBuffer::Write()
{
	for (int i = 0; i < m_numStreams; ++i)
	{
		VertexStream* buffer = &m_vertexStreams[i];
		glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffers[i]);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices*buffer->m_stride, buffer->m_data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
//			GfxDevice::Get()->BindBuffer(kArrayBuffer, m_glVertexBuffers[i]);
	//		GfxDevice::Get()->BufferData(kArrayBuffer, m_numVertices*buffer->m_stride, buffer->m_data, kStaticDraw);
		//	GfxDevice::Get()->BindBuffer(kArrayBuffer, 0);
	}

	glGenVertexArrays(1, &m_glVertexArrayObject);
	glBindVertexArray(m_glVertexArrayObject);
//	GfxDevice::Get()->GenVertexArrays(1, &m_glVertexArrayObject);
//	GfxDevice::Get()->BindVertexArray(m_glVertexArrayObject);
	for (int i = 0; i < m_numStreams; ++i)
	{
		VertexStream* buffer = &m_vertexStreams[i];
		glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffers[i]);
//		GfxDevice::Get()->BindBuffer(kArrayBuffer, m_glVertexBuffers[i]);
		VertexElement* elem = buffer->m_elements;
		for (uint32_t j = 0; j < buffer->m_numElements; ++j, ++elem)
		{
//			GfxDevice::Get()->SetVertexAttribPointer(elem, buffer->m_stride);
//			GfxDevice::Get()->EnableVertexAttribArray(elem->m_index);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		GfxDevice::Get()->BindBuffer(kArrayBuffer, 0);
	}
	glBindVertexArray(m_glVertexArrayObject);
	//	GfxDevice::Get()->BindVertexArray(0);
}

void VertexBuffer::Bind()
{
	glBindVertexArray(m_glVertexArrayObject);
}
