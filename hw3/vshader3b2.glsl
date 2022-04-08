#version 330

// PUT YOUR CODE HERE
in	vec3	a_Position;	
in	vec2	a_TexCoord;	

uniform   mat4  u_View;	  //uniform model view matrix 
uniform   mat4  u_Projection;	//uniform projection matrix 

out	vec2	vTexCoord;	//varying variable for passing texture coordinate to fragment shader

void main() 
{
	// PUT YOUR CODE HERE
	gl_Position = u_Projection * u_View * vec4(a_Position, 1);
	vTexCoord = a_TexCoord;
}
