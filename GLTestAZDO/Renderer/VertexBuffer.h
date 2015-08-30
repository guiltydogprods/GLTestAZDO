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

struct VertexElement
{
	uint8_t			m_index;
	int8_t			m_size;
	uint16_t		m_type;
	uint8_t			m_normalized;
	uint8_t			m_offset;			//CLR - is this big enough for MultiDrawIndirect?
};

struct VertexStream
{
	uint32_t		m_glBufferId;
	uint8_t			m_bufferType;
	uint8_t			m_numElements;
	uint16_t		m_stride;
	uint32_t		m_dataOffset;
	VertexElement	m_elements[kMaxVertexElems];
};

struct VertexBuffer
{
	uint32_t		m_numStreams;
	VertexStream	m_streams[kMaxStreamCount];
};
