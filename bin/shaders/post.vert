// simple post-processing vertex shader
#version 410

layout( location = 0 ) in vec4 Position;

out vec2 vTexCoord;

void main()
{
	vTexCoord = vec2( Position ) * 0.5f + 0.5f;
	gl_Position = vec4( vec2( Position ), 0, 1 );
}