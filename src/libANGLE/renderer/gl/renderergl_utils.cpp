//
// Copyright (c) 2012-2014 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// renderer11_utils.cpp: Conversion functions and other utility routines
// specific to the OpenGL renderer.

#include "libANGLE/renderer/gl/renderergl_utils.h"

#include <limits>

#include "libANGLE/Caps.h"
#include "libANGLE/renderer/gl/FunctionsGL.h"

namespace rx
{

namespace nativegl_gl
{

static GLint QuerySingleGLInt(const FunctionsGL *functions, GLenum name)
{
    GLint result;
    functions->getIntegerv(name, &result);
    return result;
}

void GenerateCaps(const FunctionsGL *functions, gl::Caps *caps, gl::TextureCapsMap *textureCapsMap,
                  gl::Extensions *extensions)
{
    // Set some minimum GLES2 caps, TODO: query for real GL caps

    // Table 6.28, implementation dependent values
    caps->maxElementIndex = static_cast<GLint64>(std::numeric_limits<unsigned int>::max());
    caps->max3DTextureSize = QuerySingleGLInt(functions, GL_MAX_3D_TEXTURE_SIZE);
    caps->max2DTextureSize = QuerySingleGLInt(functions, GL_MAX_TEXTURE_SIZE);
    caps->maxCubeMapTextureSize = QuerySingleGLInt(functions, GL_MAX_CUBE_MAP_TEXTURE_SIZE);
    caps->maxArrayTextureLayers = QuerySingleGLInt(functions, GL_MAX_ARRAY_TEXTURE_LAYERS);
    caps->maxLODBias = 2.0f;
    caps->maxRenderbufferSize = 2048;
    caps->maxDrawBuffers = QuerySingleGLInt(functions, GL_MAX_DRAW_BUFFERS);
    caps->maxColorAttachments = QuerySingleGLInt(functions, GL_MAX_COLOR_ATTACHMENTS);
    caps->maxViewportWidth = caps->max2DTextureSize;
    caps->maxViewportHeight = caps->maxViewportWidth;
    caps->minAliasedPointSize = 1.0f;
    caps->maxAliasedPointSize = 1.0f;
    caps->minAliasedLineWidth = 1.0f;
    caps->maxAliasedLineWidth = 1.0f;

    // Table 6.29, implementation dependent values (cont.)
    caps->maxElementsIndices = 0;
    caps->maxElementsVertices = 0;
    caps->vertexHighpFloat.setIEEEFloat();
    caps->vertexMediumpFloat.setIEEEFloat();
    caps->vertexLowpFloat.setIEEEFloat();
    caps->fragmentHighpFloat.setIEEEFloat();
    caps->fragmentMediumpFloat.setIEEEFloat();
    caps->fragmentLowpFloat.setIEEEFloat();
    caps->vertexHighpInt.setTwosComplementInt(32);
    caps->vertexMediumpInt.setTwosComplementInt(32);
    caps->vertexLowpInt.setTwosComplementInt(32);
    caps->fragmentHighpInt.setTwosComplementInt(32);
    caps->fragmentMediumpInt.setTwosComplementInt(32);
    caps->fragmentLowpInt.setTwosComplementInt(32);
    caps->maxServerWaitTimeout = 0;

    // Table 6.31, implementation dependent vertex shader limits
    caps->maxVertexAttributes = 16;
    caps->maxVertexUniformVectors = 1024;
    caps->maxVertexUniformVectors = 256;
    caps->maxVertexUniformBlocks = 12;
    caps->maxVertexOutputComponents = 64;
    caps->maxVertexTextureImageUnits = QuerySingleGLInt(functions, GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS);

    // Table 6.32, implementation dependent fragment shader limits
    caps->maxFragmentUniformComponents = 896;
    caps->maxFragmentUniformVectors = 224;
    caps->maxFragmentUniformBlocks = 12;
    caps->maxFragmentInputComponents = 60;
    caps->maxTextureImageUnits = QuerySingleGLInt(functions, GL_MAX_TEXTURE_IMAGE_UNITS);
    caps->minProgramTexelOffset = -8;
    caps->maxProgramTexelOffset = 7;

    // Table 6.33, implementation dependent aggregate shader limits
    caps->maxUniformBufferBindings = 24;
    caps->maxUniformBlockSize = 16384;
    caps->uniformBufferOffsetAlignment = 1;
    caps->maxCombinedUniformBlocks = 24;
    caps->maxCombinedVertexUniformComponents = (caps->maxVertexUniformBlocks * (caps->maxUniformBlockSize / 4)) + caps->maxVertexUniformComponents;
    caps->maxCombinedFragmentUniformComponents = (caps->maxFragmentUniformBlocks * (caps->maxUniformBlockSize / 4)) + caps->maxFragmentUniformComponents;
    caps->maxVaryingComponents = 60;
    caps->maxVaryingVectors = 15;
    caps->maxCombinedTextureImageUnits = caps->maxVertexTextureImageUnits + caps->maxTextureImageUnits;

    // Table 6.34, implementation dependent transform feedback limits
    caps->maxTransformFeedbackInterleavedComponents = 64;
    caps->maxTransformFeedbackSeparateAttributes = 4;
    caps->maxTransformFeedbackSeparateComponents = 4;

    // Texture Caps
    gl::TextureCaps supportedTextureFormat;
    supportedTextureFormat.texturable = true;
    supportedTextureFormat.filterable = true;
    supportedTextureFormat.renderable = true;

    textureCapsMap->insert(GL_RGB565, supportedTextureFormat);
    textureCapsMap->insert(GL_RGBA4, supportedTextureFormat);
    textureCapsMap->insert(GL_RGB5_A1, supportedTextureFormat);
    textureCapsMap->insert(GL_RGB8_OES, supportedTextureFormat);
    textureCapsMap->insert(GL_RGBA8_OES, supportedTextureFormat);

    textureCapsMap->insert(GL_DEPTH_COMPONENT16, supportedTextureFormat);
    textureCapsMap->insert(GL_STENCIL_INDEX8, supportedTextureFormat);

    // Extension support
    extensions->setTextureExtensionSupport(*textureCapsMap);
    extensions->textureNPOT = true;
    extensions->textureStorage = true;
}

}

}
