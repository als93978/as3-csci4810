#include <iostream>
#include <fstream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <GLFW/glfw3.h>

#include "lineDrawing.h"
#include "as3.h"
#include "Matrix.h"

using namespace std;

vector<Vertex> vertices; // 3D vertices/points table
vector<Vertex> projectedVertices; // 2D (projected) vertices/points table
vector<Line> lines; // lines/edges table

//const float d = 2.5; // cm
const float d = 2.5; // cm
//const int s = 50; // cm
const int s = 50; // cm
// const int viewportScaleX = 500;
// const int viewportScaleY = 500;
// const int viewportCenterX = 500;
// const int viewportCenterY = 500;
const int viewportScaleX = 500;
const int viewportScaleY = 500;
const int viewportCenterX = 500;
const int viewportCenterY = 500;

void init() {
    setWindowTitle("Assignment 3 - Austin Schultz");
    setWindowSize(1000, 1000);
    
    readVerticesFile();

    projectVertices();
}

void projectVertices() {
    for(int i = 0; i < vertices.size(); i++) {
	Vertex currentVertex = vertices[i];

	int screenX = ((d * currentVertex.x) / (s * currentVertex.z))
	    * viewportScaleX + viewportCenterX;
	int screenY = ((d * currentVertex.y) / (s * currentVertex.z))
	    * viewportScaleY + viewportCenterY;

	Vertex projectedVertex;
	projectedVertex.x = screenX;
	projectedVertex.y = screenY;

	if(projectedVertices.size() < vertices.size())
	    projectedVertices.push_back(projectedVertex);
	else
	    projectedVertices[i] = projectedVertex;
    }
}

void readVerticesFile() {
    string filename = "";
    ifstream fileInput;
    
    bool validFile = false;
    while(!validFile) {
        cout << "Please enter name of the vertices file to read from: ";

        getline(cin, filename);

        cout << endl;

        fileInput.open(filename);
    
        if(!fileInput.good()) {
            cout << "Failed to open file. Please enter a valid filename." << endl;
            cout << endl;
        }
        
        else
            validFile = true;
    }

    int count = 0;
    string inputLine;
    while(getline(fileInput, inputLine)) {
        vector<string> lineVertices = split(inputLine, " ");

        Vertex vertex1;
        vertex1.x = atoi(lineVertices[0].c_str());
        vertex1.y = atoi(lineVertices[1].c_str());
        vertex1.z = atoi(lineVertices[2].c_str());

        Vertex vertex2;
        vertex2.x = atoi(lineVertices[3].c_str());
        vertex2.y = atoi(lineVertices[4].c_str());
        vertex2.z = atoi(lineVertices[5].c_str());

        // only add vertices to table if they aren't already in it
        int vertex1Index = vertexIsUnique(vertex1);
        int vertex2Index = vertexIsUnique(vertex2);

        Line line;

	// vertex isn't in table yet
        if(vertex1Index == -1) {
            vertices.push_back(vertex1);
            line.v1Index = count;
	    count++;
        }

	// vertex is already in table
        else
            line.v1Index = vertex1Index;
        
        if(vertex2Index == -1) {
            vertices.push_back(vertex2);
            line.v2Index = count;
	    count++;
        }

        else
            line.v2Index = vertex2Index;

        lines.push_back(line);
    }
}

void startProcessing() {
    while(true) {
        getAndProcessInput();
    }
}

void getAndProcessInput() {
    string selection = "";

    while(atoi(selection.c_str()) == 0 || atoi(selection.c_str()) < 1 || atoi(selection.c_str()) > 5) {
        cout << "Options:" << endl;

        cout << "1 - Translate" << endl;
        cout << "2 - Basic scale" << endl;
        cout << "3 - Basic rotate" << endl;
        cout << "4 - Output vertices to file" << endl;
        cout << "5 - Quit" << endl;

        cout << "Please enter a number for the option "
             << "you would like to perform: ";
    
        getline(cin, selection);

        cout << endl;

        if(atoi(selection.c_str()) == 0 || atoi(selection.c_str()) < 1 || atoi(selection.c_str()) > 5) {
            cout << "Please input a number between 1 and 5 (inclusive)" << endl;
            cout << endl;
        }
    }

    switch(atoi(selection.c_str())) {
    case 1:
        getTranslateInfo();
        break;

    case 2:
        getBasicScaleInfo();
        break;
    case 3:
        getBasicRotateInfo();
        break;

    case 4:
        outputToFile();
        break;

    case 5:
        exit(0);
    }
}

void getTranslateInfo() {
    string translateX, translateY, translateZ = "";

    while(!isANumber(translateX) || !isANumber(translateY) || !isANumber(translateZ)) {
        cout << "Please enter the translate x value: ";
        getline(cin, translateX);

        cout << "Please enter the translate y value: ";
        getline(cin, translateY);
	
	cout << "Please enter the translate z value: ";
        getline(cin, translateZ);

        cout << endl;

        if(!isANumber(translateX) || !isANumber(translateY) || !isANumber(translateZ)) {
            cout << "Please enter valid numbers for translate x, translate y,"
		 << " and translate z." << endl;
            cout << endl;
        }
    }

    translate(atof(translateX.c_str()), atof(translateY.c_str()), atof(translateZ.c_str()));
}

void translate(double translateX, double translateY, double translateZ) {
    Matrix<double> tMatrix = getTranslationMatrix(translateX, translateY, translateZ);

    applyTransformation(tMatrix);

    projectVertices();
}

void getBasicScaleInfo() {
    string scaleX, scaleY, scaleZ = "";
    
    while(!isANumber(scaleX) || !isANumber(scaleY) || !isANumber(scaleZ)) {
        cout << "Please enter the scale x value: ";
        getline(cin, scaleX);

        cout << "Please enter the scale y value: ";
        getline(cin, scaleY);

	cout << "Please enter the scale z value: ";
        getline(cin, scaleZ);

        cout << endl;

        if(!isANumber(scaleX) || !isANumber(scaleY) || !isANumber(scaleZ)) {
            cout << "Please enter valid numbers for scale x, scale y, and scale z." << endl;
            cout << endl;
        }
    }

    basicScale(atof(scaleX.c_str()), atof(scaleY.c_str()), atof(scaleZ.c_str()));
}

void basicScale(double scaleX, double scaleY, double scaleZ) {
    Matrix<double> tMatrix = getBasicScaleMatrix(scaleX, scaleY, scaleZ);

    applyTransformation(tMatrix);

    projectVertices();
}

void getBasicRotateInfo() {
    string angle = ""; // aka, theta (in degrees)
    
    while(!isANumber(angle)) {
        cout << "Please enter the angle (theta) value (in degrees): ";
        getline(cin, angle);
        
        cout << endl;
        
        if(!isANumber(angle)) {
            cout << "Please enter a valid number for angle." << endl;
            cout << endl;
        }
    }

    basicRotate(atof(angle.c_str()) * M_PI / 180);
}

void basicRotate(double angle) {
    Matrix<double> tMatrix = getBasicRotateMatrix(angle);

    applyTransformation(tMatrix);

    projectVertices();
}

void applyTransformation(Matrix<double> &tMatrix) {
    for(int i = 0; i < vertices.size(); i++) {
	Vertex currentVertex = vertices[i];

	Matrix<double> vertexMatrix(1, 4);
	vertexMatrix(0, 0) = currentVertex.x;
	vertexMatrix(0, 1) = currentVertex.y;
	vertexMatrix(0, 2) = currentVertex.z;
	vertexMatrix(0, 3) = 1;

	Matrix<double> result = vertexMatrix * tMatrix;

	Vertex newVertex;
	newVertex.x = result(0, 0);
	newVertex.y = result(0, 1);
	newVertex.z = result(0, 2);

	vertices[i] = newVertex;
    }
}

Matrix<double> getTranslationMatrix(double translateX, double translateY, double translateZ) {
    Matrix<double> tMatrix(4, 4);
    tMatrix(0, 0) = 1;
    tMatrix(0, 1) = 0;
    tMatrix(0, 2) = 0;
    tMatrix(0, 3) = 0;
    tMatrix(1, 0) = 0;
    tMatrix(1, 1) = 1;
    tMatrix(1, 2) = 0;
    tMatrix(1, 3) = 0;
    tMatrix(2, 0) = 0;
    tMatrix(2, 1) = 0;
    tMatrix(2, 2) = 1;
    tMatrix(2, 3) = 0;
    tMatrix(3, 0) = translateX;
    tMatrix(3, 1) = translateY;
    tMatrix(3, 2) = translateZ;
    tMatrix(3, 3) = 1;

    return tMatrix;
}

Matrix<double> getBasicScaleMatrix(double scaleX, double scaleY, double scaleZ) {
    Matrix<double> tMatrix(4, 4);

    tMatrix(0, 0) = scaleX;
    tMatrix(0, 1) = 0;
    tMatrix(0, 2) = 0;
    tMatrix(0, 3) = 0;
    tMatrix(1, 0) = 0;
    tMatrix(1, 1) = scaleY;
    tMatrix(1, 2) = 0;
    tMatrix(1, 3) = 0;
    tMatrix(2, 0) = 0;
    tMatrix(2, 1) = 0;
    tMatrix(2, 2) = scaleZ;
    tMatrix(2, 3) = 0;
    tMatrix(3, 0) = 0;
    tMatrix(3, 1) = 0;
    tMatrix(3, 2) = 0;
    tMatrix(3, 3) = 1;

    return tMatrix;
}

Matrix<double> getBasicRotateMatrix(double angle) {
    Matrix<double> tMatrix(4, 4);

    tMatrix(0, 0) = cos(angle);
    tMatrix(0, 1) = sin(angle);
    tMatrix(0, 2) = 0;
    tMatrix(0, 3) = 0;
    tMatrix(1, 0) = -sin(angle);
    tMatrix(1, 1) = cos(angle);
    tMatrix(1, 2) = 0;
    tMatrix(1, 3) = 0;
    tMatrix(2, 0) = 0;
    tMatrix(2, 1) = 0;
    tMatrix(2, 2) = 1;
    tMatrix(2, 3) = 0;
    tMatrix(3, 0) = 0;
    tMatrix(3, 1) = 0;
    tMatrix(3, 2) = 0;
    tMatrix(3, 3) = 1;

    return tMatrix;
}

void outputToFile() {
    string filename = "";
    ofstream fileOutput;
    
    bool validFile = false;
    while(!validFile) {
        cout << "Please enter name of the file to output to "
             << "(File will be overwritten! If it doesn't exist, "
             << "it will be created): ";

        getline(cin, filename);

        cout << endl;

        fileOutput.open(filename);
    
        if(!fileOutput.good()) {
            cout << "Failed to open file. Please try again." << endl;
            cout << endl;
        }
        
        else
            validFile = true;
    }

    for(int i = 0; i < lines.size(); i++) {
        Line currentLine = lines[i];
        Vertex vertex1 = vertices[currentLine.v1Index];
        Vertex vertex2 = vertices[currentLine.v2Index];

        fileOutput << vertex1.x << " " << vertex1.y << " " << vertex1.z << " "
                   << vertex2.x << " " << vertex2.y << " " << vertex2.z << "\n";
    }
}

void draw() {
    for(int i = 0; i < lines.size(); i++) {
	Line currentLine = lines[i];
	Vertex vertex1 = projectedVertices[currentLine.v1Index];
	Vertex vertex2 = projectedVertices[currentLine.v2Index];

	drawLine(vertex1.x, vertex1.y,  vertex2.x, vertex2.y);
    }
}

vector<string> split(string str, string delim) {
    vector<string> result;

    int start = 0;
    int end = str.find(delim);

    while(end != string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + delim.length();
        end = str.find(delim, start);
    }

    result.push_back(str.substr(start, end));

    return result;
}

int vertexIsUnique(Vertex &vertex) {
    for(int i = 0; i < vertices.size(); i++) {
        Vertex currentVertex = vertices[i];

        if(currentVertex.x == vertex.x && currentVertex.y == vertex.y &&
           currentVertex.z == vertex.z)
            return i;
    }

    return -1;
}

bool isANumber(string str) {
    if(str[0] == '-')
        str = str.substr(1);

    if(str == "" || str == " ")
        return false;

    return str.find_first_not_of("0123456789.") == string::npos;
}
