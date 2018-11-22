#version 430 core

uniform mat4 modelView;
uniform mat4 ortho;

uniform bool depth;

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 col;
layout (location = 2) in vec3 col0;

out vec3 C;

void main(void) {
//	gl_Position = ortho * modelView * vec4(vertex, 1.0);   
    gl_Position =modelView * vec4(vertex, 1.0);   
//    if (depth)
//    C = vec4(vec3(-gl_Position.z),1.0f).xyz*col;
//    C = vec4(0.0f,0.0f,-.0f,1.0f).xyz*col;
//    else
    C = col;

}
