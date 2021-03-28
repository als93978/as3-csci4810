#include <iostream>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <string>
#include <thread>

#include "lineDrawing.h"
#include "as2.h"

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;

string windowTitle = "Line Drawing Engine";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int start() {
    GLFWwindow* window;

    if(!glfwInit()) {
        cerr << "GLFW initialization failed" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(WIDTH, HEIGHT, windowTitle.c_str(), NULL, NULL);
    if(!window) {
        cerr << "OpenGL Context/Window creation failed" << endl;
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cerr << "GLAD initialization failed" << endl;
        return -1;
    }
        
    //glfwGetFramebufferSize(window, &width, &height);
    
    glViewport(0, 0, WIDTH, HEIGHT);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    
    glDisable(GL_DEPTH_TEST);

    // Create processing thread
    thread processingThread(startProcessing);

    // The core of the rendering thread (this thread) is below
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0f, 1.0f, 1.0f);
        
        draw();
        
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void setWindowTitle(string newWindowTitle) {
    windowTitle = newWindowTitle;
}

void drawLine(int x0, int y0, int x1, int y1) {
    drawLineBresenham(x0, y0, x1, y1);
}

void drawLineBasic(int x0, int y0, int x1, int y1) {
    float deltaX = x1 - x0;
    float deltaY = y1 - y0;
    float slope = deltaY / deltaX;

    if(deltaY == 0) {
	if(deltaX < 0) {
	    int tempX0 = x0;
	    int tempY0 = y0;

	    x0 = x1;
	    y0 = y1;

	    x1 = tempX0;
	    y1 = tempY0;
	}
	
	for(int i = 0; i < abs(deltaX); i++) {
	    int x = x0 + i;

            drawPixel(x, y0);
	}
    }

    else if(deltaX == 0) {
	if(deltaY < 0) {
	    int tempX0 = x0;
	    int tempY0 = y0;

	    x0 = x1;
	    y0 = y1;

	    x1 = tempX0;
	    y1 = tempY0;
	}

	for(int i = 0; i < abs(deltaY); i++) {
	    int y = y0 + i;

            drawPixel(x0, y);
	}
    }
    
    else if(abs(slope) < 1) {
	if(deltaX < 0) {
	    int tempX0 = x0;
	    int tempY0 = y0;

	    x0 = x1;
	    y0 = y1;

	    x1 = tempX0;
	    y1 = tempY0;
	}

	for(int i = 0; i < abs(deltaX); i++) {
	    int x = x0 + i;
	    int y = slope * i + y0;
            
	    drawPixel(x, y);
	}
    }

    else {
	if(deltaY < 0) {
	    int tempX0 = x0;
	    int tempY0 = y0;

	    x0 = x1;
	    y0 = y1;

	    x1 = tempX0;
	    y1 = tempY0;
	}

	float intercept = -slope * x0 + y0;
	
    	for(int i = 0; i < abs(deltaY); i++) {
    	    int y = y0 + i;
    	    int x = (y - intercept) / slope;

    	    drawPixel(x, y);
    	}
    }
}

void drawLineBresenham(int x0, int y0, int x1, int y1) { 
    int deltaX = x1 - x0;
    int deltaY = y1 - y0;
    int xInc = 1;
    int yInc = 1;

    int error = 0;
    int inc1 = 0;
    int inc2 = 0;

    int x = x0;
    int y = y0;

    if(deltaY == 0) {
	if(x1 < x0) {
	    int tempX0 = x0;
	    int tempY0 = y0;
	
	    x0 = x1;
	    y0 = y1;
	
	    x1 = tempX0;
	    y1 = tempY0;

	    deltaX = x1 - x0;
	    deltaY = y1 - y0;
	}
	
	for(int x = x0; x < x1; x++) {
	    drawPixel(x, y0);
	}
    }

    else if(deltaX == 0) {
	if(y1 < y0) {
	    int tempX0 = x0;
	    int tempY0 = y0;
	
	    x0 = x1;
	    y0 = y1;
	
	    x1 = tempX0;
	    y1 = tempY0;

	    deltaX = x1 - x0;
	    deltaY = y1 - y0;
	}
	
	for(int y = y0; y < y1; y++) {
	    drawPixel(x0, y);
	}
    }

    else if(abs(deltaX) > abs(deltaY)) {
 	if(x1 < x0) {
	    int tempX0 = x0;
	    int tempY0 = y0;
	
	    x0 = x1;
	    y0 = y1;
	
	    x1 = tempX0;
	    y1 = tempY0;

	    deltaX = x1 - x0;
	    deltaY = y1 - y0;

	    y = y0;
	}

	if(deltaY < 0) {
	    yInc = -1;
	    deltaY = -deltaY;
	}

	error = (deltaY << 1) - deltaX;
	inc1 = deltaY << 1;
	inc2 = (deltaY - deltaX) << 1;
        
	for(int x = x0; x < x1; x++) {	
	    drawPixel(x, y);

	    if(error < 0)
		error += inc1;

	    else {
		y += yInc;
		error += inc2;
	    }
	}
    }

    else {
	if(y1 < y0) {
	    int tempX0 = x0;
	    int tempY0 = y0;
	
	    x0 = x1;
	    y0 = y1;
	
	    x1 = tempX0;
	    y1 = tempY0;

	    deltaX = x1 - x0;
	    deltaY = y1 - y0;

	    x = x0;
	}

	if(deltaX < 0) {
	    xInc = -1;
	    deltaX = -deltaX;
	}

	error = (deltaX << 1) - deltaY;
	inc1 = deltaX << 1;
	inc2 = (deltaX - deltaY) << 1;

	for(int y = y0; y < y1; y++) {		
	    drawPixel(x, y);

	    if(error < 0)
		error += inc1;

	    else {
		x += xInc;
		error += inc2;
	    }
	}
    }
}

void drawPixel(int x, int y) {
    glBegin(GL_POINTS);
    
    glVertex2f(x + 0.5, (HEIGHT - y) - 0.5);

    glEnd();
}

int getWidth() {
    return WIDTH;
}

int getHeight() {
    return HEIGHT;
}

int main() {    
    init();
    
    int statusCode = start();
    
    return statusCode;
}
