// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "core/Entity.hpp"

namespace fag {

Entity::Entity(void) {}
Entity::~Entity(void) {}

const std::string &Entity::get_name(void) const { return m_Name; }
void Entity::set_name(const char *new_name) { m_Name = std::string(new_name); }

} // namespace fag
