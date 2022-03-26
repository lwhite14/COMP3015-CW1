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


SceneBasic_Uniform::SceneBasic_Uniform() :  pointLight(Light(vec4(0.0, 0.0, 0.0, 1.0), vec3(1.0, 1.0, 1.0), vec3(0.5, 0.5, 1.0), vec3(1.0, 1.0, 1.0))),
                                            spotLight(Light(vec4(-25.0f, 30.0f, 0.0f, 1.0f),
                                                vec3(0.5f),
                                                vec3(0.9f),
                                                vec3(0.9f),
                                                vec3(),
                                                50.0f,
                                                glm::radians(45.0f))),
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

        spotlightProgram.compileShader("shader/spotlight.vert");
        spotlightProgram.compileShader("shader/spotlight.frag");
        spotlightProgram.link();
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
    model = glm::rotate(model, 5.0f, vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(0.0f, 20.0f, -25.0f));
    setMatrices(ufoProgram);
    bindTex(GL_TEXTURE0, ufoDiffuseTex);
    bindTex(GL_TEXTURE1, ufoNormalTex);
    ufo->render();

    basicProgram.use();
    basicProgram.setUniform("Light.Position", view * pointLight.position);
    basicProgram.setUniform("Light.La", pointLight.ambient);
    basicProgram.setUniform("Light.Ld", pointLight.diffuse);
    basicProgram.setUniform("Light.Ls", pointLight.specular);
    basicProgram.setUniform("Material.Kd", 1.0f, 1.0f, 1.0f);
    basicProgram.setUniform("Material.Ks", 1.0f, 1.0f, 1.0f);
    basicProgram.setUniform("Material.Ka", 1.0f, 1.0f, 1.0f);
    basicProgram.setUniform("Material.Shininess", 0.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(pointLight.position.x, pointLight.position.y, pointLight.position.z));
    setMatrices(basicProgram);
    lightSource.render();

    spotlightProgram.use();
    spotlightProgram.setUniform("Spot.L", vec3(0.9f));
    spotlightProgram.setUniform("Spot.La", vec3(0.5f));
    spotlightProgram.setUniform("Spot.Exponent", 50.0f);
    spotlightProgram.setUniform("Spot.Cutoff", glm::radians(15.0f));
    mat3 normalMatrix = mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));
    spotLight.direction = normalMatrix * vec3(-spotLight.position);
    spotlightProgram.setUniform("Spot.Position", vec3(view * spotLight.position));
    spotlightProgram.setUniform("Spot.Direction", spotLight.direction);
    spotlightProgram.setUniform("Material.Kd", 0.5f, 0.5f, 0.5f);
    spotlightProgram.setUniform("Material.Ks", 0.5f, 0.5f, 0.5f);
    spotlightProgram.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    spotlightProgram.setUniform("Material.Shininess", 128.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(30.0f, -40.0f, 0.0f));
    setMatrices(spotlightProgram);
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