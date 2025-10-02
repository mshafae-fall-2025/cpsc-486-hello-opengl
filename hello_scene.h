
#ifndef HELLO_SCENE_H_
#define HELLO_SCENE_H_

#include <glm/glm.hpp>

#include "glfwapp.h"
#include "glslshader.h"
#include "scene.h"

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class HelloScene : public Scene {
 public:
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init,hicpp-member-init)
  explicit HelloScene(std::shared_ptr<GLFWApp> app)
      : Scene("Hello", app, 600, 600) {};

  ~HelloScene() override = default;

  bool Begin() override {
    if (not already_initialized) {
      LoadShaderProgram(program, "shaders/triangle.vert.glsl",
                        "shaders/triangle.frag.glsl");
      ms_util::GLErrorCheck();
      program.PrintActiveAttribs();
      program.PrintActiveUniforms();
      glGenVertexArrays(1, &vao_handle);
      glBindVertexArray(vao_handle);
      glClearColor(0.5, 0.5, 0.5, 1.0);
      already_initialized = true;
    }
    return !ms_util::GLErrorCheck();
  }

  bool Draw(double time) override {
    // Purple
    glm::vec4 background_color{1.0, 0.0, 1.0, 1.0};

    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(background_color));

    // Activate the shading program
    program.Activate();

    glm::vec4 triangle_color{0.0, 1.0, 0.0, 1.0};

    // Send the values that were just calculated to the vertex shader.
    glVertexAttrib4fv(0, glm::value_ptr(triangle_color));

    glDrawArrays(GL_TRIANGLES, 0, 3);

    return !ms_util::GLErrorCheck();
  }

  bool End() override {
    GLSLProgram::Deactivate();
    // glDeleteVertexArrays(1, &vao_handle_);
    return !ms_util::GLErrorCheck();
  }

  bool Update(double time) override {
    while (not app->keyboard->IsEmpty()) {
      auto key_event = app->keyboard->PopFront();
      std::cerr << key_event;
      std::cerr << *(app->mouse);
      if (key_event.key == GLFW_KEY_N) {
        app->InvalidateScene();
      }
    }
    return !ms_util::GLErrorCheck();
  }

  bool Resize(size_t width, size_t height) override {
    this->width = width;
    this->height = height;
    glViewport(0, 0, this->width, this->height);
    return !ms_util::GLErrorCheck();
  }

 protected:
  // NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes)
  GLuint vao_handle{0};
  GLSLProgram program;
  bool already_initialized{false};
  // NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes)
};

#endif