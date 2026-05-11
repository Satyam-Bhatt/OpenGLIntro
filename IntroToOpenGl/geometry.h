#pragma once
#include "Mesh.h"

#define PI 3.14159265359

namespace Geometry
{
    // Regular function that returns a mesh object
    Mesh Cube();

    Mesh Plane(float size = 1.0f);

    Mesh Sphere(uint32_t stacks = 12, uint32_t slices = 12);

    Mesh ColoredCube();
}

