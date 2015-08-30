// GLTestAZDO.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "ScopeStackAllocator.h"
#include "Math/FMath.h"
#include "Renderer/Camera.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/Mesh.h"

#define CHECK_ERRORS

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

struct Uniforms
{
	Matrix44    modelViewMatrix;
	Matrix44	projectionMatrix;
};

Camera *g_pCamera = nullptr;
Shader *g_pShader = nullptr;
Mesh *g_pMesh = nullptr;
uint32_t g_screenWidth = 1280;
uint32_t g_screenHeight = 720;
GLuint	g_vertexBufferName;
GLuint	g_vertexArrayObject;
GLuint  g_indexBufferName;
GLint	g_mvpMatrixLocation = -1;
GLuint	g_uniformsBuffer;

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

	g_pShader = initStack.newObject<Shader>(0, "Shaders/blinnphong.vs.glsl", "Shaders/blinnphong.fs.glsl", allocator);

	g_pCamera = initStack.newObject<Camera>(90.0f, (float)g_screenWidth, (float)g_screenHeight, 0.1f, 100.f);
	g_pCamera->LookAt(Point(0.0f, 0.75f, 1.25f), Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f));
	g_pCamera->Update();

	glGenBuffers(1, &g_uniformsBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, g_uniformsBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Uniforms), NULL, GL_DYNAMIC_DRAW);


	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void Update()
{

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

	Matrix44 modelMatrix;
	modelMatrix.SetRotation(Deg2Rad((float)glfwGetTime() * 50.f), Vector(0.0f, 0.0f, 1.0f));
	Matrix44 modelViewMatrix = g_pCamera->GetViewMatrix() * modelMatrix;

	block->modelViewMatrix = modelViewMatrix;
	block->projectionMatrix = g_pCamera->GetProjectionMatrix();
	glUnmapBuffer(GL_UNIFORM_BUFFER);

	glBindVertexArray(g_pMesh->getVertexArrayObject());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_pMesh->getIndexBuffer());
	glDrawElements(GL_TRIANGLES, g_pMesh->getNumIndices(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

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

