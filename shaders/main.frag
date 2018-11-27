#version 430 core

//out vec4 colour;

in vec3 C;
in vec3 C0;
in vec2 UV;
uniform vec4 PickingColor;
uniform sampler2D ourTexture;
uniform int onScreen;

layout(location=0) out vec4 colour;
//layout(location=1) out vec4 col0;

void main(void) {    	
//	colour = vec4(0.0, 0.0, 0.0, 1.0);
//	colour = vec4(vec3(gl_FragCoord.z), 1.0);
    if (onScreen==1)
    	colour = vec4(C0, 1.0);
//    	colour = texture(ourTexture, UV);//= vec4(C, 1.0);            
//        colour = vec4(0.5, 0.5, 0.5, 1.0);
    else
    	colour = vec4(C, 1.0);
    //colour = texture(ourTexture, UV);
//    col0 = vec4(0.5, 0.5, 0.5, 1.0);//texture(ourTexture, UV);
    //colour = PickingColor;
//    float z = gl_FragCoord.z * 2.0 - 1.0;
//    float d = (2.0 * 0.1 * 100.0) / (100.0 + 0.1 - z * (100.0 - 0.1));
//    colour = vec4(vec3(C*d), 1.0);
}
