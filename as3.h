#ifndef AS3_H
#define AS3_H

#include <string>
#include <vector>
#include "Matrix.h"

struct Vertex {
    int x, y, z;
};

struct Line {
    int v1Index, v2Index;
};

void init();
void readVerticesFile();
void projectVertices();
void startProcessing();
void getAndProcessInput();
void getTranslateInfo();
void translate(double translateX, double translateY, double translateZ);
void getBasicScaleInfo();
void basicScale(double scaleX, double scaleY, double scaleZ);
void getBasicRotateInfo();
void basicRotate(double angle);
void applyTransformation(Matrix<double> &tMatrix);
Matrix<double> getTranslationMatrix(double translateX, double translateY, double translateZ);
Matrix<double> getBasicScaleMatrix(double scaleX, double scaleY, double scaleZ);
Matrix<double> getBasicRotateMatrix(double angle);
void outputToFile();
void draw();

// helper functions
std::vector<std::string> split(std::string str, std::string delim);
int vertexIsUnique(Vertex &vertex);
bool isANumber(std::string str);

#endif
