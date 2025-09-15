#include "App.h"

#include <Onyx/Onyx.h>
#include <Onyx/Utility/Logger.h>
#include <Onyx/Graphics/Backends/GPUDevice_Vulkan.h>

#if __ANDROID__

#include <game-activity/GameActivity.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>

#elif _WIN32 || __linux__ 

#include <GLFW/glfw3.h>

#endif

#include <fstream> //TODO: Move to Utility!

//TODO: Testing Device work!
Onyx::Graphics::Buffer vertexBuffer;
VkPipelineLayout pipelineLayout;
VkPipeline pipeline;
Onyx::Graphics::Fence fence = {};
uint64_t val = 1000;

App::App(void* pContext) {
    m_Context = pContext;

    m_Device = nullptr;
    m_IsInitialized = false;
}

void App::Init() {
    Onyx::Initialize();

    m_Window.Create(1600, 900, m_Context);
    m_Device = new Onyx::Graphics::Vulkan::GPUDevice_Vulkan;
    m_Device->Init(&m_Window);


    //Testing Resource Creation
    {
        Onyx::Graphics::BufferCreateInfo createInfo = {};
        createInfo.name = "Debug Buffer";
        createInfo.size = 1u << 12u;
        createInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;// | VK_BUFFER_USAGE_TRANSFER_DST_BIT; 
        createInfo.flags = {};

        Onyx::Graphics::Buffer buffer = {};
        m_Device->CreateBuffer(&createInfo, &buffer);


        Onyx::Utility::Log::Debug("Sizeof(Buffer) = %d\n", sizeof(Onyx::Graphics::Buffer));
        //Check buffer values
        Onyx::Utility::Log::Debug("Created Buffer!\nName: %s\nSize: %d\nVkBuffer Handle: <0x%08x>\nVmaAllocation Handle: <0x%08x>\n", buffer.name, buffer.size, buffer._buffer, buffer._alloc);

        m_Device->DestroyBuffer(buffer);
        Onyx::Utility::Log::Debug("Destroyed Buffer!\nName: %s\nSize: %d\nVkBuffer Handle: <0x%08x>\nVmaAllocation Handle: <0x%08x>\n", buffer.name, buffer.size, buffer._buffer, buffer._alloc);
    }
    {
        Onyx::Utility::Log::Debug("Sizeof(Texture) = %d\n", sizeof(Onyx::Graphics::Texture));
        Onyx::Graphics::TextureCreateInfo createInfo = {};
        createInfo.name = "Debug Texture";
        createInfo.width = 1024;
        createInfo.height = 1024;
        createInfo.depth = 1;
        createInfo.type = Onyx::Graphics::ETextureType::TEXTURE_2D;
        createInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
        createInfo.mipLevels = 1;
        createInfo.sampleCount = Onyx::Graphics::ESampleCount::SAMPLE_COUNT_1;
        createInfo.tiling = VK_IMAGE_TILING_LINEAR;
        createInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT;


        Onyx::Graphics::Texture texture = {};
        m_Device->CreateTexture(&createInfo, &texture);
        Onyx::Utility::Log::Debug("Created Texture!\nName: %s\nWidth:%d\nHeight: %d\nDepth: %d\nVkImage Handle: <0x%08x>\nVmaAllocation Handle: <0x%08x>\n", texture.name, texture.width, texture.height, texture.depth, texture._image, texture._alloc);

        m_Device->DestroyTexture(texture);
        Onyx::Utility::Log::Debug("Destroyed Texture!\nName: %s\nWidth:%d\nHeight: %d\nDepth: %d\nVkImage Handle: <0x%08x>\nVmaAllocation Handle: <0x%08x>\n", texture.name, texture.width, texture.height, texture.depth, texture._image, texture._alloc);

    }

    //Fence Testing
    {

        m_Device->CreateFence(0, &fence, "Debug Fence");

        fence.Signal(m_Device, 10);
        Onyx::Utility::Log::Debug("Waiting...\n");
        fence.Wait(m_Device, 100, 5000);
        Onyx::Utility::Log::Debug("Finished Waiting!\n");

        Onyx::Graphics::GPUQueue queue = {};
        //TODO: Testing queue creation...
        Onyx::OnyxResult res = m_Device->CreateQueue(&queue, Onyx::Graphics::EQueueType::QUEUE_TYPE_GRAPHICS);


        //TODO: API Consistent overload...
        //Onyx::Graphics::CommandBuffer cmd = queue.GetCommandBuffer(m_Device, Onyx::Graphics::ECommandBufferType::ECommandBufferType_MAX);
        Onyx::Graphics::CommandBuffer cmd = m_Device->AllocateCommandBuffer(Onyx::Graphics::ECommandBufferType::PRIMARY);

        cmd.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        //TODO: Pipelines!!!
        //vkCmdDispatch(cmd._commandBuffer, 1, 1, 1);
        {
            //Transition Swapchain Image -> Present Src Optimal 
            auto swapchainImages = m_Device->GetSwapchainImages();
            for (int i = 0; i < swapchainImages.size(); i++)
            {
                VkImageSubresourceRange range = {};
                range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                range.baseArrayLayer = 0;
                range.baseMipLevel = 0;
                range.layerCount = 1;
                range.levelCount = 1;

                VkImage swapchainImage = swapchainImages[i]._image;

                VkImageMemoryBarrier undefined_to_colour_attachment_optimal = {};
                undefined_to_colour_attachment_optimal.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                undefined_to_colour_attachment_optimal.pNext = nullptr;
                undefined_to_colour_attachment_optimal.image = swapchainImage;
                undefined_to_colour_attachment_optimal.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                undefined_to_colour_attachment_optimal.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                undefined_to_colour_attachment_optimal.subresourceRange = range;
                undefined_to_colour_attachment_optimal.srcQueueFamilyIndex = 0;
                undefined_to_colour_attachment_optimal.dstQueueFamilyIndex = 0;
                undefined_to_colour_attachment_optimal.srcAccessMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                undefined_to_colour_attachment_optimal.dstAccessMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

                vkCmdPipelineBarrier(cmd._commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &undefined_to_colour_attachment_optimal);
            }
        }

        cmd.End();


        Onyx::Graphics::SubmitInfo info = {};
        info.commandBufferCount = 1;
        info.pCommandBuffers = &cmd;
        info.waitFenceCount = 1;
        uint64_t waitValues = 10;
        info.pWaitValues = &waitValues;
        info.pWaitFences = &fence;
        VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        info.pWaitDstStageMask = &waitStages;
        info.signalFenceCount = 1;
        info.pSignalFences = &fence;
        uint64_t signalValues = 100;
        info.pSignalValues = &signalValues;

        m_Device->Submit(&info);
        /*
        Onyx::Graphics::SubmitInfo* infos[] = {
            &info
        };

        queue.Submit(m_Device, 1, infos);
        */

        fence.Wait(m_Device, 100, UINT64_MAX);


        Onyx::Utility::Log::Debug("Finished Work!\n");
        vkResetCommandBuffer(cmd._commandBuffer, 0);    //TODO: Device-level API for managing command buffer state! 
        //queue.ResetCommandBuffer(m_Device, cmd); 

        m_Device->DestroyQueue(queue);
        // m_Device->DestroyFence(fence);
    }

    m_IsInitialized = true;

    //TODO: Set up the testing app...
    /**/

    {
        //Create the Vertex Buffer 
        {
            Onyx::Utility::Log::Message("Creating Vertex Buffer.\n");
            std::vector<float> verts = {
                -0.5, -0.5, 0.0,    //0
                0.0, 0.5, 0.0,      //1
                0.5, -0.5, 0.0,     //2
            };

            Onyx::Graphics::BufferCreateInfo vertexBufferCreateInfo = {};
            vertexBufferCreateInfo.name = "Triangle Vertex Buffer";
            vertexBufferCreateInfo.size = sizeof(float) * verts.size();
            vertexBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            vertexBufferCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;

            m_Device->CreateBuffer(&vertexBufferCreateInfo, &vertexBuffer);

            //vertexBuffer.map(); 
            void* pData = nullptr;
            if (m_Device->MapBufferMemory(vertexBuffer, &pData) == Onyx::OnyxResult::ONYX_SUCCESS) {
                memcpy(pData, verts.data(), sizeof(float) * verts.size());
            }
            m_Device->UnmapBufferMemory(vertexBuffer);
        }

        //Create the pipeline. 
        {
            Onyx::Utility::Log::Message("Creating Graphics Pipeline.\n");

            VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
            pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutCreateInfo.pNext = nullptr;

            VkDevice dev = ((Onyx::Graphics::Vulkan::GPUDevice_Vulkan*)m_Device)->GetVKDevice();
            vkCreatePipelineLayout(dev, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout);

            //Create the Vertex Input Info. 
            VkVertexInputBindingDescription vertexInputBindingDesc = {};
            vertexInputBindingDesc.stride = 0;
            vertexInputBindingDesc.binding = 0;
            vertexInputBindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            VkVertexInputAttributeDescription vertexInputAttribDesc = {};
            vertexInputAttribDesc.binding = 0;
            vertexInputAttribDesc.format = VK_FORMAT_R32G32B32_SFLOAT;
            vertexInputAttribDesc.location = 0;
            vertexInputAttribDesc.offset = 0;

            VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
            vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            vertexInputInfo.pNext = nullptr;
            vertexInputInfo.vertexBindingDescriptionCount = 1;
            vertexInputInfo.pVertexBindingDescriptions = &vertexInputBindingDesc;
            vertexInputInfo.vertexAttributeDescriptionCount = 1;
            vertexInputInfo.pVertexAttributeDescriptions = &vertexInputAttribDesc;


            //Configure the Input Assembler 
            VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
            inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            inputAssemblyInfo.pNext = nullptr;
            inputAssemblyInfo.flags = 0;
            inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

            //TODO: Dynamic State!
            //Configure the Viewport 
            VkViewport viewport = {};
            viewport.x = 0.0f;
            viewport.y = (float)m_Window.GetHeight();   //See https://www.saschawillems.de/blog/2019/03/29/flipping-the-vulkan-viewport/
            viewport.width = (float)m_Window.GetWidth();
            viewport.height = -(float)m_Window.GetHeight();
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;


            //Configure the Scissor Rect
            VkRect2D scissor = {};
            scissor.offset = { 0, 0 };
            scissor.extent.width = m_Window.GetWidth();
            scissor.extent.height = m_Window.GetHeight();

            //Viewport State
            VkPipelineViewportStateCreateInfo viewportState = {};
            viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            viewportState.pNext = nullptr;
            viewportState.flags = 0;
            viewportState.viewportCount = 1;
            viewportState.pViewports = &viewport;
            viewportState.scissorCount = 1;
            viewportState.pScissors = &scissor;

            //Configure Rasterizer State
            VkPipelineRasterizationStateCreateInfo rasterizerState = {};
            rasterizerState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            rasterizerState.pNext = nullptr;
            rasterizerState.flags = 0;
            rasterizerState.lineWidth = 1.0f;
            rasterizerState.polygonMode = VK_POLYGON_MODE_FILL;
            rasterizerState.rasterizerDiscardEnable = VK_FALSE;
            rasterizerState.cullMode = VK_CULL_MODE_NONE;
            rasterizerState.frontFace = VK_FRONT_FACE_CLOCKWISE;
            rasterizerState.depthClampEnable = VK_FALSE;
            rasterizerState.depthBiasEnable = VK_FALSE;
            rasterizerState.depthBiasClamp = 0.0f;
            rasterizerState.depthBiasConstantFactor = 0.0f;
            rasterizerState.depthBiasSlopeFactor = 0.0f;

            //Configure MSAA
            VkPipelineMultisampleStateCreateInfo msaaState = {};
            msaaState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            msaaState.pNext = nullptr;
            msaaState.flags = 0;
            msaaState.sampleShadingEnable = VK_FALSE;
            msaaState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
            msaaState.minSampleShading = 1.0f;
            msaaState.pSampleMask = nullptr;
            msaaState.alphaToCoverageEnable = VK_FALSE;
            msaaState.alphaToOneEnable = VK_FALSE;

            //Configure the Blend State
            VkPipelineColorBlendAttachmentState blendAttachment = {};
            blendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            blendAttachment.blendEnable = VK_FALSE;
            blendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
            blendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
            blendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
            blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
            blendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

            VkPipelineColorBlendStateCreateInfo blendState = {};
            blendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            blendState.pNext = nullptr;
            blendState.logicOpEnable = VK_FALSE;
            blendState.logicOp = VK_LOGIC_OP_COPY;
            blendState.attachmentCount = 1;
            blendState.pAttachments = &blendAttachment;
            blendState.blendConstants[0] = 0.0f;
            blendState.blendConstants[1] = 0.0f;
            blendState.blendConstants[2] = 0.0f;
            blendState.blendConstants[3] = 0.0f;

            //Configure the Depth State
            VkPipelineDepthStencilStateCreateInfo depthState = {};
            depthState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
            depthState.pNext = nullptr;
            depthState.depthWriteEnable = VK_TRUE;
            depthState.depthTestEnable = VK_TRUE;
            depthState.minDepthBounds = 0.0f;
            depthState.maxDepthBounds = 1.0f;
            depthState.depthCompareOp = VK_COMPARE_OP_LESS;
            depthState.stencilTestEnable = VK_FALSE;
            depthState.depthBoundsTestEnable = VK_FALSE;
            depthState.front = {};
            depthState.back = {};

            std::vector<VkPipelineShaderStageCreateInfo> stages = {};
            {
                VkPipelineShaderStageCreateInfo vs = {};
                vs.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                vs.pNext = nullptr;   //The application can override this if it wants to. 
                vs.pName = "main";
                vs.flags = 0;
                vs.pSpecializationInfo = nullptr;
                vs.stage = VK_SHADER_STAGE_VERTEX_BIT;
                vs.module = [dev](const char* path) {
                    std::ifstream binary(path, std::ios::ate | std::ios::binary);
                    size_t size = (size_t)binary.tellg();
                    binary.seekg(0);
                    std::vector<char> blob(size);
                    binary.read(blob.data(), size);
                    binary.close();

                    VkShaderModule module = {};
                    VkShaderModuleCreateInfo createInfo = {};
                    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
                    createInfo.pNext = nullptr;
                    createInfo.flags = 0;
                    createInfo.codeSize = blob.size();
                    createInfo.pCode = (uint32_t*)blob.data();

                    vkCreateShaderModule(dev, &createInfo, nullptr, &module);
                    return module;
                    }("vs.vert.spv");
                stages.push_back(vs);
            }
            {
                VkPipelineShaderStageCreateInfo fs = {};
                fs.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                fs.pNext = nullptr;   //The application can override this if it wants to. 
                fs.pName = "main";
                fs.flags = 0;
                fs.pSpecializationInfo = nullptr;
                fs.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                fs.module = [dev](const char* path) {
                    //TODO: Android builds won't work; need AAssetManager to load files!
                    std::ifstream binary(path, std::ios::ate | std::ios::binary);
                    size_t size = (size_t)binary.tellg();
                    binary.seekg(0);
                    std::vector<char> blob(size);
                    binary.read(blob.data(), size);
                    binary.close();

                    VkShaderModule module = {};
                    VkShaderModuleCreateInfo createInfo = {};
                    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
                    createInfo.pNext = nullptr;
                    createInfo.flags = 0;
                    createInfo.codeSize = blob.size();
                    createInfo.pCode = (uint32_t*)blob.data();

                    vkCreateShaderModule(dev, &createInfo, nullptr, &module);
                    return module;
                    }("fs.frag.spv");
                stages.push_back(fs);
            }
            //Configure Dynamic Rendering

            VkFormat format = ((Onyx::Graphics::Vulkan::GPUDevice_Vulkan*)m_Device)->GetVkSwapchainFormat();
            VkFormat depthFormat = VK_FORMAT_D32_SFLOAT;

            VkPipelineRenderingCreateInfo pipelineRenderingCreateInfo = {};
            pipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
            pipelineRenderingCreateInfo.pNext = VK_NULL_HANDLE;
            pipelineRenderingCreateInfo.colorAttachmentCount = 1;
            pipelineRenderingCreateInfo.pColorAttachmentFormats = &format;
            pipelineRenderingCreateInfo.depthAttachmentFormat = depthFormat;
            pipelineRenderingCreateInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;

            VkGraphicsPipelineCreateInfo pci = {};
            pci.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            pci.pNext = &pipelineRenderingCreateInfo;
            pci.flags = 0;
            pci.renderPass = VK_NULL_HANDLE; //Using Dynamic Rendering! 
            pci.subpass = 0;
            pci.pVertexInputState = &vertexInputInfo;
            pci.pInputAssemblyState = &inputAssemblyInfo;
            pci.pViewportState = &viewportState;
            pci.pRasterizationState = &rasterizerState;
            pci.pMultisampleState = &msaaState;
            pci.pColorBlendState = &blendState;
            pci.pDepthStencilState = &depthState;
            pci.stageCount = stages.size();
            pci.pStages = stages.data();
            pci.layout = pipelineLayout;
            pci.basePipelineHandle = VK_NULL_HANDLE;
            pci.basePipelineIndex = 0;
            //...

            vkCreateGraphicsPipelines(dev, VK_NULL_HANDLE, 1, &pci, nullptr, &pipeline);

        }
        Onyx::Utility::Log::Message("Finished Init!\n");

    }
    //*/
}

void App::Shutdown() {
    m_Device->Shutdown();
    delete m_Device;

    m_Window.Destroy();

    //Terminate the Engine
    Onyx::Shutdown();
}

bool App::PollEvents() {

#if __ANDROID__

    bool done = false;
    while (!done) {
        android_poll_source* pSource = nullptr;
        int events = 0;
        int timeout = 0;
        int r = ALooper_pollOnce(timeout, nullptr, &events, reinterpret_cast<void**>(&pSource));

        switch (r) {
        case ALOOPER_POLL_TIMEOUT:
            [[clang::fallthrough]];
        case ALOOPER_POLL_WAKE:
            done = true;
            break;
        case ALOOPER_EVENT_ERROR:
            break;
        case ALOOPER_POLL_CALLBACK:
            break;
        default:
            if (pSource) {
                pSource->process((reinterpret_cast<android_app*>(m_Context)), pSource);
            }
        }
    }

    android_app* pApp = reinterpret_cast<android_app*>(m_Context);
    return (!pApp->destroyRequested);
#elif _WIN32 || __linux__
    glfwPollEvents();
    return !glfwWindowShouldClose(reinterpret_cast<GLFWwindow*>(m_Window.GetHandle()));

#endif
}

Onyx::Graphics::CommandBuffer commandBuffer;
VkCommandBuffer cmd; 

void App::Update() {
    if (m_IsInitialized) {

        static uint64_t frameIdx = 0;
        Onyx::Utility::Log::Print("Frame %d\r", frameIdx++);

        uint32_t imageIdx = m_Device->GetSwapchainImageIndex();
        //TODO: Perform some dummy work. 
        /**/
        {
            static bool cmdAlloc = false;
            if (!cmdAlloc) {    //Lovely, hacky, evil... and prevents memory leaks!
                commandBuffer = m_Device->AllocateCommandBuffer(Onyx::Graphics::ECommandBufferType::PRIMARY);
                cmdAlloc = true;
            }
            cmd = commandBuffer._commandBuffer;

            commandBuffer.Begin(0);

            //Resolve the initial image layout. 
            {
                VkImageSubresourceRange range = {};
                range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                range.baseArrayLayer = 0;
                range.baseMipLevel = 0;
                range.layerCount = 1;
                range.levelCount = 1;

                VkImageMemoryBarrier present_src_to_colour_attachment_optimal = {};
                present_src_to_colour_attachment_optimal.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                present_src_to_colour_attachment_optimal.pNext = nullptr;
                present_src_to_colour_attachment_optimal.image = m_Device->GetSwapchainImages()[imageIdx]._image;
                present_src_to_colour_attachment_optimal.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                present_src_to_colour_attachment_optimal.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                present_src_to_colour_attachment_optimal.subresourceRange = range;
                present_src_to_colour_attachment_optimal.srcQueueFamilyIndex = 0;
                present_src_to_colour_attachment_optimal.dstQueueFamilyIndex = 0;
                present_src_to_colour_attachment_optimal.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                //present_src_to_colour_attachment_optimal.srcAccessMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

                vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &present_src_to_colour_attachment_optimal);
            }


            VkClearValue clearColour = {};
            clearColour.color.float32[0] = (sin((float)frameIdx * 0.005) + 1.0f) / 2.0f;
            clearColour.color.float32[1] = 0.0f;
            clearColour.color.float32[2] = (cos((float)frameIdx * 0.005) + 1.0f) / 2.0f;
            clearColour.color.float32[3] = 1.0f;

            VkRenderingAttachmentInfo colourAttachmentInfo = {};
            colourAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
            colourAttachmentInfo.pNext = nullptr;
            colourAttachmentInfo.imageView = ((Onyx::Graphics::Vulkan::GPUDevice_Vulkan*)m_Device)->GetVKSwapchainImageViews()[imageIdx];
            colourAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
            colourAttachmentInfo.clearValue = clearColour;
            colourAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colourAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            //...

            VkRenderingInfo renderingInfo = {};
            renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
            renderingInfo.pNext = nullptr;
            renderingInfo.flags = 0;
            renderingInfo.renderArea.extent = { m_Window.GetWidth(), m_Window.GetHeight() };
            renderingInfo.renderArea.offset = { 0u, 0u };
            renderingInfo.viewMask = 0;
            renderingInfo.colorAttachmentCount = 1;
            renderingInfo.pColorAttachments = &colourAttachmentInfo;
            renderingInfo.layerCount = 1;
            renderingInfo.pDepthAttachment = nullptr;
            renderingInfo.pStencilAttachment = nullptr;


#if __ANDROID__
            //TODO: Remove this! Nicked from samples
            {
                VkInstance instance = ((Onyx::Graphics::Vulkan::GPUDevice_Vulkan*)m_Device)->GetVKInstance();
                //assert(!!instance);
                auto cmdBeginRendering = (PFN_vkCmdBeginRendering)vkGetInstanceProcAddr(instance, "vkCmdBeginRendering");
                auto cmdEndRendering = (PFN_vkCmdEndRendering)vkGetInstanceProcAddr(instance, "vkCmdEndRendering");
                if (!cmdBeginRendering || !cmdEndRendering)
                {
                    throw std::runtime_error("Unable to dynamically load cmdBeginRendering and cmdEndRendering");
                }
                cmdBeginRendering(cmd, &renderingInfo);
                cmdEndRendering(cmd);
            }

#else
            vkCmdBeginRendering(cmd, &renderingInfo);
            VkDeviceSize offsets = 0;
            vkCmdBindVertexBuffers(cmd, 0, 1, &vertexBuffer._buffer, &offsets);
            vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
            vkCmdDraw(cmd, 3, 1, 0, 0);
            vkCmdEndRendering(cmd);

#endif
            //Attachment Optimal -> Present Src0.

           
            {
                VkImageSubresourceRange range = {};
                range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                range.baseArrayLayer = 0;
                range.baseMipLevel = 0;
                range.layerCount = 1;
                range.levelCount = 1;

                VkImageMemoryBarrier colour_attachment_optimal_to_present_src = {};
                colour_attachment_optimal_to_present_src.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                colour_attachment_optimal_to_present_src.pNext = nullptr;
                colour_attachment_optimal_to_present_src.image = m_Device->GetSwapchainImages()[imageIdx]._image;
                colour_attachment_optimal_to_present_src.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                colour_attachment_optimal_to_present_src.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
                colour_attachment_optimal_to_present_src.subresourceRange = range;
                colour_attachment_optimal_to_present_src.srcQueueFamilyIndex = 0;
                colour_attachment_optimal_to_present_src.dstQueueFamilyIndex = 0;
                colour_attachment_optimal_to_present_src.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                //colour_attachment_optimal_to_present_src.dstAccessMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

                vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &colour_attachment_optimal_to_present_src);
            }

            commandBuffer.End();

            Onyx::Graphics::SubmitInfo submitInfo = {};
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;
            VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
            submitInfo.pWaitDstStageMask = &waitStages;
            uint64_t vals = val + 100;
            submitInfo.pSignalValues = &vals;
            submitInfo.signalFenceCount = 1;
            submitInfo.pSignalFences = &fence;

            m_Device->Submit(&submitInfo);

            fence.Wait(m_Device, val + 100);
            val += 100;

            vkResetCommandBuffer(cmd, 0); 
        }
        //*/

        Onyx::OnyxResult res = m_Device->Present(imageIdx);
        if (res == Onyx::OnyxResult::ONYX_OUT_OF_DATE) {
            while (m_Window.GetWidth() == 0 || m_Window.GetHeight() == 0) {
                //Wait for something to happen! 
                PollEvents();
            }
            m_Device->CreateSwapchain(&m_Window);
        }


    }
}
