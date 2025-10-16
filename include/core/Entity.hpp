// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_ENTITY_HPP
#define FAG_CORE_ENTITY_HPP

#include "core/typedefs.h"

#include "dev/allocators.hpp"

#include <string>

namespace fag {

class Entity {
public:
  enum class Basetype { Entity2D, Entity3D };

public:
  Entity(void);
  virtual ~Entity(void) = 0;

  virtual void update() = 0;
  virtual void fixed_update() = 0;

  const std::string &get_name() const;
  void set_name(const char *);

  Basetype get_basetype() const;

protected:
  void _set_basetype(Basetype);

private:
  std::string m_Name;
  Basetype m_Basetype;

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
      Old *new_object = FAG_HEAP_CONSTRUCT(New);
      *new_object = *from;

      retval = dynamic_cast<New *>(new_object);
      if (nullptr == retval)
        delete new_object;
    }

    return retval;
  }
};

} // namespace fag

#endif // FAG_CORE_ENTITY_HPP
