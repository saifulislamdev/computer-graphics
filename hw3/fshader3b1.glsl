#version 330

// PUT YOUR CODE HERE
in	vec2	  vTexCoord;	// texture coordinate from application 
	
out	vec4	  fragColor; 

uniform sampler2D u_texture; // texture object from application 

void main() 
{
	// PUT YOUR CODE HERE

	fragColor = texture(u_texture, vTexCoord);
}
