

#include <cstdio>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>

#include "glad/gl.h"
// Must come after glad/gl.h
#include "msutil.h"

// NOLINTBEGIN(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
namespace ms_util {

bool _GLErrorCheck(FILE* out, const char* filename, int line) {
  bool ret = false;
  std::string error_string;
  std::string msg;
  GLenum err = glGetError();
  while (err != GL_NO_ERROR) {
    ret = true;
    switch (err) {
      case GL_INVALID_ENUM:
        error_string = "GL_INVALID_ENUM";
        msg = "Invalid enum";
        break;
      case GL_INVALID_VALUE:
        error_string = "GL_INVALID_VALUE";
        msg = "Invalid value";
        break;
      case GL_INVALID_OPERATION:
        error_string = "GL_INVALID_OPERATION";
        msg = "Invalid operation";
        break;
      case GL_STACK_OVERFLOW:
        error_string = "GL_STACK_OVERFLOW";
        msg = "Stack overflow";
        break;
      case GL_STACK_UNDERFLOW:
        error_string = "GL_STACK_UNDERFLOW";
        msg = "Stack underflow";
        break;
      // case GL_TABLE_TOO_LARGE:
      //   error_string = "GL_TABLE_TOO_LARGE";
      //   msg = "Table too large";
      //   break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        // OpenGL 3.0 and later
        error_string = "GL_INVALID_FRAMEBUFFER_OPERATION";
        msg = "Invalid framebuffer operation";
        break;
      case GL_OUT_OF_MEMORY:
        error_string = "GL_OUT_OF_MEMORY";
        msg = "Out of memory";
        break;
      default:
        msg = "Unknown error";
    }
    fprintf(out, "%s:%d:GL ERROR(%d): %s\n", filename, line, err, msg.c_str());
    err = glGetError();
  }
  return ret;
}

void GLVersion(bool do_dump_extensions, FILE* out) {
  fprintf(stderr, "Vendor: %s\n", glGetString(GL_VENDOR));
  fprintf(stderr, "Renderer: %s\n", glGetString(GL_RENDERER));
  fprintf(stderr, "OpenGL Version: %s\n", glGetString(GL_VERSION));
  fprintf(stderr, "GLSL Version: %s\n",
          glGetString(GL_SHADING_LANGUAGE_VERSION));
  GLint major{0};
  GLint minor{0};
  GLint samples{0};
  GLint sample_buffers{0};
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);
  glGetIntegerv(GL_SAMPLES, &samples);
  glGetIntegerv(GL_SAMPLE_BUFFERS, &sample_buffers);

  fprintf(stderr, "-------------------------------------------\n");
  fprintf(stderr, "GL Version   : %d.%d\n", major, minor);
  // fprintf(stderr, "GLSL Version : %s\n", glslVersion);
  fprintf(stderr, "MSAA samples : %d\n", samples);
  fprintf(stderr, "MSAA buffers : %d\n", sample_buffers);
  fprintf(stderr, "-------------------------------------------\n");

  if (do_dump_extensions) {
    GLint num_extensions{0};
    glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);
    for (int i = 0; i < num_extensions; i++) {
      fprintf(stderr, "%s\n", glGetStringi(GL_EXTENSIONS, i));
    }
  }
}

void GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                     GLsizei length, const GLchar* msg, const void* param) {
  std::string source_str;
  switch (source) {
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
      source_str = "Window System";
      break;
    case GL_DEBUG_SOURCE_APPLICATION:
      source_str = "Application";
      break;
    case GL_DEBUG_SOURCE_API:
      source_str = "OpenGL";
      break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
      source_str = "Shader Compiler";
      break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
      source_str = "3rd Party";
      break;
    case GL_DEBUG_SOURCE_OTHER:
      source_str = "Other";
      break;
    default:
      source_str = "Unknown";
      break;
  }

  std::string type_str;
  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      type_str = "Error";
      break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      type_str = "Deprecated";
      break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      type_str = "Undefined";
      break;
    case GL_DEBUG_TYPE_PORTABILITY:
      type_str = "Portability";
      break;
    case GL_DEBUG_TYPE_PERFORMANCE:
      type_str = "Performance";
      break;
    case GL_DEBUG_TYPE_MARKER:
      type_str = "Marker";
      break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
      type_str = "Push Group";
      break;
    case GL_DEBUG_TYPE_POP_GROUP:
      type_str = "Pop Group";
      break;
    case GL_DEBUG_TYPE_OTHER:
      type_str = "Other";
      break;
    default:
      type_str = "Unknown";
      break;
  }

  std::string severity_str;
  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
      severity_str = "HIGH";
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      severity_str = "MEDIUM";
      break;
    case GL_DEBUG_SEVERITY_LOW:
      severity_str = "LOW";
      break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      severity_str = "NOTIFY";
      break;
    default:
      severity_str = "UNKOWN";
      break;
  }
  fprintf(stderr, "%s(%d):%s(%d) [%s] (id: %d) \"%s\"\n", source_str.c_str(),
          source, type_str.c_str(), type, severity_str.c_str(), id, msg);
  // std::cerr << source_str << ":" << type_str << "[" << sev_str << "]" << "("
  // << id << "): " << msg << std::endl;
}
void GLFWErrorCallback(int error, const char* description) {
  fprintf(stderr, "GLFW Error (%d): \"%s\"\n", error, description);
  fprintf(stderr,
          "If this is a Linux system and there is a \"GLXBadFBConfig\" error,\n"
          "then collect the following information to seek help:\n"
          " - Use the `glxinfo` command to identify the maximum OpenGL and\n"
          "   GLSL version this system supports.\n"
          " - Use the `lsb_release -a` command to identify the version of\n"
          "   Linux you are using.\n"
          " - Use the `sudo lshw -c video` command to get information about\n"
          "   your video hardware (your GPU).\n"
          " - Use the command `lspci -k | grep -EA3 'VGA|3D|Display'` to\n"
          "   identify the graphics hardware (your GPU).\n"
          " - If you are contacting your instructor, commit and push all your\n"
          "   changes to your repository. Include a link to your repository\n"
          "   in your message.\n");
}

// Project a 2D point onto a sphere of radius r OR a hyperbolic sheet
// if we are away from the center of the sphere.
float ProjectToSphere(float radius, glm::vec2 point) {
  const float distance{glm::length(point)};
  // float t{0.0F};
  float z{0.0F};
  // Inside sphere
  if (distance < (radius * glm::one_over_root_two<float>())) {
    z = sqrt((radius * radius) - (distance * distance));
  } else {
    // On hyperbola
    const float t{radius / glm::root_two<float>()};
    z = (t * t) / distance;
  }
  return z;
}

glm::quat Trackballx(const glm::vec2& previous_mous_pos,
                     const glm::vec2& current_mous_pos) {
  const float trackball_size{0.8};
  // Axis of rotation
  glm::vec3 axis;
  // How much to rotate around the axis
  float phi{0.0};

  // float p1[3], p2[3], d[3];
  // float t;

  if (glm::all(glm::equal(previous_mous_pos, current_mous_pos, 0))) {
    // Trackball hasn't changed - zero rotation
    fprintf(stderr, "return null quat.\n");
    phi = 0;
    // axis doesn't matter
    axis = glm::vec3(1.0, 0.0, 0.0);
    return glm::angleAxis(phi, axis);
  }

  /*
   * First, figure out z-coordinates for projection of P1 and P2 to
   * deformed sphere
   */
  const glm::vec3 p1(previous_mous_pos,
                     ProjectToSphere(trackball_size, previous_mous_pos));
  const glm::vec3 p2(current_mous_pos,
                     ProjectToSphere(trackball_size, current_mous_pos));
  // vset(p1, p1x, p1y, tb_project_to_sphere(TRACKBALLSIZE, p1x, p1y));
  // vset(p2, p2x, p2y, tb_project_to_sphere(TRACKBALLSIZE, p2x, p2y));

  /*
   *  Now, we want the cross product of P1 and P2
   */
  // vcross(p2, p1, a);
  axis = glm::cross(p2, p1);

  /*
   *  Figure out how much to rotate around that axis.
   */
  // vsub(p1, p2, d);
  const glm::vec3 d = p1 - p2;

  // t = vlength(d) / (2.0 * TRACKBALLSIZE);
  float t = glm::length(d) / (2.0 * trackball_size);

  /*
   * Avoid problems with out-of-control values...
   */
  if (t > 1.0) {
    t = 1.0;
  }
  if (t < -1.0) {
    t = -1.0;
  }
  phi = 2.0 * asin(t);

  // axis_to_quat(axis, phi, q);
  fprintf(stderr, "angle: %g\n", phi);
  return glm::angleAxis(phi, axis);
}

bool FileToString(const std::string& file_path, std::string& contents) {
  bool status{false};
  std::ifstream input_file_stream{file_path};
  if (input_file_stream.is_open()) {
    contents = std::string((std::istreambuf_iterator<char>(input_file_stream)),
                           (std::istreambuf_iterator<char>()));
    input_file_stream.close();
    status = true;
  }
  return status;
}

// void SaveColorBufferToFile(const size_t width, const size_t height, const std::string& output_file_name) {
//   auto buffer = std::make_shared(array<int*, width*height*3>);
//   // watch out, it's synchronous and it will block
//   // use pixel buffer objects for asynchronous readback
//   glReadPixel(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
//   stbi_flip_vertically_on_write(true);
//   stbi_write_png(output_file_name.c_str(), width, height, 4, buffer.get(), width * 4);
// }

}  // namespace ms_util
// NOLINTEND(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
