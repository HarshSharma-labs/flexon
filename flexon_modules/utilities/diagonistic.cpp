#include "../componentbase/component.hpp"
#include <iostream>

void printColor(int r, int g, int b);
void resetColor();

void print_node_diagonistics(struct base_view *node) {
  std::cout << "\n Node diagonostics :" << std::endl;
  std::cout << " Node mem addr                            : " << std::hex
            << node << std::endl;
  std::cout << " Node style : " << std::endl;
  std::cout << "   Node  height width                     : "
            << node->layout.height << " " << node->layout.width << std::endl
            << "   Node  x  y                             : " << node->layout.x
            << " " << node->layout.y << std::endl

            << "   Node padding{t,l,r,b}                  : "
            << node->layout.padding.top << "," << node->layout.padding.left
            << "," << node->layout.padding.right << ","
            << node->layout.padding.bottom << "\n"
            << "   Node margin{t,l,r,b}                   : "
            << node->layout.margin.top << "," << node->layout.margin.left << ","
            << node->layout.margin.right << "," << node->layout.margin.bottom
            << "\n"
            << "   Node border{t,l,r,b}                   : "
            << node->layout.border.top << "," << node->layout.border.left << ","
            << node->layout.border.right << "," << node->layout.border.bottom
            << "\n"
            << "   Node flex                              : "
            << node->layout.flex << "\nNode Decoration(colors) : " << std::endl
            << "   Node bgColor{rgba}                     : ";
  std::cout << "\n Node NDC coordinates :                   : \n";
  std::cout << " Node NDC coordinates topright :          :  x,y,z "
            << node->gpu_vertex.vertex[0] << ' ' << node->gpu_vertex.vertex[1]
            << ' ' << node->gpu_vertex.vertex[2];
  std::cout << "\n Node NDC coordinates botright :          :  x,y,z "
            << node->gpu_vertex.vertex[3] << ' ' << node->gpu_vertex.vertex[4]
            << ' ' << node->gpu_vertex.vertex[5];
  std::cout << "\n Node NDC coordinates botleft  :          :  x,y,z "
            << node->gpu_vertex.vertex[6] << ' ' << node->gpu_vertex.vertex[7]
            << ' ' << node->gpu_vertex.vertex[8];
  std::cout << "\n Node NDC coordinates topleft  :          :  x,y,z "
            << node->gpu_vertex.vertex[9] << ' ' << node->gpu_vertex.vertex[10]
            << ' ' << node->gpu_vertex.vertex[11];

  printColor(static_cast<int>(node->style.color.bgColor.r),
             static_cast<int>(node->style.color.bgColor.g),
             static_cast<int>(node->style.color.bgColor.b));

  std::cout << static_cast<int>(node->style.color.bgColor.r)
            << static_cast<int>(node->style.color.bgColor.g)
            << static_cast<int>(node->style.color.bgColor.b)
            << static_cast<int>(node->style.color.bgColor.a);
  resetColor();

  std::cout << "\n   Node Color{rgba}                       : ";
  printColor(static_cast<int>(node->style.color.color.r),
             static_cast<int>(node->style.color.color.g),
             static_cast<int>(node->style.color.color.b));

  std::cout << static_cast<int>(node->style.color.color.r)
            << static_cast<int>(node->style.color.color.g)
            << static_cast<int>(node->style.color.color.b)
            << static_cast<int>(node->style.color.color.a);
  resetColor();
  std::cout << "\n   Node border{t|b|l|r}                   : ";

  printColor(static_cast<int>(node->style.color.border.top.r),
             static_cast<int>(node->style.color.border.top.g),
             static_cast<int>(node->style.color.border.top.b));

  std::cout << static_cast<int>(node->style.color.border.top.r)
            << static_cast<int>(node->style.color.border.top.g)
            << static_cast<int>(node->style.color.border.top.b)
            << static_cast<int>(node->style.color.border.top.a);
  resetColor();
  std::cout << ' ';
  printColor(static_cast<int>(node->style.color.border.bottom.r),
             static_cast<int>(node->style.color.border.bottom.g),
             static_cast<int>(node->style.color.border.bottom.b));

  std::cout << static_cast<int>(node->style.color.border.bottom.r)
            << static_cast<int>(node->style.color.border.bottom.g)
            << static_cast<int>(node->style.color.border.bottom.b)
            << static_cast<int>(node->style.color.border.bottom.a);
  resetColor();
  std::cout << ' ';
  printColor(static_cast<int>(node->style.color.border.left.r),
             static_cast<int>(node->style.color.border.left.g),
             static_cast<int>(node->style.color.border.left.b));

  std::cout << static_cast<int>(node->style.color.border.left.r)
            << static_cast<int>(node->style.color.border.left.g)
            << static_cast<int>(node->style.color.border.left.b)
            << static_cast<int>(node->style.color.border.left.a);
  resetColor();
  std::cout << ' ';
  printColor(static_cast<int>(node->style.color.border.right.r),
             static_cast<int>(node->style.color.border.right.g),
             static_cast<int>(node->style.color.border.right.b));

  std::cout << static_cast<int>(node->style.color.border.right.r)
            << static_cast<int>(node->style.color.border.right.g)
            << static_cast<int>(node->style.color.border.right.b)
            << static_cast<int>(node->style.color.border.right.a);
  resetColor();
  std::cout << "  \n\n\n\n" << std::endl;
  return;
}

//"\033[48;2;255;255;255;38;2;255;0;0mhello\033[m\n"

void printColor(int r, int g, int b) {
  std::cout.flush();
  std::cout << "\033[48;2;0;0;0;1;2;7;38;2;" << std::to_string(r) << ';'
            << std::to_string(g) << ';' << std::to_string(b) << 'm';
  std::cout.flush();
};
void resetColor() {
  std::cout.flush();
  std::cout << "\033[m";
  std::cout.flush();
}
