#include "core/Window.hpp"
#include "debug.h"
#include "error/Fatal.hpp"

#include "glfw/include/GLFW/glfw3.h"

#include <sstream>

#include "core/GlfwWindow.hpp"

#define DEFAULT_WINDOW_WIDTH 500
#define DEFAULT_WINDOW_HEIGHT 500

static void glfw_error_callback(int err_code, const char *description) {
  std::ostringstream msg;

  msg << "GLFW (in the fag::Vulkan::GlfwWindow class) has thrown a fatal "
         "error.\n"
         "\tGLFW error code: "
      << err_code << ".\n\tGLFW error description: " << description;

  glfwTerminate();
  throw fag::Error::Fatal(msg.str().c_str(), __FILE__, __LINE__);
}
static bool firstrun = true;

namespace fag {

GlfwWindow::GlfwWindow(void)
    : GlfwWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT) {}
GlfwWindow::GlfwWindow(int w, int h) {
  if (firstrun) {
    firstrun = false;
    glfwSetErrorCallback(glfw_error_callback);
  }

  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  glfwWindowHintString(GLFW_X11_CLASS_NAME, "fag_engine_game_window");

  glfwWindowHintString(GLFW_X11_INSTANCE_NAME, "fag_engine_game_window");
  glfwWindowHintString(GLFW_WAYLAND_APP_ID, "fag_engine_game_window");
  FAG_TODO("make window X11 instance and Wayland App-ID dependent on the game "
           "name supplied by the programmer");

  m_GlfwWindowPtr = glfwCreateWindow(w, h, "Game Window", nullptr, nullptr);

  if (!m_GlfwWindowPtr)
    throw fag::Error::Fatal("could not create game window", __FILE__, __LINE__);
}

GlfwWindow::~GlfwWindow(void) { glfwDestroyWindow(m_GlfwWindowPtr); }

void GlfwWindow::poll_events(void) { glfwPollEvents(); }
bool GlfwWindow::has_closed(void) const {
  return glfwWindowShouldClose(m_GlfwWindowPtr);
}

struct fag::WindowDimensions GlfwWindow::get_dimensions(void) const {
  struct fag::WindowDimensions dims{};
  glfwGetWindowSize(m_GlfwWindowPtr, &dims.width, &dims.height);
  return dims;
}

std::string GlfwWindow::get_window_title(void) const {
  return {glfwGetWindowTitle(m_GlfwWindowPtr)};
}
void GlfwWindow::set_window_title(const std::string &new_name) {
  glfwSetWindowTitle(m_GlfwWindowPtr, new_name.c_str());
}

GLFWwindow *GlfwWindow::_get_glfw_window(void) { return m_GlfwWindowPtr; }

} // namespace fag
