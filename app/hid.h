
#ifndef HID_H_
#define HID_H_

#include <deque>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <unordered_map>

// NOLINTNEXTLINE(modernize-use-using)
typedef struct {
  int key;
  int scancode;
  int action;
  int mods;
  // NOLINTNEXTLINE(readability-identifier-naming)
} key_event_t;

std::string KeyCodeToGLFWKeyString(const int key_code);

inline std::ostream& PrintKeyEvent(std::ostream& out,
                                   const key_event_t& event) {
  std::string key_string{KeyCodeToGLFWKeyString(event.key)};
  out << "Key: " << key_string << "<" << event.key << ">"
      << " scancode: " << event.scancode << " action: " << event.action
      << " mods: " << event.mods << "\n";
  return out;
}

inline std::ostream& operator<<(std::ostream& out, const key_event_t& event) {
  return PrintKeyEvent(out, event);
}

class Keyboard {
 public:
  Keyboard() = default;

  void PushBack(int key, int scancode, int action, int mods) {
    const key_event_t e{key, scancode, action, mods};
    events_.push_back(e);
  }

  bool IsEmpty() { return events_.empty(); }

  key_event_t PopFront() {
    const key_event_t e{events_.front()};
    events_.pop_front();
    return e;
  }

  // void set_app(std::shared_ptr<GLFWApp> app){
  //   app_ = app;
  // }
  // protected:
  //   std::shared_ptr<GLFWApp> app_;
 private:
  std::deque<key_event_t> events_;
};

class Mouse {
 public:
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init,hicpp-member-init)
  Mouse()
      : position_{glm::vec2(0.0, 0.0)},
        left_is_pressed_{false},
        middle_is_pressed_{false},
        right_is_pressed_{false} {}
  void LeftIsPressed() { left_is_pressed_ = true; }
  void MiddleIsPressed() { middle_is_pressed_ = true; }
  void RightIsPressed() { right_is_pressed_ = true; }

  void LeftIsReleased() { left_is_pressed_ = false; }
  void MiddleIsReleased() { middle_is_pressed_ = false; }
  void RightIsReleased() { right_is_pressed_ = false; }

  bool IsLeftPressed() const { return left_is_pressed_; }
  bool IsMiddlePressed() const { return middle_is_pressed_; }
  bool IsRightPressed() const { return right_is_pressed_; }

  glm::vec2 Position() const { return position_; }

  glm::vec2 PrevPosition() const { return prev_position_; }

  void UpdatePosition(double x, double y) {
    prev_position_ = position_;
    position_ = glm::vec2(x, y);
  }
  // void set_app(std::shared_ptr<GLFWApp> app){
  //   app_ = app;
  // }
  // protected:
  //   std::shared_ptr<GLFWApp> app_;
 private:
  glm::vec2 position_;
  glm::vec2 prev_position_;
  // NOLINTBEGIN(cppcoreguidelines-use-default-member-init,modernize-use-default-member-init)
  bool left_is_pressed_;
  bool middle_is_pressed_;
  bool right_is_pressed_;
  // NOLINTEND(cppcoreguidelines-use-default-member-init,modernize-use-default-member-init)
};

inline std::ostream& PrintMouseState(std::ostream& out, const Mouse& mouse) {
  out << "Mouse: " << (mouse.IsLeftPressed() ? "T" : "F")
      << (mouse.IsMiddlePressed() ? "T" : "F")
      << (mouse.IsRightPressed() ? "T" : "F") << " "
      << glm::to_string(mouse.Position()) << "\n";
  return out;
}

inline std::ostream& operator<<(std::ostream& out, const Mouse& mouse) {
  return PrintMouseState(out, mouse);
}

inline std::string KeyCodeToGLFWKeyString(const int key_code) {
  std::unordered_map<int, std::string> key_strings{
      {32, "GLFW_KEY_SPACE"},
      {39, "GLFW_KEY_APOSTROPHE"},
      {44, "GLFW_KEY_COMMA"},
      {45, "GLFW_KEY_MINUS"},
      {46, "GLFW_KEY_PERIOD"},
      {47, "GLFW_KEY_SLASH"},
      {48, "GLFW_KEY_0"},
      {49, "GLFW_KEY_1"},
      {50, "GLFW_KEY_2"},
      {51, "GLFW_KEY_3"},
      {52, "GLFW_KEY_4"},
      {53, "GLFW_KEY_5"},
      {54, "GLFW_KEY_6"},
      {55, "GLFW_KEY_7"},
      {56, "GLFW_KEY_8"},
      {57, "GLFW_KEY_9"},
      {59, "GLFW_KEY_SEMICOLON"},
      {61, "GLFW_KEY_EQUAL"},
      {65, "GLFW_KEY_A"},
      {66, "GLFW_KEY_B"},
      {67, "GLFW_KEY_C"},
      {68, "GLFW_KEY_D"},
      {69, "GLFW_KEY_E"},
      {70, "GLFW_KEY_F"},
      {71, "GLFW_KEY_G"},
      {72, "GLFW_KEY_H"},
      {73, "GLFW_KEY_I"},
      {74, "GLFW_KEY_J"},
      {75, "GLFW_KEY_K"},
      {76, "GLFW_KEY_L"},
      {77, "GLFW_KEY_M"},
      {78, "GLFW_KEY_N"},
      {79, "GLFW_KEY_O"},
      {80, "GLFW_KEY_P"},
      {81, "GLFW_KEY_Q"},
      {82, "GLFW_KEY_R"},
      {83, "GLFW_KEY_S"},
      {84, "GLFW_KEY_T"},
      {85, "GLFW_KEY_U"},
      {86, "GLFW_KEY_V"},
      {87, "GLFW_KEY_W"},
      {88, "GLFW_KEY_X"},
      {89, "GLFW_KEY_Y"},
      {90, "GLFW_KEY_Z"},
      {91, "GLFW_KEY_LEFT_BRACKET"},
      {92, "GLFW_KEY_BACKSLASH"},
      {93, "GLFW_KEY_RIGHT_BRACKET"},
      {96, "GLFW_KEY_GRAVE_ACCENT"},
      {161, "GLFW_KEY_WORLD_1"},
      {162, "GLFW_KEY_WORLD_2"},
      {256, "GLFW_KEY_ESCAPE"},
      {257, "GLFW_KEY_ENTER"},
      {258, "GLFW_KEY_TAB"},
      {259, "GLFW_KEY_BACKSPACE"},
      {260, "GLFW_KEY_INSERT"},
      {261, "GLFW_KEY_DELETE"},
      {262, "GLFW_KEY_RIGHT"},
      {263, "GLFW_KEY_LEFT"},
      {264, "GLFW_KEY_DOWN"},
      {265, "GLFW_KEY_UP"},
      {266, "GLFW_KEY_PAGE_UP"},
      {267, "GLFW_KEY_PAGE_DOWN"},
      {268, "GLFW_KEY_HOME"},
      {269, "GLFW_KEY_END"},
      {280, "GLFW_KEY_CAPS_LOCK"},
      {281, "GLFW_KEY_SCROLL_LOCK"},
      {282, "GLFW_KEY_NUM_LOCK"},
      {283, "GLFW_KEY_PRINT_SCREEN"},
      {284, "GLFW_KEY_PAUSE"},
      {290, "GLFW_KEY_F1"},
      {291, "GLFW_KEY_F2"},
      {292, "GLFW_KEY_F3"},
      {293, "GLFW_KEY_F4"},
      {294, "GLFW_KEY_F5"},
      {295, "GLFW_KEY_F6"},
      {296, "GLFW_KEY_F7"},
      {297, "GLFW_KEY_F8"},
      {298, "GLFW_KEY_F9"},
      {299, "GLFW_KEY_F10"},
      {300, "GLFW_KEY_F11"},
      {301, "GLFW_KEY_F12"},
      {302, "GLFW_KEY_F13"},
      {303, "GLFW_KEY_F14"},
      {304, "GLFW_KEY_F15"},
      {305, "GLFW_KEY_F16"},
      {306, "GLFW_KEY_F17"},
      {307, "GLFW_KEY_F18"},
      {308, "GLFW_KEY_F19"},
      {309, "GLFW_KEY_F20"},
      {310, "GLFW_KEY_F21"},
      {311, "GLFW_KEY_F22"},
      {312, "GLFW_KEY_F23"},
      {313, "GLFW_KEY_F24"},
      {314, "GLFW_KEY_F25"},
      {320, "GLFW_KEY_KP_0"},
      {321, "GLFW_KEY_KP_1"},
      {322, "GLFW_KEY_KP_2"},
      {323, "GLFW_KEY_KP_3"},
      {324, "GLFW_KEY_KP_4"},
      {325, "GLFW_KEY_KP_5"},
      {326, "GLFW_KEY_KP_6"},
      {327, "GLFW_KEY_KP_7"},
      {328, "GLFW_KEY_KP_8"},
      {329, "GLFW_KEY_KP_9"},
      {330, "GLFW_KEY_KP_DECIMAL"},
      {331, "GLFW_KEY_KP_DIVIDE"},
      {332, "GLFW_KEY_KP_MULTIPLY"},
      {333, "GLFW_KEY_KP_SUBTRACT"},
      {334, "GLFW_KEY_KP_ADD"},
      {335, "GLFW_KEY_KP_ENTER"},
      {336, "GLFW_KEY_KP_EQUAL"},
      {340, "GLFW_KEY_LEFT_SHIFT"},
      {341, "GLFW_KEY_LEFT_CONTROL"},
      {342, "GLFW_KEY_LEFT_ALT"},
      {343, "GLFW_KEY_LEFT_SUPER"},
      {344, "GLFW_KEY_RIGHT_SHIFT"},
      {345, "GLFW_KEY_RIGHT_CONTROL"},
      {346, "GLFW_KEY_RIGHT_ALT"},
      {347, "GLFW_KEY_RIGHT_SUPER"},
      {348, "GLFW_KEY_MENU"}};
  return key_strings[key_code];
}

#endif  // HID_H_