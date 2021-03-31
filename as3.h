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
void outputToFile();
void draw();

// helper functions
std::vector<std::string> split(std::string str, std::string delim);
int vertexIsUnique(Vertex &vertex);
bool isANumber(std::string str);

#endif
