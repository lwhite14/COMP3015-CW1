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
    GLSLProgram cubeProgram;
    std::unique_ptr<ObjMesh> ogre;
    SkyBox sky;
    Cube cube;
    Cube light;

    glm::vec3 lightPosition;

    float cameraAngle;

    void setMatrices(GLSLProgram& prog);
    void compile();
public:
    SceneBasic_Uniform();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H