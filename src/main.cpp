#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "gui.h"
#include <vector>

std::set<int> keys;

void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		keys.insert(key);
	}
}

void glfw_error_callback(int error, const char *description)
{
    std::cout << "error code: " << error << " " << description << std::endl;
}

int main(int argv, char *argc[])
{
    GLFWwindow *glfwWindow = nullptr;

    glfwInit();
    glfwSetErrorCallback(glfw_error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindow = glfwCreateWindow(640, 480, "Wave", nullptr, nullptr);

    glfwMakeContextCurrent(glfwWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    ImGui_ImplGlfwGL3_Init(glfwWindow, true);
	glfwSetKeyCallback(glfwWindow, glfw_key_callback);

    while (!glfwWindowShouldClose(glfwWindow))
	{
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();


		gui_main(glfwWindow, keys);
		keys.clear();

        glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        glfwSwapBuffers(glfwWindow);
    }

    glfwTerminate();
}