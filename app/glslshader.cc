
#include "glslshader.h"

#include <array>

void GetInfoLog(GLuint id, std::string& log) {
  GLint info_log_length{0};
  glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length > 0) {
    std::cout << "info log length: " << info_log_length << "\n";
    GLsizei written{0};
#if __cplusplus >= 201703L
    // C++ 17
    // std::cout << "C++ 17\n";
    // glGetShaderInfoLog(id, info_log_length, &written, &log[0]);
    glGetShaderInfoLog(id, info_log_length, &written, log.data());
#else
    // Pre-C++ 17
    std::vector<char> log_vector(info_log_length);
    glGetShaderInfoLog(id, info_log_length, &written, log_vector.data());
    log = std::string(begin(log_vector), end(log_vector));
#endif
  } else {
    log = "[Info log is zero length.]";
  }
}

const char* GetTypeString(GLenum type) {
  // There are many more types than are covered here, but
  // these are the most common in these examples.
  switch (type) {
    case GL_FLOAT:
      return "float";
    case GL_FLOAT_VEC2:
      return "vec2";
    case GL_FLOAT_VEC3:
      return "vec3";
    case GL_FLOAT_VEC4:
      return "vec4";
    case GL_DOUBLE:
      return "double";
    case GL_INT:
      return "int";
    case GL_UNSIGNED_INT:
      return "unsigned int";
    case GL_BOOL:
      return "bool";
    case GL_FLOAT_MAT2:
      return "mat2";
    case GL_FLOAT_MAT3:
      return "mat3";
    case GL_FLOAT_MAT4:
      return "mat4";
    default:
      return "?";
  }
}

std::string ShaderTypeToString(shader_t shader_type) {
  std::string type_string{"Unknown"};
  switch (shader_type) {
    case VERTEXSHADER:
      type_string = "Vertex Shader";
      break;
    case FRAGMENTSHADER:
      type_string = "Fragment Shader";
      break;
    default:
      std::ostringstream msg;
      msg << "Could not identify the shader type of " << shader_type << ".\n";
      throw GLSLException(msg.str());
      break;
  }
  return type_string;
}

Shader::Shader(const std::string& src_file_path, shader_t shader_type)
    : src_file_path_{src_file_path}, shader_type_{shader_type}, id_{0} {
  if (!ms_util::FileToString(src_file_path_, src_)) {
    std::ostringstream msg;
    msg << "Could not open and load shader source path " << src_file_path_
        << ".\n";
    // fprintf(stderr, "%s\n", msg.str().c_str());
    throw GLSLException(msg.str());
  }
  id_ = glCreateShader(shader_type_);
  if (id_ == 0) {
    // fprintf(stderr, "Can't generate vertex shader.\n");
    std::ostringstream msg;
    msg << "Can't create a " << ShaderTypeToString(shader_type_) << ".\n";
    // fprintf(stderr, "%s\n", msg.str().c_str());
    throw GLSLException(msg.str());
  }
  ms_util::GLErrorCheck();
  CompileShader();
  ms_util::GLErrorCheck();
}

bool Shader::CompileShader() {
  GLint compiled_ok{-1};
  const GLint length{static_cast<GLint>(src_.size())};
  const char* c_str{src_.c_str()};
  glShaderSource(id_, 1, &c_str, &length);
  glCompileShader(id_);
  ms_util::GLErrorCheck();
  glGetShaderiv(id_, GL_COMPILE_STATUS, &compiled_ok);
  ms_util::GLErrorCheck();
  if (compiled_ok == 0) {
    // fprintf(stderr, "Compilation failed for shader %s\n", src_file_path_);
    std::string log_msg;
    GetInfoLog(id_, log_msg);
    // fprintf(stderr, "Info Log:\n%s\n", log_msg.c_str());
    std::ostringstream msg;
    msg << "Compilation failed for shader " << src_file_path_
        << ".\nInfo Log:\n"
        << log_msg << "\n";
    // fprintf(stderr, "%s\n", msg.str().c_str());
    throw GLSLException(msg.str());
    // char *msg  = GetInfoLog( );
    // fprintf( stderr, "Info Log:\n%s\n", msg );
    // free( msg );
  }
  return (compiled_ok == 1);
}

GLSLProgram::GLSLProgram() : id_{glCreateProgram()}, linked_{false} {
  // id_ = glCreateProgram();
  if (id_ == 0) {
    // fprintf(stderr, "Can't generate GLSL program. Exiting.\n");
    std::ostringstream msg;
    msg << "Can't generate GLSL program.\n";
    // fprintf(stderr, "%s\n", msg.str().c_str());
    throw GLSLException(msg.str());
  }
  // _texture = NULL;
}

GLSLProgram::~GLSLProgram() {
  if (id_ == 0) {
    return;
  }
  DetachAll();
  glDeleteProgram(id_);
}

bool GLSLProgram::DetachAll() const {
  bool ret{true};
  GLsizei const max_count{32};
  GLsizei count{0};
  // GLuint shaders[32];
  std::array<GLuint, 32> shaders{};
  glGetAttachedShaders(id_, max_count, &count, shaders.data());
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
  for (int i = 0; i < count; i++) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
    if (Detach(shaders[i])) {
      ret = false;
      break;
    }
  }
  return (ret);
}

void GLSLProgram::Link() noexcept(false) {
  GLint linked_ok{-1};
  // bool ret{true};

  glLinkProgram(id_);

  glGetProgramiv(id_, GL_LINK_STATUS, &linked_ok);
  if (linked_ok == 0) {
    // ret = false;
    // fprintf(stderr, "Linking failed.\n");
    std::string log_msg;
    GetInfoLog(id_, log_msg);
    // fprintf(stderr, "Info Log:\n%s\n", log_msg.c_str());
    std::ostringstream msg;
    msg << "Linking failed.\nInfo Log:\n" << log_msg << "\n";
    // fprintf(stderr, "%s\n", msg.str().c_str());
    std::cerr << msg.str().c_str() << "\n";
    throw GLSLException(msg.str());

    // msg = GetInfoLog();
    // fprintf(stderr, "%s\n", msg);
    // free(msg);
  }
  linked_ = true;
}

bool GLSLProgram::Activate() {
  ActivateUniforms();
  ms_util::GLErrorCheck();
  glUseProgram(id_);
  return (!ms_util::GLErrorCheck());
}

bool GLSLProgram::Deactivate() {
  glUseProgram(0);
  return (!ms_util::GLErrorCheck());
}

bool GLSLProgram::IsActive() const {
  int id{-1};
  glGetIntegerv(GL_CURRENT_PROGRAM, &id);
  return (id == id_);
}

void GLSLProgram::FindUniformLocations() {
  uniform_locations_.clear();
  GLint num_uniforms{0};
  // For OpenGL 3.3 or 4.1, use glGetActiveUniform
  // OpenGL 4.3 and above, use glGetProgramResource

  GLint max_length{-1};
  GLchar* name{nullptr};

  glGetProgramiv(id_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length);
  glGetProgramiv(id_, GL_ACTIVE_UNIFORMS, &num_uniforms);

  name = new GLchar[max_length];
  for (GLuint i = 0; i < num_uniforms; i++) {
    GLint size{0};
    GLenum type{0};
    GLsizei written{0};
    glGetActiveUniform(id_, i, max_length, &written, &size, &type, name);
    const GLint location{glGetUniformLocation(id_, name)};
    uniform_locations_[name] = location;
  }
  delete[] name;
}

int GLSLProgram::GetUniformLocation(const char* name) {
  auto pos = uniform_locations_.find(name);
  if (pos == uniform_locations_.end()) {
    const GLint loc{glGetUniformLocation(id_, name)};
    uniform_locations_[name] = loc;
    return loc;
  }
  return pos->second;
}

void GLSLProgram::SetUniform(const char* name, const float scalar) {
  const GLint loc{GetUniformLocation(name)};
  glUniform1f(loc, scalar);
}

void GLSLProgram::SetUniform(const char* name, const glm::vec3& vec) {
  const GLint loc{GetUniformLocation(name)};
  glUniform3fv(loc, 1, glm::value_ptr(vec));
}
void GLSLProgram::SetUniform(const char* name, const glm::vec4& vec) {
  const GLint loc{GetUniformLocation(name)};
  glUniform4fv(loc, 1, glm::value_ptr(vec));
}
void GLSLProgram::SetUniform(const char* name, const glm::mat3& mat) {
  const GLint loc{GetUniformLocation(name)};
  glUniformMatrix3fv(loc, 1, GL_FALSE, &mat[0][0]);
}
void GLSLProgram::SetUniform(const char* name, const glm::mat4& mat) {
  const GLint loc{GetUniformLocation(name)};
  glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void GLSLProgram::PrintActiveUniforms() const {
#ifdef __APPLE__
  // For OpenGL 4.1, use glGetActiveUniform
  GLint num_uniforms{0};
  GLint size{0};
  GLint location{0};
  GLint max_len{0};
  GLchar* name{nullptr};
  GLsizei written{0};
  GLenum type{0};

  glGetProgramiv(id_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_len);
  glGetProgramiv(id_, GL_ACTIVE_UNIFORMS, &num_uniforms);

  name = new GLchar[max_len];

  std::cout << "Active uniforms:\n";
  std::cout << "------------------------------------------------\n";
  for (GLuint i = 0; i < num_uniforms; ++i) {
    glGetActiveUniform(id_, i, max_len, &written, &size, &type, name);
    location = glGetUniformLocation(id_, name);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    printf(" %-5d %s (%s)\n", location, name, GetTypeString(type));
  }
  std::cout << "------------------------------------------------\n";
  delete[] name;
#else
  // For OpenGL 4.3 and above, use glGetProgramResource
  GLint num_uniforms{0};
  glGetProgramInterfaceiv(id_, GL_UNIFORM, GL_ACTIVE_RESOURCES, &num_uniforms);

  GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX};

  printf("Active uniforms:\n");
  std::cout << "------------------------------------------------\n";
  for (int i = 0; i < num_uniforms; ++i) {
    GLint results[4];
    glGetProgramResourceiv(id_, GL_UNIFORM, i, 4, properties, 4, NULL, results);

    if (results[3] != -1) continue;  // Skip uniforms in blocks
    GLint nameBufSize = results[0] + 1;
    char* name = new char[nameBufSize];
    glGetProgramResourceName(id_, GL_UNIFORM, i, nameBufSize, NULL, name);
    printf("%-5d %s (%s)\n", results[2], name, GetTypeString(results[1]));
    delete[] name;
  }
  std::cout << "------------------------------------------------\n";
#endif
}

void GLSLProgram::PrintActiveUniformBlocks() const {
#ifdef __APPLE__
  // For OpenGL 4.1, use glGetActiveUniformBlockiv
  GLint written{0};
  GLint max_length{0};
  GLint max_uniform_length{0};
  GLint n_blocks{0};
  GLint binding{0};
  GLchar* name{nullptr};

  glGetProgramiv(id_, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &max_length);
  glGetProgramiv(id_, GL_ACTIVE_UNIFORM_BLOCKS, &n_blocks);
  glGetProgramiv(id_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_uniform_length);
  auto* uni_name = new GLchar[max_uniform_length];
  name = new GLchar[max_length];

  std::cout << "Active Uniform blocks: \n";
  std::cout << "------------------------------------------------\n";
  for (GLuint i = 0; i < n_blocks; i++) {
    glGetActiveUniformBlockName(id_, i, max_length, &written, name);
    glGetActiveUniformBlockiv(id_, i, GL_UNIFORM_BLOCK_BINDING, &binding);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    printf("Uniform block \"%s\" (%d):\n", name, binding);

    GLint n_unis{-1};
    glGetActiveUniformBlockiv(id_, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS,
                              &n_unis);
    auto* unif_indexes = new GLint[n_unis];
    glGetActiveUniformBlockiv(id_, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
                              unif_indexes);

    for (int unif = 0; unif < n_unis; ++unif) {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
      const GLuint uni_index = unif_indexes[unif];
      GLint size{-1};
      GLenum type{0};

      glGetActiveUniform(id_, uni_index, max_uniform_length, &written, &size,
                         &type, uni_name);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
      printf("    %s (%s)\n", name, GetTypeString(type));
    }
    delete[] unif_indexes;
  }
  std::cout << "------------------------------------------------\n";
  delete[] name;
  delete[] uni_name;
#else
  GLint num_blocks{0};

  glGetProgramInterfaceiv(id_, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES,
                          &num_blocks);
  GLenum block_props[] = {GL_NUM_ACTIVE_VARIABLES, GL_NAME_LENGTH};
  GLenum block_index[] = {GL_ACTIVE_VARIABLES};
  GLenum props[] = {GL_NAME_LENGTH, GL_TYPE, GL_BLOCK_INDEX};

  for (int block = 0; block < num_blocks; ++block) {
    GLint block_info[2];
    glGetProgramResourceiv(id_, GL_UNIFORM_BLOCK, block, 2, block_props, 2,
                           NULL, block_info);
    GLint num_unis = block_info[0];

    char* block_name = new char[block_info[1] + 1];
    glGetProgramResourceName(id_, GL_UNIFORM_BLOCK, block, block_info[1] + 1,
                             NULL, block_name);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    printf("Uniform block \"%s\":\n", block_name);
    delete[] block_name;

    GLint* unif_indexes = new GLint[num_unis];
    glGetProgramResourceiv(id_, GL_UNIFORM_BLOCK, block, 1, block_index,
                           num_unis, NULL, unif_indexes);

    for (int unif = 0; unif < num_unis; ++unif) {
      GLint uni_index = unif_indexes[unif];
      GLint results[3];
      glGetProgramResourceiv(id_, GL_UNIFORM, uni_index, 3, props, 3, NULL,
                             results);

      GLint name_buf_size = results[0] + 1;
      char* name = new char[name_buf_size];
      glGetProgramResourceName(id_, GL_UNIFORM, uni_index, name_buf_size, NULL,
                               name);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
      printf("    %s (%s)\n", name, GetTypeString(results[1]));
      delete[] name;
    }

    delete[] unif_indexes;
  }
#endif
}

void GLSLProgram::PrintActiveAttribs() const {
#ifdef __APPLE__
  // For OpenGL 4.1, use glGetActiveAttrib
  GLint written{0};
  GLint size{0};
  GLint location{0};
  GLint max_length{0};
  GLint n_attribs{0};
  GLenum type{0};
  GLchar* name{nullptr};

  glGetProgramiv(id_, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length);
  glGetProgramiv(id_, GL_ACTIVE_ATTRIBUTES, &n_attribs);

  name = new GLchar[max_length];
  std::cout << "Active Attributes: \n";
  std::cout << "------------------------------------------------\n";
  for (int i = 0; i < n_attribs; i++) {
    glGetActiveAttrib(id_, i, max_length, &written, &size, &type, name);
    location = glGetAttribLocation(id_, name);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    printf(" %-5d %s (%s)\n", location, name, GetTypeString(type));
  }
  std::cout << "------------------------------------------------\n";
  delete[] name;
#else
  // >= OpenGL 4.3, use glGetProgramResource
  GLint num_attribs{0};
  glGetProgramInterfaceiv(id_, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES,
                          &num_attribs);

  GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION};

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
  printf("Active attributes:\n");
  for (int i = 0; i < num_attribs; ++i) {
    GLint results[3];
    glGetProgramResourceiv(id_, GL_PROGRAM_INPUT, i, 3, properties, 3, NULL,
                           results);

    GLint name_buf_size = results[0] + 1;
    char* name = new char[name_buf_size];
    glGetProgramResourceName(id_, GL_PROGRAM_INPUT, i, name_buf_size, NULL,
                             name);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    printf("%-5d %s (%s)\n", results[2], name, GetTypeString(results[1]));
    delete[] name;
  }
#endif
}

bool LoadShaderProgram(GLSLProgram& shader_program,
                       const std::string& vertex_shader_source_path,
                       const std::string& fragement_shader_source_path) {
  bool rv = true;
  VertexShader vertex_shader(vertex_shader_source_path);
  FragmentShader fragment_shader(fragement_shader_source_path);
  shader_program.Attach(vertex_shader);
  shader_program.Attach(fragment_shader);
  shader_program.Link();
  shader_program.Activate();

  std::cout << "Shader program built from " << vertex_shader_source_path
            << " and " << fragement_shader_source_path << ".\n";

  if (shader_program.IsActive()) {
    std::cout << "Shader program is loaded and active with id "
              << shader_program.Id() << ".\n";
  } else {
    std::cout << "Shader program did not load and activate correctly.";
    rv = false;
  }
  return rv;
}