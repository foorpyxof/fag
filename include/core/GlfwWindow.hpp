// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_GLFWWINDOW_HPP
#define FAG_CORE_GLFWWINDOW_HPP

#include "./Window.hpp"

#include "../../modules/glfw/include/GLFW/glfw3.h"

#include <string>

namespace fag {

class GlfwWindow : public virtual fag::Window {
public:
  void poll_events(void) override;
  bool has_closed(void) const override;

  struct fag::WindowDimensions get_dimensions(void) const override;

  std::string get_window_title(void) const override;
  void set_window_title(const std::string &) override;

public:
  GlfwWindow(void);
  GlfwWindow(int width, int height);

  ~GlfwWindow(void);

protected:
  GLFWwindow *_get_glfw_window(void);

private:
  GLFWwindow *m_GlfwWindowPtr;
};

} // namespace fag

#endif // FAG_CORE_GLFWWINDOW_HPP
