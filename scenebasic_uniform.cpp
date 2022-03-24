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


SceneBasic_Uniform::SceneBasic_Uniform() :  cameraAngle(35.0f),
                                            cube(1.25f),
                                            light(0.1f),
                                            lightPosition(vec3(2.0, 2.0, -2.0))
{ }


void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);

    projection = mat4(1.0f);

    GLuint skyboxTex = Texture::loadCubeMap("../COMP3015-CW1/media/texture/nova/nova");
    GLuint cubeTex = Texture::loadTexture("../COMP3015-CW1/media/texture/brick1.jpg");

    cubeProgram.setUniform("Light.Position", view * vec4(lightPosition, 1.0f));
    cubeProgram.setUniform("Light.La", vec3(0.2f, 0.2f, 0.2f));
    cubeProgram.setUniform("Light.Ld", vec3(0.5f, 0.5f, 0.5f));
    cubeProgram.setUniform("Light.Ls", vec3(1.0f, 1.0f, 1.0f));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, cubeTex);
}

void SceneBasic_Uniform::compile()
{
    try
    {
        skyboxProgram.compileShader("shader/skybox.vert");
        skyboxProgram.compileShader("shader/skybox.frag");        
        cubeProgram.compileShader("shader/basic.vert");
        cubeProgram.compileShader("shader/basic.frag");
        skyboxProgram.link();
        cubeProgram.link();
        skyboxProgram.use();
        cubeProgram.use();
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

    vec3 cameraPos = vec3(7.0f * cos(glm::radians(cameraAngle)), 2.0f, 7.0f * sin(glm::radians(cameraAngle)));
    view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

    skyboxProgram.use();
    model = mat4(1.0f);
    setMatrices(skyboxProgram);
    sky.render();

    cubeProgram.use();
    cubeProgram.setUniform("Material.Kd", 1.0f, 0.5f, 0.31f);
    cubeProgram.setUniform("Material.Ks", 1.0f, 1.0f, 1.0f);
    cubeProgram.setUniform("Material.Ka", 1.0f, 0.5f, 0.31f);
    cubeProgram.setUniform("Material.Shininess", 64.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    setMatrices(cubeProgram);
    cube.render();

    cubeProgram.use();
    cubeProgram.setUniform("Material.Kd", 1.0f, 1.0f, 1.0f);
    cubeProgram.setUniform("Material.Ks", 1.0f, 1.0f, 1.0f);
    cubeProgram.setUniform("Material.Ka", 1.0f, 1.0f, 1.0f);
    cubeProgram.setUniform("Material.Shininess", 0.0f);
    model = mat4(1.0f);
    model = glm::translate(model, lightPosition);
    setMatrices(cubeProgram);
    light.render();
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
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}