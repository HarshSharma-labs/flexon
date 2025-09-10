const char *vertex_shader_code = R"(
#version 450

layout (location = 0) in vec2 inPosition;

void main(){
    gl_Position = vec4(inPosition,0.0,1.0);
})";

const char *fragment_shader_code = R"(
#version 450

layout(location = 0) out vec4 outColor;

void main(){
   outColor = vec4(1.0,0.0,0.0,0.9);
}

)";
