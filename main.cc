
#include <iostream>
#include <memory>

#include "glad/gl.h"
#include "glfwapp.h"
#include "hello_scene.h"

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
std::shared_ptr<GLFWApp> g_app;

void GLFWBasicKeyCallback(GLFWwindow* window, int key, int scancode, int action,
                          int mods) {
  if (action == GLFW_PRESS) {
    // NOLINTNEXTLINE(hicpp-multiway-paths-covered)
    switch (key) {
      case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GL_TRUE);
        break;
      case GLFW_KEY_SPACE:
        g_app->ToggleAnimation();
        break;
      default:
        g_app->keyboard->PushBack(key, scancode, action, mods);
        break;
    }
  }
}

void GLFWBasicMouseButtonCallback(GLFWwindow* window, int button, int action,
                                  int mods) {
  // NOLINTNEXTLINE(hicpp-multiway-paths-covered)
  switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
      if (action == GLFW_PRESS) {
        std::cerr << "main: left pressed.\n";
        g_app->mouse->LeftIsPressed();
      } else {
        std::cerr << "main: left released.\n";
        g_app->mouse->LeftIsReleased();
      }
      break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
      if (action == GLFW_PRESS) {
        g_app->mouse->MiddleIsPressed();
      } else {
        g_app->mouse->MiddleIsReleased();
      }
      break;
    case GLFW_MOUSE_BUTTON_RIGHT:
      if (action == GLFW_PRESS) {
        std::cerr << "main: right pressed.\n";
        g_app->mouse->RightIsPressed();
      } else {
        std::cerr << "main: right released.\n";
        g_app->mouse->RightIsReleased();
      }
      break;
    default:
      // do nothing
      break;
  }
}

void GLFWBasicCursorPositionCallback(GLFWwindow* window, double mouse_x,
                                     double mouse_y) {
  g_app->mouse->UpdatePosition(mouse_x, mouse_y);
}

int main(int argc, char const* argv[]) {
  try {
    g_app = std::make_shared<GLFWApp>();
  } catch (const GLFWAppException& exception) {
    std::cout << "Error: " << exception.what() << "\n";
    return 1;
  } catch (...) {
    std::cout << "Unknown Error.\n";
    return 1;
  }

  g_app->SetKeyCallback(GLFWBasicKeyCallback);
  g_app->SetCursorPosCallback(GLFWBasicCursorPositionCallback);
  g_app->SetMouseButtonCallback(GLFWBasicMouseButtonCallback);

  auto hello_scene{make_shared<HelloScene>(g_app)};

  return g_app->Run(hello_scene);
}