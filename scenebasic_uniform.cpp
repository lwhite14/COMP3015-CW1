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


SceneBasic_Uniform::SceneBasic_Uniform() :  pointLight(PointLight(vec4(2.0, 2.0, -2.0, 1.0), vec3(1.0, 1.0, 1.0), vec3(1.0, 1.0, 1.0), vec3(1.0, 1.0, 1.0))),
                                            sky(250.0f),
                                            lightSource(0.5f)
{ 
    ufo = ObjMesh::load("../COMP3015-CW1/media/ufo.obj");
    meteor = ObjMesh::load("../COMP3015-CW1/media/meteor.obj");
}


void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);

    projection = mat4(1.0f);

    skyboxTex = Texture::loadCubeMap("../COMP3015-CW1/media/texture/nova/nova");
    ufoDiffuseTex = Texture::loadTexture("../COMP3015-CW1/media/texture/ufo_diffuse.png");
    ufoNormalTex = Texture::loadTexture("../COMP3015-CW1/media/texture/ufo_normal.png");
    rockTex = Texture::loadTexture("../COMP3015-CW1/media/texture/rock.jpg");
}

void SceneBasic_Uniform::compile()
{
    try
    {
        skyboxProgram.compileShader("shader/skybox.vert");
        skyboxProgram.compileShader("shader/skybox.frag");
        skyboxProgram.link();

        ufoProgram.compileShader("shader/normal_map.vert");
        ufoProgram.compileShader("shader/normal_map.frag");
        ufoProgram.link();

        basicProgram.compileShader("shader/basic.vert");
        basicProgram.compileShader("shader/basic.frag");
        basicProgram.link();

        basicTexturedProgram.compileShader("shader/basic_textured.vert");
        basicTexturedProgram.compileShader("shader/basic_textured.frag");
        basicTexturedProgram.link();
    }
    catch (GLSLProgramException& e)
    {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

void SceneBasic_Uniform::update(float t, GLFWwindow* window)
{
    camera.UpdateDeltaTime();
    camera.Movement();
    camera.KeyCallback(window);
    camera.MouseCallback(window);
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = camera.ViewLookAt(view);

    skyboxProgram.use();
    model = mat4(1.0f);
    setMatrices(skyboxProgram);
    sky.render();

    ufoProgram.use();
    ufoProgram.setUniform("Light.Position", view * pointLight.position);
    ufoProgram.setUniform("Light.La", pointLight.ambient);
    ufoProgram.setUniform("Light.Ld", pointLight.diffuse);
    ufoProgram.setUniform("Light.Ls", pointLight.specular);
    ufoProgram.setUniform("Material.Kd", 0.5f, 0.5f, 0.5f);
    ufoProgram.setUniform("Material.Ks", 0.5f, 0.5f, 0.5f);
    ufoProgram.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    ufoProgram.setUniform("Material.Shininess", 128.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(-30.0f, -40.0f, 0.0f));
    setMatrices(ufoProgram);
    bindTex(GL_TEXTURE0, ufoDiffuseTex);
    bindTex(GL_TEXTURE1, ufoNormalTex);
    ufo->render();

    basicProgram.use();
    ufoProgram.setUniform("Light.Position", view * pointLight.position);
    ufoProgram.setUniform("Light.La", pointLight.ambient);
    ufoProgram.setUniform("Light.Ld", pointLight.diffuse);
    ufoProgram.setUniform("Light.Ls", pointLight.specular);
    ufoProgram.setUniform("Material.Kd", 1.0f, 1.0f, 1.0f);
    ufoProgram.setUniform("Material.Ks", 1.0f, 1.0f, 1.0f);
    ufoProgram.setUniform("Material.Ka", 1.0f, 1.0f, 1.0f);
    ufoProgram.setUniform("Material.Shininess", 0.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(pointLight.position.x, pointLight.position.y, pointLight.position.z));
    setMatrices(basicProgram);
    lightSource.render();

    basicTexturedProgram.use();
    basicTexturedProgram.setUniform("Light.Position", view * pointLight.position);
    basicTexturedProgram.setUniform("Light.La", pointLight.ambient);
    basicTexturedProgram.setUniform("Light.Ld", pointLight.diffuse);
    basicTexturedProgram.setUniform("Light.Ls", pointLight.specular);
    basicTexturedProgram.setUniform("Material.Kd", 0.5f, 0.5f, 0.5f);
    basicTexturedProgram.setUniform("Material.Ks", 0.5f, 0.5f, 0.5f);
    basicTexturedProgram.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    basicTexturedProgram.setUniform("Material.Shininess", 128.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(30.0f, 40.0f, 0.0f));
    setMatrices(basicTexturedProgram);
    bindTex(GL_TEXTURE0, rockTex);
    meteor->render();
}

void SceneBasic_Uniform::setMatrices(GLSLProgram& prog)
{
    mat4 mv = view * model;
    prog.setUniform("MVP", projection * mv);
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 500.0f);
}

void SceneBasic_Uniform::bindTex(GLuint unit, GLuint texture)
{
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, texture);
}