// GLTestAZDO.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "ScopeStackAllocator.h"
#include "Math/FMath.h"
#include "Renderer/Camera.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

struct Uniforms
{
	Matrix44     mvp_matrix;
//	vmath::mat4     view_matrix;
//	vmath::mat4     proj_matrix;
};

GLuint	g_vertexBufferName;
GLuint	g_vertexArrayObject;
GLuint  g_indexBufferName;
GLint	g_mvpMatrixLocation = -1;
GLuint	g_uniformsBuffer;

Camera *g_pCamera = nullptr;
Shader *g_pShader = nullptr;
uint32_t g_screenWidth = 1280;
uint32_t g_screenHeight = 720;

struct errtable
{
	GLenum err;
	const char* string;
	const char* description;
};

errtable glErrorTable[] =
{
	{ GL_INVALID_ENUM, "GL_INVALID_ENUM", " given when an enumeration parameter contains an enum that is not allowed for that function.", },
	{ GL_INVALID_VALUE, "GL_INVALID_VALUE", " given when a numerical parameter does not conform to the range requirements that the function places upon it.", },
	{ GL_INVALID_OPERATION, "GL_INVALID_OPERATION", " given when the function in question cannot be executed because of state that has been set in the context.", },
	//	{	GL_STACK_OVERFLOW,					"GL_STACK_OVERFLOW",	" given when a stack pushing operation causes a stack to overflow the limit of that stack's size.",},
	//	{	GL_STACK_UNDERFLOW,					"GL_STACK_UNDERFLOW",	" given when a stack popping operation is given when the stack is already at its lowest point.",},
	{ GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY", " given when performing an operation that can allocate memory, when the memory in question cannot be allocated.", },
	//	{	GL_TABLE_TOO_LARGE,					"GL_TABLE_TOO_LARGE",	"if the optional imaging subset (GL_ARB_imaging) is supported)",},
	//	{	GL_INVALID_FRAMEBUFFER_OPERATION ,	"GL_INVALID_FRAMEBUFFER_OPERATION","GL_INVALID_FRAMEBUFFER_OPERATION",},
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
#if 0		
	GLenum err;
	err = glGetError();
	if (err != GL_NO_ERROR)
	{
		fprintf(stderr, "GL Error: %s\n", GLFindError(err));
	}
#endif
}

void Initialize()
{
	struct Vertex
	{
		float		position[3];
		float		normal[3];
		float		color[4];
//		uint32_t	color;
	};

	Vertex vertices[] =
	{
		-1.0f, -1.0f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f, //*/0xff0000ff,
		1.0f, -1.0f, 0.0f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f, //*/0x00ff00ff,
		-1.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f, //*/0x0000ffff,
		1.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f, 1.0f, 1.0f, //*/0xffffffff,
	};

	uint32_t indices[] =
	{
		0, 1, 2, 2, 1, 3,
	};

	glGenBuffers(1, &g_vertexBufferName);
	glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferName);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &g_vertexArrayObject);
	glBindVertexArray(g_vertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferName);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(12));
	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(2, 1, GL_UNSIGNED_INT_8_8_8_8, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(24));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(24));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &g_indexBufferName);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indexBufferName);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	g_pShader = new Shader(0, "Shaders/blinnphong.vs.glsl", "Shaders/blinnphong.fs.glsl");
	CheckGLError();
	glGenBuffers(1, &g_uniformsBuffer);
	CheckGLError();
	glBindBuffer(GL_UNIFORM_BUFFER, g_uniformsBuffer);
	CheckGLError();
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Uniforms), NULL, GL_DYNAMIC_DRAW);
	CheckGLError();
//	g_mvpMatrixLocation = glGetUniformLocation(g_pShader->GetProgram(), "mvp_matrix");
//	CheckGLError();
	//	m_parameters[kParamMVPMatrix] = ion::GfxDevice::Get()->GetUniformLocation(m_glslProgram, "g_mTransform");

	g_pCamera = new Camera(90.0f, (float)g_screenWidth, (float)g_screenHeight, 0.1f, 100.f);
	g_pCamera->LookAt(Point(0.0f, 0.0f, 5.0f), Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f));
	g_pCamera->Update();

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void Update()
{

}

void Render(GLFWwindow *window)
{
//	glMatrixMode(GL_PROJECTION);
//	glLoadMatrixf((float*)&g_pCamera->GetProjectionMatrix());
//	glMatrixMode(GL_MODELVIEW);
	Matrix44 modelMatrix;
	modelMatrix.SetRotation(Deg2Rad((float)glfwGetTime() * 50.f), Vector(0.0f, 0.0f, 1.0f));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(g_pShader->GetProgram());
	CheckGLError();
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, g_uniformsBuffer);
	CheckGLError();
	Uniforms *block = (Uniforms *)glMapBufferRange(GL_UNIFORM_BUFFER,
		0,
		sizeof(Uniforms),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	CheckGLError();

	Matrix44 modelViewMatrix = g_pCamera->GetViewMatrix() * modelMatrix;

	block->mvp_matrix = g_pCamera->GetProjectionMatrix() * g_pCamera->GetViewMatrix() * modelMatrix;
	glUnmapBuffer(GL_UNIFORM_BUFFER);
	CheckGLError();

	glBindVertexArray(g_vertexArrayObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indexBufferName);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

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

/*
struct Foo
{
	static int count;
	int num;
	Foo() : num(count++) { printf("Foo ctor %d\n", num); }
	~Foo() { printf("Foo dtor %d\n", num); }
};
int Foo::count;
uint32_t File::ms_count = 0;

uint8_t test_mem[65536];
*/
int _tmain(int argc, _TCHAR* argv[])
{
	/*
	LinearAllocator allocator(test_mem, sizeof(test_mem));

	{
		ScopeStack outerScope(allocator);
		Foo* foo0 = outerScope.newObject<Foo>();
		{
			ScopeStack innerScope(allocator);
			Foo* foo1 = innerScope.newObject<Foo>();
			Foo* foo2 = innerScope.newObject<Foo>();
			File *data1 = innerScope.newObject<File>(innerScope, "data.txt", "rb");
		}
	}
	ScopeStack initScope(allocator);
*/
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

	Initialize();

	glfwSetKeyCallback(window, key_callback);

	while (!glfwWindowShouldClose(window))
	{
		Update();
		Render(window);
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}

