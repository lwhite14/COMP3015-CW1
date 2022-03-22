#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

#include "helper/plane.h"
#include "helper/objmesh.h"
#include "helper/teapot.h"
#include <glm/glm.hpp>
#include "helper/cube.h"
#include "helper/skybox.h"
#include "helper/torus.h"

class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog;

    GLuint fsQuad, fboHandle, renderTex;
    Plane plane;
    Torus torus;
    Teapot teapot;
    float angle;
    float tPrev, rotSpeed;
    void setMatrices();
    void compile();
    void setupFBO();
    void pass1();
    void pass2();

public:
    SceneBasic_Uniform();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H