#include "core/BaseObject.hpp"

namespace fag {

BaseObject::BaseObject(void) : m_IsValid(true) {}
BaseObject::~BaseObject(void) {}

void BaseObject::invalidate(void) { m_IsValid = false; }
bool BaseObject::is_valid(void) const { return m_IsValid; }

} // namespace fag
