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
	Shader	*m_pBlinnPhongShader;
	Shader	*m_pCullShader;
	Mesh	*m_pMesh;

	uint32_t m_screenWidth;
	uint32_t m_screenHeight;
	GLuint	m_vertexArrayObject;
	GLuint	m_vertexBufferName;
	GLuint  m_indexBufferName;
	GLuint	m_transformsBuffer;
	GLuint	m_modelMatricesBuffer;
	GLuint	m_materialBuffer;
	GLuint	m_parameterBuffer;
	GLuint	m_drawCandidatesBuffer;
	GLuint	m_drawCommandBuffer;
	GLuint	m_visibleMatricesBuffer;
};
