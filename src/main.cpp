#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3.h>
#include <GLM/glm/glm/glm.hpp>
#include <GLM/glm/glm/gtc/matrix_transform.hpp>
#include <GLM/glm/glm/gtc/type_ptr.hpp>
#include "shader.h"
GLFWwindow* window;

using namespace std; 
using namespace glm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main( void )
{
	// Initialize GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make macOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
	
    // coordinates to create a 2d triangle
    static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};
//create vertex buffers
GLuint vertexbuffer;
// Generate 1 buffer, put the resulting identifier in vertexbuffer
glGenBuffers(1, &vertexbuffer);
glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
// Give our vertices to OpenGL.
glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // load our shaders
    GLuint programID = LoadShaders("src/myVertexShader.txt", "src/myFragmentShader.txt");
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");


    glm::mat4 myMatrix;
    glm::vec4 myVector;
    glm::vec4 transformedVector = myMatrix * myVector;

    // create a projection matrix this just means you get one perspective of the shape since its still just static shape
        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    // create camera matrix (used to see the objects and world)
    glm::mat4 View = glm::lookAt(
        glm::vec3(4,3,3),
        glm::vec3(0,0,0),
        glm::vec3(0,1,0)
    );

    // create model matrix (triangle, circle, etc)
    glm::mat4 Model = glm::mat4(1.0f);

    // create the final modelviewprojection
    glm::mat4 MVP = Projection * View * Model; 
	do{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       
    glUseProgram(programID);
      
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
     // enable the first vertex array we created a few lines ago
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        // giving attributes to the vertex
        glVertexAttribPointer(
        0,                
        3,                 
        GL_FLOAT,         
        GL_FALSE,           
        0,                  
        (void*)0           
        );

          // Draw the triangle 
        glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

                // disable the first vertex array we created a few lines ago

        glDisableVertexAttribArray(0);
		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

glDeleteBuffers(1, &vertexbuffer);
glDeleteProgram(programID);
glDeleteVertexArrays(1, &VertexArrayID);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
