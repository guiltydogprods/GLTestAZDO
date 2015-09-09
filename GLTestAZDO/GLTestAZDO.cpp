#include "stdafx.h"
#include "GLTestAZDO.h"
#include "Math/FMath.h"
#include "Renderer/Camera.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/Mesh.h"

#define CHECK_ERRORS

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

const uint32_t kNumX = 20;	// 20;
const uint32_t kNumZ = 20;	// 20;
const uint32_t kNumY = 20;	// 20;
const uint32_t kNumDraws = kNumX * kNumZ * kNumY;

struct Uniforms
{
	Matrix44    viewMatrix;
	Matrix44	projectionMatrix;
	Matrix44	viewProjMatrix;
};

struct Transforms
{
	Matrix44	modelMatrices[kNumDraws];
};

struct CandidateDraw
{
	Vector4		sphere;			//x, y & z = center w = radius
	uint32_t	firstIndex;
	uint32_t	indexCount;
	uint32_t : 32;
	uint32_t : 32;
};

struct DrawElementsIndirectCommand
{
	uint32_t	count;
	uint32_t	instanceCount;
	uint32_t	firstIndex;
	uint32_t	baseVertex;
	uint32_t	baseInstance;
};

struct MaterialProperties
{
	float diffuse[4];
	float specular[3];
	float specularPower;
};

struct errtable
{
	GLenum err;
	const char* string;
	const char* description;
};

errtable glErrorTable[] =
{
	{ GL_INVALID_ENUM,						"GL_INVALID_ENUM", " given when an enumeration parameter contains an enum that is not allowed for that function.", },
	{ GL_INVALID_VALUE,						"GL_INVALID_VALUE", " given when a numerical parameter does not conform to the range requirements that the function places upon it.", },
	{ GL_INVALID_OPERATION,					"GL_INVALID_OPERATION", " given when the function in question cannot be executed because of state that has been set in the context.", },
	{ GL_STACK_OVERFLOW,					"GL_STACK_OVERFLOW",	" given when a stack pushing operation causes a stack to overflow the limit of that stack's size.",},
	{ GL_STACK_UNDERFLOW,					"GL_STACK_UNDERFLOW",	" given when a stack popping operation is given when the stack is already at its lowest point.",},
	{ GL_OUT_OF_MEMORY,						"GL_OUT_OF_MEMORY", " given when performing an operation that can allocate memory, when the memory in question cannot be allocated.", },
	{ GL_TABLE_TOO_LARGE,					"GL_TABLE_TOO_LARGE",	" if the optional imaging subset (GL_ARB_imaging) is supported)",},
	{ GL_INVALID_FRAMEBUFFER_OPERATION ,	"GL_INVALID_FRAMEBUFFER_OPERATION","GL_INVALID_FRAMEBUFFER_OPERATION",},
};

const char *GLFindError(int32_t errcode)
{
	const int32_t arraysize = 4;	//Z_ARRAY_SIZE(glErrorTable);
	for (int32_t i = 0; i < arraysize; i++)
	{
		if (errcode == glErrorTable[i].err)
			return glErrorTable[i].string;
	}
	return "unknown error";
}

void CheckGLError()
{
#if defined(CHECK_ERRORS)	
	GLenum err;
	err = glGetError();
	if (err != GL_NO_ERROR)
	{
		fprintf(stderr, "GL Error: %s\n", GLFindError(err));
	}
#endif
}

TestAZDOApp::TestAZDOApp(uint32_t screenWidth, uint32_t screenHeight, LinearAllocator &allocator, ScopeStack &initStack)
: m_pCamera(nullptr)
, m_pShader(nullptr)
, m_pComputeShader(nullptr)
, m_pMesh(nullptr)
, m_screenWidth(screenWidth)
, m_screenHeight(screenHeight)
{
	m_pMesh = initStack.newObject<Mesh>("assets/Donut.s3d", allocator);

	m_pShader = initStack.newObject<Shader>("Shaders/blinnphong.vs.glsl", "Shaders/blinnphong.fs.glsl", allocator);
	m_pComputeShader = initStack.newObject<Shader>("Shaders/cull.cs.glsl", allocator);

	m_pCamera = initStack.newObject<Camera>(90.0f, (float)screenWidth, (float)screenHeight, 0.1f, 100.f);
	m_pCamera->LookAt(Point(0.0f, 0.0f, 9.0f + (float(kNumZ - 1) / 2.0f)), Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f));
	m_pCamera->Update();

	glGenBuffers(1, &m_uniformsBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uniformsBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Uniforms), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &m_modelMatrixBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_modelMatrixBuffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(Transforms), nullptr, GL_MAP_WRITE_BIT);

	glGenBuffers(1, &m_parameterBuffer);
	glBindBuffer(GL_PARAMETER_BUFFER_ARB, m_parameterBuffer);
	glBufferStorage(GL_PARAMETER_BUFFER_ARB, 256, nullptr, GL_MAP_READ_BIT);

	glGenBuffers(1, &m_drawCandidatesBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_drawCandidatesBuffer);

	{
		ScopeStack tempScopeStack(allocator);
		CandidateDraw *pDraws = static_cast<CandidateDraw *>(tempScopeStack.alloc(sizeof(CandidateDraw) * kNumDraws));

		for (uint32_t i = 0; i < kNumDraws; ++i)
		{
			pDraws[i].sphere = Vector4(0.0f, 0.0f, 0.0f, 0.5f);
			pDraws[i].firstIndex = 0;
			pDraws[i].indexCount = m_pMesh->getNumIndices();			
		}
		glBufferStorage(GL_SHADER_STORAGE_BUFFER, kNumDraws * sizeof(CandidateDraw), pDraws, 0);
	}

	glGenBuffers(1, &m_drawCommandBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_drawCommandBuffer);
	glBufferStorage(GL_SHADER_STORAGE_BUFFER, kNumDraws * sizeof(DrawElementsIndirectCommand), nullptr, GL_MAP_READ_BIT);

	glGenBuffers(1, &m_materialBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_materialBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, kNumDraws * sizeof(MaterialProperties), NULL, GL_STATIC_DRAW);
	MaterialProperties *materials = (MaterialProperties *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, kNumDraws * sizeof(MaterialProperties), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	uint32_t materialIndex = 0;
	float green = 0.0f;
	float deltaGreen = 1.0f / (float)kNumY;
	for (uint32_t y = 0; y < kNumY; ++y)
	{
		float blue = 0.0f;
		float deltaBlue = 1.0f / (float)kNumZ;
		for (uint32_t z = 0; z < kNumZ; ++z)
		{
			for (uint32_t x = 0; x < kNumX; ++x)
			{
				materials[materialIndex].diffuse[0] = 1.0f;
				materials[materialIndex].diffuse[1] = green;
				materials[materialIndex].diffuse[2] = blue;;
				materials[materialIndex].specular[0] = materials[materialIndex].specular[1] = materials[materialIndex].specular[2] = 0.7f;
				materials[materialIndex].specularPower = 5.0f + 10.0f * x;
				materialIndex++;
			}
			blue += deltaBlue;
		}
		green += deltaGreen;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

TestAZDOApp::~TestAZDOApp()
{
	printf("TestAZDOApp dtor\n");
}

void TestAZDOApp::Update()
{
	static float angle = 0.0f;
	float sina = sinf(Deg2Rad(angle));
	float cosa = cosf(Deg2Rad(angle));
	float radius = 4.5f + (float)(kNumZ - 1) / 2.0f;
	m_pCamera->LookAt(Point(sina * radius, -sina * radius, cosa * radius), Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f));
	m_pCamera->Update();
	angle += 0.25f;
	if (angle >= 360.0f)
		angle -= 360.0f;
}

void TestAZDOApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_screenWidth, m_screenHeight);

	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, m_parameterBuffer);
	glClearBufferSubData(GL_ATOMIC_COUNTER_BUFFER, GL_R32UI, 0, sizeof(GLuint), GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);

	// Bind shader storage buffers
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_drawCandidatesBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_drawCommandBuffer);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uniformsBuffer);
	Uniforms *block = (Uniforms *)glMapBufferRange(GL_UNIFORM_BUFFER,
		0,
		sizeof(Uniforms),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	block->viewMatrix = m_pCamera->GetViewMatrix();
	block->projectionMatrix = m_pCamera->GetProjectionMatrix();
	block->viewProjMatrix = m_pCamera->GetProjectionMatrix() * m_pCamera->GetViewMatrix();
	glUnmapBuffer(GL_UNIFORM_BUFFER);

	Matrix44 modelMatrix;
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_modelMatrixBuffer);
	Transforms *transformsBlock = (Transforms *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Transforms), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	uint32_t modelIndex = 0;
	float startY = -(float)(kNumY - 1) / 2.0f;
	for (uint32_t y = 0; y < kNumY; ++y)
	{
		float baseRotation = (float)glfwGetTime() * 50.0f;
		float startZ = (float)(kNumZ - 1) / 2.0f;
		for (uint32_t z = 0; z < kNumZ; ++z)
		{
			modelMatrix.SetRotation(Deg2Rad(baseRotation + (360.0f / (float)kNumZ) * (float)z), Vector(0.0f, 0.0f, 1.0f));
			float startX = -(float)(kNumX - 1) / 2.0f;
			for (uint32_t x = 0; x < kNumX; ++x)
			{
				transformsBlock->modelMatrices[modelIndex] = modelMatrix;
				transformsBlock->modelMatrices[modelIndex].SetTranslate(Point(startX + x, startY + y, startZ - z));
				modelIndex++;
			}
		}
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	glUseProgram(m_pComputeShader->GetProgram());
	glDispatchCompute(kNumDraws / 16, 1, 1);

	glMemoryBarrier(GL_COMMAND_BARRIER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_materialBuffer);

	glBindVertexArray(m_pMesh->getVertexArrayObject());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pMesh->getIndexBuffer());

	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_drawCommandBuffer);
	glBindBuffer(GL_PARAMETER_BUFFER_ARB, m_parameterBuffer);

	/*
	uint32_t drawCount = 0;
	uint32_t *countPtr = (uint32_t *)glMapNamedBufferRange(m_parameterBuffer, 0, sizeof(uint32_t), GL_MAP_READ_BIT);
	if (countPtr)
	{
		drawCount = *countPtr;
	}
	glUnmapNamedBuffer(m_parameterBuffer);
	printf("Draw Count = %d\n", drawCount);
	*/

	glUseProgram(m_pShader->GetProgram());

	glMultiDrawElementsIndirectCountARB(GL_TRIANGLES, GL_UNSIGNED_INT, 0, 0, kNumDraws, 0);


}

