
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GL/glut.h>
#include <algorithm>

using namespace std;


GLint vtxPosition;
GLuint programID;
GLuint VBO[1];
GLuint EBO[1];
GLuint VAO;

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path)
{
	//create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	//Read the vertex shader code from the file
	string VertexShaderCode;
	ifstream VertexShaderStream(vertex_file_path, ios::in);
	if (VertexShaderStream.is_open())
	{
		string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	//Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	//Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength != 0) {
		vector<char> VertexShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
	}

	//Read the fragment shader code from the file
	string FragmentShaderCode;
	ifstream FragmentShaderStream(fragment_file_path, ios::in);
	if(FragmentShaderStream.is_open())
	{
		string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	//Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	//Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength != 0) {
		vector<char> FragmentShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
	}

	//Link the program
	fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
 
    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength != 0) {
		vector<char> ProgramErrorMessage(max(InfoLogLength, int(1)));
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
	}
 
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
 
    return ProgramID;
}

void renderScene(void)
{
	//Clear all pixels
	//glClearColor(1.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//Let's draw something here
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//gluLookAt(0.0, 0.0, 5.0,  // 카메라 위치
	//	0.0, 0.0, 0.0,  // 카메라가 바라보는 지점
	//	0.0, 1.0, 0.0); // 카메라의 위쪽 방향

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	vtxPosition = glGetAttribLocation(programID, "vtxPosition");
	glVertexAttribPointer(vtxPosition, 3, GL_FLOAT, GL_FALSE, 0, ((void*)(0)));
	glEnableVertexAttribArray(vtxPosition);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBindVertexArray(vtxPosition);

	// glBindVertexArray(VAO);

	//define the size of point and draw a point.
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glutPostRedisplay();
	//Double buffer
	glutSwapBuffers();
}


void init()
{
	//initilize the glew and check the errors.
    GLenum res = glewInit();
	if(res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s' \n", glewGetErrorString(res));
	}

	//select the background color
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	// cube vertex
	GLfloat cubeVertices[8][3] = {
		//front
		{-0.5, -0.5, 0.5},
		{-0.5, 0.5, 0.5},
		{0.5, -0.5, 0.5},
		{0.5, 0.5, 0.5},
		////back
		{-0.5, -0.5, -0.5},
		{-0.5, 0.5, -0.5},
		{0.5, -0.5, -0.5},
		{0.5, 0.5, -0.5}
	};

	// index buffer
	GLuint indices[12][3] = {
		{0, 1, 2},
		{2, 3, 0},
		{1, 5, 6},
		{6, 2, 1},
		{7, 6, 5},
		{5, 4, 7},
		{4, 0, 3},
		{3, 7, 4},
		{4, 5, 1},
		{1, 0, 4},
		{3, 2, 6},
		{6, 7, 3}
	};

	//generate buffer
	glGenBuffers(1, VBO); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, cubeVertices, GL_STATIC_DRAW);

	glGenBuffers(1, EBO);
	glBindBuffer(GL_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * 36, indices, GL_STATIC_DRAW);
	
} 


int main(int argc, char **argv)
{
	//init GLUT and create Window
	//initialize the GLUT
	glutInit(&argc, argv);
	//GLUT_DOUBLE enables double buffering (drawing to a background buffer while the other buffer is displayed)
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	//These two functions are used to define the position and size of the window. 
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(480, 480);
	//This is used to define the name of the window.
	glutCreateWindow("Simple OpenGL Window");

	//call initization function
	init();
	
	//1.
	//Generate VAO
	// glGenVertexArrays(1, &VAO);
	// glBindVertexArray(VAO);

	//3.
	programID = LoadShaders("VertexShader.txt", "FragmentShader.txt");
	glUseProgram(programID);


	//glVertexAttrib3f(index, 0.0, 0.0, 0.0);

	glutDisplayFunc(renderScene);

	//enter GLUT event processing cycle
	glutMainLoop();

	glDeleteVertexArrays(1, &VAO);
	// glDeleteBuffers(1, buf);
	 
	return 1;
}

