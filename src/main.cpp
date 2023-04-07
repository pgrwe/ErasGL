#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<math.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


#include"shaderclass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

const unsigned int screen_width = 800;
const unsigned int screen_height = 800;
// Vertices coordinates
GLfloat vertices[] = {		// COLORs
//    x      y      z	// r     g     b
    -0.5f,  0.5f, 0.5f,  0.8f, 0.3f, 0.02f, // Upper left front
    -0.5f,  0.5f,-0.5f,  0.8f, 0.3f, 0.02f, // Upper left back
     0.5f,  0.5f,-0.5f,	 0.8f, 0.3f, 0.02f, // Upper right back
     0.5f,  0.5f, 0.5f,  0.8f, 0.3f, 0.02f, // Upper right front
    -0.5f,  -0.5f, 0.5f,  0.8f, 0.3f, 0.02f, // Lower left front
    -0.5f,  -0.5f,-0.5f,  0.8f, 0.3f, 0.02f, // Lower left back
     0.5f,  -0.5f,-0.5f,  0.8f, 0.3f, 0.02f, // Lower right back
     0.5f,  -0.5f, 0.5f,  0.8f, 0.3f, 0.02f, // Lower right front
};

GLuint indices[] = {
	0, 3, 4,
	3, 7, 4,
	3, 2, 7,
	2, 7, 6,
	2, 1, 6,
	5, 1, 6,
	1, 0, 4,
	0, 4, 5,
	1, 2, 0,
	0, 2, 3,
	4, 5, 6,
	6, 7, 4
};
void processInput(GLFWwindow* window);


int main(){
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "ErasGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, screen_width, screen_height);



	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shader_program("default.vert", "default.frag");



	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	glEnable(GL_DEPTH_TEST);
	// Main while loop
	while (!glfwWindowShouldClose(window)){
		// Check quit input
		processInput(window);
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shader_program.Activate();
		float timeval = (float)glfwGetTime();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		float rotationx = sin(timeval)/ 2.0f + 0.5f;
		float rotationy = cos(timeval)/ 2.0f + 0.5f;
		float rotationz = sin(timeval)/ 2.0f + 0.5f;

		model = glm::rotate(model, timeval * glm::radians(60.0f), glm::vec3(rotationx,rotationy,rotationz));
		view = glm::translate(view, glm::vec3(0.0f,0.0f,-4.0f));
		projection = glm::perspective(glm::radians(40.0f),(float)screen_width/screen_height,0.1f,100.0f);

		int model_location = glGetUniformLocation(shader_program.ID, "model");
		glUniformMatrix4fv(model_location, 1,GL_FALSE, glm::value_ptr(model));
		int view_location = glGetUniformLocation(shader_program.ID, "view");
		glUniformMatrix4fv(view_location, 1,GL_FALSE, glm::value_ptr(view));
		int projection_location = glGetUniformLocation(shader_program.ID, "projection");
		glUniformMatrix4fv(projection_location, 1,GL_FALSE, glm::value_ptr(projection));



		// Code for changing colors with a uniform fragment shader
		float greenValue = cos(timeval) / 2.0f + 0.5f;
        float blueValue = cos(timeval) / 2.0f + 0.5f;
		float redValue = sin(timeval) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shader_program.ID, "uniColor");
		glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);

		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shader_program.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}


void processInput(GLFWwindow* window){
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
}