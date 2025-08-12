#ifndef __FLEXON_LAYOUT_MANAGER__
#define __FLEXON_LAYOUT_MANAGER__

#include "../componentbase/component.hpp"
#include "../utilities/diagonistics.hpp"
#include <cstring>
struct local_layout {
  float total_flex;
  flexon_view parent_layout;
  float offsetXY;
};

class layoutModule {
public:
  void mountWH(float height, float width) {
    ndc_w = width;
    ndc_h = height;
  };

  void calcFromNode(flexon_view *tree) {

    struct local_layout config_layout;
    mount_parent_node(tree, &config_layout);

    return;
  }

private:
  local_layout config_layout;
  float indicis[6] = {0, 1, 3, 1, 2, 3};
  float ndc_w{0.0f};
  float ndc_h{0.0f};

  void loop_and_call(flexon_view *loop_node, local_layout *config_layout,
                     void (*cp)(flexon_view *, local_layout *)) {
    for (flexon_view *layout_view = loop_node; layout_view;
         layout_view = layout_view->neighbour) {

      cp(layout_view, config_layout);
    }
  };

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

      for (flexon_view *layout_view = tree; layout_view;
           layout_view = layout_view->neighbour) {

        resolve_flex(layout_view, config_layout);
      }

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

  void resolve_flex(flexon_view *node, local_layout *config_layout) {

    for (int i = 0; i < 6; i++)
      node->gpu_vertex.indices[i] = indicis[i];

    // node->gpu_vertex.indices[1] = 1;
    // node->gpu_vertex.indices[2] = 3;
    // node->gpu_vertex.indices[3] = 1;
    // node->gpu_vertex.indices[4] = 2;
    // node->gpu_vertex.indices[5] = 3;

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

      node->layout.width =
          config_layout->parent_layout.layout.width - paddinglr;
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
      calcFromNode(node->child);
    }

    node->gpu_vertex.vertex[0] =
        ((2.0f * (node->layout.x + node->layout.width)) / ndc_w) - 1;

    node->gpu_vertex.vertex[1] = 1 - ((2 * node->layout.y) / ndc_h);
    node->gpu_vertex.vertex[2] = 0.0f;

    node->gpu_vertex.vertex[3] =
        (2 * (node->layout.x + node->layout.width) / ndc_w) - 1;

    node->gpu_vertex.vertex[4] =
        1 - ((2 * (node->layout.height + node->layout.y)) / ndc_h);
    node->gpu_vertex.vertex[5] = 0.0f;

    node->gpu_vertex.vertex[6] = ((2 * node->layout.x) / ndc_w) - 1;
    node->gpu_vertex.vertex[7] =
        1 - ((2 * (node->layout.y + node->layout.height)) / ndc_h);
    node->gpu_vertex.vertex[8] = 0.0f;

    node->gpu_vertex.vertex[9] = ((2 * node->layout.x) / ndc_w) - 1;
    node->gpu_vertex.vertex[10] = 1 - ((2 * node->layout.y) / ndc_h);
    node->gpu_vertex.vertex[11] = 0.0f;

    print_node_diagonistics(node);

    return;
  }
};

#endif
