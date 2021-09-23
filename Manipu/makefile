TARGET = Manipulation

SRCS =  main.cpp 
SRCS += Algorithm.cpp Robot.cpp Shape.cpp Configuration.cpp LShape.cpp
SRCS += Node.cpp RRT.cpp Wall.cpp maniMath.cpp dbscan.cpp Rectangle.cpp
OBJS = $(SRCS:.cpp=.o)

CXX = g++
CXXFLAGS = -g -Wall
INCDIR = -I/usr/include

#LIBDIR = -L/usr/local/lib
#LIBS = -lompl -lmlpack

all: $(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LIBDIR) $(LIBS)

clean:
	rm -f *.o

depend:
	makedepend $(INCDIR) $(SRCS)
