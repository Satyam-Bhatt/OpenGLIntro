#pragma once
#include "Mesh.h"

#define PI 3.14159265359

namespace Geometry
{
    // Regular function that returns a mesh object
	Mesh Cube()
	{
        std::vector<Vertex> verts = {
            // position                  normal          uv          color
            // Front
            {{ -0.5f, -0.5f,  0.5f }, {  0,  0,  1 }, { 0, 0 }, { 1,1,1,1 }},
            {{  0.5f, -0.5f,  0.5f }, {  0,  0,  1 }, { 1, 0 }, { 1,1,1,1 }},
            {{  0.5f,  0.5f,  0.5f }, {  0,  0,  1 }, { 1, 1 }, { 1,1,1,1 }},
            {{ -0.5f,  0.5f,  0.5f }, {  0,  0,  1 }, { 0, 1 }, { 1,1,1,1 }},
            // Back
            {{  0.5f, -0.5f, -0.5f }, {  0,  0, -1 }, { 0, 0 }, { 1,1,1,1 }},
            {{ -0.5f, -0.5f, -0.5f }, {  0,  0, -1 }, { 1, 0 }, { 1,1,1,1 }},
            {{ -0.5f,  0.5f, -0.5f }, {  0,  0, -1 }, { 1, 1 }, { 1,1,1,1 }},
            {{  0.5f,  0.5f, -0.5f }, {  0,  0, -1 }, { 0, 1 }, { 1,1,1,1 }},
            // Left
            {{ -0.5f, -0.5f, -0.5f }, { -1,  0,  0 }, { 0, 0 }, { 1,1,1,1 }},
            {{ -0.5f, -0.5f,  0.5f }, { -1,  0,  0 }, { 1, 0 }, { 1,1,1,1 }},
            {{ -0.5f,  0.5f,  0.5f }, { -1,  0,  0 }, { 1, 1 }, { 1,1,1,1 }},
            {{ -0.5f,  0.5f, -0.5f }, { -1,  0,  0 }, { 0, 1 }, { 1,1,1,1 }},
            // Right
            {{  0.5f, -0.5f,  0.5f }, {  1,  0,  0 }, { 0, 0 }, { 1,1,1,1 }},
            {{  0.5f, -0.5f, -0.5f }, {  1,  0,  0 }, { 1, 0 }, { 1,1,1,1 }},
            {{  0.5f,  0.5f, -0.5f }, {  1,  0,  0 }, { 1, 1 }, { 1,1,1,1 }},
            {{  0.5f,  0.5f,  0.5f }, {  1,  0,  0 }, { 0, 1 }, { 1,1,1,1 }},
            // Top
            {{ -0.5f,  0.5f,  0.5f }, {  0,  1,  0 }, { 0, 0 }, { 1,1,1,1 }},
            {{  0.5f,  0.5f,  0.5f }, {  0,  1,  0 }, { 1, 0 }, { 1,1,1,1 }},
            {{  0.5f,  0.5f, -0.5f }, {  0,  1,  0 }, { 1, 1 }, { 1,1,1,1 }},
            {{ -0.5f,  0.5f, -0.5f }, {  0,  1,  0 }, { 0, 1 }, { 1,1,1,1 }},
            // Bottom
            {{ -0.5f, -0.5f, -0.5f }, {  0, -1,  0 }, { 0, 0 }, { 1,1,1,1 }},
            {{  0.5f, -0.5f, -0.5f }, {  0, -1,  0 }, { 1, 0 }, { 1,1,1,1 }},
            {{  0.5f, -0.5f,  0.5f }, {  0, -1,  0 }, { 1, 1 }, { 1,1,1,1 }},
            {{ -0.5f, -0.5f,  0.5f }, {  0, -1,  0 }, { 0, 1 }, { 1,1,1,1 }},
        };

        // Face 0 (Front)— vertices 0, 1, 2, 3       | f=0 → b=0   (face starts at vertex 0)
        // Face 1 (Back)— vertices 4, 5, 6, 7        | f=1 → b=4   (face starts at vertex 4)
        // Face 2 (Left)— vertices 8, 9, 10, 11      | f=2 → b=8
        // Face 3 (Right)— vertices 12, 13, 14, 15   | f=3 → b=12
        // Face 4 (Top)— vertices 16, 17, 18, 19     | f=4 → b=16
        // Face 5 (Bottom)— vertices 20, 21, 22, 23  | f=5 → b=20
        // b + 3 -------- b + 2
        //     |          /|
        //     |        /  |
        //     |      /    |
        //     |    /      |
        //     |  /        |
        // b + 0 -------- b + 1
        // 
        // Triangle 1: b, b + 1, b + 2  (bottom - left, bottom - right, top - right)
        // Triangle 2 : b + 2, b + 3, b(top - right, top - left, bottom - left)
        // Winding order anti clockwise. 
        // Winding order helps in determining which side of triangle is the front face. By default
        // glFrontFace(GL_CCW);  // counter-clockwise = front face (this is OpenGL's default)
        // if face culling is on then openGL will discard any triangles whose winding order appears clockwise from the camera's POV. Which means inside faces get culled automatically, saving GPU work.
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);  // discard back faces
        std::vector<uint32_t> indices;
        for (uint32_t f = 0; f < 6; f++) {
            uint32_t b = f * 4;
            indices.insert(indices.end(), { b,b + 1,b + 2, b + 2,b + 3,b });
        }

        // Return the mesh object
        return Mesh(verts, indices);
	}



    Mesh Plane(float size = 1.0f) {
        float h = size * 0.5f;
        std::vector<Vertex> verts = {
            {{ -h, 0, -h }, { 0, 1, 0 }, { 0, 0 }, { 1,1,1,1 }},
            {{  h, 0, -h }, { 0, 1, 0 }, { 1, 0 }, { 1,1,1,1 }},
            {{  h, 0,  h }, { 0, 1, 0 }, { 1, 1 }, { 1,1,1,1 }},
            {{ -h, 0,  h }, { 0, 1, 0 }, { 0, 1 }, { 1,1,1,1 }},
        };
        std::vector<uint32_t> indices = { 0,1,2, 2,3,0 };
        return Mesh(verts, indices);
    }

    Mesh Geometry::Sphere(uint32_t stacks, uint32_t slices) {
        std::vector<Vertex> verts;
        std::vector<uint32_t> indices;

        // Plot points from top to bottom
        for (uint32_t i = 0; i <= stacks; i++) {
            // cos(phi) - when i is 0 then value is 1 || when i is stacks/2 then value is 0 || when i = stacks then value is -1
            // This maps exactly to how we want the Y-axis to vary from top to bottom
            // sin(phi) - when i is 0 then value is 0 || when i is stacks/2 then value is 1 || when i = stacks then vale is 0
            // This maps exactly to how we want the radius to fluctuate at different values. 0 at top and bottom and maximum at the center
            // We multiplu PI because it shrinks the cos curve and gives us integer values
            float phi = PI * i / stacks;

            // Plot points to create a 2D cirlce around the points in Y axis
            for (uint32_t j = 0; j <= slices; j++) {
                // Helps to create a 2D circle. We need 2*PI so that all the points are spread in 360 degrees 
                // cos(theta) - projection on X axis in XZ plane 
                // sin(theta) - projection on Y axis in XZ plane
                // cos and sin are offsetted by PI/2 so we get these values
                //   x  cos(x)  sin(x)
                //   0    1       0
                //  pi/2  0       1
                //  pi    -1      0
                //  3pi/2  0     -1
                //  2pi    1      0
                // As both are offsetted the values in between when plotted on XZ plane helps to create a circle. Values at 0 = values at 2pi hence making an entire rotation and giving us a cirle
                float theta = 2.0f * PI * j / slices;
                Vector3 pos = {
                    std::sin(phi) * std::cos(theta), // Radius * X axis component in XZ plane
                    std::cos(phi), // Y axis component
                    std::sin(phi) * std::sin(theta) // Radius * Z axis component in XZ plane
                };
                verts.push_back({
                    pos * 0.5f, // Multiply with 0.5 to get diameter as 1 and radius as 0.5. Before it was diameter 2 and radius 1
                    pos.Normalize(), // Normal
                    { (float)j / slices, (float)i / stacks }, // uv - 0 to 1 across the surface
                    { 1, 1, 1, 1 } // Color
                    });
            }
        }

        for (uint32_t i = 0; i < stacks; i++) {
            for (uint32_t j = 0; j < slices; j++) {
                uint32_t a = i * (slices + 1) + j, b = a + slices + 1;
                indices.insert(indices.end(), { a,b,a + 1, b,b + 1,a + 1 });
            }
        }

        return Mesh(verts, indices);
    }
}

