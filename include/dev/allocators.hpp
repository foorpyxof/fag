#ifndef FAG_DEV_ALLOCATORS_HPP
#define FAG_DEV_ALLOCATORS_HPP

#define FAG_HEAP_CONSTRUCT(_construction) new _construction
#define FAG_HEAP_DESTRUCT(_ptr_arg) delete _ptr_arg

#endif // FAG_DEV_ALLOCATORS_HPP
