#include "Sun.h"

#include "template.h"

void Sun::Draw()
{
    glBindVertexArray(VAO);


    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Sun::Init()
{
    //texture loading

    //Vertex array object creation
    //first triangle
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    //Vertex buffer object creation
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}
