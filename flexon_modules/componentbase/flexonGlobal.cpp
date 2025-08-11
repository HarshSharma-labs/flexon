#include "./flexonGlobal.hpp"
#include "../components/Decorator.hpp"
#include "./component.hpp"
#include <cstdint>

flexon_view **global_stitch = nullptr;
flexon_view *global_parent_stitch = nullptr;
flexon_view *global_previous_stitch = nullptr;

modifier<uint32_t> Modifier;
