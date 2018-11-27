#version 430 core

uniform mat4 modelView;
uniform mat4 ortho;

uniform bool depth;

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 col;
layout (location = 2) in vec2 texpos;
layout (location = 3) in vec3 col0;
layout (location = 4) in vec3 verm;
layout (location = 5) in vec3 colm;

out vec3 C;
out vec3 C0;
out vec2 UV;

uniform int fishnet;

void main(void) {
//	gl_Position = ortho * modelView * vec4(vertex, 1.0);   
    if (fishnet == 1) {
        gl_Position =modelView * vec4(verm, 1.0);   
        C0 = colm;
}
    else {
        gl_Position =modelView * vec4(vertex, 1.0);   
        C0 = col0;
    }
//    if (depth)
//    C = vec4(vec3(-gl_Position.z),1.0f).xyz*col;
//    C = vec4(0.0f,0.0f,-.0f,1.0f).xyz*col;
//    else
    C = col;
//    C0 = col0;
    UV = texpos;
}
