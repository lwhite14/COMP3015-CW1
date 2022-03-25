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


SceneBasic_Uniform::SceneBasic_Uniform() :  cube(1.25f),
                                            light(0.1f),
                                            lightPosition(vec3(3.0, -2.0, 5.0)),
                                            angle(0.0f), 
                                            tPrev(0.0f), 
                                            rotSpeed(glm::pi<float>() / 4.0f), 
                                            sky(100.0f)
{ 
    ufo = ObjMesh::load("../COMP3015-CW1/media/ufo.obj");
}


void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);

    projection = mat4(1.0f);

    skyboxTex = Texture::loadCubeMap("../COMP3015-CW1/media/texture/nova/nova");
    cubeTex = Texture::loadTexture("../COMP3015-CW1/media/texture/metal.jpg");
    ufoDiffuseTex = Texture::loadTexture("../COMP3015-CW1/media/texture/ufo_diffuse.png");
    ufoNormalTex = Texture::loadTexture("../COMP3015-CW1/media/texture/ufo_normal.png");

    textureProgram.setUniform("Light.Position", view * vec4(lightPosition, 1.0f));
    textureProgram.setUniform("Light.La", vec3(0.15f, 0.15f, 0.2f));
    textureProgram.setUniform("Light.Ld", vec3(0.45f, 0.45f, 0.5f));
    textureProgram.setUniform("Light.Ls", vec3(1.0f, 1.0f, 1.0f));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, cubeTex);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, ufoDiffuseTex);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, ufoNormalTex);
}

void SceneBasic_Uniform::compile()
{
    try
    {
        skyboxProgram.compileShader("shader/skybox.vert");
        skyboxProgram.compileShader("shader/skybox.frag");
        textureProgram.compileShader("shader/basic_textured.vert");
        textureProgram.compileShader("shader/basic_textured.frag");
        skyboxProgram.link();
        textureProgram.link();
        skyboxProgram.use();
        textureProgram.use();
    }
    catch (GLSLProgramException& e)
    {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

void SceneBasic_Uniform::update(float t)
{
    float deltaT = t - tPrev;
    if (tPrev == 0.0f)
    {
        deltaT = 0.0f;
    }
    tPrev = t;
    angle += rotSpeed * deltaT;
    if (angle > glm::two_pi<float>())
    {
        angle -= glm::two_pi<float>();
    }
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos = vec3(7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

    skyboxProgram.use();
    model = mat4(1.0f);
    setMatrices(skyboxProgram);
    sky.render();

    textureProgram.use();
    textureProgram.setUniform("Material.Kd", 0.65f, 0.65f, 0.65f);
    textureProgram.setUniform("Material.Ks", 1.0f, 1.0f, 1.0f);
    textureProgram.setUniform("Material.Ka", 0.65f, 0.65f, 0.65f);
    textureProgram.setUniform("Material.Shininess", 64.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    setMatrices(textureProgram);
    cube.render();

    textureProgram.use();
    textureProgram.setUniform("Material.Kd", 1.0f, 1.0f, 1.0f);
    textureProgram.setUniform("Material.Ks", 1.0f, 1.0f, 1.0f);
    textureProgram.setUniform("Material.Ka", 1.0f, 1.0f, 1.0f);
    textureProgram.setUniform("Material.Shininess", 0.0f);
    model = mat4(1.0f);
    model = glm::translate(model, lightPosition);
    setMatrices(textureProgram);
    light.render();

    textureProgram.use();
    textureProgram.setUniform("Material.Kd", 0.5f, 0.5f, 0.5f);
    textureProgram.setUniform("Material.Ks", 1.0f, 1.0f, 1.0f);
    textureProgram.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    textureProgram.setUniform("Material.Shininess", 64.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(-40.0f, -10.0f, -40.0f));
    setMatrices(textureProgram);
    ufo->render();
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
    projection = glm::perspective(glm::radians(95.0f), (float)w / h, 0.3f, 100.0f);
}