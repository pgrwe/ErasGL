#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Vertex shader source code (string of glsl)
const char *vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// Fragment shader source code (string of glsl) 
const char *fragment_shader_source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
    "}\n\0";


// Closes window on q press (for quit)
void process_input(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}


int main(){
    // Initialize glfw
    glfwInit();
    // Configure glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,4);
    // Using opengl CORE profile - only modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    
    // Verticies coords
    GLfloat vertices[] = {
//    x      y      z
    -0.5f, -0.5f, 0.0f, // Lower left corner
     0.5f, -0.5f, 0.0f, // Lower right corner
     0.5f,  0.5f, 0.0f, // Top right corner
    -0.5f,  0.5f, 0.0f  // Top left corner
    };
    GLuint indicies[] = {
        0, 1, 3,
        1, 2, 3
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


    // load GLAD to configure opengl - cannot use OpenGL functions till this is invoked 
    gladLoadGL();
    // create viewport of opengl in the glfw window
    std::cout << glGetString(GL_VERSION) << std::endl;
    glViewport(0, 0, 900, 800);

    // create shader object
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    // attach/point shader source (code) to vertex_shader_source
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    // compile shader (gpu cannot understand our shader source code -> must compile early to machine code)
    glCompileShader(vertex_shader);

    // same for fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    // time to wrap both shaders in a shader program
    // create shader program object and get its reference(?)
    GLuint shader_program = glCreateProgram();

    // attach vertex and fragment shaders to the shader program
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    // wrap/link all shaders together into shader program
    glLinkProgram(shader_program);

    // delete used shader objects
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    
    // create reference containers for VAO and VBO
    GLuint vertex_array_object, vertex_buffer_object, element_buffer_object;

    // generate buffers
    glGenVertexArrays(1, &vertex_array_object);
    glGenBuffers(1, &vertex_buffer_object);
    glGenBuffers(1, &element_buffer_object);

    // binding - must make a certain object our current/binded object - 
    // whenever a function is used to affect an object, binded object is affected
    glBindVertexArray(vertex_array_object);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

    // feed in the vertex buffer data from the earlier matrix (verticies)
    // draw to buffers
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    /*  create an attribute pointer, starts at index 0, size of 3 (3 verticies), type is a glfloat(float)
        next param checks if data should be normalized, verticies are already between 0-1 so set to false
        next param checks byte offset between consecutive generic vertex attributes 
            -> in verticies there are 3 floats, thus the offset is the size of 3xfloats

    */

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);


    // bind buffers to 0 to prevent changes
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);


    while(!glfwWindowShouldClose(window)){

        // specify background color
        glClearColor(0.10f,0.30f,0.35f,1.0f);
        // clean + assign new color to back buffer
        glClear(GL_COLOR_BUFFER_BIT);
        // tells opengl which shader program to use
        glUseProgram(shader_program);
        // bind VAO so opengl knows to use it (current vao)
        glBindVertexArray(vertex_array_object);
        // draw the triangle using the triangles primative
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // swap back buffer with front buffer
        glfwSwapBuffers(window);
        // process_input closes the program on q
        process_input(window);
        // checks events
        glfwPollEvents();
    
    }


    // deleting created objects/cleaning up
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteBuffers(1,&vertex_buffer_object);
    glDeleteProgram(shader_program);

    // destroy window and terminate glfw instance before program ends
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}