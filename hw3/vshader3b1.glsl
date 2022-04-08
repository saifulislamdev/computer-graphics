#version 330

// PUT YOUR CODE HERE

// Input data
in vec3 a_Position;
in vec2 a_TexCoord;
in vec3 a_Color;
in vec3 a_Normal;

// Output data
out vec2 v_TexCoord;

// Uniforms
uniform mat4 u_Projection;
uniform mat4 u_View;

void main() 
{
	// PUT YOUR CODE HERE

	// Output position of the vertex
	gl_Position = u_Projection * u_View * vec4(a_Position, 1);

	// Pass texture information
	v_TexCoord = a_TexCoord;
}
