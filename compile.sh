g++ src/main.cpp \
    dependencies/imgui/*.cpp \
    dependencies/imgui/backends/imgui_impl_sdl.cpp \
    dependencies/imgui/backends/imgui_impl_opengl3.cpp \
    dependencies/tinyfiledialogs/tinyfiledialogs.c \
    -o \
    bin/linux/graphics \
    -std=c++17 \
    -w \
    -Idependencies/imgui \
    -Idependencies/tinyfiledialogs \
    -Idependencies/stb-master \
    -I/usr/include/SDL2 \
    -Iinclude \
    -Llib \
    -lGL \
    -lGLU \
    -lGLEW \
    -lSDL2 \
    -g \
    -D LA_OPEN_GL
    