// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2022 by George Wolberg
//
// HW2b.cpp - HW2b class
//
// Written by: George Wolberg, 2022
// ===============================================================

#include "HW2b.h"

// shader ID
enum {HW2B};

// uniform ID
enum {
	MV,
	PROJ,
	THETA,
	SUBDIV,
	TWIST
};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2b::HW2b:
//
// HW2b constructor.
//
HW2b::HW2b(const QGLFormat &glf, QWidget *parent) : HW(glf, parent)
{
	// init vars
	m_theta = 0.0f;
	m_subdivisions = 4;
	m_twist = true;
	m_modelview .setToIdentity();
	m_projection.setToIdentity();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2b::initializeGL:
//
// Initialization routine before display loop.
// Gets called once before the first time resizeGL() or paintGL() is called.
//
void
HW2b::initializeGL()
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
// HW2b::resizeGL:
//
// Resize event handler.
// The input parameters are the window width (w) and height (h).
//
void
HW2b::resizeGL(int w, int h)
{
	// PUT YOUR CODE HERE

	// save window dimensions
	m_winW = w;
	m_winH = h;

	// compute aspect ratio
	float xmax, ymax;
	float ar = (float)w / h;
	if (ar > 1.0)
	{ // wide screen
		xmax = ar;
		ymax = 1.;
	}
	else
	{ // tall screen
		xmax = 1.;
		ymax = 1 / ar;
	}

	// set viewport to occupy full canvas
	glViewport(0, 0, w, h);

	// compute orthographic projection from viewing coordinates;
	// we use Qt's 4x4 matrix class in place of legacy OpenGL code
	// init viewing coordinates for orthographic projection
	m_projection.setToIdentity();
	m_projection.ortho(-xmax, xmax, -ymax, ymax, -1.0, 1.0);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2b::paintGL:
//
// Update GL scene.
//
void
HW2b::paintGL()
{
	// PUT YOUR CODE HERE

	// clear canvas with background color
	glClear(GL_COLOR_BUFFER_BIT);

	// bind vertex buffer to the GPU; enable buffer to be accessed
	// via the attribute vertex variable and specify data format
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, false, 0, NULL);

	// bind color buffer to the GPU; enable buffer to be accessed
	// via the attribute color variable and specify data format
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
	glEnableVertexAttribArray(ATTRIB_COLOR);
	glVertexAttribPointer(ATTRIB_COLOR, 3, GL_FLOAT, false, 0, NULL);

	// use glsl program
	glUseProgram(m_program[HW2B].programId());

	// pass the following parameters to vertex the shader:
	glUniformMatrix4fv(m_uniform[HW2B][PROJ], 1, GL_FALSE, m_projection.constData()); // projection matrix
	glUniformMatrix4fv(m_uniform[HW2B][MV], 1, GL_FALSE, m_modelview.constData()); // model view matrix
	glUniform1f(m_uniform[HW2B][THETA], m_theta); // theta (float)
	glUniform1i(m_uniform[HW2B][TWIST], m_twist); // twist flag (int)

	// draw triangles
	glDrawArrays(GL_TRIANGLES, 0, m_numPoints);

	// terminate program; rendering is done
	glUseProgram(0);
	glDisableVertexAttribArray(ATTRIB_COLOR);
	glDisableVertexAttribArray(ATTRIB_VERTEX);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2b::controlPanel:
//
// Create control panel groupbox.
//
QGroupBox*
HW2b::controlPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Homework 2b");
	groupBox->setStyleSheet(GroupBoxStyle);

	// create labels
	QLabel *label[2];
	label[0] = new QLabel("Theta");
	label[1] = new QLabel("Subdivide");

	// create sliders
	m_sliderTheta  = new QSlider(Qt::Horizontal);
	m_sliderSubdiv = new QSlider(Qt::Horizontal);

	// init sliders
	m_sliderTheta->setRange(0, 360);
	m_sliderTheta->setValue(0);
	m_sliderSubdiv->setRange(0, 6);
	m_sliderSubdiv->setValue(m_subdivisions);

	// create spinBoxes
	m_spinBoxTheta = new QSpinBox;
	m_spinBoxTheta->setRange(0, 360);
	m_spinBoxTheta->setValue(0);
	m_spinBoxSubdiv = new QSpinBox;
	m_spinBoxSubdiv->setRange(0, 6);
	m_spinBoxSubdiv->setValue(m_subdivisions);

	// init checkbox
	m_checkBoxTwist = new QCheckBox("Twist");
	m_checkBoxTwist->setChecked(m_twist);

	// layout for assembling widgets
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(label[0],	   0, 0);
	layout->addWidget(m_sliderTheta,   0, 1);
	layout->addWidget(m_spinBoxTheta,  0, 2);
	layout->addWidget(label[1],	   1, 0);
	layout->addWidget(m_sliderSubdiv,  1, 1);
	layout->addWidget(m_spinBoxSubdiv, 1, 2);
	layout->addWidget(m_checkBoxTwist, 2, 0);

	// assign layout to group box
	groupBox->setLayout(layout);

	// init signal/slot connections
	connect(m_sliderTheta,   SIGNAL(valueChanged(int)), this, SLOT(changeTheta (int)));
	connect(m_sliderSubdiv,  SIGNAL(valueChanged(int)), this, SLOT(changeSubdiv(int)));
	connect(m_spinBoxTheta,  SIGNAL(valueChanged(int)), this, SLOT(changeTheta (int)));
	connect(m_spinBoxSubdiv, SIGNAL(valueChanged(int)), this, SLOT(changeSubdiv(int)));
	connect(m_checkBoxTwist, SIGNAL(stateChanged(int)), this, SLOT(changeTwist (int)));

	return(groupBox);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2b::reset:
//
// Reset parameters.
//
void
HW2b::reset()
{
	// reset parameters
	m_theta		= 0.0f;
	m_subdivisions	= 4;
	m_twist		= true;

	// reset sliders and spinboxes
	m_sliderTheta->blockSignals(true);
	m_sliderTheta->setValue(0.0f);
	m_sliderTheta->blockSignals(false);

	m_spinBoxTheta->blockSignals(true);
	m_spinBoxTheta->setValue(0.0f);
	m_spinBoxTheta->blockSignals(false);
	m_sliderSubdiv->blockSignals(true);
	m_sliderSubdiv->setValue(m_subdivisions);
	m_sliderSubdiv->blockSignals(false);

	m_spinBoxSubdiv->blockSignals(true);
	m_spinBoxSubdiv->setValue(m_subdivisions);
	m_spinBoxSubdiv->blockSignals(false);

	// reset twist checkbox
	m_checkBoxTwist->setChecked(m_twist);

	// recompute geometry
	initVertexBuffer();

	// reset 4x4 modelview matrix
	m_modelview.setToIdentity();

	// draw
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2b::initShaders:
//
// Initialize vertex and fragment shaders.
//
void
HW2b::initShaders()
{

	// init uniforms hash table based on uniform variable names and location IDs
	UniformMap uniforms;
	uniforms["u_Modelview" ] = MV;
	uniforms["u_Projection"] = PROJ;
	uniforms["u_Theta"     ] = THETA;
	uniforms["u_Twist"     ] = TWIST;

	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	initShader(HW2B, QString(":/hw2/vshader2b.glsl"), 
	                 QString(":/hw2/fshader2b.glsl"), uniforms);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2b::initVertexBuffer:
//
// Initialize vertex buffer.
//
void
HW2b::initVertexBuffer()
{
	// init geometry data
	const vec2 vertices[] = {
		vec2( 0.0f,   0.75f ),
		vec2( 0.65f, -0.375f),
		vec2(-0.65f, -0.375f)
	};

	// recursively subdivide triangle into triangular facets;
	// store vertex positions and colors in m_points and m_colors, respectively
	divideTriangle(vertices[0], vertices[1], vertices[2], m_subdivisions);
	m_numPoints = (int) m_points.size();		// save number of vertices
	
	// CODE ADDED HERE BY STUDENT
	// create vertex and color buffers
	glGenBuffers(1, &m_vertexBuffer);
	glGenBuffers(1, &m_colorBuffer);
	// END CODE ADDED HERE
	
	// bind vertex buffer to the GPU and copy the vertices from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec2), &m_points[0], GL_STATIC_DRAW);

	// bind color buffer to the GPU and copy the colors from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec3), &m_colors[0], GL_STATIC_DRAW);

	// clear vertex and color vectors because they have already been copied into GPU
	m_points.clear();
	m_colors.clear();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2b::divideTriangle:
//
// Recursive subdivision of triangle (a,b,c). Recurse count times.
//
void
HW2b::divideTriangle(vec2 a, vec2 b, vec2 c, int count)
{
	// PUT YOUR CODE HERE

	// count was initialized as m_subdivisions and will end when count == 0
	if (count > 0) {
		// Get midpoints of each vertex for the subdivision
		vec2 ab = vec2((a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0);
		vec2 ac = vec2((a[0] + c[0]) / 2.0, (a[1] + c[1]) / 2.0);
		vec2 bc = vec2((b[0] + c[0]) / 2.0, (b[1] + c[1]) / 2.0);

		// Subdivide triangle from vertices of original/bigger triangle
		int newCount = count - 1;
		divideTriangle(a, ab, ac, newCount);
		divideTriangle(ab, b, bc, newCount);
		divideTriangle(ac, bc, c, newCount);

		// Subdivide center/middle triangle
		divideTriangle(ab, ac, bc, newCount);
	}
	else triangle(a, b, c);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2b::triangle:
//
// Push positions and colors of the three triangle vertices.
//
void
HW2b::triangle(vec2 v1, vec2 v2, vec2 v3)
{
	// init geometry
	m_points.push_back(v1);
	m_points.push_back(v2);
	m_points.push_back(v3);

	// init color
	float r = (float) rand() / RAND_MAX;
	float g = (float) rand() / RAND_MAX;
	float b = (float) rand() / RAND_MAX;
	m_colors.push_back(vec3(r, g, b));
	m_colors.push_back(vec3(r, g, b));
	m_colors.push_back(vec3(r, g, b));
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2b::changeTheta:
//
// Slot function to change rotation angle.
//
void
HW2b::changeTheta(int angle)
{
	// update slider and spinbox
	m_sliderTheta->blockSignals(true);
	m_sliderTheta->setValue(angle);
	m_sliderTheta->blockSignals(false);

	m_spinBoxTheta->blockSignals(true);
	m_spinBoxTheta->setValue(angle);
	m_spinBoxTheta->blockSignals(false);

	// init vars
	m_theta = angle * (M_PI / 180.);	// convert angle to radians

	// update model's rotation matrix
	m_modelview.setToIdentity();
	m_modelview.rotate(angle, QVector3D(0.0f, 0.0f, 1.0f));

	// draw
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2b::changeSubdiv:
//
// Slot function to change number of recursive subdivisions.
//
void
HW2b::changeSubdiv(int subdivisions)
{
	// update slider and spinbox
	m_sliderSubdiv->blockSignals(true);
	m_sliderSubdiv->setValue(subdivisions);
	m_sliderSubdiv->blockSignals(false);

	m_spinBoxSubdiv->blockSignals(true);
	m_spinBoxSubdiv->setValue(subdivisions);
	m_spinBoxSubdiv->blockSignals(false);

	// init vars
	m_subdivisions = subdivisions;

	// compute new vertices and colors
	initVertexBuffer();

	// draw
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2b::changeTwist:
//
// Slot function to turn on/off m_twist;
//
void
HW2b::changeTwist(int twist)
{
	// init vars
	m_twist = twist;

	// draw
	updateGL();
}
