#include "../include/flexon.hpp"
#include "../include/flexon/Text.hpp"
#include "../include/flexon/View.hpp"
#include "../renderer/GUI/flexonGuiRenderer.hpp"
#include <cassert>
int main() {

  char *hello = "hello world!";

  flexon([&hello](flexon_view **&mineNode) {
    View([&hello](mine mineNode) {
      Text([&hello](mine mineNode) { mineNode->Text = hello; });
    });
  });

  //  flexonRender render;
  return 0;
}
