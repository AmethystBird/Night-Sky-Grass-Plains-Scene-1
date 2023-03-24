#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"

#include "helper/glslprogram.h"

#include <glm/ext/matrix_transform.hpp>

//#include "torus.h"
#include <glm/glm.hpp>

#include <glm/ext/matrix_clip_space.hpp>

using glm::vec3;

//lab 3
#include <sstream>
#include <GLFW/glfw3.h>

//lab 4
#include "helper/texture.h"

//Camera
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//SceneBasic_Uniform::SceneBasic_Uniform() : torus(0.7f, 0.3f, 30.f, 30.f) {}
//SceneBasic_Uniform::SceneBasic_Uniform() : torus(0.7f, 0.3f, 10.f, 100.f) {} //view with cell shading

//SceneBasic_Uniform::SceneBasic_Uniform() : teapot(50.f, glm::translate(glm::mat4(1.0f), vec3(8.0f, 0.0f, 2.0f))) {}

//SceneBasic_Uniform::SceneBasic_Uniform() : teapot(50.f, glm::translate(glm::mat4(1.0f), vec3(8.0f, 0.0f, 2.0f))), angle(0.0f), timePrev(0.0f), rotationSpeed(glm::pi<float>() / 8.0f), skyBox(100.0f) {}

//SceneBasic_Uniform::SceneBasic_Uniform() : plane(50.f, 50.f, 1, 1), angle(0.0f), timePrev(0.0f), rotationSpeed(glm::pi<float>() / 8.0f), skyBox(100.0f)
SceneBasic_Uniform::SceneBasic_Uniform() : angle(0.0f), timePrev(0.0f), rotationSpeed(glm::pi<float>() / 8.0f), plane(50.f, 50.f, 1, 1), skyBox(100.f) {
    //grassPlane = ObjMesh::load("media/grassPlane/scene.gltf", true);
    //grassPlane = ObjMesh::load("media/TOL/source/Tree_of_LifeSub4.obj", true, false); //works
    //grassPlane = ObjMesh::load("media/bs_ears.obj", true, false);
    
    //C:\Users\soutram\Github\COMP3015\Coursework1\3015-CW1\media\tree\source
    tree = ObjMesh::load("media/tree/source/JASMIM+MANGA.obj", true, false);
    //torch = ObjMesh::load("media/torch/source/torch.obj", true, false);
    rock = ObjMesh::load("media/rock/rock.obj", true, false);
}

//SceneBasic_Uniform::SceneBasic_Uniform() : timePrev(0.f), teapot(50.f, glm::translate(glm::mat4(1.0f), vec3(8.0f, 0.0f, 2.0f))) {}

//SceneBasic_Uniform::SceneBasic_Uniform() : timePrev(0.f), plane(50.f, 50.f, 1, 1) {}

/*SceneBasic_Uniform::SceneBasic_Uniform() : teapot(50.f, glm::translate(glm::mat4(1.0f), vec3(8.0f, 0.0f, 2.0f)))
{
    teapotMesh = ObjMesh::load("../Project_Template/media/pig_triangulated.obj", true);
}*/

//glm::mat4 rotationMatrix;

void SceneBasic_Uniform::initScene()
{
    compile();

    glEnable(GL_DEPTH_TEST);

    model = glm::mat4(1.0f);
    //model = glm::rotate(model, glm::radians(-35.0f), vec3(1.0f, 0.0f, 0.0f));
    projection = glm::mat4(1.0f);

    //projector matrix
    vec3 projPos = vec3(2.0f, 5.0f, 5.0f);
    vec3 projAt = vec3(-2.0f, -4.0f, 0.0f);
    vec3 projUp = vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 projView = glm::lookAt(projPos, projAt, projUp);
    glm::mat4 projProj = glm::perspective(glm::radians(30.0f), 1.0f, 0.2f, 1000.0f);
    glm::mat4 bias = glm::translate(glm::mat4(1.0f), vec3(0.5f));
    bias = glm::scale(bias, vec3(0.5f));
    prog.setUniform("ProjectorMatrix", bias * projProj * projView);

    view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp); //vec3(-1.f, 6.f, 5.8f), vec3(0.0f, 1.0f, 0.0f)

    timePrev = 0.f;
    angle = 0.f;

    //rotationMatrix = glm::rotate(glm::mat4(1.f), angle, vec3(0.f, 0.f, 0.f));

    /*
    //Query location of RotationMatrix variable in GLSL
    GLuint programHandle = prog.getHandle();
    GLuint location = glGetUniformLocation(programHandle, "RotationMatrix");

    //Assign values to RotationMatrix variable
    glUniformMatrix4fv(location, 1, GL_FALSE, &rotationMatrix[0][0]);
    */

    //prog.setUniform("RotationMatrix", rotationMatrix);

    //prog.setUniform("textureIndex", 0);

    float x, z;
    for (int i = 0; i < 3; i++)
    {
        std::stringstream name;
        name << "lights[" << i << "].lightPosition";
        x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
        z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
        prog.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z + 1.0f, 0.0f));
    }

    prog.setUniform("lights[0].lightAmbient", vec3(5.0f, 5.0f, 5.0f)); //set these to 0
    prog.setUniform("lights[0].lightDiffuse", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("lights[0].lightSpecular", vec3(0.5f, 0.5f, 0.5f));

    prog.setUniform("lights[1].lightAmbient", vec3(0.25f, 0.0625f, 0.0625f));
    prog.setUniform("lights[1].lightDiffuse", vec3(0.0f, 0.0f, 0.0f));
    prog.setUniform("lights[1].lightSpecular", vec3(0.0f, 0.0f, 0.0f));

    prog.setUniform("lights[2].lightAmbient", vec3(0.25f, 0.25f, 2.0f));
    prog.setUniform("lights[2].lightDiffuse", vec3(0.0f, 0.0f, 0.f));
    prog.setUniform("lights[2].lightSpecular", vec3(0.0f, 0.0f, 0.0f));

    /*prog.setUniform("lightPosition", view * glm::vec4(5.0f, 5.0f, 2.0f, 0.0f));
    prog.setUniform("light.lightAmbient", 0.4f, 0.4f, 0.4f);
    prog.setUniform("light.lightDiffuse", 1.0f, 1.0f, 1.0f);
    prog.setUniform("light.lightSpecular", 1.0f, 1.0f, 1.0f);*/

    prog.setUniform("material.materialAmbient", 0.1f, 0.1f, 0.1f);
    prog.setUniform("material.materialDiffuse", 0.3f, 0.3f, 0.3f); //0.9
    prog.setUniform("material.materialSpecular", 0.1f, 0.1f, 0.1f);
    prog.setUniform("material.shinyness", 45.0f); //180

    prog.setUniform("fog.MaxDistance", 16.f);
    prog.setUniform("fog.MinDistance", 0.f);
    prog.setUniform("fog.Colour", 0.0f, 0.25f, 0.5f);

    prog.setUniform("Spot.L", vec3(0.9f));
    prog.setUniform("Spot.La", vec3(0.5f));
    prog.setUniform("Spot.Exponent", 50.0f);
    prog.setUniform("Spot.Cutoff", glm::radians(15.0f));

    //GLuint planeTexture = Texture::loadHdrCubeMap("media/grassPlane/textures/Material_baseColor.png");
    //GLuint skyBoxTexture = Texture::loadCubeMap("media/texture/cube/pisa", ".png");
    //GLuint skyBoxTexture = Texture::loadHdrCubeMap("media/texture/cube/pisa");

    GLuint baseTexture = Texture::loadTexture("media/tree/textures/_Vegetation_Bark_Maple_1.jpeg");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, baseTexture);

    GLuint overlayTexture = Texture::loadTexture("media/tree/textures/_40.png");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, overlayTexture);

    GLuint skyBoxTexture = Texture::loadHdrCubeMap("media/texture/cube/pisa-hdr/pisa");
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTexture);

    GLuint rockTexture = Texture::loadTexture("media/texture/brick.jpg");
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, rockTexture);

    /*//Texturing
    GLuint baseTexture = Texture::loadTexture("media/texture/brick1.jpg");
    GLuint alphaTexture = Texture::loadTexture("media/texture/moss.png");
    GLuint skyBoxTexture = Texture::loadHdrCubeMap("media/texture/cube/pisa-hdr/pisa");
    GLuint flowerTexture = Texture::loadTexture("media/texture/flower.png");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, baseTexture);

    //GLuint alphaTexture = Texture::loadTexture("media/texture/moss.png");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, alphaTexture);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTexture);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, flowerTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);*/
}

void SceneBasic_Uniform::compile()
{
    try {
        prog.compileShader("shader/basic_uniform.vert");
        prog.compileShader("shader/basic_uniform.frag");
        prog.link();
        prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

void SceneBasic_Uniform::SetMatrices()
{
    glm::mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
    prog.setUniform("ProjectionMatrix", projection);
    prog.setUniform("ModelMatrix", model);
    //prog.setUniform("RotationMatrix", rotationMatrix);

    /*prog.setUniform("Fog.MaxDistance", 30.f);
    prog.setUniform("Fog.MinDistance", 1.f);
    prog.setUniform("Fog.Color", vec3(0.5f, 0.5f, 0.5f));*/
}

//Called by scenerunner.h
void SceneBasic_Uniform::update(float t)
{
    float deltaTime = t - timePrev;

    if (timePrev == 0.f)
    {
        deltaTime = 0.f;
    }

    timePrev = t;
    angle += rotationSpeed * deltaTime;

    if (angle > glm::two_pi<float>())
    {
        angle -= glm::two_pi<float>();
    }
}

float fogIntensity = 16.f;
float fogChangeSpeed = 1.f / 16.f;
bool fogIncreasing = true;

//Called by scenerunner.h
void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    //vec3 cameraPosition = vec3(7.f * cos(angle), 2.f, 7.f * sin(angle));
    //view = glm::lookAt(cameraPosition, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

    //Draw skybox
    prog.use();
    model = glm::mat4(1.f);

    //float t = static_cast<float>(glfwGetTime());
    //update(t);

    SetMatrices();

    //update light position; work on this to get light into position of torch flame
    //glm::vec4 lightPosition = glm::vec4(10.f * cos(angle), 10.f, 10.f * sin(angle), 1.f); //if camera not moving
    glm::vec4 lightPosition = glm::vec4(10.f * cos(angle * 2), 10.f, 10.f * sin(angle * 2), 1.f); //if camera moving
    prog.setUniform("lights[0].lightPosition", view * lightPosition);

    if (fogIncreasing == true)
    {
        if (fogIntensity < 128.f)
        {
            fogIntensity = fogIntensity + fogChangeSpeed;
        }
        else
        {
            fogIncreasing = false;
            fogIntensity = fogIntensity - fogChangeSpeed;
        }
    }
    else if (fogIncreasing == false)
    {
        if (fogIntensity > 16.f)
        {
            fogIntensity = fogIntensity - fogChangeSpeed;
        }
        else
        {
            fogIncreasing = true;
            fogIntensity = fogIntensity + fogChangeSpeed;
        }
    }
    prog.setUniform("fog.MaxDistance", fogIntensity);

    //teapot.render();
    skyBox.render();
    plane.render();
    //cube.render();

    glm::vec3 scale = glm::vec3(0.025f, 0.025f, 0.025f);
    model = glm::scale(model, scale);

    view = glm::translate(view, glm::vec3(0.f, 3.f, 0.f));

    /*glm::mat4 mvp = model * view * projection;
    scale = glm::vec3(1.0f, -4.0f, 1.0f);
    mvp = glm::scale(mvp, scale);
    prog.setUniform("MVP", mvp);*/

    SetMatrices();

    tree->render();

    //scale = glm::vec3(7.5f, 7.5f, 7.5f);
    scale = glm::vec3(72.f, 72.f, 72.f);
    model = glm::scale(model, scale);

    view = glm::translate(view, glm::vec3(0.f, -1.7f, 2.f));

    SetMatrices();

    /*GLuint baseTexture = Texture::loadTexture("media/texture/brick.jpg");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, baseTexture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, baseTexture);*/

    //prog.setUniform("rockMatrix", projection * view * model);

    //prog.setUniform("textureIndex", 1);
    rock->render();

    //glm::vec4 lightPosition = glm::vec4(10.f * cos(angle), 10.f, 10.f * sin(angle), 1.f);
    //prog.setUniform("lights[0].lightPosition", view * lightPosition);
    //prog.setUniform("lights[0].lightPosition", view * glm::vec4(20.f, 20.f, 0.f, 1.f));

    vec3 cameraPosition = vec3(7.f * cos(angle), 2.f, 7.f * sin(angle));
    //vec3 cameraPosition = vec3(7.f, 2.f, 7.f);
    //vec3 cameraPosition = vec3(7.f, 20.f, 7.f); //panned out
    view = glm::lookAt(cameraPosition, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    SetMatrices();
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}