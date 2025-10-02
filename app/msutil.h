
#ifndef MSUTIL_H_
#define MSUTIL_H_

#include <GLFW/glfw3.h>

#include <cmath>
#include <cstdio>
#include <glm/glm.hpp>
#include <string>

namespace ms_util {

// NOLINTNEXTLINE(readability-identifier-naming,bugprone-reserved-identifier)
bool _GLErrorCheck(FILE* out, const char* filename, int line);

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage,readability-identifier-naming)
#define GLErrorCheck() _GLErrorCheck(stderr, __FILE__, __LINE__)

void GLVersion(bool do_dump_extensions = false, FILE* out = stderr);

void GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                     GLsizei length, const GLchar* msg, const void* param);

void GLFWErrorCallback(int error, const char* description);

glm::quat Trackballx(const glm::vec2& previous_mous_pos,
                     const glm::vec2& current_mous_pos);

bool FileToString(const std::string& file_path, std::string& contents);

}  // namespace ms_util
#endif  // MSUTIL_H
