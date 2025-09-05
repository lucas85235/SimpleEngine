#include "Renderer.h"

#ifdef __APPLE__
  #include <OpenGL/gl.h>
#else
  #include <GL/gl.h>
#endif

void Renderer::draw() {
    // Clear background
    glClearColor(0.08f, 0.10f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a colorful triangle using fixed pipeline (immediate mode)
    glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);  glVertex2f(-0.6f, -0.5f);
        glColor3f(0.f, 1.f, 0.f);  glVertex2f( 0.6f, -0.5f);
        glColor3f(0.f, 0.f, 1.f);  glVertex2f( 0.0f,  0.6f);
    glEnd();
}
