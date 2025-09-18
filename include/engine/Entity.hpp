// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_ENGINE_ENTITY_HPP
#define FAG_ENGINE_ENTITY_HPP

#include "engine/typedefs.h"

#include <cstring>
#include <string>

namespace fag {

class Entity {
public:
  Entity(void);
  virtual ~Entity(void) = 0;

  virtual void update() = 0;
  virtual void fixed_update() = 0;

  const std::string &get_name() const;
  void set_name(const char *);

private:
  std::string m_Name;

public:
  template <class Old, class New> static New *promote(Old *from, Allocator *a) {
    New *retval = nullptr;

    if (a && a->allocFunc && a->freeFunc) {
      Old *new_object = static_cast<Old *>(a->allocFunc(sizeof(New)));
      *new_object = *from;

      retval = dynamic_cast<New *>(new_object);
      if (nullptr == retval)
        a->freeFunc(new_object);
    } else {
      Old *new_object = new New;
      *new_object = *from;

      retval = dynamic_cast<New *>(new_object);
      if (nullptr == retval)
        delete new_object;
    }

    return retval;
  }
};

} // namespace fag

#endif // FAG_ENGINE_ENTITY_HPP
