// Copyright (C) 2018-2020 - DevSH Graphics Programming Sp. z O.O.
// This file is part of the "Nabla Engine".
// For conditions of distribution and use, see copyright notice in nabla.h

#version 430 core

#include "commonVertexShader.glsl"

layout(push_constant) uniform PushConstants
{
    uint objectUUID;
} pc;

void main()
{
	impl(pc.objectUUID);
}
