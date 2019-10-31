#ifndef __IRR_I_DESCRIPTOR_SET_H_INCLUDED__
#define __IRR_I_DESCRIPTOR_SET_H_INCLUDED__

#include "irr/core/memory/refctd_dynamic_array.h"
#include "irr/asset/IDescriptorSetLayout.h"//for E_DESCRIPTOR_TYPE
#include "irr/asset/format/EFormat.h"
#include "irr/asset/IDescriptor.h"
#include "irr/core/SRange.h"
#include <algorithm>

namespace irr { namespace asset
{

enum E_IMAGE_LAYOUT : uint32_t
{
    EIL_UNDEFINED = 0,
    EIL_GENERAL = 1,
    EIL_COLOR_ATTACHMENT_OPTIMAL = 2,
    EIL_DEPTH_STENCIL_ATTACHMENT_OPTIMAL = 3,
    EIL_DEPTH_STENCIL_READ_ONLY_OPTIMAL = 4,
    EIL_SHADER_READ_ONLY_OPTIMAL = 5,
    EIL_TRANSFER_SRC_OPTIMAL = 6,
    EIL_TRANSFER_DST_OPTIMAL = 7,
    EIL_PREINITIALIZED = 8,
    EIL_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL = 1000117000,
    EIL_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL = 1000117001,
    EIL_PRESENT_SRC_KHR = 1000001002,
    EIL_SHARED_PRESENT_KHR = 1000111000,
    EIL_SHADING_RATE_OPTIMAL_NV = 1000164003,
    EIL_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT = 1000218000
};

template<typename LayoutType, typename BufferType, typename TextureType, typename BufferViewType, typename SamplerType>
class IDescriptorSet
{
    using this_type = IDescriptorSet<LayoutType, BufferType, TextureType, BufferViewType, SamplerType>;

public:
    struct SDescriptorInfo
    {
        core::smart_refctd_ptr<IDescriptor> desc;
        union
        {
            struct SDescriptorBufferInfo
            {
                size_t offset;
                size_t size;//in Vulkan it's called `range` but IMO it's misleading so i changed to `size`
            } buffer;
            struct SDescriptorImageInfo
            {
                core::smart_refctd_ptr<SamplerType> sampler;
                //! Irrelevant in OpenGL backend
                E_IMAGE_LAYOUT imageLayout;
            } image;
        };

        SDescriptorInfo& operator=(const SDescriptorInfo& _other)
        {
            desc = _other.desc;
            if (desc)
            {
                switch (desc->getTypeCategory())
                {
                case IDescriptor::EC_IMAGE:
                    image = _other.image;
                    break;
                case IDescriptor::EC_BUFFER:
                    buffer = _other.buffer;
                    break;
                case IDescriptor::EC_BUFFER_VIEW:
                    //nothing, just descriptor, no extra info
                    break;
                }
            }

            return (*this);
        }

        SDescriptorInfo()
        {
            memset(&buffer, 0, std::max(sizeof(buffer), sizeof(image)));
        }
        ~SDescriptorInfo()
        {
            if (desc->getTypeCategory()==IDescriptor::EC_IMAGE)
                image.sampler.~smart_refctd_ptr();
        }
    };

    struct SDescriptorBinding
    {
        uint32_t binding = 0u;
        E_DESCRIPTOR_TYPE descriptorType = EDT_COMBINED_IMAGE_SAMPLER;//whatever, default value
        core::smart_refctd_dynamic_array<SDescriptorInfo> info;
    };

    struct SWriteDescriptorSet
    {
        uint32_t binding;
        uint32_t arrayElement;
        uint32_t count;
        E_DESCRIPTOR_TYPE descriptorType;
        SDescriptorInfo* info;
    };

    struct SCopyDescriptorSet
    {
        this_type* srcSet;//smart pointer not needed here
        uint32_t srcBinding;
        uint32_t srcArrayElement;
        uint32_t dstBinding;
        uint32_t dstArrayElement;
        uint32_t count;
    };

    //CVulkanDescriptorSet might want to override this, hence `virtual`
    virtual void updateDescriptorSet(uint32_t _writeCount, const SWriteDescriptorSet* _descWrites, uint32_t _copyCount, const SCopyDescriptorSet* _descCopies)
    {
        for (uint32_t i = 0u; i < _writeCount; ++i)
        {
            const SWriteDescriptorSet& wrt = _descWrites[i];
            uint32_t ix = (*m_bindingToIx)[wrt.binding];
            uint32_t startIx = wrt.arrayElement;
            uint32_t remaining = wrt.count;

            while ((remaining > 0u) && (ix < m_descriptors->size()))
            {
                SDescriptorBinding& descBinding = (*m_descriptors)[ix];

                assert(wrt.descriptorType == descBinding.descriptorType);

                const uint32_t cnt = std::min<uint32_t>(descBinding.info->size()-startIx, remaining);

                const uint32_t updatedSoFar = (wrt.count - remaining);
                for (uint32_t j = 0u; j < cnt; ++j)
                {
                    descBinding.info->operator[](startIx + j) = wrt.info[updatedSoFar + j];
                }

                remaining -= cnt;
                startIx = 0u;
                ++ix;
            }
            assert(remaining==0u);
        }
        for (uint32_t i = 0u; i < _copyCount; ++i)
        {
            const SCopyDescriptorSet& cpy = _descCopies[i];
            const uint32_t ix = (*m_bindingToIx)[cpy.dstBinding];

            const SDescriptorBinding* src = cpy.srcSet->getDescriptorFromBindingNum(cpy.srcBinding);
            SDescriptorBinding& dst = (*m_descriptors)[ix];

            assert(dst.descriptorType == src->descriptorType);

            for (uint32_t j = 0u; j < cpy.count; ++j)
                dst.info->operator[](cpy.dstArrayElement + j) = src->info->operator[](cpy.srcArrayElement + j);
        }
    }

    const LayoutType* getLayout() const { return m_layout.get(); }
    core::SRange<const SDescriptorBinding> getDescriptors() const 
    { 
        return m_descriptors ? core::SRange<const SDescriptorBinding>{m_descriptors->begin(), m_descriptors->end()} : core::SRange<const SDescriptorBinding>{nullptr, nullptr};
    }

protected:
    const SDescriptorBinding* getDescriptorFromBindingNum(uint32_t _binding) const
    {
        if (!m_bindingToIx)
            return nullptr;
        const uint32_t ix = (*m_bindingToIx)[_binding];
        return getDescriptors().begin()+ix;
    }

public:
    IDescriptorSet(core::smart_refctd_ptr<LayoutType>&& _layout) :
        m_layout(std::move(_layout)),
        m_descriptors(core::make_refctd_dynamic_array<core::smart_refctd_dynamic_array<SDescriptorBinding>>(m_layout->getBindings().length())),
        m_bindingToIx(core::make_refctd_dynamic_array<core::smart_refctd_dynamic_array<uint32_t>>((m_layout->getBindings().end() - 1)->binding + 1u))
    {
        for (auto& item : (*m_bindingToIx))
            item = (~0u);
        uint32_t i = 0u;
        for (const typename LayoutType::SBinding& bnd : m_layout->getBindings())
        {
            auto& d = (*m_descriptors)[i];
            d.binding = bnd.binding;
            d.descriptorType = bnd.type;
            d.info = core::make_refctd_dynamic_array<core::smart_refctd_dynamic_array<SDescriptorInfo>>(bnd.count);
            ++i;
        }
        for (i = 0u; i < m_descriptors->size(); ++i)
            (*m_bindingToIx)[(*m_descriptors)[i].binding] = i;
    }

    //leaving this ctor below for sake of faster cpu->gpu conversions
    /**
    @param _layout Bindings in layout must go in the same order as corresponding descriptors (SDescriptorBinding) in `_descriptors` parameter (this requirement should be probably dropped in the future)
    @param _descriptors Entries must be sorted by binding number
    */
    IDescriptorSet(core::smart_refctd_ptr<LayoutType>&& _layout, core::smart_refctd_dynamic_array<SDescriptorBinding>&& _descriptors) :
        m_layout(std::move(_layout)), m_descriptors(std::move(_descriptors)),
        m_bindingToIx(core::make_refctd_dynamic_array<core::smart_refctd_dynamic_array<uint32_t>>((m_layout->getBindings().end()-1)->binding + 1u))
    {
        for (auto& item : (*m_bindingToIx))
            item = (~0u);

        auto is_not_sorted = [this] {
            for (auto it = m_descriptors->cbegin()+1; it != m_descriptors->cend(); ++it)
                if (it->binding <= (it-1)->binding)
                    return false;
            return true;
        };
        assert(!is_not_sorted());

        for (uint32_t i = 0u; i < m_descriptors->size(); ++i)
            (*m_bindingToIx)[(*m_descriptors)[i].binding] = i;
    }

protected:
    virtual ~IDescriptorSet() = default;

    core::smart_refctd_ptr<LayoutType> m_layout;
    core::smart_refctd_dynamic_array<SDescriptorBinding> m_descriptors;
    core::smart_refctd_dynamic_array<uint32_t> m_bindingToIx;
};

}}

#endif