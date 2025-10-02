
#ifndef GLFWAPP_H_
#define GLFWAPP_H_

#include "glad/gl.h"

// #define GLFW_INCLUDE_GLU
// #define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>

#include "hid.h"
#include "msutil.h"
#include "scene.h"

constexpr int kDefaultWindowWidth{600};
constexpr int kDefaultWindowHeight{600};
// #define MS_DEFAULT_WINDOW_WIDTH 600
// #define MS_DEFAULT_WINDOW_HEIGHT 600
// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class GLFWAppException : public std::runtime_error {
 public:
  explicit GLFWAppException(const std::string& msg) : std::runtime_error(msg) {}
};

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class GLFWApp {
 public:
  // NOLINTNEXTLINE(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes)
  std::shared_ptr<Mouse> mouse;
  // NOLINTNEXTLINE(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes)
  std::shared_ptr<Keyboard> keyboard;

  explicit GLFWApp(const std::string& window_title = "GLFW Demo",
                   size_t width = kDefaultWindowWidth,
                   size_t height = kDefaultWindowHeight, unsigned int fps = 60,
                   bool debug = true,
                   GLFWerrorfun glfw_err_func = ms_util::GLFWErrorCallback);

  virtual ~GLFWApp() {
    if (window_ != nullptr) {
      glfwDestroyWindow(window_);
    }
    glfwTerminate();
  }

  void AttachSceneManager(std::unique_ptr<SceneManager> scene_manager) {
    scene_manager_ = std::move(scene_manager);
  }

  void AddScene(std::shared_ptr<Scene> scene) {
    scene_manager_->AddScene(scene);
  }

  void InvalidateScene() { scene_->Invalidate(); }

  int Run(std::shared_ptr<Scene> scene);

  int Run();

  void SetKeyCallback(GLFWkeyfun callback_func);

  void SetMouseButtonCallback(GLFWmousebuttonfun callback_func);

  void SetCursorPosCallback(GLFWcursorposfun callback_func);

  void ToggleAnimation();

  int GlVersionMajor() const { return GLAD_VERSION_MAJOR(version_); }

  int GlVersionMinor() const { return GLAD_VERSION_MINOR(version_); }

 private:
  int RunLoop();
  GLFWwindow* window_;
  int fb_width_;
  int fb_height_;
  double prev_time_;
  double curr_time_;
  unsigned int fps_;
  bool debug_;
  int version_;
  std::unique_ptr<SceneManager> scene_manager_;
  std::shared_ptr<Scene> scene_;
};

#endif
