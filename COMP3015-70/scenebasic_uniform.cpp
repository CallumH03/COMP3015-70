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
using glm::vec4;
using glm::mat3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() :
    tPrev(0),
    plane(25.0f,25.0f,1,1) { //Initializes plane object
    mesh = ObjMesh::load("media/palmtree.obj",true); //Loads in custom model
}


void SceneBasic_Uniform::initScene()
{
    compile();
    glClearColor(0.0f, 0.5f, 0.7f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    projection = mat4(1.0f);
    angle = 0.0f;

    setupFBO();

    //Vertex arrays
    GLfloat verts[] = {
        -1.0f,-1.0f,0.0f,1.0f,-1.0f,0.0f,1.0f,1.0f,0.0f,
        -1.0f,-1.0f,0.0f,1.0f,1.0f,0.0f,-1.0f,1.0f,0.0f
    };
    GLfloat tc[] = {
        0.0f,0.0f,1.0f,0.0f,1.0f,1.0f,
        0.0f,0.0f,1.0f,1.0f,0.0f,1.0f
    };

    //Buffer setup
    unsigned int handle[2];
    glGenBuffers(2, handle);
    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

    glGenVertexArrays(1, &fsQuad);
    glBindVertexArray(fsQuad);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0,0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0,0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    //Edge uniforms
    prog.setUniform("EdgeThreshold", 0.02f);

    //Light uniforms
    prog.setUniform("Light.L", vec3(0.9f));
    prog.setUniform("Light.La", vec3(0.5f));

    //Fog uniforms
    prog.setUniform("Fog.MaxDist", 12.0f);
    prog.setUniform("Fog.MinDist", 3.0f);
    prog.setUniform("Fog.Color", vec3(0.5f, 0.5f, 0.5f));

}

void SceneBasic_Uniform::compile() //Compile shaders
{
    //Compile shaders
    try { 
        prog.compileShader("shader/basic_uniform.vert");
        prog.compileShader("shader/basic_uniform.frag");
        prog.link();
        prog.use();
    }
    //Error handling
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

void SceneBasic_Uniform::update(float t) //Rotation update
{
    float deltaT = t - tPrev;
    if (tPrev == 0.0f) deltaT = 0.0f;
    tPrev = t;
    angle += 0.1f * deltaT;
    if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
}

void SceneBasic_Uniform::render() { //Rendering Pass 1 and 2
    pass1();
    glFlush();
    pass2();

}
void SceneBasic_Uniform::pass1() // Sets uniforms, binds the FBO, and renders objects like mesh and plane, also sets up view and projection matrices, lighting, and material properties
{
    prog.setUniform("Pass", 1);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Projection matrices
    view = glm::lookAt(vec3(7.0f * cos(angle), 4.0f, 7.0f * sin(angle)), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(60.0f), (float)width / height, 0.3f, 100.0f);
    
    //Light setup
    vec4 lightPos = vec4(10.0f*cos(angle), 10.0f, 10.0f*sin(angle), 1.0f);
    prog.setUniform("Light.Position", vec4(view*lightPos));

    //Set material properties for mesh
    prog.setUniform("Material.Kd", vec3(0.45f, 0.25f, 0.1f));
    prog.setUniform("Material.Ks", vec3(0.5f, 0.5f, 0.5f));
    prog.setUniform("Material.Ka", vec3(0.1f, 0.05f, 0.02f));
    prog.setUniform("Material.Shininess", 100.0f);

    //Render mesh at certain point + rotation
    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, vec3(2.0f, 1.0f, -2.25f));
    model = glm::scale(model, vec3(2.0f, 2.0f, 2.0f));
    setMatrices();
    mesh->render();

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, vec3(7.0f, 1.0f, -4.25f));
    model = glm::scale(model, vec3(2.0f, 2.0f, 2.0f));
    setMatrices();
    mesh->render();

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, vec3(1.0f, 1.0f, 7.25f));
    model = glm::scale(model, vec3(2.0f, 2.0f, 2.0f));
    setMatrices();
    mesh->render();

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, vec3(-2.0f, 1.0f, 2.25f));
    model = glm::scale(model, vec3(2.0f, 2.0f, 2.0f));
    setMatrices();
    mesh->render();

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, vec3(2.9f, 1.0f, 3.0f));
    model = glm::scale(model, vec3(2.0f, 2.0f, 2.0f));
    setMatrices();
    mesh->render();

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, vec3(-3.0f, 1.0f, -3.0f));
    model = glm::scale(model, vec3(2.0f, 2.0f, 2.0f));
    setMatrices();
    mesh->render();

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, vec3(8.0f, 1.0f, 1.25f));
    model = glm::scale(model, vec3(2.0f, 2.0f, 2.0f));
    setMatrices();
    mesh->render();

    //Set material properties for plane
    prog.setUniform("Material.Kd", vec3(0.76f, 0.7f, 0.5f));
    prog.setUniform("Material.Ks", vec3(0.5f, 0.5f, 0.5f));
    prog.setUniform("Material.Ka", vec3(0.1f, 0.1f, 0.05f));
    prog.setUniform("Material.Shininess", 100.0f);

    //Render plane at certain point + rotation
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.75f, 0.0f));
    setMatrices();
    plane.render();

}

void SceneBasic_Uniform::pass2() { // Bind back to default framebuffer, set uniforms for second pass.
    prog.setUniform("Pass", 2);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTex);

    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    model = mat4(1.0f);
    view = mat4(1.0f);
    projection = mat4(1.0f);

    setMatrices();

    glBindVertexArray(fsQuad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SceneBasic_Uniform::resize(int w, int h) //Viewpoint settings
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);

}

void SceneBasic_Uniform::setMatrices() { //Calculate matrixe's
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::setupFBO() { //SetupFBO
    glGenFramebuffers(1, &fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

    glGenTextures(1, &renderTex);
    glBindTexture(GL_TEXTURE_2D, renderTex);

    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);

    GLuint depthBuf;
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);
    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(result == GL_FRAMEBUFFER_COMPLETE) {
        std:: cout << "Framebuffer is complete" << endl;

    }
    else {
        std::cout << "Framebuffer error:" << result << endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}