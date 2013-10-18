//A simple demo rendering particles as spheres. The fragment shader is inspired from the particles demo
//in the nvidia gpu computing sdk. You will need the GLM library.

//Author: Movania Muhammad Mobeen

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <iostream>
#include "GLSLShader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cassert>

#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);

using namespace std;

int screen_width=1024;
int screen_height=768;

const int TOTAL=	9;
GLfloat positions[3*TOTAL]={-1,0,-1, 0,0,-1, 1,0,-1,
                            -1,0, 0, 0,0, 0, 1,0, 0,
                            -1,0, 1, 0,0, 1, 1,0,1};

GLuint vboID,  vaoID;
GLsizei stride = sizeof(GLfloat)*3;
GLSLShader shader;

int filling=1;
// Absolute rotation values (0-359 degrees) and rotiation increments for each frame
float rotation_x=0, rotation_x_increment=0.1f;
float rotation_y=0, rotation_y_increment=0.05f;
float rotation_z=0, rotation_z_increment=0.03f;

glm::mat4 P;	//projection matrix;
bool bRotate=true;
void InitShaders(void)
{
	shader.LoadFromFile(GL_VERTEX_SHADER, "shader.vert");
	shader.LoadFromFile(GL_FRAGMENT_SHADER, "shader.frag");
	shader.CreateAndLinkProgram();
	shader.Use();
		shader.AddAttribute("vVertex");
		shader.AddUniform("Color");
		shader.AddUniform("lightDir");
		shader.AddUniform("MVP");
		glUniform3f(shader("lightDir"), 0,0,1);
		glUniform3f(shader("Color"),1,0,0);
	shader.UnUse();

	GL_CHECK_ERRORS
}
void InitVAO() {
	GL_CHECK_ERRORS
	//Create vao and vbo stuff
	glGenVertexArrays(1, &vaoID);
	glGenBuffers (1, &vboID);

	GL_CHECK_ERRORS

	glBindVertexArray(vaoID);
	glBindBuffer (GL_ARRAY_BUFFER, vboID);
	glBufferData (GL_ARRAY_BUFFER, sizeof(positions), &positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer (shader["vVertex"], 3, GL_FLOAT, GL_FALSE,stride,0);
	glBindVertexArray(0);


	GL_CHECK_ERRORS
}
void InitGL() {
	glGetError();
	GL_CHECK_ERRORS
	glClearColor(0.0f,0.0f,0.2f,0.0f);
	GL_CHECK_ERRORS
	InitShaders();
	InitVAO();
	glEnable(GL_DEPTH_TEST); // We enable the depth test (also called z buffer)
    GL_CHECK_ERRORS
	glPointSize(50);
}

void OnRender() {
	GL_CHECK_ERRORS
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	//setup matrices
	glm::mat4 T		= glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, -5));
	glm::mat4 Rx	= glm::rotate(T,  rotation_x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 Ry	= glm::rotate(Rx, rotation_y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 MV	= glm::rotate(Ry, rotation_z, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 MVP	= P*MV;

	//draw the points
	shader.Use();
		glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
		glBindVertexArray(vaoID);
			glDrawArrays(GL_POINTS, 0, TOTAL);
		glBindVertexArray(0);
	shader.UnUse();

	glBindVertexArray(0);

	glutSwapBuffers();
}
void OnResize(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	//setup the projection matrix
	P = glm::perspective(45.0f, (GLfloat)w/h, 1.f, 1000.f);
}
void OnShutdown() {
	glDeleteBuffers(1, &vboID);
	glDeleteVertexArrays(1, &vaoID);
}
void OnKey(unsigned char key, int x, int y)
{
    switch (key)
	{
		case ' ': bRotate=!bRotate; break;
        case 'r': case 'R':
            if (filling==0)
            {
                glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
                filling=1;
            }
            else
            {
                glPolygonMode (GL_FRONT_AND_BACK, GL_LINE); // Polygon rasterization mode (polygon outlined)
                filling=0;
            }
        break;
    }
}
void OnSpecialKey(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP:
            rotation_x_increment = rotation_x_increment +0.005f;
        break;
        case GLUT_KEY_DOWN:
            rotation_x_increment = rotation_x_increment -0.005f;
        break;
        case GLUT_KEY_LEFT:
            rotation_y_increment = rotation_y_increment +0.005f;
        break;
        case GLUT_KEY_RIGHT:
            rotation_y_increment = rotation_y_increment -0.005f;
        break;
    }
}
void OnIdle() {
	if(bRotate) {
		rotation_x = rotation_x + rotation_x_increment;
		rotation_y = rotation_y + rotation_y_increment;
		rotation_z = rotation_z + rotation_z_increment;
	}
    if (rotation_x > 359) rotation_x = 0;
    if (rotation_y > 359) rotation_y = 0;
    if (rotation_z > 359) rotation_z = 0;

	glutPostRedisplay();
}

void main(int argc, char** argv) {
	atexit(OnShutdown);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitContextVersion (3, 3);
	glutInitContextFlags (GLUT_CORE_PROFILE | GLUT_DEBUG);
	glutInitWindowSize(screen_width, screen_height);
	glutCreateWindow("Point sprites as spheres in OpenGL 3.3");
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)	{
		cerr<<"Error: "<<glewGetErrorString(err)<<endl;
	} else {
		if (GLEW_VERSION_3_3)
		{
			cout<<"Driver supports OpenGL 3.3\nDetails:"<<endl;
		}
	}
	cout<<"Using GLEW "<<glewGetString(GLEW_VERSION)<<endl;
	cout<<"Vendor: "<<glGetString (GL_VENDOR)<<endl;
	cout<<"Renderer: "<<glGetString (GL_RENDERER)<<endl;
	cout<<"Version: "<<glGetString (GL_VERSION)<<endl;
	cout<<"GLSL: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<endl;
	InitGL();
	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnResize);
	glutKeyboardFunc(OnKey);
	glutSpecialFunc(OnSpecialKey);
	glutIdleFunc(OnIdle);
	glutMainLoop();
}
