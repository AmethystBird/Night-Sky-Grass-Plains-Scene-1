#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

#include "torus.h"
#include "teapot.h"
#include "helper/cube.h"
#include "plane.h"
#include "skybox.h"
#include "helper/objmesh.h"

using namespace std;

class SceneBasic_Uniform : public Scene
{
private:
    //GLuint vaoHandle;
    GLSLProgram prog;
    //float angle;

    void compile();

    //Teapot teapot;
    SkyBox skyBox;
    Cube cube;
    //Plane plane;
    unique_ptr<ObjMesh> grassPlane;

    void SetMatrices();

    float timePrev;
    float angle;

    float rotationSpeed;

public:
    SceneBasic_Uniform();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H