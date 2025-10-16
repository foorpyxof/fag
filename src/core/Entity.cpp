// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "core/Entity.hpp"

namespace fag {

Entity::Entity(void) {}
Entity::~Entity(void) {}

const std::string &Entity::get_name(void) const { return m_Name; }
void Entity::set_name(const char *new_name) { m_Name = std::string(new_name); }

Entity::Basetype Entity::get_basetype(void) const { return m_Basetype; }
void Entity::_set_basetype(Entity::Basetype type) { m_Basetype = type; }

} // namespace fag
