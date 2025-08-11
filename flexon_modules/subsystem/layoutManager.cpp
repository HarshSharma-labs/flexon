#include "../componentbase/component.hpp"
#include "../utilities/defs.hpp"
#include "./flexonSubsystems.hpp"
#include <cstring>

flexon_unit_factor *unit_factor_ref = nullptr;

struct local_layout {
  // some parameter after the values are calculated;
  float total_flex;
  // copy of teh parent node for processing of the other nodes
  flexon_view parent_node_data;
};

void loop_and_call(flexon_view *, local_layout *,
                   void (*cp)(flexon_view *, local_layout *));

void resolve_grid(flexon_view *, local_layout *);
void resolve_flex(flexon_view *, local_layout *);
void resolve_none(flexon_view *, local_layout *);
void mount_parent_node(flexon_view *, local_layout *);
void resolve_none(flexon_view *, local_layout *) { return; }

void resolve_flex(flexon_view *node, local_layout *config_layout) { return; }
void resolve_grid(flexon_view *, local_layout *) { return; }

void subsystem::layoutManager::calcFromNode(flexon_view *tree) {

  struct local_layout config_layout;
  mount_parent_node(tree, &config_layout);

  return;
}

void loop_and_call(flexon_view *loop_node, local_layout *config_layout,
                   void (*cp)(flexon_view *, local_layout *)) {
  for (flexon_view *layout_view = loop_node; layout_view;
       layout_view = layout_view->neighbour) {
    cp(layout_view, config_layout);
  }
};

void subsystem::layoutManager::mountFactors(flexon_unit_factor *mount) {
  unit_factor_ref = mount;
}

void mount_parent_node(flexon_view *tree, local_layout *config_layout) {

  memset(config_layout, 0, sizeof(struct local_layout));

  if (tree->parent != nullptr) {
    memcpy(&config_layout->parent_node_data, tree->parent, sizeof(flexon_view));
  }

  switch (config_layout->parent_node_data.style.display) {
  case DISPLAY_FLEX:
    loop_and_call(tree, config_layout,
                  [](flexon_view *node, local_layout *config_layout) -> void {
                    config_layout->total_flex += node->layout.flex;
                  });
    loop_and_call(tree, config_layout, resolve_flex);
    break;
  case DISPLAY_GRID:
    break;
  case DISPLAY_NONE:
    break;
  default:
    return;
    break;
  };

  return;
}
