#ifndef __APP_H
#define __APP_H

#include <Onyx/Graphics/Window.h>
#include <Onyx/Graphics/GPUDevice.h>

class App {
public:
    App(void* pContext = nullptr);
    void Init();
    void Shutdown();
    bool PollEvents();
    void Update();

    bool m_IsInitialized = false;
private:
    Onyx::Graphics::Window m_Window;
    Onyx::Graphics::GPUDevice* m_Device;
    void* m_Context;
};

#endif//__APP_H