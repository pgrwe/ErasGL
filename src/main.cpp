#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>

#include <shaderclass.h>
#include <vao.h>
#include <ebo.h>
#include <vbo.h>


// Closes window on q press (for quiteboeboeboeboebo)
void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}
// Vertices coordinates
GLfloat vertices[] = {
//    x      y      z
   -0.5f, -0.5f, 0.0f, // Lower left corner
    0.5f, -0.5f, 0.0f, // Lower right corner
    0.5f,  0.5f, 0.0f, // Top right corner
   -0.5f,  0.5f, 0.0f  // Top left corner
};
GLuint indices[] = {
    0, 1, 3,
    1, 2, 3
};

int main(){
    // Initialize glfw
    glfwInit();
    // Configure glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    // Using opengl CORE profile - only modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

     
    // Error handling 
    GLFWwindow* window = glfwCreateWindow(900,800,"ErasGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1; 
    }


    glfwMakeContextCurrent(window);
    /* Quick Buffer Rundown: 
    Screens display frames by changing the pixels of the display to the pixels of a frame
    Two frames - buffers - space of storage in memory of pixels 
    Front buffer: information being read to be displayed onto screen
    Back buffer: information being written 
    */  


    // load GLAD to configure opengl - cannot use OpenGL functions till this is invoked 
    gladLoadGL();
    // create viewport of opengl in the glfw window
    std::cout << glGetString(GL_VERSION) << std::endl;
    glViewport(0, 0, 900, 800);

    Shader shader_program("default.vert","default.frag");

    VAO vArrayObj1; 
    vArrayObj1.Bind();

    VBO vBufferObj1(vertices, sizeof(vertices));

    EBO eBufferObj1(indices,sizeof(indices));

    vArrayObj1.LinkVBO(vBufferObj1,0);
    vArrayObj1.Unbind();
    vBufferObj1.Unbind();
    eBufferObj1.Unbind();

    while(!glfwWindowShouldClose(window)){

        // specify background color
        glClearColor(0.10f,0.30f,0.35f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_program.Activate();
        vArrayObj1.Bind();
        
        glDrawArrays(GL_TRIANGLES, 0, 3);

        processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents(); 
    }


    // deleting created objects/cleaning up
    vArrayObj1.Delete();
    vBufferObj1.Delete();
    eBufferObj1.Delete();
    shader_program.Delete();

    // destroy window and terminate glfw instance before program ends
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}