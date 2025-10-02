
// MS: New GLSLShader class

#ifndef GLSLSHADER_H_
#define GLSLSHADER_H_

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "glad/gl.h"
#include "msutil.h"

// #define NOTEXTURE
// #ifndef NOTEXTURE
// #include "GLTexture.h"
// #else
// #define Texture2D void*
// #endif

class GLSLException : public std::runtime_error {
 public:
  explicit GLSLException(const std::string& msg) : std::runtime_error(msg) {}
};

// NOLINTBEGIN
typedef enum {
  VERTEXSHADER = GL_VERTEX_SHADER,
  FRAGMENTSHADER = GL_FRAGMENT_SHADER,
} shader_t;
// NOLINTEND

std::string ShaderTypeToString(shader_t shader_type);

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class Shader {
 private:
  std::string src_file_path_;
  shader_t shader_type_;
  GLuint id_;
  std::string src_;

 public:
  Shader(const std::string& src_file_path, shader_t shader_type);

  ~Shader() {
    glDeleteShader(id_);
    ms_util::GLErrorCheck();
  }

  std::string SrcFilePath() { return src_file_path_; }

  shader_t ShaderType() { return shader_type_; }

  GLuint Id() const { return id_; }

  std::string Src() { return src_; }

  bool CompileShader();
};

class VertexShader : public Shader {
 public:
  explicit VertexShader(const std::string& src_file_path)
      : Shader(src_file_path, VERTEXSHADER) {}
};

class FragmentShader : public Shader {
 public:
  explicit FragmentShader(const std::string& src_file_path)
      : Shader(src_file_path, FRAGMENTSHADER) {}
};

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class GLSLProgram {
 private:
  GLuint id_;
  bool linked_;
  std::map<std::string, GLint> uniform_locations_;

  // Texture2D *_texture;

 public:
  GLSLProgram();

  virtual ~GLSLProgram();

  GLuint Id() const { return id_; }

  bool Attach(FragmentShader& fs) const {
    glAttachShader(id_, fs.Id());
    return (!ms_util::GLErrorCheck());
  }

  bool Attach(VertexShader& vs) const {
    glAttachShader(id_, vs.Id());
    return (!ms_util::GLErrorCheck());
  }

  bool DetachAll() const;

  bool Detach(GLuint shader_id) const {
    glDetachShader(id_, shader_id);
    return (!ms_util::GLErrorCheck());
  }

  bool Detach(FragmentShader& fs) const {
    glDetachShader(id_, fs.Id());
    return (!ms_util::GLErrorCheck());
  }

  bool Detach(VertexShader& vs) const {
    glDetachShader(id_, vs.Id());
    return (!ms_util::GLErrorCheck());
  }

  void Link() noexcept(false);

  virtual bool ActivateUniforms() { return false; }

  bool Activate();

  static bool Deactivate();

  // void AttachTexture(Texture2D* texture) { _texture = texture; }

  // Texture2D* texture() { return _texture; }

  bool IsActive() const;

  void FindUniformLocations();

  int GetUniformLocation(const char* name);

  void SetUniform(const char* name, float scalar);

  void SetUniform(const char* name, const glm::vec3& vec);

  void SetUniform(const char* name, const glm::vec4& vec);

  void SetUniform(const char* name, const glm::mat3& mat);

  void SetUniform(const char* name, const glm::mat4& mat);

  void PrintActiveUniforms() const;

  void PrintActiveUniformBlocks() const;

  void PrintActiveAttribs() const;
};

bool LoadShaderProgram(GLSLProgram& shader_program,
                       const std::string& vertex_shader_source_path,
                       const std::string& fragement_shader_source_path);

#endif
