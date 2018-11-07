#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in float curAlpha;

uniform sampler2D texture_diffuse1;
uniform vec3 mColor;

float near = 0.1;
float far = 50.0;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 -1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}


void main()
{   float depth = (LinearizeDepth(gl_FragCoord.z)) / far; 
    //FragColor = texture(texture_diffuse1, TexCoords);
	FragColor = vec4(mColor, curAlpha);
	//FragColor = vec4(vec3(depth), 1.0);
}