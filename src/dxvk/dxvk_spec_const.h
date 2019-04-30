#pragma once

#include "dxvk_limits.h"
#include "dxvk_shader.h"

namespace dxvk {
  
  constexpr uint32_t MaxNumSpecConstants = 1
    + uint32_t(DxvkSpecConstantId::SpecConstantIdMax)
    - uint32_t(DxvkSpecConstantId::SpecConstantIdMin);
  
  /**
   * \brief Spec costant data
   * 
   * The values are derived from the pipeline
   * state vector so that they can be used by
   * the shaders.
   */
  struct DxvkSpecConstantData {
    uint32_t rasterizerSampleCount;
    VkBool32 alphaTestEnable;
    VkCompareOp alphaCompareOp;
    uint32_t outputMappings[MaxNumRenderTargets * 4];
    VkBool32 activeBindings[MaxNumActiveBindings];
  };
  
  
  /**
   * \brief Spec constant map
   * 
   * Stores the specialization constant map.
   * This can be passed to Vulkan when compiling
   * both graphics and compute pipelines.
   */
  class DxvkSpecConstantMap {
    
  public:
    
    DxvkSpecConstantMap();
    
    /**
     * \brief Map entry count
     * 
     * \param [in] bindingCount Number of active bindings
     * \returns The number of map entries to read
     */
    uint32_t mapEntryCount() const {
      return m_mapEntries.size();
    }
    
    /**
     * \brief Map entry data
     * \returns Map entries
     */
    const VkSpecializationMapEntry* mapEntryData() const {
      return m_mapEntries.data();
    }
    
  private:
    
    std::array<VkSpecializationMapEntry,
      MaxNumSpecConstants  +
      MaxNumActiveBindings +
      MaxNumRenderTargets * 4> m_mapEntries;
    
    void setConstantEntry(
            DxvkSpecConstantId  specId,
            uint32_t            offset,
            uint32_t            size);
    
    void setBindingEntry(
            uint32_t            binding);
    
    void setOutputMappingEntry(
            uint32_t            output);
    
  };
  
  extern DxvkSpecConstantMap g_specConstantMap;

  /**
   * \brief Specialization constant info
   * 
   * Accumulates specialization constant data for
   * constants that use non-default values.
   */
  class DxvkSpecConstants {

  public:

    DxvkSpecConstants();

    ~DxvkSpecConstants();

    /**
     * \brief Sets specialization constant value
     *
     * If the given value is different from the constant's
     * default value, this will store the new value and add
     * a map entry so that it gets applied properly. Each
     * constant may only be set once.
     * \param [in] specId Specialization constant ID
     * \param [in] value Specialization constant value
     * \param [in] defaultValue Default value
     */
    template<typename T>
    void set(uint32_t specId, T value, T defaultValue) {
      if (value != defaultValue)
        setAsUint32(specId, uint32_t(value));
    }

    /**
     * \brief Generates specialization info structure
     * \returns Specialization info for shader module
     */
    VkSpecializationInfo getSpecInfo() const;

  private:

    std::vector<uint32_t>                 m_data = { };
    std::vector<VkSpecializationMapEntry> m_map  = { };

    void setAsUint32(uint32_t specId, uint32_t value);

  };
  
}