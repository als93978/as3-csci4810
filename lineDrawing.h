#ifndef LINE_DRAWING_H
#define LINE_DRAWING_H

#include <GLFW/glfw3.h>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int start();
void setWindowTitle(std::string newWindowTitle);
void drawLine(int x0, int y0, int x1, int y1);
void drawLineBasic(int x0, int y0, int x1, int y1);
void drawLineBresenham(int x0, int y0, int x1, int y1);
void drawPixel(int x, int y);
int getWidth();
int getHeight();

#endif
