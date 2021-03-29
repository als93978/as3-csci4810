FLAGS = -g -Wall -O0 -pedantic-errors
GLFWCFLAGS = `pkg-config --cflags glfw3`
GLFWLIBS = `pkg-config --static --libs glfw3`
OTHERLIBS = -pthread

compile: as2.cpp lineDrawing.cpp
	g++ $(FLAGS) $(OTHERLIBS) $(GLFWCFLAGS) -o as2.out as2.cpp lineDrawing.cpp glad.c $(GLFWLIBS)

run: as2.out
	./as2.out

clean:
	rm as2.out
