#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>


// Vertex shader
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// Fragment shader
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
    "}\n\0";


// Takes input and closes window if input == q (for quit)
void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}


int main(){
    // Initialize glfw
    glfwInit();
    std::cout <<"initializing..." << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };  
    
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
    gladLoadGL();
    glViewport(0, 0, 900, 800);

    // Create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Create fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // vertexBufferObject
    GLuint vBO;
    glGenBuffers(1, &vBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glClearColor(0.80f,0.40f,0.17f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    float prev_time = float(glfwGetTime());
    float angle = 0.0f;
                
    while(!glfwWindowShouldClose(window)){
        float time = float(glfwGetTime());
        processInput(window);
        if (time - prev_time >= 0.1f){
            angle += 0.1f;
            prev_time = time;
        }
        glClearColor(float(sin(angle)), float(cos(angle)), float(tan(angle)), 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}