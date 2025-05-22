#ifndef __FLEXON_RECT__
#define __FLEXON_RECT__

/*/ Set up rect2
glBindVertexArray(VAOs[1]);
glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
glBufferData(GL_ARRAY_BUFFER, sizeof(rect2), rect2, GL_STATIC_DRAW);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Reuse EBO
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
glEnableVertexAttribArray(0);
*/

/*
 glUniform3f(colorLocation, genCol(), genCol(), genCol());
 glBindVertexArray(VAOs[1]);
 glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
*/

#endif
