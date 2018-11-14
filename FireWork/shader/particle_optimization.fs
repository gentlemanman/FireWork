#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in float curAlpha;

uniform sampler2D texture_diffuse1;
uniform vec3 mColor;
uniform int isRGB;
uniform float near;
uniform float far;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 -1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}


void main()
{   
	//FragColor = texture(texture_diffuse1, TexCoords);
	float depth = (LinearizeDepth(gl_FragCoord.z)) / far;
	if(isRGB > 0){
		FragColor = vec4(mColor, curAlpha);
	} 
	else{
		FragColor = vec4(vec3(depth), 1.0);
	}
	
}