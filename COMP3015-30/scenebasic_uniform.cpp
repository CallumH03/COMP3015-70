#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"
#include "helper/texture.h"

using glm::vec3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() : plane(18.0f,18.0f,100,100) { //Initializes plane object
    mesh = ObjMesh::load("media/palmtree.obj",true); //Loads in custom model
}


void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);
    model = mat4(1.0f);
    view = glm::lookAt(vec3(2.75f, 1.00f, 2.00f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)); //Camera positions
    projection = mat4(1.0f);

    float x, z;
    for (int i = 0; i < 3; i++) { //Calculate light positions (3 lights)
        std::stringstream name;
        name << "lights[" << i << "].Position";
        x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
        z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
        prog.setUniform(name.str().c_str(), view * glm::vec4(x, 5.0f, z + 1.0f, 1.0f));
    }
    //Fog parameters
    prog.setUniform("Fog.MaxDist", 10.0f);
    prog.setUniform("Fog.MinDist", 1.0f);
    prog.setUniform("Fog.Color", vec3(0.5f, 0.5f, 0.5f));

    //Light properties
    prog.setUniform("lights[0].L", vec3(0.0f, 0.0f, 0.8f));
    prog.setUniform("lights[1].L", vec3(0.0f, 0.01f, 0.0f));
    prog.setUniform("lights[2].L", vec3(0.8f, 0.4f, 0.0f));

    prog.setUniform("lights[0].La", vec3(0.0f, 0.0f, 0.2f));
    prog.setUniform("lights[1].La", vec3(0.0f, 0.2f, 0.0f));
    prog.setUniform("lights[2].La", vec3(0.2f, 0.1f, 0.0f));

}

void SceneBasic_Uniform::compile()
{
    try { //Compile shaders
        prog.compileShader("shader/basic_uniform.vert");
        prog.compileShader("shader/basic_uniform.frag");
        prog.link();
        prog.use();
    }
    catch (GLSLProgramException& e) { //Error handling
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

    //Set material properties for mesh
    prog.setUniform("Material.Kd", vec3(0.3f, 0.15f, 0.05f));
    prog.setUniform("Material.Ka", vec3(0.2f, 0.1f, 0.03f));
    prog.setUniform("Material.Ks", vec3(0.6f, 0.6f, 0.6f));
    prog.setUniform("Material.Shininess", 180.0f);

    //Dont enable sand texture for mesh
    prog.setUniform("Sand", false);

    //Load tree texture
    GLuint texID2 = Texture::loadTexture("media/texture/tree.png");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texID2);

    //Render mesh at certain point + rotation
    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(2.0f, 1.0f, -2.25f));
    setMatrices();
    mesh->render();

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(1.5f, 0.8f, 1.5f));
    setMatrices();
    mesh->render();

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(0.0f, 0.8f, 2.0f));
    setMatrices();
    mesh->render();

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(0.0f, 0.8f, -2.0f));
    setMatrices();
    mesh->render();

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(-0.25f, 0.8f, 0.0f));
    setMatrices();
    mesh->render();

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(1.20f, 0.8f, 0.20));
    setMatrices();
    mesh->render();

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(3.0f, 0.8f, 0.20));
    setMatrices();
    mesh->render();


    //Enable sand texture for plane
    prog.setUniform("Sand", true);

    //Load sand texture
    GLuint texID = Texture::loadTexture("media/texture/sand.png");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);

    //Render plane with sand texture
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    setMatrices();
    plane.render();



}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h); //Viewpoint settings
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f); //Projection matrix

}

void SceneBasic_Uniform::setMatrices() { //Calculate matrixe's
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}