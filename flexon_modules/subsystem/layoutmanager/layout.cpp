#include "../../components/decorator.hpp"
#include "../../utilities/utility.hpp"
#include "./layout.hpp"
#include <bitset>
#include <iostream>

static uint32_t auxcounter = 0;


static void calculatelayout(base_paint *ppaint, fiber *childs) {

  // static auto composeUniform = [](){}

  /*
   * used to strip the padding and margin andl align the child;
   * stripextra(vec4 &ppadding , vec4 &cmargin) => ppadding = padding of parent;
   *                  => cmargin = margin of child;
   */

  static auto stripvec4 = [](vec4<float> &vector, vec4<float> &geometry,
                             uint8_t flagsbits, enum unitypes STRIDE) {
    bool vppadding = utility::decorator::checkZero(flagsbits, STRIDE);

    if (vppadding)
      return;

    geometry.x += vector.z;
    geometry.y += vector.x;
    geometry.z -= vector.z + vector.w;
    geometry.w -= vector.x + vector.y;
  };

  static auto calculateNDC = [](base_quad &destmat,vec4<float> &dimension){
   mat4x3<float> vertex = {
     { dimension.x,dimension.y,1.0f},
     { dimension.x + dimension.z,dimension.y,1.0f},
     { dimension.x + dimension.z,dimension.y+dimension.w,1.0f},
     { dimension.x, dimension.y + dimension.w ,1.0f}
    };
    destmat.NDC = utility::ndc::NDCgen(vertex);
  };

  static auto flexcalculator = [](base_paint &ppaint, base_geo &geometry,
                                  fiber *childs) {
    float flexTotal = 0.0f;
    for (fiber *itr = childs; itr != nullptr; itr = itr->next) {
      if (itr->paint != nullptr)
        flexTotal += itr->paint->flex;
    }

    base_geo localGeo;
    vec4<float> parentstripped = geometry.dimension;
    stripvec4(ppaint.geometry.padding, parentstripped, ppaint.flagpadmar,
              UNITYPE_STRIDE_PADDING);
    stripvec4(ppaint.geometry.borderWidth, parentstripped, ppaint.flagborrad,
              UNITYPE_STRIDE_BORDERWIDTH);

    /*
     * px = parent position-X.
     * py = parent position-Y.
     */

    float px = parentstripped.x;
    float py = parentstripped.y;

     for (fiber *itr = childs; itr != nullptr; itr = itr->next) {
      if (itr->paint != nullptr) {

        utility::strings::memcpy64(&itr->paint->geometry, &localGeo,
                                   modifier::geosize64);
        localGeo.dimension.x = px;
        localGeo.dimension.y = py;
 
        switch (ppaint.layout_direction) {

        case LAYOUT_DIRECTION_ROW:
          localGeo.dimension.z =
              (itr->paint->flex / flexTotal) * parentstripped.z;
          localGeo.dimension.w = parentstripped.w;
                px += localGeo.dimension.z;
          stripvec4(localGeo.margin, localGeo.dimension, itr->paint->flagpadmar,
                    UNITYPE_STRIDE_MARGIN);

          break;
        case LAYOUT_DIRECTION_COL:

          localGeo.dimension.w =
              (itr->paint->flex / flexTotal) * parentstripped.w;
          localGeo.dimension.z = parentstripped.z;
          stripvec4(localGeo.margin, localGeo.dimension, itr->paint->flagpadmar,
                    UNITYPE_STRIDE_MARGIN);
          py += localGeo.dimension.w;

          break;
        };
        calculateNDC(itr->quad,localGeo.dimension);
        utility::strings::memcpy64(&localGeo.dimension,
                                   &itr->paint->geometry.dimension,
                                   modifier::vec4floatsize64);
      };
    };
  };

  if (ppaint == nullptr)
    return;

  switch (ppaint->display) {
  case DISPLAY_FLEX:
    flexcalculator(*ppaint, ppaint->geometry, childs);
  case DISPLAY_GRID:
  case DISPLAY_CUSTOM:
  case DISPLAY_NONE:
    return;
    break;
  };
};

// used to get a new fiber tree if there is a view added
fiber *getfiberfromview(pview *which) {
  fiber *tmp = new fiber;
  tmp->fiberid = utility::strings::fnv_1hash(auxcounter);
  tmp->child = which->child;
  if (which->style)
    which->style(tmp);

  return tmp;
};

void deepCloneFiber(fiber *which) {
  fiber *tmp = new fiber;
  tmp->clone = which;
  which->clone = tmp;
  tmp->fiberid = utility::strings::fnv_1hash(auxcounter);
  tmp->paint = modifier::deepClone(which->paint, tmp->fiberid);
};

void buildfibertree(void (*node)(), fiber *parent) {

  node();
  pview *view = gettop();
  if (view == nullptr)
    return;

  fiber *topFiber = getfiberfromview(view);
  topFiber->parent = parent;
  deepCloneFiber(topFiber);

  fiber *nextFiber = topFiber;
  fiber *tmp = nullptr;

  // function to create fiber from tree
  for (auto *itr = view->next; itr != nullptr; itr = itr->next) {
    tmp = getfiberfromview(itr);
    tmp->parent = parent;
    deepCloneFiber(tmp);
    nextFiber->next = tmp;
    nextFiber = tmp;
  };

  flushViewTree();
  calculatelayout(parent->paint, topFiber);

  // function to evaluate the entire child and create the fiber
  for (auto *itr = topFiber; itr != nullptr; itr = itr->next) {
    if (itr->child)
      buildfibertree(itr->child, itr);
  };
};

/* here parameters are as follow:
 * void(*node)() = app_main() function in app.cpp file
 * width = width of the screen
 * height = height of the screen
 */

void initial_commit(void (*node)(), void (*main)(fiber *wrap)) {
  // std::cout<<std::hex<<fnv_1hash(0xffcaffac)<<std::endl;

  fiber *fibermain = new fiber;
  fibermain->fiberid = 1;
  main(fibermain);
  deepCloneFiber(fibermain);

  if (fibermain->paint == nullptr) {
    std::cout << "post startup failed exiting.\nprobable cause not"
                 " commiting a root layout in post_startup function"
              << std::endl;
    return;
  };
  utility::ndc::setNDCmat(fibermain->paint->geometry.dimension.z,
                          fibermain->paint->geometry.dimension.w);
  buildfibertree(node, fibermain);
};
