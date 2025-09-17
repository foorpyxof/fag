#include "engine/Basis.hpp"

namespace fag {

Basis2D::Basis2D() : x(1, 0), y(0, 1) {}
Basis3D::Basis3D() : x(1, 0, 0), y(0, 1, 0), z(0, 0, 1) {}

} // namespace fag
