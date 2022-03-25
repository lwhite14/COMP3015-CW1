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
    GLSLProgram skyboxProgram;
    GLSLProgram ufoProgram;
    GLSLProgram basicProgram;
    std::unique_ptr<ObjMesh> ufo;
    SkyBox sky;
    Cube lightSource;

    glm::vec3 lightPosition;

    void setMatrices(GLSLProgram& prog);
    void compile();
public:
    SceneBasic_Uniform();

    void initScene();
    void update(float t, GLFWwindow* window);
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H