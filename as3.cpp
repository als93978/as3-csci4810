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

const float d = 2.5; // cm
const int s = 50; // cm
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

	projectedVertices.push_back(projectedVertex);
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
        // getTranslateInfo();
        cout << "Doing getTranslateInfo()" << endl << endl;
        break;

    case 2:
        // getBasicScaleInfo();
        cout << "Doing getBasicScaleInfo()" << endl << endl;
        break;
    case 3:
        // getBasicRotateInfo();
        cout << "Doing getBasicRotateInfo()" << endl << endl;
        break;

    case 4:
        outputToFile();
        break;

    case 5:
        exit(0);
    }
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
