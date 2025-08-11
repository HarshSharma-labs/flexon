#include "../componentbase/component.hpp"
#include "../utilities/defs.hpp"
#include "../utilities/diagonistics.hpp"
#include "./flexonSubsystems.hpp"
#include <cstring>
#include <iostream>

flexon_unit_factor *unit_factor_ref = nullptr;

struct local_layout {
  // some parameter after the values are calculated;
  float total_flex;
  // copy of teh parent node for processing of the other nodes
  flexon_view parent_layout;

  float offsetXY; // node position for flex style
};

struct NDC {
  float drawableHeight;
  float drawableWidth;
};
struct NDC ndc;

void loop_and_call(flexon_view *, local_layout *,
                   void (*cp)(flexon_view *, local_layout *));

void resolve_grid(flexon_view *, local_layout *);
void resolve_flex(flexon_view *, local_layout *);
void resolve_none(flexon_view *, local_layout *);
void mount_parent_node(flexon_view *, local_layout *);
void resolve_none(flexon_view *, local_layout *) { return; }

void subsystem::layoutManager::mountDimensions(int height, int width) {
  ndc.drawableHeight = static_cast<float>(height);
  ndc.drawableWidth = static_cast<float>(width);

  // ndc.drawableWidth /= 2;
  //  ndc.drawableHeight /= 2;
  return;
}

void resolve_flex(flexon_view *node, local_layout *config_layout) {

  node->gpu_vertex.indices[0] = 0;
  node->gpu_vertex.indices[1] = 1;
  node->gpu_vertex.indices[2] = 3;
  node->gpu_vertex.indices[3] = 1;
  node->gpu_vertex.indices[4] = 2;
  node->gpu_vertex.indices[5] = 3;

  float paddingtb = config_layout->parent_layout.layout.padding.bottom +
                    config_layout->parent_layout.layout.padding.top +
                    node->layout.margin.top + node->layout.margin.bottom;
  float paddinglr = config_layout->parent_layout.layout.padding.left +
                    config_layout->parent_layout.layout.padding.right +
                    node->layout.margin.left + node->layout.margin.right;

  switch (config_layout->parent_layout.style.layout_direction) {
  case LAYOUT_COLOUMN:

    node->layout.height = ((node->layout.flex / config_layout->total_flex) *
                           config_layout->parent_layout.layout.height) -
                          paddingtb;
    node->layout.y = config_layout->offsetXY +
                     config_layout->parent_layout.layout.y +
                     config_layout->parent_layout.layout.padding.top +
                     node->layout.margin.top;

    node->layout.width = config_layout->parent_layout.layout.width - paddinglr;
    node->layout.x =
        config_layout->parent_layout.layout.x + node->layout.margin.left;
    config_layout->offsetXY += node->layout.height + paddingtb;

    break;
  case LAYOUT_ROW:

    node->layout.width = ((node->layout.flex / config_layout->total_flex) *
                          config_layout->parent_layout.layout.width) -
                         paddinglr;

    node->layout.x = config_layout->offsetXY +
                     config_layout->parent_layout.layout.x +
                     config_layout->parent_layout.layout.padding.left +
                     node->layout.margin.left;

    node->layout.height =
        config_layout->parent_layout.layout.height - paddingtb;

    node->layout.y =
        config_layout->parent_layout.layout.y + node->layout.margin.top;

    config_layout->offsetXY += node->layout.width + paddinglr;

    break;
  }
  if (node->child != nullptr) {
    subsystem::layoutManager::calcFromNode(node->child);
  }

  node->gpu_vertex.vertex[0] =
      ((2.0f * (node->layout.x + node->layout.width)) / ndc.drawableWidth) - 1;

  node->gpu_vertex.vertex[1] = 1 - ((2 * node->layout.y) / ndc.drawableHeight);
  node->gpu_vertex.vertex[2] = 0.0f;

  node->gpu_vertex.vertex[3] =
      (2 * (node->layout.x + node->layout.width) / ndc.drawableWidth) - 1;

  node->gpu_vertex.vertex[4] =
      1 - ((2 * (node->layout.height + node->layout.y)) / ndc.drawableHeight);
  node->gpu_vertex.vertex[5] = 0.0f;

  node->gpu_vertex.vertex[6] = ((2 * node->layout.x) / ndc.drawableWidth) - 1;
  node->gpu_vertex.vertex[7] =
      1 - ((2 * (node->layout.y + node->layout.height)) / ndc.drawableHeight);
  node->gpu_vertex.vertex[8] = 0.0f;

  node->gpu_vertex.vertex[9] = ((2 * node->layout.x) / ndc.drawableWidth) - 1;
  node->gpu_vertex.vertex[10] = 1 - ((2 * node->layout.y) / ndc.drawableHeight);
  node->gpu_vertex.vertex[11] = 0.0f;

  print_node_diagonistics(node);

  return;
}
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

  memset(&config_layout->parent_layout, 0, sizeof(flexon_view));
  config_layout->offsetXY = 0.0f;
  config_layout->total_flex = 0.0f;
  flexon_view *tmp_ptr = tree;

  if (tree->parent != nullptr) {
    memcpy(&config_layout->parent_layout, tree->parent, sizeof(flexon_view));
    print_node_diagonistics(&config_layout->parent_layout);
  }

  switch (config_layout->parent_layout.style.display) {
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
