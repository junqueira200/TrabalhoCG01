cmake_minimum_required(VERSION 3.13)
project(TrabalhoCG01)

set(CMAKE_CXX_STANDARD 14)

add_executable(TrabalhoCG01 main.cpp
        TexturaPlano.cpp
        TexturaPlano.h
        glcWavefrontObject.h
        glcWavefrontObject.cpp Retangulo.cpp Retangulo.h cor.h Normal.cpp Normal.h Posicao.h Posicao.cpp OpenGL_CallBack.cpp OpenGL_CallBack.h Colisao.cpp Colisao.h objectsHandler.cpp objectsHandler.h cor.cpp)

target_link_libraries(TrabalhoCG01 -lglut -lGL -lGLU )

