#ifndef FAG_ENGINE_ENTITY_HPP
#define FAG_ENGINE_ENTITY_HPP

#include <string>

namespace fag {

class Entity {
public:
  virtual void update() = 0;
  virtual void fixed_update() = 0;

  const std::string &get_name() const;
  void set_name(const char *);

private:
  std::string m_Name;

public:
  template <class Old, class New> static New *promote(Old *from) {
    Old *new_object = new New;
    *new_object = *from;

    New *retval = dynamic_cast<New *>(new_object);
    if (nullptr == retval)
      delete new_object;

    return retval;
  }
};

} // namespace fag

#endif // FAG_ENGINE_ENTITY_HPP
