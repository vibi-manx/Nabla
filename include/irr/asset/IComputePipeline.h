#ifndef __IRR_I_COMPUTE_PIPELINE_H_INCLUDED__
#define __IRR_I_COMPUTE_PIPELINE_H_INCLUDED__

#include <utility>
#include "irr/asset/IPipeline.h"
#include "irr/asset/ShaderCommons.h"

namespace irr {
namespace asset
{

template<typename SpecShaderType, typename LayoutType>
class IComputePipeline : public IPipeline<LayoutType>
{
public:
    IComputePipeline(
        core::smart_refctd_ptr<IComputePipeline>&& _parent,
        core::smart_refctd_ptr<LayoutType>&& _layout,
        core::smart_refctd_ptr<SpecShaderType>&& _cs
    ) : IPipeline<LayoutType>(std::move(_parent),std::move(_layout)),
        m_shader(std::move(_cs))
    {
        assert(m_shader->getStage() == ESS_COMPUTE);
    }

protected:
    virtual ~IComputePipeline() = default;

    core::smart_refctd_ptr<SpecShaderType> m_shader;

public:
    const SpecShaderType* getShader() const { return m_shader.get(); }
};

}}


#endif