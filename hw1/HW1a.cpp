// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW1a.cpp - HW1a class
//
// Written by: George Wolberg, 2022
// ===============================================================

#include "HW1a.h"

// init array of 16 vertices for letter 'P'
float Vertices[] = {
	-0.5f , -0.75f,
	-0.5f , -0.5f,
	-0.5f , -0.25f,
	-0.5f ,  0.0f,
	-0.5f ,  0.25f,
	-0.5f ,  0.5f,
	-0.25f,  0.75f,
	 0.0f ,  0.75f,
	 0.25f,  0.75f,
	 0.5f ,  0.75f,
	 0.75f,  0.5f,
	 0.75f,  0.25f,
	 0.5f ,  0.0f,
	 0.25f,  0.0f,
	 0.0f ,  0.0f,
	-0.25f,  0.0f
};

static int DrawModes[] = {
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
// HW1a::HW1a:
//
// HW1a constructor.
//
HW1a::HW1a(const QGLFormat &glf, QWidget *parent)
	: HW(glf, parent)
{
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1a::initializeGL:
//
// Initialization routine before display loop.
// Gets called once before the first time resizeGL() or paintGL() is called.
//
void
HW1a::initializeGL()
{
	// PUT YOUR CODE HERE

	// init state variables
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// set background color
	glColor3f   (1.0F, 1.0f, 1.0f);		// set foreground color
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1a::resizeGL:
//
// Resize event handler.
// The input parameters are the window width (w) and height (h).
//
void
HW1a::resizeGL(int w, int h)
{
	// PUT YOUR CODE HERE
	m_winW = w;
	m_winH = h;

	// compute aspect ratio
	float xmax, ymax;
	float ar = (float) w / h;
	if(ar > 1.0) {		// wide screen
		xmax = ar;
		ymax = 1.;
	} else {		// tall screen
		xmax = 1.;
		ymax = 1/ar;
	}

	// init viewing coordinates for orthographic projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-xmax, xmax, -ymax, ymax, -1.0, 1.0);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1a::paintGL:
//
// Update GL scene.
//
void
HW1a::paintGL()
{
	// PUT YOUR CODE HERE
	int gridX = 3;
	int gridY = 3;
	int viewportWidth = m_winW/gridX;
	int viewportHeight = m_winH/gridY;

	// clear canvas with background values
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// display each P
	for (size_t i = 0; i < gridX; i++) { // each row
		for (size_t j = 0; j < gridY; j++) { // each column
			int x = m_winW*j/3; // bottom-left x coordinate of viewport
			int y = m_winH*i/3; // bottom-left y coordinate of viewport
			glViewport(x, y, viewportWidth, viewportHeight); 
			glBegin(DrawModes[i*3 + j]);
			size_t verticesLength = sizeof(Vertices)/sizeof(Vertices[0]);
			for (size_t k = 0; k < verticesLength; k+=2) { // draw vertex list
				glVertex2f(Vertices[k], Vertices[k+1]); // draw each vertex
			}
			glEnd();
		}
	}
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW1a::controlPanel:
//
// Create control panel groupbox.
//
QGroupBox*
HW1a::controlPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Homework 1a");
	groupBox->setStyleSheet(GroupBoxStyle);

	return(groupBox);
}
