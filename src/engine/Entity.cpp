// Copyright (c) Erynn Scholtes
// SPDX-License-Identifier: MIT

#include "engine/Entity.hpp"

namespace fag {

Entity::Entity() {}
Entity::~Entity() {}

const std::string &Entity::get_name() const { return m_Name; }
void Entity::set_name(const char *new_name) { m_Name = std::string(new_name); }

} // namespace fag
