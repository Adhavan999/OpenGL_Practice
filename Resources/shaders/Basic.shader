#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
}

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform float u_TMix;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main()
{
	vec4 texColor = mix(texture(u_Texture2, v_TexCoord), texture(u_Texture1, v_TexCoord), u_TMix);
	color = texColor;
}