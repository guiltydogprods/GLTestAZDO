/*
 *  Mesh.h
 *  ionEngine
 *
 *  Created by Claire Rogers on 21/07/2007.
 *  Copyright 2007 Digital Frelp. All rights reserved.
 *
 */

#pragma once

#include "Math/Point.h"
#include "Math/Quat.h"
#include "Math/Matrix44.h"

#include "VertexBuffer.h"

/*
 namespace ionp
 {
 class ObjMaterial;
 };
 */

	struct CRVertexElement
	{
		uint8_t			m_index;
		int8_t			m_size;
		uint16_t		m_type;
		uint8_t			m_normalized;
		uint8_t			m_offset;
	};
	
	struct CRVertexStream
	{
		uint32_t		m_glBufferId;
		uint8_t			m_bufferType;
		uint8_t			m_numElements;
		uint16_t		m_stride;
//		void			*m_data;
		uint32_t		m_dataOffset;
		CRVertexElement	m_elements[kMaxVertexElems];
		
		CRVertexStream();
		void AddElement(uint8_t index, int8_t size, uint16_t type, uint8_t normalized, uint8_t offset);
	};
	
	struct CRVertexBuffer
	{
		//		uint32_t        m_vao;
		uint32_t		m_numStreams;
		CRVertexStream	m_streams[kMaxStreamCount];
		
		void Init(uint32_t numStreams);
	};


	//------------------------------------------------------------------------------------------
	// Renderable |
	//------------------------------------------------------------------------------------------
	class Renderable			//CLR: Do we need to split this down further a seperate Mesh and Material?
	{
	public:
		Renderable();
		~Renderable();
		
//		inline void SetMaterial(MaterialPtr pMaterial)			{	m_pMaterial = pMaterial;	}
		inline void	SetNumVertices(uint32_t num )				{	m_numVertices = num;		}
		inline void	SetNumIndices(uint32_t num )				{	m_numIndices = num;			}
//		inline void SetIndexBuffer(IndexBuffer* indexBuffer)	{	m_indexBuffer = indexBuffer;	}
		inline uint32_t	GetNumVertices()				{	return m_numVertices;		}
		inline uint32_t	GetNumIndices()					{	return m_numIndices;		}
//		inline Material* GetMaterial()					{	return m_pMaterial;			}
		inline VertexBuffer* GetVertexBuffer()			{	return m_vertexBuffer;		}
//		inline IndexBuffer*	 GetIndexBuffer()			{	return m_indexBuffer;		}
		void WriteIndices(uint32_t* indices, uint32_t size, bool keepLocal=false);
		void DispatchImmediate();
		
	private:
		uint32_t			m_numVertices;
		uint32_t			m_numIndices;
//		MaterialPtr			m_pMaterial;
		VertexBuffer*		m_vertexBuffer;
//		IndexBuffer*		m_indexBuffer;
	};
	
	class Model
	{
	public:
		Model();
		~Model();
		void Render(Matrix44* pWorldMatrix = NULL);
		inline void AddRenderables(uint32_t numRenderables, Renderable* renderables)
		{
			m_numRenderables = numRenderables;
			m_renderables = renderables;
		}
		
		inline void SetPositionAndOrientation(Point pos, Quat orientation)
		{
			Matrix44 rot = orientation.GetMatrix44();
			Matrix44 trans(kIdentity);
			trans.SetTranslate(pos);
			m_worldMatrix = trans * rot;
		}

		inline void SetPosition(Point pos)
		{
			Matrix44 trans(kIdentity);
			trans.SetTranslate(pos);
			m_worldMatrix = trans;
		}
		
		inline void SetWorldMatrix(Matrix44& worldMatrix)
		{
			m_worldMatrix = worldMatrix;
		}
						
		void AllocateChildren(uint32_t childCount);
		
		inline Matrix44&	GetWorldMatrix()				{ return m_worldMatrix;			}
		inline uint32_t		GetNumRenderables()				{ return m_numRenderables;		}
		inline Renderable*	GetRenderable(uint32_t index)	{ return &m_renderables[index];	}
		inline uint32_t		GetNumChildren()				{ return m_numChildren;			}
		inline Model*		GetChild( uint32_t child )		{ return &m_children[child];	}
//		inline Matrix44*	GetInverseBindPose()			{ return m_inverseBindPose;		}
//		inline Matrix44&	GetBone( int index )			{ return m_bonePalette[index];  }
		
		friend class MeshExporter;
		
	protected:
//		std::string			m_name;
//		HashedString		m_hash;
		Matrix44			m_worldMatrix;
				
		uint32_t			m_numRenderables;
		Renderable*			m_renderables;
		
//		uint32_t			m_numBones;
//		Matrix44*			m_inverseBindPose;
//		Matrix44*			m_bonePalette;
		
		uint32_t			m_numChildren;
		Model*				m_children;
	};

