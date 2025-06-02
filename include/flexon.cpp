#include "flexon.hpp"

void flexon::flexonInitGraphics(void) {
  if (initGUI(&flexonGraphics)) {
    flexonContextGui = flexonGraphics.flexonWindow;
    root.lay.height = flexonGraphics.windowHeight;
    root.lay.width = flexonGraphics.windowWidth;
    root.lay.x = 0.0f;
    root.lay.y = 0.0f;
    fragShader.shaderProgram = SHADER::loadShader();
    SHADER::loadShaderUniform(fragShader.shaderProgram, &fragShader,
                              SHADER::viewUniforms, &objRender);

    calculatelayout(root.children, &root.lay, &root.lay);

    startGui(&flexonGraphics, root.children, &fragShader);
  }
  return;
}

flexon::flexon(std::function<void(flexonView **&)> composable) {
  tmp = &root.children;
  composable(tmp);

  flexonInitGraphics();
  return;
}
