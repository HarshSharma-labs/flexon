#ifndef __FLEXON_LAYOUT_MANAGER__
#define __FLEXON_LAYOUT_MANAGER__
#include "../../components/base.hpp"
#include "../../components/View.hpp"
#include "../../components/matrices.hpp"
#include "../flexon_commits.hpp"
#include <vector>

void initial_commit(void (*node)(), 
                    void(*main)(fiber *wrap,vec2<float> &windowDimension)
                    ,struct commit_wm *commit);

namespace layoutmanager{
 void sortEventView();
 uint32_t CheckBound(float x, float y,
                     std::vector<fiber_event_wrapper> &ref);

};

#endif
