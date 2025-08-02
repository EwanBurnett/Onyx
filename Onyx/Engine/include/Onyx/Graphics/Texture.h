#ifndef __TEXTURE_H
#define __TEXTURE_H

#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"

#include <cstdint> 
#include <shared_mutex>

namespace Onyx{
    namespace Graphics{

        enum class ETextureType{
            TEXTURE_1D,
            TEXTURE_2D, 
            TEXTURE_3D,
            TEXTURE_CUBE, 
            ETextureType_MAX, 
        };

        enum class ESampleCount{
            SAMPLE_COUNT_1, 
            SAMPLE_COUNT_2, 
            SAMPLE_COUNT_4, 
            SAMPLE_COUNT_8, 
            SAMPLE_COUNT_16, 
            SAMPLE_COUNT_32, 
            SAMPLE_COUNT_64, 
            ESampleCount_MAX
        };

        enum class EFormat{
            FORMAT_UNDEFINED, 
            FORMAT_R8_UNORM,    //VK_FORMAT_R8_UNORM / DXGI_FORMAT_R8_UNORM
            FORMAT_R8_SNORM, 
            //TODO: Format.h 

            EFormat_MAX
        };

        struct TextureCreateInfo{
            const char* name;
            ETextureType type;
            uint32_t width; 
            uint32_t height; 
            uint32_t depth; 

            //TODO: 
            VkFormat format; 
            uint32_t mipLevels; 
            ESampleCount sampleCount; 
            VkImageTiling tiling; 
            VkImageUsageFlags usage; 
        };

        struct alignas(16) Texture{
            const char* name; 
            uint32_t width; 
            uint32_t height; 
            uint32_t depth; 
            ETextureType type; 

            union{
                struct{
                    VkImage _image; 
                    VmaAllocation _alloc; 
                };
            };

            std::shared_mutex _Lock; 
        };
    }
}

#endif// __TEXTURE_H
