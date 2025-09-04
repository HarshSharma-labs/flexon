#ifndef __FLEXON_LAYOUT_MANAGER__
#define __FLEXON_LAYOUT_MANAGER__

#include "../componentbase/component.hpp"

class layoutModule {
public:
  void mountWH(float height, float width);
  void calcFromNode(flexon_view *tree);

private:
  float lindicis[6] = {0, 1, 3, 1, 2, 3};
  float ndc_w{0.0f};
  float ndc_h{0.0f};

  void loop_and_call(flexon_view *, void *, void (*cp)(flexon_view *, void *));
  void mount_parent_node(flexon_view *, void *);
  void resolve_flex(flexon_view *, void *);
  void resolve_ndc(flexon_view *, float *, float *);
};

#endif
