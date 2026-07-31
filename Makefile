# Compiler
CXX := C:\\msys64\\ucrt64\\bin\\g++.exe # This will probably break for you!

# Library name
LIB := libRasterizationRenderer.a

# Compiler flags
CXXFLAGS := -std=c++17 -O3 -ffast-math -DNDEBUG \
	-I. \
	-IC:/msys64/mingw64/include \
	-Isrc/Helper \
	-Isrc/Math \
	-Isrc/Object \
	-Isrc/Rendering \
	-Isrc/Scenes

# Libraries needed by the demo executable
LDLIBS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lgdi32 -lwinmm


# Source files for the library
LIB_SRCS := \
	main.cpp \
	$(wildcard src/Helper/*.cpp) \
	$(wildcard src/Math/*.cpp) \
	$(wildcard src/Object/*.cpp) \
	$(wildcard src/Rendering/*.cpp) \
	$(wildcard src/Scenes/*.cpp)

# Object files
LIB_OBJS := $(LIB_SRCS:.cpp=.o)

# Demo executable
DEMO := renderer_demo
DEMO_SRC := src/example.cpp
DEMO_OBJ := $(DEMO_SRC:.cpp=.o)

# Default target
all: $(LIB) $(DEMO)

# Build static library
$(LIB): $(LIB_OBJS)
	ar rcs $@ $^

# Build demo executable
$(DEMO): $(DEMO_OBJ) $(LIB)
	$(CXX) $^ -o $@ $(LDLIBS)

# Compile .cpp -> .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(LIB_OBJS) $(DEMO_OBJ) $(LIB) $(DEMO)

.PHONY: all clean