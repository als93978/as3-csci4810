FLAGS = -g -Wall -O0 -pedantic-errors
GLFWCFLAGS = `pkg-config --cflags glfw3`
GLFWLIBS = `pkg-config --static --libs glfw3`
OTHERLIBS = -pthread

compile: as3.cpp lineDrawing.cpp
	g++ $(FLAGS) $(OTHERLIBS) $(GLFWCFLAGS) -o as3.out as3.cpp lineDrawing.cpp glad.c $(GLFWLIBS)

run: as3.out
	./as3.out

clean:
	rm as3.out
