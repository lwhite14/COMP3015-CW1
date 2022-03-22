#include "scenebasic_uniform.h"

#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;

#include "helper/texture.h"


SceneBasic_Uniform::SceneBasic_Uniform() : angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 8.0f), sky(100.0f)
{
    //ogre = ObjMesh::load("../COMP3015-CW1/media/bs_ears.obj", false, true);
}


void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);

    projection = mat4(1.0f);
    angle = glm::radians(90.0f); //set the initial angle

    //extract the cube texture
    GLuint cubeTex = Texture::loadHdrCubeMap("../COMP3015-CW1/media/texture/cube/pisa-hdr/pisa");
    
    //activate and bindtexture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);


}

void SceneBasic_Uniform::compile()
{
    try
    {
        //prog.compileShader("shader/basic_uniform.vert");
        //prog.compileShader("shader/basic_uniform.frag");
        prog.compileShader("shader/skybox.vert");
        prog.compileShader("shader/skybox.frag");
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

    // Draw sky
    prog.use();
    model = mat4(1.0f);
    setMatrices();
    sky.render();
}

void SceneBasic_Uniform::setMatrices()
{
    mat4 mv = view * model; //we create a model view matrix
    //prog.setUniform("ModelViewMatrix", mv); //set the uniform for the model view matrix
    //prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]))); //we set the uniform for normal matrix 
    prog.setUniform("MVP", projection * mv); //we set the model view matrix by multiplying the mv with the projection matrix
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}