// GLTestAZDO.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "ScopeStackAllocator.h"
#include "FIle.h"
void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

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

int _tmain(int argc, _TCHAR* argv[])
{

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

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwSetErrorCallback(error_callback);

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "GL Test AZDO", NULL, NULL);
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

	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-0.6f, -0.4f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.6f, -0.4f, 0.f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.6f, 0.f);
		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}

