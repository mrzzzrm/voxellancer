

#include <iostream>
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glow/logging.h>
#include <glow/global.h>
#include <glow/ShaderFile.h>

#include "property/propertymanager.h"

#include "game.h"

static GLint MajorVersionRequire = 3;
static GLint MinorVersionRequire = 1;

static Game * game;

using namespace std;

static void checkVersion() {
    glow::info("OpenGL Version Needed %;.%; (%;.%; Found)",
        MajorVersionRequire, MinorVersionRequire,
        glow::query::majorVersion(), glow::query::minorVersion());
    glow::info("version %;", glow::query::version().toString());
    glow::info("vendor: %;", glow::query::vendor());
    glow::info("renderer %;", glow::query::renderer());
    glow::info("core profile: %;", glow::query::isCoreProfile() ? "true" : "false");
    glow::info("GLSL version: %;\n", glow::query::getString(GL_SHADING_LANGUAGE_VERSION));
}

static void errorCallback(int error, const char* description)
{
    glow::warning(description);
}

static void resizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    game->m_inputHandler->resizeEvent(width, height);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
		game->m_inputHandler->toggleControls();
	if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
		glow::ShaderFile::reloadAll();
	if (key == GLFW_KEY_F6 && action == GLFW_PRESS)
		game->reloadConfig();
}

static void mouseButtonCallback(GLFWwindow* window, int Button, int Action, int mods) {

}

static void cursorPositionCallback(GLFWwindow* window, double x, double y) {

}

void setCallbacks(GLFWwindow* window)
{
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowSizeCallback(window, resizeCallback);
}


int main(void)
{
	GLFWwindow* window = nullptr;
   // PropertyManager::getInstance()->load("data/global.ini");

	if (!glfwInit()) {
		glow::fatal("could not init glfw");
		exit(-1);
	}

	glfwSetErrorCallback(errorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MajorVersionRequire);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MinorVersionRequire);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#if defined(NDEBUG)
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_FALSE);
#else
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

    window = glfwCreateWindow(1280, 720, "Voxellancer", NULL, NULL);
	if (!window) {
		glfwTerminate();
		glow::fatal("could not create window");
		exit(-1);
	}

	glfwMakeContextCurrent(window);

	setCallbacks(window);

	checkVersion();

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		glow::fatal("glewInit() failed");
	}
	glGetError();

#ifdef WIN32 // TODO: find a way to correctly detect debug extension in linux
	glow::DebugMessageOutput::enable();
#endif

#ifdef WIN32
	wglSwapIntervalEXT(1); // glfw doesn't work!?
#else
	glfwSwapInterval(1);
#endif
	try {
		game = new Game(window);
		game->initialize();

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		game->m_inputHandler->resizeEvent(width, height);

		glow::debug("Entering mainloop");
		double time = glfwGetTime();
		while (!glfwWindowShouldClose(window))
		{
			double delta = glfwGetTime() - time;
			time += delta;
			game->update(static_cast<float>(delta));
			game->draw();
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		delete game;
		glfwDestroyWindow(window);
		glfwTerminate();

	}
	catch (std::exception &e){
		glfwDestroyWindow(window);
		glfwTerminate();
        glow::fatal("Termination after Exception: %;", e.what());
		cout << "Hit enter to quit" << endl;
		cin.ignore(1, '\n');
	}


    return 0;
}

