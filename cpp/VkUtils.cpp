#include "VkUtils.h"

namespace tgl {
    void VkUtils::beginCommandBuffer(VkCommandPool &vkCommandPool,
                                     VkCommandBufferUsageFlags vkCommandBufferUsageFlags,
                                     VkCommandBuffer *vkCommandBuffer) {
        VkCommandBufferBeginInfo vkCommandBufferBeginInfo{};
        vkCommandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkCommandBufferBeginInfo.flags = vkCommandBufferUsageFlags;
        VK_HANDLE_ERROR(vkBeginCommandBuffer(*vkCommandBuffer, &vkCommandBufferBeginInfo),
                        "Failed to begin a command buffer!");
    }

    std::vector<uint32_t> VkUtils::readFile(const std::string &fileName) {
        std::ifstream file(fileName, std::ios::binary | std::ios::ate);
        if (file) {
            size_t fileSize = (size_t) file.tellg();
            std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));
            file.seekg(0);
            file.read((char *) buffer.data(), fileSize);
            file.close();
            return buffer;
        }
        throw std::runtime_error("Failed to read file: " + fileName);
    }

    VkShaderModule VkUtils::createShaderModule(VkDevice &vkLogicalDevice, std::vector<uint32_t> &shaderCode) {
        VkShaderModuleCreateInfo vkShaderModuleCreateInfo{};
        vkShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        vkShaderModuleCreateInfo.pNext = nullptr;
        vkShaderModuleCreateInfo.codeSize = shaderCode.size() * sizeof(uint32_t);
        vkShaderModuleCreateInfo.pCode = shaderCode.data();

        VkShaderModule vkShaderModule;
        VK_HANDLE_ERROR(vkCreateShaderModule(vkLogicalDevice, &vkShaderModuleCreateInfo, nullptr, &vkShaderModule),
                        "Failed to read shader module!");
        return vkShaderModule;
    }

    int max(int a, int b) {
        if (a > b) {
            return a;
        }
        else {
            return b;
        }
    }

    int VkUtils::getOptimalSwapchainImageCount(GPU &gpu) {
        int maxImageCount = gpu.vkSurfaceCapabilities.maxImageCount;
        int minImageCount = gpu.vkSurfaceCapabilities.minImageCount;
        if (maxImageCount == 0 ||
            maxImageCount > 3) { //lets use 3 as we don't ever need more than 3, 0 means there isn't a limit
            return 3;
        }
        int countA = maxImageCount - 1;
        int countB = minImageCount;
        if (countA < minImageCount) {
            countA = minImageCount;
        }
        return max(countA, countB);
    }

    VkFormat VkUtils::getOptimalSwapchainFormat() {
        return VK_FORMAT_B8G8R8A8_UNORM;
    }

    VkPresentModeKHR VkUtils::getOptimalPresentMode(std::vector<VkPresentModeKHR>& vkPresentModes) {
        auto begin = vkPresentModes.begin();
        auto end = vkPresentModes.end();
        //Search for the most efficient
        if (std::find(begin, end, VK_PRESENT_MODE_MAILBOX_KHR) != end) { //immidiate is best but has tearing
            return VK_PRESENT_MODE_MAILBOX_KHR;
        }
            //Search for the second most efficient
        else if (std::find(begin, end, VK_PRESENT_MODE_FIFO_RELAXED_KHR) != end) {
            return VK_PRESENT_MODE_FIFO_RELAXED_KHR;
        }
            //Search for the third most efficient.
            //This one actually should always be valid...
        else if (std::find(begin, end, VK_PRESENT_MODE_FIFO_KHR) != end) {
            return VK_PRESENT_MODE_FIFO_KHR;
        } else {
            //Pick the first present mode(random).
            return vkPresentModes[0];
        }
    }

    void VkUtils::createImageView(VkDevice &vkLogicalDevice, VkImage &vkImage, VkFormat vkFormat,
                                  VkImageAspectFlags vkImageAspectFlags, VkImageView *vkImageView) {
        VkImageViewCreateInfo vkImageViewCreateInfo;
        vkImageViewCreateInfo.pNext = nullptr;
        vkImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        vkImageViewCreateInfo.flags = 0;
        vkImageViewCreateInfo.image = vkImage;
        vkImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        vkImageViewCreateInfo.format = vkFormat;
        //Don't modify the RGBA channels, keep them at their default values. R = R, G = G, B = B, A = A.
        //If one wanted one could use the r channel as a green channel for example.
        vkImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        vkImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        vkImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        vkImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        vkImageViewCreateInfo.subresourceRange.aspectMask = vkImageAspectFlags;
        vkImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        vkImageViewCreateInfo.subresourceRange.layerCount = 1;
        vkImageViewCreateInfo.subresourceRange.levelCount = 1;
        vkImageViewCreateInfo.subresourceRange.baseMipLevel = 0;

        VK_HANDLE_ERROR(vkCreateImageView(vkLogicalDevice, &vkImageViewCreateInfo, nullptr, vkImageView),
                        "Failed to create an image view!");
    }

}