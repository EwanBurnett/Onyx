#include "../../include/Onyx/Graphics/Window.h"
#include "../../include/Onyx/Utility/Logger.h"

#if _WIN32 || __LINUX__
#include <GLFW/glfw3.h>
#elif __ANDROID__
#include <game-activity/native_app_glue/android_native_app_glue.h>
#endif

Onyx::Graphics::Window::Window()
{
    m_pHandle = nullptr;
    m_Width = 0u;
    m_Height = 0u;
}

Onyx::Graphics::Window::~Window()
{
}

void Onyx::Graphics::Window::Create(const uint32_t width, const uint32_t height, void* pContext, const char* title)
{
    Utility::Log::Message("Creating Window.\n");
#if __ANDROID__
    if (pContext != nullptr) {
        m_pHandle = pContext;   //Cache the android_app*

        android_app* pApp = reinterpret_cast<android_app*>(m_pHandle);

        if (pApp->window != nullptr) {
            m_Height = ANativeWindow_getHeight(pApp->window);
            m_Width = ANativeWindow_getWidth(pApp->window);
            //ANativeWindow_getFormat(pApp->window);
        }
        else {
            throw(std::runtime_error("Android App Window was nullptr!\n"));
        }
    }
    else {
        throw(std::runtime_error("Android App Context was nullptr!\n"));
    }
#elif _WIN32 || __LINUX__
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_pHandle = static_cast<void*>(glfwCreateWindow(width, height, title, nullptr, nullptr));
    if (!m_pHandle) {
        throw std::runtime_error("Failed to Create Window!\n");
    }
    m_Height = height;
    m_Width = width;
#endif
    Utility::Log::Message("Window Created!\nWidth: %d\nHeight: %d\nTitle: %s\n", m_Width, m_Height, title);
}

void Onyx::Graphics::Window::Destroy() {

#if _WIN32 || __LINUX__
    glfwDestroyWindow(static_cast<GLFWwindow*>(m_pHandle));
#endif
}

void* Onyx::Graphics::Window::GetHandle() {
    return m_pHandle;
}


uint32_t Onyx::Graphics::Window::GetWidth() const {
    return m_Width; 
}

uint32_t Onyx::Graphics::Window::GetHeight() const {
    return m_Height; 
}

