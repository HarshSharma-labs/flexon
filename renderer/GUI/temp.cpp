/*
void initRectangle(graphics *test) {
  float height = (float)test->windowHeight;
  float width = (float)test->windowWidth;
  float x = 0.0f;
  float y = 0.0f;
  rect1[0] = (2.0f * (x + width)) / (float)test->windowWidth - 1;
  rect1[1] = 1 - (2 * y) / (float)test->windowHeight;

  rect1[3] = (2 * (x + width)) / (float)test->windowWidth - 1;
  rect1[4] = 1 - 2 * (y + height) / (float)test->windowHeight;

  rect1[6] = (2 * x) / (float)test->windowWidth - 1;
  rect1[7] = 1 - (2 * (y + height)) / (float)test->windowHeight;

  rect1[9] = (2 * x) / (float)test->windowWidth - 1;
  rect1[10] = 1 - (2 * y) / (float)test->windowHeight;
  BoxSize[0] = width;
  BoxSize[1] = height;
  BoxSize[2] = x;
  BoxSize[3] = y;
};
*/

/*
* init code of vao
*
  initRectangle(window);
  tmp_graphics = window;
  glGenVertexArrays(1, VAOs);

  glGenBuffers(1, VBOs);
  glGenBuffers(1, &EBO);

  // Set up rect1
  glBindVertexArray(VAOs[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rect1), rect1, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glClearColor(genCol(), genCol(), genCol(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(window->flexonWindow);

*/

/*
void glrefresh(GLFWwindow *window) {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // glUseProgram(glshaderprogram);
  initRectangle(tmp_graphics);
  glBindVertexArray(VAOs[0]);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glfwSwapBuffers(window);
};
*/
