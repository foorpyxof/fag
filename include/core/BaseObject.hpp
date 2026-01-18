#ifndef FAG_CORE_BASEOBJECT_HPP
#define FAG_CORE_BASEOBJECT_HPP

#include <type_traits>
#include <typeinfo>

namespace fag {

class BaseObject {
public:
  virtual ~BaseObject(void);

public:
  template <class T> constexpr bool is(void) const {
    return (nullptr != dynamic_cast<const T *>(this));
  }

public:
  void invalidate(void);
  bool is_valid(void) const;

public:
  BaseObject(void);

private:
  bool m_IsValid;
};

} // namespace fag

#endif // FAG_CORE_BASEOBJECT_HPP
