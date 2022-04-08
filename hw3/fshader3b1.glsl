#version 330

// PUT YOUR CODE HERE

// Input data
in vec2 v_TexCoord;

// Ouput data
out vec4 fragColor;

// Uniforms
uniform sampler2D u_Sampler;

void main() 
{
	// PUT YOUR CODE HERE

	fragColor = vec4(texture(u_Sampler, v_TexCoord).xyz, 1);
}
