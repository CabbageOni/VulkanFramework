#pragma once

#include <Windows.h>

#include "resources\vulkan\vulkan.h"

extern class Vulkan
{
private:
  HMODULE m_vulkanLibrary;
  VkInstance m_instance;
  //there should be one per devices, but now all in here since I'm using only one device and queue
  VkDevice m_device;
  uint32_t m_queue_family_index;
  VkQueue m_queue;

  bool LoadVulkanLibrary();
  bool LoadExportedEntryPoints();
  bool LoadGlobalLevelEntryPoints();
  bool CreateInstance();
  bool LoadInstanceLevelEntryPoints();
  bool CreateDevice();
  bool CheckPhysicalDeviceProperties(VkPhysicalDevice physical_device, uint32_t& queue_family_index);
  bool LoadDeviceLevelEntryPoints();
  bool GetDeviceQueue();

  bool Initialize();
  void Terminate();

  //Vulkan functions
  #define LOAD_EXPORTED(func) PFN_##func func;
  #define LOAD_GLOBAL_LEVEL(func) PFN_##func func;
  #define LOAD_INSTANCE_LEVEL(func) PFN_##func func;
  #define LOAD_DEVICE_LEVEL(func) PFN_##func func;
  #include "vulkanFunctions.inl"
  #undef LOAD_EXPORTED
  #undef LOAD_GLOBAL_LEVEL
  #undef LOAD_INSTANCE_LEVEL
  #undef LOAD_DEVICE_LEVEL
public:

  friend bool Initialize();
  friend void Terminate();

} vulkan;