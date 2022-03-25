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


SceneBasic_Uniform::SceneBasic_Uniform() :  lightPosition(vec3(2.0, 2.0, -2.0)), 
                                            sky(250.0f)
{ 
    ufo = ObjMesh::load("../COMP3015-CW1/media/ufo.obj");
}


void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);

    projection = mat4(1.0f);

    GLuint skyboxTex = Texture::loadCubeMap("../COMP3015-CW1/media/texture/nova/nova");
    GLuint ufoDiffuseTex = Texture::loadTexture("../COMP3015-CW1/media/texture/ufo_diffuse.png");
    GLuint ufoNormalTex = Texture::loadTexture("../COMP3015-CW1/media/texture/ufo_normal.png");


    ufoProgram.setUniform("Light.Position", view * vec4(lightPosition, 1.0f));
    ufoProgram.setUniform("Light.La", vec3(0.4f, 0.4f, 0.45f));
    ufoProgram.setUniform("Light.Ld", vec3(0.75f, 0.75f, 0.8f));
    ufoProgram.setUniform("Light.Ls", vec3(1.0f, 1.0f, 1.0f));

    // Load texture file into channel 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    // Load texture file into channel 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ufoDiffuseTex);
    // Load texture file into channel 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, ufoNormalTex);
}

void SceneBasic_Uniform::compile()
{
    try
    {
        skyboxProgram.compileShader("shader/skybox.vert");
        skyboxProgram.compileShader("shader/skybox.frag");
        skyboxProgram.link();
        skyboxProgram.use();

        ufoProgram.compileShader("shader/normal_map.vert");
        ufoProgram.compileShader("shader/normal_map.frag");
        ufoProgram.link();
        ufoProgram.use();
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
    ufoProgram.setUniform("Material.Kd", 0.5f, 0.5f, 0.5f);
    ufoProgram.setUniform("Material.Ks", 0.5f, 0.5f, 0.5f);
    ufoProgram.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    ufoProgram.setUniform("Material.Shininess", 128.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(-30.0f, -40.0f, 0.0f));
    setMatrices(ufoProgram);
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
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 500.0f);
}