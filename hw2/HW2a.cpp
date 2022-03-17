// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW2a.cpp - HW2a class
//
// Written by: George Wolberg, 2022
// ===============================================================

#include "HW2a.h"

// shader ID
enum { HW2A };

// uniform ID
enum { PROJ };

const int DrawModes[] = {
	GL_POINTS,
	GL_LINES,
	GL_LINE_STRIP,
	GL_LINE_LOOP,
	GL_TRIANGLES,
	GL_TRIANGLE_STRIP,
	GL_TRIANGLE_FAN,
	GL_QUADS,
	GL_POLYGON
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2a::HW2a:
//
// HW2a constructor.
//
HW2a::HW2a(const QGLFormat &glf, QWidget *parent) : HW(glf, parent)
{
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2a::initializeGL:
//
// Initialization routine before display loop.
// Gets called once before the first time resizeGL() or paintGL() is called.
//
void
HW2a::initializeGL()
{
	// initialize GL function resolution for current context
	initializeGLFunctions();

	// init vertex and fragment shaders
	initShaders();

	// initialize vertex buffer and write positions to vertex shader
	initVertexBuffer();

	// init state variables
	glClearColor(0.0, 0.0, 0.0, 0.0);	// set background color
	glColor3f   (1.0, 1.0, 0.0);		// set foreground color
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2a::resizeGL:
//
// Resize event handler.
// The input parameters are the window width (w) and height (h).
//
void
HW2a::resizeGL(int w, int h)
{
	// PUT YOUR CODE HERE

	// assign private member m_winW and m_winH to the windows w and h
	m_winW = w;
	m_winH = h;

	// compute aspect ratio
	float xmax, ymax;
	float ar = (float)w / h;
	if (ar > 1.0) {		// wide screen
		xmax = ar;
		ymax = 1.;
	}
	else {		// tall screen
		xmax = 1.;
		ymax = 1 / ar;
	}

	// set viewport to occupy full canvas
	glViewport(0, 0, w, h);

	// compute orthographic projection from viewing coordinates;
	m_projection.setToIdentity();
	m_projection.ortho(-xmax, xmax, -ymax, ymax, -1.0, 1.0);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2a::paintGL:
//
// Update GL scene.
//
void
HW2a::paintGL()
{
	// clear canvas with background color
	glClear(GL_COLOR_BUFFER_BIT);
	
	// enable vertex shader point size adjustment
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	// draw data in nine viewports after splitting canvas into 3x3 cells;
	// a different drawing mode is used in each viewport
	// ********************
	// *      *     *     *
	// *   7  *  8  *  9  *
	// *      *     *     *
	// ********************
	// *      *     *     *
	// *   4  *  5  *  6  *
	// *      *     *     *
	// ********************
	// *      *     *     *
	// *   1  *  2  *  3  *
	// *      *     *     *
	// ********************

	// viewport dimensions
	int w = m_winW / 3;
	int h = m_winH / 3;

	// use glsl program
	// PUT YOUR CODE HERE

	glUseProgram(m_program[HW2A].programId());

	// pass the projection matrix to the vertex shader
	glUniformMatrix4fv(m_uniform[HW2A][PROJ], 1, GL_FALSE, m_projection.constData());

	int drawModeCursor = 0;

	// iterate through a 3x3 matrix from left to right, bottom to top
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			// set the view port for each P
			glViewport(col*w, row*h, w, h);

			// draw the object
			glDrawArrays(DrawModes[drawModeCursor++], 0, m_vertNum); // strating index is 0, count is m_vertNum
		}
	}

	// it means that no program is current, and therefore no program will be used for things that use programs.
	glUseProgram(0);

	// disable vertex shader point size adjustment
	glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2a::controlPanel:
//
// Create control panel groupbox.
//
QGroupBox*
HW2a::controlPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Homework 2a");
	groupBox->setStyleSheet(GroupBoxStyle);
	return(groupBox);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2a::reset:
//
// function to reset parameters.
//
void
HW2a::reset()
{
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2a::initShaders:
//
// Initialize shaders.
//
void
HW2a::initShaders()
{

	// init uniforms hash table based on uniform variable names and location IDs
	UniformMap uniforms;
	uniforms["u_Projection"] = PROJ;

	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	initShader(HW2A, QString(":/hw2/vshader2a.glsl"), QString(":/hw2/fshader2a.glsl"), uniforms);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2a::initVertexBuffer:
//
// Initialize vertex buffer.
//
void
HW2a::initVertexBuffer()
{
	float vv[] = {
	        -0.5 , -0.75,
	        -0.5 , -0.5 ,
	        -0.5 , -0.25,
	        -0.5 ,  0.0 ,
	        -0.5 ,  0.25,
	        -0.5 ,  0.5 ,
	        -0.25,  0.75,
	         0.0 ,  0.75,
	         0.25,  0.75,
	         0.5 ,  0.75,
	         0.75 , 0.5 ,
	         0.75,  0.25,
	         0.5 ,  0.0 ,
	         0.25,  0.0 ,
	         0.0,   0.0 ,
	        -0.25,  0.0 
	};
	std::vector<float> v (&vv[0], &vv[0]+sizeof(vv)/sizeof(float));

	// init number of vertices
	m_vertNum = (int) v.size() / 2;

	// create a vertex buffer
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);

	// bind vertex buffer to the GPU and copy the vertices from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, v.size()*sizeof(float), &v[0], GL_STATIC_DRAW);

	// enable vertex buffer to be accessed via the attribute vertex variable and specify data format
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer	 (ATTRIB_VERTEX, 2, GL_FLOAT, false, 0, 0);
}
