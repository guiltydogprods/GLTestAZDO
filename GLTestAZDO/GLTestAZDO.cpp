#include "stdafx.h"
#include "ScopeStackAllocator.h"
#include "Math/FMath.h"
#include "Renderer/Camera.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/Mesh.h"

#define CHECK_ERRORS

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

const uint32_t g_screenWidth = 1280;
const uint32_t g_screenHeight = 720;
const uint32_t kNumX = 20;
const uint32_t kNumZ = 20;
const uint32_t kNumY = 20;
const uint32_t kNumDraws = kNumX * kNumZ * kNumY;

struct Uniforms
{
	Matrix44    viewMatrix;
	Matrix44	projectionMatrix;
};

struct Transforms
{
	Matrix44	modelMatrices[kNumDraws];
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

Camera *g_pCamera = nullptr;
Shader *g_pShader = nullptr;
Shader *g_pComputeShader = nullptr;
Mesh *g_pMesh = nullptr;
GLuint	g_vertexBufferName;
GLuint	g_vertexArrayObject;
GLuint  g_indexBufferName;
GLint	g_mvpMatrixLocation = -1;
GLuint	g_uniformsBuffer;
GLuint	g_shaderStorageBuffer;
GLuint	g_indirectDrawBuffer;
GLuint	g_materialBuffer;

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

void Initialize(LinearAllocator& allocator, ScopeStack& initStack)
{
	g_pMesh = initStack.newObject<Mesh>("assets/Donut.s3d", allocator);

	g_pShader = initStack.newObject<Shader>("Shaders/blinnphong.vs.glsl", "Shaders/blinnphong.fs.glsl", allocator);
	g_pComputeShader = initStack.newObject<Shader>("Shaders/cull.cs.glsl", allocator);

	g_pCamera = initStack.newObject<Camera>(90.0f, (float)g_screenWidth, (float)g_screenHeight, 0.1f, 100.f);
	g_pCamera->LookAt(Point(0.0f, 0.0f, 9.0f + (float(kNumZ-1) / 2.0f)), Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f));
	g_pCamera->Update();

	glGenBuffers(1, &g_uniformsBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, g_uniformsBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Uniforms), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &g_shaderStorageBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_shaderStorageBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Transforms), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &g_indirectDrawBuffer);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, g_indirectDrawBuffer);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, kNumDraws * sizeof(DrawElementsIndirectCommand), NULL, GL_STATIC_DRAW);
	DrawElementsIndirectCommand *cmd = (DrawElementsIndirectCommand *)glMapBufferRange(GL_DRAW_INDIRECT_BUFFER, 0, kNumDraws * sizeof(DrawElementsIndirectCommand), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	for (uint32_t i = 0; i < kNumDraws; ++i)
	{
		cmd[i].count = g_pMesh->getNumIndices();
		cmd[i].instanceCount = 1;
		cmd[i].firstIndex = 0;
		cmd[i].baseVertex = 0;
		cmd[i].baseInstance = i;
	}
	glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);

	glGenBuffers(1, &g_materialBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_materialBuffer);
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

void Update()
{
	static float angle = 0.0f;
	float sina = sinf(Deg2Rad(angle));
	float cosa = cosf(Deg2Rad(angle));
	float radius = 4.5f + (float)(kNumZ - 1) / 2.0f;
	g_pCamera->LookAt(Point(sina * radius, -sina * radius, cosa * radius), Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f));
	g_pCamera->Update();
	angle += 0.25f;
	if (angle >= 360.0f)
		angle -= 360.0f;
}

void Render(GLFWwindow *window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(g_pShader->GetProgram());

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, g_uniformsBuffer);
	Uniforms *block = (Uniforms *)glMapBufferRange(GL_UNIFORM_BUFFER,
		0,
		sizeof(Uniforms),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	block->viewMatrix = g_pCamera->GetViewMatrix();
	block->projectionMatrix = g_pCamera->GetProjectionMatrix();
	glUnmapBuffer(GL_UNIFORM_BUFFER);

	Matrix44 modelMatrix;
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, g_shaderStorageBuffer);
	Transforms *transformsBlock = (Transforms *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER,
		0,
		sizeof(Transforms),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

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

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, g_materialBuffer);

	glBindVertexArray(g_pMesh->getVertexArrayObject());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_pMesh->getIndexBuffer());

	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, g_indirectDrawBuffer);

	glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, BUFFER_OFFSET(0), kNumDraws, 0);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwSetErrorCallback(error_callback);

	GLFWwindow* window = glfwCreateWindow(g_screenWidth, g_screenHeight, "GL Test AZDO", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Vendor: %s\n", glGetString(GL_VENDOR));
	fprintf(stdout, "Renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, "Version: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "Extensions:\n");
	GLint n, i;
	glGetIntegerv(GL_NUM_EXTENSIONS, &n);
	for (i = 0; i < n; i++)
	{
		fprintf(stdout, "%s\n", glGetStringi(GL_EXTENSIONS, i));
	}

	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	glfwSetKeyCallback(window, key_callback);

	size_t memorySize = 1024 * 1024 * 32;
	uint8_t *memoryBlock = static_cast<uint8_t *>(_aligned_malloc(memorySize, 32));
	{
		LinearAllocator allocator(memoryBlock, memorySize);
		ScopeStack initStack(allocator);

		Initialize(allocator, initStack);
		while (!glfwWindowShouldClose(window))
		{
			Update();
			Render(window);
		}
	}

	_aligned_free(memoryBlock);
	memoryBlock = nullptr;

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}

