/*
 *  ScopeStackAllocator.cpp
 *  ion::Engine
 *
 *  Created by Claire Rogers on 01/07/2013.
 *  Copyright 2013 Guilty Dog Productions Ltd. All rights reserved.
 *
 */

#pragma once

#include "stdafx.h"
#include <new>
#include <cstdio>
#include <cassert>
#include <utility>
#include "System/System.h"

class LinearAllocator {
public:
	static const size_t Alignment = 16;

	LinearAllocator()
	: m_ptr(nullptr)
	, m_end(nullptr)
	{
	}

	LinearAllocator(void *ptr_, size_t size)
	: m_ptr((uint8_t*)ptr_)
	, m_end(m_ptr + size)
	{
	}

	void initialize(void *ptr, size_t size)
	{
		m_ptr = (uint8_t *)ptr;
		m_end = m_ptr + size;
	}

	static size_t alignedSize(size_t size) 
	{
		return (size + (Alignment - 1)) & ~(Alignment - 1);
	}

	void* allocate(size_t size) 
	{
		size = alignedSize(size);
		uint8_t *result = m_ptr;
		m_ptr = m_ptr + size;
//		AssertMsg((result + size) <= m_end, "Out of memory.");
		return result;
	}

	void *ptr() { return m_ptr; }

	void rewind(void *ptr) 
	{
		m_ptr = (uint8_t*)ptr;
	}

private:
	uint8_t *m_ptr, *m_end;
};

struct Finalizer 
{
	void(*fn)(void *ptr);
	Finalizer *chain;
};

template <typename T>
void destructorCall(void *ptr) 
{
	static_cast<T*>(ptr)->~T();
}

class ScopeStack 
{
private:
	LinearAllocator& m_alloc;
	void *m_rewindPoint;
	Finalizer *m_finalizerChain;

	static void* objectFromFinalizer(Finalizer *f) 
	{
		return ((uint8_t*)f) + LinearAllocator::alignedSize(sizeof(Finalizer));
	}

	Finalizer *allocWithFinalizer(size_t size)
	{
		size_t alignedFinalizerSize = LinearAllocator::alignedSize(sizeof(Finalizer));
		return (Finalizer*)m_alloc.allocate(size + LinearAllocator::alignedSize(sizeof(Finalizer)));
	}

public:
	explicit ScopeStack(LinearAllocator& a)
		: m_alloc(a)
		, m_rewindPoint(a.ptr())
		, m_finalizerChain(0)
	{}

	~ScopeStack() 
	{
		for (Finalizer *f = m_finalizerChain; f; f = f->chain)
		{
			(*f->fn)(objectFromFinalizer(f));
		}
		m_alloc.rewind(m_rewindPoint);
	}

	template <typename T, typename... Args>
	T* newObject(Args&&... params) 
	{
		// Allocate memory for finalizer + object.
		Finalizer* f = allocWithFinalizer(sizeof(T));

		// Placement construct object in space after finalizer. Do this before
		// linking in the finalizer for this object so nested calls will be
		// finalized after this object.
		void *allocAddress = objectFromFinalizer(f);
		T* result = new (objectFromFinalizer(f)) T(std::forward<Args>(params)...);

		// Link this finalizer onto the chain.
		f->fn = &destructorCall<T>;
		f->chain = m_finalizerChain;
		m_finalizerChain = f;
		return result;
	}

	template <typename T>
	T* newPOD() 
	{
		return new (m_alloc.allocate(sizeof(T))) T;
	}

	void *alloc(size_t size)
	{
		return m_alloc.allocate(size);
	}
};
