/*
 * Source code for the NPGR019 lab practices. Copyright Martin Kahoun 2021.
 * Licensed under the zlib license, see LICENSE.txt in the root directory.
 */

#include "shaders.h"

GLuint shaderProgram[ShaderProgram::NumShaderPrograms] = {0};

bool compileShaders()
{
  GLuint vertexShader[VertexShader::NumVertexShaders] = {0};
  GLuint fragmentShader[FragmentShader::NumFragmentShaders] = {0};
  GLuint geometryShader[FragmentShader::NumFragmentShaders] = {0};

  // Cleanup lambda
  auto cleanUp = [&]()
  {
    for (int i = 0; i < VertexShader::NumVertexShaders; ++i)
    {
      if (glIsShader(vertexShader[i]))
        glDeleteShader(vertexShader[i]);
    }

    for (int i = 0; i < FragmentShader::NumFragmentShaders; ++i)
    {
      if (glIsShader(fragmentShader[i]))
        glDeleteShader(fragmentShader[i]);
    }

    for (int i = 0; i < GeometryShader::NumGeometryShaders; ++i)
    {
      if (glIsShader(geometryShader[i]))
        glDeleteShader(geometryShader[i]);
    }
  };

  // Compile all vertex shaders
  for (int i = 0; i < VertexShader::NumVertexShaders; ++i)
  {
    vertexShader[i] = ShaderCompiler::CompileShader(vsSource, i, GL_VERTEX_SHADER);
    if (!vertexShader[i])
    {
      cleanUp();
      return false;
    }
  }

  // Compile all fragment shaders
  for (int i = 0; i < FragmentShader::NumFragmentShaders; ++i)
  {
    fragmentShader[i] = ShaderCompiler::CompileShader(fsSource, i, GL_FRAGMENT_SHADER);
    if (!fragmentShader[i])
    {
      cleanUp();
      return false;
    }
  }

  // Compile all geomeyry shaders
  for (int i = 0; i < GeometryShader::NumGeometryShaders; ++i)
  {
    geometryShader[i] = ShaderCompiler::CompileShader(gsSource, i, GL_GEOMETRY_SHADER);
    if (!geometryShader[i])
    {
      cleanUp();
      return false;
    }
  }

  // Shader program for non-instanced geometry w/ color
  shaderProgram[ShaderProgram::Default] = glCreateProgram();
  glAttachShader(shaderProgram[ShaderProgram::Default], vertexShader[VertexShader::Default]);
  glAttachShader(shaderProgram[ShaderProgram::Default], fragmentShader[FragmentShader::Default]);
  if (!ShaderCompiler::LinkProgram(shaderProgram[ShaderProgram::Default]))
  {
    cleanUp();
    return false;
  }

  // Shader program for non-instanced geometry w/o color
  shaderProgram[ShaderProgram::DefaultDepthPass] = glCreateProgram();
  glAttachShader(shaderProgram[ShaderProgram::DefaultDepthPass], vertexShader[VertexShader::Default]);
  glAttachShader(shaderProgram[ShaderProgram::DefaultDepthPass], fragmentShader[FragmentShader::Null]);
  if (!ShaderCompiler::LinkProgram(shaderProgram[ShaderProgram::DefaultDepthPass]))
  {
    cleanUp();
    return false;
  }

  // Shader program for instanced geometry w/ color
  shaderProgram[ShaderProgram::Instancing] = glCreateProgram();
  glAttachShader(shaderProgram[ShaderProgram::Instancing], vertexShader[VertexShader::Instancing]);
  glAttachShader(shaderProgram[ShaderProgram::Instancing], fragmentShader[FragmentShader::Default]);
  if (!ShaderCompiler::LinkProgram(shaderProgram[ShaderProgram::Instancing]))
  {
    cleanUp();
    return false;
  }

  // Shader program for instanced geometry w/o color
  shaderProgram[ShaderProgram::InstancingDepthPass] = glCreateProgram();
  glAttachShader(shaderProgram[ShaderProgram::InstancingDepthPass], vertexShader[VertexShader::Instancing]);
  glAttachShader(shaderProgram[ShaderProgram::InstancingDepthPass], fragmentShader[FragmentShader::Null]);
  if (!ShaderCompiler::LinkProgram(shaderProgram[ShaderProgram::InstancingDepthPass]))
  {
    cleanUp();
    return false;
  }

  // Shader program for instanced geometry w/ shadow volume extrusion
  shaderProgram[ShaderProgram::InstancedShadowVolume] = glCreateProgram();
  glAttachShader(shaderProgram[ShaderProgram::InstancedShadowVolume], vertexShader[VertexShader::InstancedShadowVolume]);
  glAttachShader(shaderProgram[ShaderProgram::InstancedShadowVolume], geometryShader[GeometryShader::ShadowVolume]);
  glAttachShader(shaderProgram[ShaderProgram::InstancedShadowVolume], fragmentShader[FragmentShader::Null]);
  if (!ShaderCompiler::LinkProgram(shaderProgram[ShaderProgram::InstancedShadowVolume]))
  {
    cleanUp();
    return false;
  }

  // Shader program for point rendering w/ constant color
  shaderProgram[ShaderProgram::PointRendering] = glCreateProgram();
  glAttachShader(shaderProgram[ShaderProgram::PointRendering], vertexShader[VertexShader::Point]);
  glAttachShader(shaderProgram[ShaderProgram::PointRendering], fragmentShader[FragmentShader::SingleColor]);
  if (!ShaderCompiler::LinkProgram(shaderProgram[ShaderProgram::PointRendering]))
  {
    cleanUp();
    return false;
  }

  // Shader program for rendering tonemapping post-process
  shaderProgram[ShaderProgram::Tonemapping] = glCreateProgram();
  glAttachShader(shaderProgram[ShaderProgram::Tonemapping], vertexShader[VertexShader::ScreenQuad]);
  glAttachShader(shaderProgram[ShaderProgram::Tonemapping], fragmentShader[FragmentShader::Tonemapping]);
  if (!ShaderCompiler::LinkProgram(shaderProgram[ShaderProgram::Tonemapping]))
  {
    cleanUp();
    return false;
  }

  cleanUp();
  return true;
}
