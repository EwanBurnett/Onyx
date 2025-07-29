#ifndef __WINDOW_H
#define __WINDOW_H

#include <cstdint> 

namespace Onyx {
    namespace Graphics {
        class Window {
        public:
            Window();
            ~Window();

            void Create(const uint32_t width, const uint32_t height, void* pContext = nullptr, const char* title = "Onyx Application");
            void Destroy();

            void* GetHandle();

            uint32_t GetWidth() const; 
            uint32_t GetHeight() const; 
        private:
            void* m_pHandle;
            uint32_t m_Width;
            uint32_t m_Height;
        };
    }
}

#endif//__WINDOW_H