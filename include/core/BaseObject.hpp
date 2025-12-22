#ifndef FAG_CORE_BASEOBJECT_HPP
#define FAG_CORE_BASEOBJECT_HPP

namespace fag {

class BaseObject {
public:
  virtual ~BaseObject(void);

public:
  template <class T> constexpr bool is() const {
    return (nullptr != dynamic_cast<const T *>(this));
  }
};

} // namespace fag

#endif // FAG_CORE_BASEOBJECT_HPP
