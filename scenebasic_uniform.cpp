#include "scenebasic_uniform.h"

#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;

#include "helper/texture.h"

SceneBasic_Uniform::SceneBasic_Uniform() : teapot(14, mat4(1.0f)) { }

void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);

    view = glm::lookAt(vec3(1.0f, 1.25f, 1.25f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);


    float x = 2.0f * cosf((glm::two_pi<float>() / 3) * 1);
    float z = 2.0f * sinf((glm::two_pi<float>() / 3) * 1);
    prog.setUniform("Light.Position", view * glm::vec4(x, 1.2f, z + 1.0f, 1.0f));
    prog.setUniform("Light.La", vec3(0.5f, 0.5f, 0.5f));
    prog.setUniform("Light.Ld", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Light.Ls", vec3(1.0f, 1.0f, 1.0f));

    GLuint brick = Texture::loadTexture("../COMP3015-CW1/media/texture/brick1.jpg");
    GLuint moss = Texture::loadTexture("../COMP3015-CW1/media/texture/moss.png");
    // Load brick texture file into channel 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, brick);
    // Load moss texture file into channel 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, moss);

}

void SceneBasic_Uniform::compile()
{
    try
    {
        prog.compileShader("shader/basic_uniform.vert");
        prog.compileShader("shader/basic_uniform.frag");
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
    //update your angle here
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prog.setUniform("Material.Ka", 0.3f, 0.3f, 0.3f);
    prog.setUniform("Material.Kd", 0.15f, 0.15f, 0.15f);
    prog.setUniform("Material.Ks", 0.3f, 0.3f, 0.3f);
    prog.setUniform("Material.Shininess", 180.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(-1.0f, -2.5f, -1.5f));
    model = glm::rotate(model, glm::radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    teapot.render();
    //cube.render();
}

void SceneBasic_Uniform::setMatrices()
{
    mat4 mv = view * model; //we create a model view matrix

    prog.setUniform("ModelViewMatrix", mv); //set the uniform for the model view matrix

    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]))); //we set the uniform for normal matrix

    prog.setUniform("MVP", projection * mv); //we set the model view matrix by multiplying the mv with the projection matrix
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}