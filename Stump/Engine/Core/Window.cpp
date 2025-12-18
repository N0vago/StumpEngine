#include "Core/Window.h"

#include "Core/WindowEvents.h"
#include "Core/InputEvents.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Core {
    Window::Window(const WindowInfo& r_windowInfo) : windowInfo(r_windowInfo)
    {

    }
    Window::~Window()
    {
        Destroy();
    }

    void Window::Create()
    {
        //Setup glfw
        if (!glfwInit())
        {
            std::cout << "GLFW init failed!" << std::endl;
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        windowHandle = glfwCreateWindow(windowInfo.Width, windowInfo.Height, windowInfo.Title.c_str(), NULL, NULL);

        if (windowHandle == NULL) {

            std::cout << "Failed to create window" << std::endl;

            glfwTerminate();

            return;
        }

        glfwMakeContextCurrent(windowHandle);

        gladLoadGL();
        glfwSwapInterval(windowInfo.VSync ? 1 : 0);
        glViewport(0, 0, windowInfo.Width, windowInfo.Height);

        glfwSetWindowUserPointer(windowHandle, this);

        glfwSetWindowCloseCallback(windowHandle, [](GLFWwindow* handle)
            {
                Window& window = *((Window*)glfwGetWindowUserPointer(handle));
                WindowClosedEvent event;
                window.RaiseEvent(event);
            });

        glfwSetWindowSizeCallback(windowHandle, [](GLFWwindow* handle, int width, int height)
            {
                Window& window = *((Window*)glfwGetWindowUserPointer(handle));
                WindowResizeEvent event((uint32_t)width, (uint32_t)height);
                window.RaiseEvent(event);
            });

        glfwSetKeyCallback(windowHandle, [](GLFWwindow* handle, int key, int scancode, int action, int mods)
            {
                Window& window = *((Window*)glfwGetWindowUserPointer(handle));

                switch (action) {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, false);
                    window.RaiseEvent(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    window.RaiseEvent(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, action == GLFW_REPEAT);
                    window.RaiseEvent(event);
                }
                }
            });

        glfwSetMouseButtonCallback(windowHandle, [](GLFWwindow* handle, int button, int action, int mods)
            {
                Window& window = *((Window*)glfwGetWindowUserPointer(handle));

                switch (action) {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    window.RaiseEvent(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    window.RaiseEvent(event);
                    break;
                }
                }
            });

        glfwSetCursorPosCallback(windowHandle, [](GLFWwindow* handle, double xpos, double ypos)
            {
                Window& window = *((Window*)glfwGetWindowUserPointer(handle));
                MouseMovedEvent event(xpos, ypos);
                window.RaiseEvent(event);
            });
        glfwSetScrollCallback(windowHandle, [](GLFWwindow* handle, double xoffset, double yoffset)
            {
                Window& window = *((Window*)glfwGetWindowUserPointer(handle));
                MouseScrolledEvent event(xoffset, yoffset);
                window.RaiseEvent(event);
            });

    }

    void Window::Destroy()
    {
        if (windowHandle) {
            glfwDestroyWindow(windowHandle);
            glfwTerminate();
            windowHandle = nullptr;
        }
    }

    void Window::Update()
    {
        glfwSwapBuffers(windowHandle);
    }

    void Window::RaiseEvent(Event& r_event)
    {
        if (windowInfo.EventCallback)
            windowInfo.EventCallback(r_event);
    }

    Vector2 Window::GetFrameBufferSize() const
    {
        int width, height;
        glfwGetFramebufferSize(windowHandle, &width, &height);
        return Vector2((float)width, (float)height);
    }

    Vector2 Window::GetMousePos() const
    {
        double xpos, ypos;
        glfwGetCursorPos(windowHandle, &xpos, &ypos);
        return Vector2((float)xpos, (float)ypos);
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(windowHandle);
    }
}