#include "stdafx.h"
#include "GLTestAZDO.h"

const uint32_t SCREEN_WIDTH = 1920;
const uint32_t SCREEN_HEIGHT = 1080;

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

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GL Test AZDO", NULL, NULL);
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

		TestAZDOApp *pApp = initStack.newObject<TestAZDOApp>(SCREEN_WIDTH, SCREEN_HEIGHT, allocator, initStack);
		while (!glfwWindowShouldClose(window))
		{
			pApp->update();
			pApp->render();
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	_aligned_free(memoryBlock);
	memoryBlock = nullptr;

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}

