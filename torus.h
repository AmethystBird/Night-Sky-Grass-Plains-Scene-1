#pragma once

#include "helper/trianglemesh.h"
#include <glad/glad.h>

class Torus : public TriangleMesh
{
public:
    Torus(GLfloat outerRadius, GLfloat innerRadius, GLuint nsides, GLuint nrings);
};
