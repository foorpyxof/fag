// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_WINDOW_HPP
#define FAG_CORE_WINDOW_HPP

#include "./BaseObject.hpp"

#include <string>

namespace fag {

struct WindowDimensions;

class Window : public BaseObject {
public:
  virtual void poll_events(void) = 0;
  virtual bool has_closed(void) const = 0;

  virtual struct WindowDimensions get_dimensions(void) const = 0;

  virtual std::string get_window_title(void) const = 0;
  virtual void set_window_title(const std::string &) = 0;

public:
  virtual ~Window(void);
};

struct WindowDimensions {
  int width, height;
};

} // namespace fag

#endif // FAG_CORE_WINDOW_HPP
