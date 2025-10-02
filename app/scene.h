
#ifndef SCENE_H_
#define SCENE_H_

#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "hid.h"

class GLFWApp;

class SceneException : public std::runtime_error {
 public:
  explicit SceneException(const std::string& msg) : std::runtime_error(msg) {}
};

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class Scene {
 public:
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init,hicpp-member-init)
  Scene(const std::string name, std::shared_ptr<GLFWApp> app, size_t width,
        size_t height)
      : name_{name},
        app{app},
        width{width},
        height{height},
        animate_on{false},
        is_valid{true} {}

  virtual ~Scene() = default;

  // Initialization
  virtual bool Begin() = 0;

  // Clean up
  virtual bool End() = 0;

  // Update the scene, called prior to redrawing
  virtual bool Update(double time) = 0;

  // Draw the scene
  virtual bool Draw(double time) = 0;

  // Resize
  virtual bool Resize(size_t width, size_t height) = 0;

  std::ostream& Out(std::ostream& out) const {
    out << "Scene: " << name_ << "\n";
    return out;
  }

  bool IsValid() { return is_valid; }

  void Validate() { is_valid = true; }
  void Invalidate() { is_valid = false; }

  void SetDimension(size_t width, size_t height) {
    this->width = width;
    this->height = height;
  }

  void ToggleAnimation() { animate_on = !animate_on; }

  bool IsAnimated() const { return animate_on; }

  void SetMouse(std::shared_ptr<Mouse> mouse) { this->mouse = mouse; }

  void SetKeyboard(std::shared_ptr<Keyboard> keyboard) {
    this->keyboard = keyboard;
  }

 protected:
  // NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes)
  std::string name_;
  std::shared_ptr<GLFWApp> app;
  size_t width;
  size_t height;
  // NOLINTNEXTLINE(cppcoreguidelines-use-default-member-init,modernize-use-default-member-init)
  bool animate_on;

  std::shared_ptr<Mouse> mouse;
  std::shared_ptr<Keyboard> keyboard;

  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;
  // NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes)
  bool is_valid;
};

inline std::ostream& operator<<(std::ostream& out, const Scene& scene) {
  return scene.Out(out);
}

class SceneManager {
 public:
  SceneManager() = default;
  virtual ~SceneManager() = default;
  virtual void AddScene(std::shared_ptr<Scene> scene) {}
  virtual std::shared_ptr<Scene> GetByName(const std::string scene_name) {
    return nullptr;
  }
  virtual std::shared_ptr<Scene> First() { return nullptr; }
  virtual std::shared_ptr<Scene> At(unsigned int index) { return nullptr; }
  virtual std::shared_ptr<Scene> Next() { return nullptr; }
  virtual unsigned int Size() { return 0; }
};

class CircularQueueSceneManager : public SceneManager {
 public:
  explicit CircularQueueSceneManager() : scene_index_{0} {};
  ~CircularQueueSceneManager() {};
  void AddScene(std::shared_ptr<Scene> scene) override {
    scenes_.emplace_back(std::move(scene));
  }
  std::shared_ptr<Scene> First() override {
    if (scenes_.size() > 0) {
      return scenes_.at(0);
    }
    return nullptr;
  }
  // std::shared_ptr<Scene> At(unsigned int index) override {
  //   if (index >= scenes_.size()) {
  //     throw SceneException("SimpleSceneManager: index out of range");
  //   }
  //   return scenes_.at(index);
  // }
  std::shared_ptr<Scene> Next() override {
    const size_t next_scene_index{(scene_index_ + 1L) % scenes_.size()};
    scene_index_ = next_scene_index;
    auto scene = scenes_.at(scene_index_);
    scene->Validate();
    return scene;
  }
  unsigned int Size() override { return scenes_.size(); }

 private:
  std::vector<std::shared_ptr<Scene>> scenes_;
  size_t scene_index_;
};

#endif