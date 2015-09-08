#pragma once

class Camera;
class Shader;
class Mesh;

class TestAZDOApp
{
public:
	TestAZDOApp(uint32_t screenWidth, uint32_t screenHeight, LinearAllocator &allocator, ScopeStack &initStack);
	~TestAZDOApp();

	void Update();
	void Render();
private:
	Camera	*m_pCamera;
	Shader	*m_pShader;
	Shader	*m_pComputeShader;
	Mesh	*m_pMesh;

	GLuint	m_vertexArrayObject;
	GLuint	m_vertexBufferName;
	GLuint  m_indexBufferName;
	GLuint	m_uniformsBuffer;
	GLuint	m_modelMatrixBuffer;
//	GLuint	m_indirectDrawBuffer;
	GLuint	m_materialBuffer;
	GLuint	m_parameterBuffer;
	GLuint	m_drawCandidatesBuffer;
	GLuint	m_drawCommandBuffer;
};
