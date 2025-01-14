// Copyright (C) 2018-2020 - DevSH Graphics Programming Sp. z O.O.
// This file is part of the "Nabla Engine".
// For conditions of distribution and use, see copyright notice in nabla.h

#version 460 core

	#define NBL_IMPL_GL_NV_viewport_array2
	#define NBL_IMPL_GL_NV_stereo_view_rendering
	#define NBL_IMPL_GL_NV_sample_mask_override_coverage
	#define NBL_IMPL_GL_NV_geometry_shader_passthrough
	#define NBL_IMPL_GL_NV_shader_subgroup_partitioned
	#define NBL_IMPL_GL_ARB_shading_language_include
	#define NBL_IMPL_GL_ARB_enhanced_layouts
	#define NBL_IMPL_GL_ARB_bindless_texture
	#define NBL_IMPL_GL_ARB_shader_draw_parameters
	#define NBL_IMPL_GL_ARB_shader_group_vote
	#define NBL_IMPL_GL_ARB_cull_distance
	#define NBL_IMPL_GL_ARB_derivative_control
	#define NBL_IMPL_GL_ARB_shader_texture_image_samples
	#define NBL_IMPL_GL_KHR_blend_equation_advanced
	#define NBL_IMPL_GL_KHR_blend_equation_advanced_coherent
	#define NBL_IMPL_GL_ARB_fragment_shader_interlock
	#define NBL_IMPL_GL_ARB_gpu_shader_int64
	#define NBL_IMPL_GL_ARB_post_depth_coverage
	#define NBL_IMPL_GL_ARB_shader_ballot
	#define NBL_IMPL_GL_ARB_shader_clock
	#define NBL_IMPL_GL_ARB_shader_viewport_layer_array
	#define NBL_IMPL_GL_ARB_sparse_texture2
	#define NBL_IMPL_GL_ARB_sparse_texture_clamp
	#define NBL_IMPL_GL_ARB_gl_spirv
	#define NBL_IMPL_GL_ARB_spirv_extensions
	#define NBL_IMPL_GL_AMD_vertex_shader_viewport_index
	#define NBL_IMPL_GL_AMD_vertex_shader_layer
	#define NBL_IMPL_GL_NV_bindless_texture
	#define NBL_IMPL_GL_NV_shader_atomic_float
	#define NBL_IMPL_GL_EXT_shader_integer_mix
	#define NBL_IMPL_GL_NV_shader_thread_group
	#define NBL_IMPL_GL_NV_shader_thread_shuffle
	#define NBL_IMPL_GL_EXT_shader_image_load_formatted
	#define NBL_IMPL_GL_NV_shader_atomic_int64
	#define NBL_IMPL_GL_EXT_post_depth_coverage
	#define NBL_IMPL_GL_EXT_sparse_texture2
	#define NBL_IMPL_GL_NV_fragment_shader_interlock
	#define NBL_IMPL_GL_NV_sample_locations
	#define NBL_IMPL_GL_NV_shader_atomic_fp16_vector
	#define NBL_IMPL_GL_NV_command_list
	#define NBL_IMPL_GL_OVR_multiview
	#define NBL_IMPL_GL_OVR_multiview2
	#define NBL_IMPL_GL_NV_shader_atomic_float64
	#define NBL_IMPL_GL_NV_gpu_shader5

#ifdef NBL_IMPL_GL_AMD_gpu_shader_half_float
#define NBL_GL_EXT_shader_explicit_arithmetic_types_float16
#endif

#ifdef NBL_IMPL_GL_NV_gpu_shader5
#define NBL_GL_EXT_shader_explicit_arithmetic_types_float16
#define NBL_GL_EXT_nonuniform_qualifier
#define NBL_GL_KHR_shader_subgroup_vote_subgroup_any_all_equal_bool
#endif

#ifdef NBL_IMPL_GL_AMD_gpu_shader_int16
#define NBL_GL_EXT_shader_explicit_arithmetic_types_int16
#endif

#ifdef NBL_IMPL_GL_NV_shader_thread_group
#define NBL_GL_KHR_shader_subgroup_ballot_subgroup_mask
#define NBL_GL_KHR_shader_subgroup_basic_subgroup_size
#define NBL_GL_KHR_shader_subgroup_basic_subgroup_invocation_id
#define NBL_GL_KHR_shader_subgroup_ballot_subgroup_ballot
#define NBL_GL_KHR_shader_subgroup_ballot_inverse_ballot_bit_count
#endif

#if defined(NBL_IMPL_GL_ARB_shader_ballot) && defined(NBL_IMPL_GL_ARB_shader_int64)
#define NBL_GL_KHR_shader_subgroup_ballot_subgroup_mask
#define NBL_GL_KHR_shader_subgroup_basic_subgroup_size
#define NBL_GL_KHR_shader_subgroup_basic_subgroup_invocation_id
#define NBL_GL_KHR_shader_subgroup_ballot_subgroup_broadcast_first
#define NBL_GL_KHR_shader_subgroup_ballot_subgroup_ballot
#define NBL_GL_KHR_shader_subgroup_ballot_inverse_ballot_bit_count
#endif

#if defined(NBL_IMPL_GL_AMD_gcn_shader) && (defined(NBL_IMPL_GL_AMD_gpu_shader_int64) || defined(NBL_IMPL_GL_NV_gpu_shader5))
#define NBL_GL_KHR_shader_subgroup_basic_subgroup_size
#define NBL_GL_KHR_shader_subgroup_vote_subgroup_any_all_equal_bool
#endif

#ifdef NBL_IMPL_GL_NV_shader_thread_shuffle
#define NBL_GL_KHR_shader_subgroup_ballot_subgroup_broadcast_first
#endif

#ifdef NBL_IMPL_GL_ARB_shader_group_vote
#define NBL_GL_KHR_shader_subgroup_vote_subgroup_any_all_equal_bool
#endif

#if defined(NBL_GL_KHR_shader_subgroup_ballot_subgroup_broadcast_first) && defined(NBL_GL_KHR_shader_subgroup_vote_subgroup_any_all_equal_bool)
#define NBL_GL_KHR_shader_subgroup_vote_subgroup_all_equal_T
#endif

#if defined(NBL_GL_KHR_shader_subgroup_ballot_subgroup_ballot) && defined(NBL_GL_KHR_shader_subgroup_basic_subgroup_invocation_id)
#define NBL_GL_KHR_shader_subgroup_basic_subgroup_elect
#endif

#ifdef NBL_GL_KHR_shader_subgroup_ballot_subgroup_mask
#define NBL_GL_KHR_shader_subgroup_ballot_inverse_ballot
#define NBL_GL_KHR_shader_subgroup_ballot_inclusive_bit_count
#define NBL_GL_KHR_shader_subgroup_ballot_exclusive_bit_count
#endif

#ifdef NBL_GL_KHR_shader_subgroup_ballot_subgroup_ballot
#define NBL_GL_KHR_shader_subgroup_ballot_bit_count
#endif
#line 6
#define _NBL_VERT_INPUTS_DEFINED_
#define _NBL_VERT_MAIN_DEFINED_
#define _NBL_VERT_OUTPUTS_DEFINED_
#define _NBL_VERT_SET1_BINDINGS_DEFINED_

vec4 nbl_glsl_decodeRGB10A2(in uint x)
{
	const uvec3 rgbMask = uvec3(0x3ffu);
	const uvec4 shifted = uvec4(x,uvec3(x)>>uvec3(10,20,30));
	return vec4(vec3(shifted.rgb&rgbMask),shifted.a)/vec4(vec3(rgbMask),3.0);
}

//pos
layout(set = 0, binding = 0) uniform samplerBuffer MeshPackedData_R32G32B32_SFLOAT;

//uv
layout(set = 0, binding = 1) uniform samplerBuffer MeshPackedData_R32G32_SFLOAT;

//normal
layout(set = 0, binding = 2) uniform usamplerBuffer MeshPackedData_A2B10G10R10_SNORM_PACK32;

layout(set = 0, binding = 3) readonly buffer VertexDataOffsetTable
{
    int dataOffsetTable[];
} vertexPosition;

layout(set = 0, binding = 4) readonly buffer VertexNormalOffsetTable
{
    int dataOffsetTable[];
} vertexNormal;

layout(push_constant, row_major) uniform PushConstants
{
	mat4 vp;
} pc;

//outputs
layout(location = 0) out vec3 normal;



#include "nbl/builtin/shader/loader/mtl/vertex_impl.glsl"

void main()
{
    int vtxPosOffset = int(gl_VertexIndex) + vertexPosition.dataOffsetTable[gl_DrawID];
    vec3 pos = texelFetch(MeshPackedData_R32G32B32_SFLOAT, vtxPosOffset).xyz;
    gl_Position = nbl_glsl_pseudoMul4x4with3x1(pc.vp, pos);
    
    int vtxNormOffset = int(gl_VertexIndex) + vertexNormal.dataOffsetTable[gl_DrawID];
    normal = normalize(nbl_glsl_decodeRGB10A2(texelFetch(MeshPackedData_A2B10G10R10_SNORM_PACK32, vtxNormOffset).x).xyz);
    
    //vertex outputs
    //LocalPos = pos;
    //ViewPos = nbl_glsl_pseudoMul3x4with3x1(CamData.params.MV, pos);
    
    //mat3 normalMat = nbl_glsl_SBasicViewParameters_GetNormalMat(CamData.params.NormalMatAndEyePos);

    //vec3 normal = texelFetch(MeshPackedData_A2B10G10R10_SNORM_PACK32, int(gl_VertexIndex)+vertexNormal.dataOffsetTable[gl_DrawID]).xyz;
    //Normal = normalMat*normalize(vNormal);
    //Normal = vec3(1.0, 0.0, 0.0);
}
 