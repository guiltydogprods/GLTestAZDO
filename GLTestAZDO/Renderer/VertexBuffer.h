/*
 *  VertexBuffer.h
 *  ionEngine
 *
 *  Created by Claire Rogers on 01/07/2013.
 *  Copyright 2013 Guilty Dog Productions Ltd. All rights reserved.
 *
 */

#pragma once

static const uint32_t kMaxVertexElems = 16;
static const uint32_t kMaxStreamCount = 2;

class VertexElement
{
public:
	uint8_t		m_index;
	int8_t		m_size;
	uint16_t	m_type;
	uint8_t		m_normalized;
	uint32_t	m_offset;
};

class VertexStream
{
public:
	VertexStream();
	~VertexStream();
		
	void WriteData(void* data, size_t size);
		
	uint32_t		m_numElements;
	void*			m_data;
	uint32_t		m_stride;
	VertexElement	m_elements[kMaxVertexElems];
};
	
class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();
		
	void SetNumStreams(uint32_t numStreams);	//			{	m_numStreams = numStreams;	}
	void SetNumVertices(uint32_t numVertices);
//	void AddElement(uint32_t stream, VertexElement& elem);
	void AddElement(uint32_t stream, VertexElement& elem);

	inline uint32_t			GetNumStreams()			{	return m_numStreams;	}
	inline VertexStream*	GetVertexStreams()		{	return m_vertexStreams;	}
	inline uint32_t			GetNumVertices()		{	return m_numVertices;	}
	void SetData(uint32_t stream, VertexElement& elem, const void* data, uint32_t srcStride);
	void SetData(uint32_t stream, void* data);
		
	void Write();
	void Bind();
	
//		virtual uint32_t	GetVertexArrayObject() = 0;
//		virtual uint32_t	GetVertexBuffer(uint32_t index) = 0;
		
protected:
	uint32_t		m_numVertices;
	uint32_t		m_numStreams;
	VertexStream	m_vertexStreams[kMaxStreamCount];
	uint32_t		m_glVertexArrayObject;
	uint32_t		m_glVertexBuffers[kMaxStreamCount];
};
