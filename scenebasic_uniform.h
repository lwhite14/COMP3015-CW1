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
#include "helper/sphere.h"

class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog;

    GLuint hdrFBO;
    GLuint quad;
    GLuint hdrTex, avgTex;
    Plane plane;
    Sphere sphere;
    Teapot teapot;
    void setMatrices();
    void compile();
    void setupFBO();
    void pass1();
    void pass2();
    void computeLogAveLuminance();
    void drawScene();

public:
    SceneBasic_Uniform();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H