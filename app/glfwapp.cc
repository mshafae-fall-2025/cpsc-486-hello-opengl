
#include "glfwapp.h"

#include <GLFW/glfw3.h>

// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init,hicpp-member-init)
GLFWApp::GLFWApp(const std::string& window_title, size_t width, size_t height,
                 unsigned int fps, bool debug, GLFWerrorfun glfw_err_func)
    : fps_{fps}, debug_{debug} {
  glfwSetErrorCallback(glfw_err_func);
  if (glfwInit() == 0) {
    throw GLFWAppException("Could not initialize GLFW.");
  }
  // #ifdef __APPLE__
  // Apple only supports OpenGL up to 4.1.
  // See
  // https://www.glfw.org/faq.html#41---how-do-i-create-an-opengl-30-context
  // and Apple deprecated OpenGL as of 2018 (macOS 10.14). Highest revision
  // is OpenGL 4.1
  // There exists a project to implement OpenGL 4 on Apple's Metal
  // https://github.com/openglonmetal/MGL
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
  // #else
  // If you're looking here, you may have a situation where your
  // hardware doesn't support OpenGL 3.3.
  // Intel's page detailing what they support on what chipset
  // https://www.intel.com/content/www/us/en/support/articles/000005524/graphics.html
  //   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  //   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // #endif
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  if (debug) {
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
  }
  window_ =
      glfwCreateWindow(width, height, window_title.c_str(), nullptr, nullptr);
  if (window_ == nullptr) {
    throw GLFWAppException("Could not create window");
  }

  glfwMakeContextCurrent(window_);
  glfwGetFramebufferSize(window_, &fb_width_, &fb_height_);

  mouse = std::make_shared<Mouse>();
  // NOLINTBEGIN(misc-const-correctness)
  double x{-1.0};
  double y{-1.0};
  // NOLINTEND(misc-const-correctness)
  glfwGetCursorPos(window_, &x, &y);
  mouse->UpdatePosition(x, y);

  keyboard = std::make_shared<Keyboard>();

  version_ = gladLoadGL(glfwGetProcAddress);
  if (version_ == 0) {
    throw GLFWAppException("Could not load OpenGL functions.");
  }

  // Successfully loaded OpenGL
  std::cout << "Loaded OpenGL " << GlVersionMajor() << "." << GlVersionMinor()
            << "\n";

  if (debug && GlVersionMajor() >= 4 && GlVersionMinor() >= 3) {
    std::cerr << "GL Debugging Callback Enabled.\n";
    glDebugMessageCallback(ms_util::GLDebugCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
                          GL_TRUE);
    glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0,
                         GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Start debugging");
  }

  ms_util::GLVersion();
};

int GLFWApp::Run(std::shared_ptr<Scene> scene) {
  int status{EXIT_FAILURE};
  scene_ = std::move(scene);
  if (scene_) {
    scene_->SetMouse(mouse);
    scene_->SetKeyboard(keyboard);
    status = RunLoop();
  }
  return status;
}

int GLFWApp::Run() {
  int status{EXIT_FAILURE};
  scene_ = scene_manager_->First();
  while (glfwWindowShouldClose(window_) == 0 and scene_ and scene_->IsValid()) {
    std::cerr << *scene_;
    while (glfwWindowShouldClose(window_) == 0 and scene_->IsValid()) {
      scene_->SetMouse(mouse);
      scene_->SetKeyboard(keyboard);
      status = RunLoop();
    }
    scene_ = scene_manager_->Next();
  }
  // if (scene_) {
  //   scene_->SetMouse(mouse);
  //   scene_->SetKeyboard(keyboard);
  //   status = RunLoop();
  // }
  return status;
}

void GLFWApp::SetKeyCallback(GLFWkeyfun callback_func) {
  glfwSetKeyCallback(window_, callback_func);
}

void GLFWApp::SetMouseButtonCallback(GLFWmousebuttonfun callback_func) {
  glfwSetMouseButtonCallback(window_, callback_func);
}

void GLFWApp::SetCursorPosCallback(GLFWcursorposfun callback_func) {
  glfwSetCursorPosCallback(window_, callback_func);
}

void GLFWApp::ToggleAnimation() {
  if (scene_) {
    scene_->ToggleAnimation();
  }
}

int GLFWApp::RunLoop() {
  scene_->SetDimension(fb_width_, fb_height_);
  scene_->Begin();
  scene_->Resize(fb_width_, fb_height_);
  curr_time_ = glfwGetTime();
  // double last_frame_update_time{0};
  double time_per_frame = 1.0 / fps_;
  double accumlated_time{0};
  while (glfwWindowShouldClose(window_) == 0 and scene_->IsValid()) {
    assert(!ms_util::GLErrorCheck());
    prev_time_ = curr_time_;
    curr_time_ = glfwGetTime();
    double delta_time = curr_time_ - prev_time_;
    accumlated_time += delta_time;
    scene_->Update(curr_time_);
    if (accumlated_time >= time_per_frame) {
      scene_->Draw(curr_time_);
      glfwSwapBuffers(window_);
      // last_frame_update_time = curr_time_;
      accumlated_time = 0.0;
    }
    glfwPollEvents();
  }
  scene_->End();
  return EXIT_SUCCESS;
}
