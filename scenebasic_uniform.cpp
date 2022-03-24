#include "scenebasic_uniform.h"

#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

#include "helper/texture.h"


SceneBasic_Uniform::SceneBasic_Uniform() 
{
    ogre = ObjMesh::load("../COMP3015-CW1/media/bs_ears.obj", false, true);
}


void SceneBasic_Uniform::initScene()
{
    compile();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    float c = 1.5f;
    projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f * c, 0.3f * c, 0.1f, 100.0f);

    prog.setUniform("Line.Width", 0.75f);
    prog.setUniform("Line.Color", vec4(0.05f, 0.0f, 0.05f, 1.0f));
    prog.setUniform("Material.Kd", 0.7f, 0.7f, 0.7f);
    prog.setUniform("Light.Position", vec4(0.0f, 0.0f, 0.0f, 1.0f));
    prog.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
    prog.setUniform("Light.Intensity", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
    prog.setUniform("Material.Shininess", 100.0f);

}


void SceneBasic_Uniform::compile()
{
    try
    {
        prog.compileShader("shader/wireframe.vert");
        prog.compileShader("shader/wireframe.frag");
        prog.compileShader("shader/wireframe.geom");
        prog.link();
        prog.use();
    }
    catch (GLSLProgramException& e)
    {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

void SceneBasic_Uniform::update(float t)
{

}


void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vec3 cameraPos(0.0f, 0.0f, 3.0f);
    view = glm::lookAt(cameraPos, 
        vec3(0.0f, 0.0f, 0.0f), 
        vec3(0.0f, 1.0f, 0.0f));

    model = mat4(1.0f);
    setMatrices();
    ogre->render();

    glFinish();
}


void SceneBasic_Uniform::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
    prog.setUniform("ViewportMatrix", viewport);
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);

    float w2 = w / 2.0f;
    float h2 = h / 2.0f;
    viewport = mat4(vec4(w2, 0.0f, 0.0f, 0.0f),
        vec4(0.0f, h2, 0.0f, 0.0f),
        vec4(0.0f, 0.0f, 1.0f, 0.0f),
        vec4(w2 + 0, h2 + 0, 0.0f, 1.0f));
}