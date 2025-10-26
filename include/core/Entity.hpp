// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#ifndef FAG_CORE_ENTITY_HPP
#define FAG_CORE_ENTITY_HPP

#include "./typedefs.h"

#include "../dev/allocators.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <type_traits>

namespace fag {

class Entity {
public:
  template <class From, class To>
  static std::shared_ptr<From> promote(From *from) {
    static_assert(std::is_base_of<Entity, From>(),
                  "\"From\" class in entity promotion is not derived from the "
                  "base \"Entity\" class");
    static_assert(std::is_base_of<From, To>(),
                  "\"To\" class in entity promotion is not derived from the "
                  "\"From\" class");

    To *retval = nullptr;

    From *FAG_HEAP_CONSTRUCT(To, new_object, ());
    *new_object = *from;

    retval = dynamic_cast<To *>(new_object);
    if (nullptr == retval) {
      // this should never happen
      FAG_HEAP_DESTRUCT(From, new_object);
    }

    return std::shared_ptr<From>(retval);
  }

public:
  virtual void update() = 0;
  virtual void fixed_update() = 0;

public:
  const std::string &get_name() const;
  void set_name(const char *);

public:
  virtual ~Entity(void);

protected:
  Entity(void);

private:
  std::string m_Name;
};

} // namespace fag

#endif // FAG_CORE_ENTITY_HPP
