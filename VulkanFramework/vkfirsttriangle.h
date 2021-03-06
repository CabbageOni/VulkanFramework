#pragma once

#include <vector>

#include "vkbase.h"

class VKFirstTriangle : public VKBase
{
private:
  struct ImageInfo
  {
    VkImage handle;
    VkImageView view;
  };

  uint32_t                      m_present_queue_family_index;
  uint32_t                      m_graphics_queue_family_index;
  VkQueue                       m_present_queue;
  VkQueue                       m_graphics_queue;
  VkCommandPool                 m_graphics_queue_command_pool;
  std::vector<VkCommandBuffer>  m_graphics_queue_command_buffers;
  VkSwapchainKHR                m_swap_chain;
  VkFormat                      m_swap_chain_format;
  VkExtent2D                    m_swap_chain_extent;
  std::vector<ImageInfo>        m_swap_chain_images;
  VkRenderPass                  m_render_pass;
  std::vector<VkFramebuffer>    m_frame_buffers;
  VkPipeline                    m_pipeline;

  VkSemaphore                   m_image_available_semaphore;
  VkSemaphore                   m_rendering_finished_semaphore;

  virtual bool CreateDevice() override;
  bool CheckPhysicalDeviceProperties(VkPhysicalDevice physical_device,
    uint32_t& selected_graphics_queue_family_index,
    uint32_t &selected_present_queue_family_index);
  bool GetDeviceQueue();
  bool OnWindowSizeChanged();
  void Clear();

  //extensions required
  bool CreateSemaphores();
  bool CreateSwapChain();
  uint32_t GetSwapChainNumImages(VkSurfaceCapabilitiesKHR &surface_capabilities);
  VkSurfaceFormatKHR GetSwapChainFormat(std::vector<VkSurfaceFormatKHR> &surface_formats);
  VkExtent2D GetSwapChainExtent(VkSurfaceCapabilitiesKHR &surface_capabilities);
  VkImageUsageFlags GetSwapChainUsageFlags(VkSurfaceCapabilitiesKHR& surface_capabilities);
  VkSurfaceTransformFlagBitsKHR GetSwapChainTransform(VkSurfaceCapabilitiesKHR& surface_capabilities);
  VkPresentModeKHR GetSwapChainPresentMode(std::vector<VkPresentModeKHR>& present_modes);
  bool CreateSwapChainImageViews();
  bool CreateRenderPass();
  bool CreateFrameBuffers();
  bool CreatePipeline();
  VkShaderModule CreateShaderModule(const char* shader_code);
  VkPipelineLayout CreatePipelineLayout();
  bool CreateCommandBuffers();
  bool CreateCommandPool(uint32_t queue_family_index, VkCommandPool* pool);
  bool AllocateCommandBuffers(VkCommandPool pool, uint32_t count, VkCommandBuffer* command_buffers);
  bool RecordCommandBuffers();

  virtual bool Initialize() override;
  virtual bool Update() override;
  virtual void Terminate() override;

public:
  friend bool Initialize();
  friend bool Update();
  friend void Terminate();
};