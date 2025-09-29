const char *normalShaderCodeVertex = R"(
#version 450

layout (location = 0) in vec2 inPosition;

void main(){
    gl_Position = vec4(inPosition,0.0,1.0);
})";

const char *normalShaderCodeFragment = R"(
#version 450
layout(binding = 0) uniform ubo {
  vec4 color;
  };
layout(location = 0) out vec4 outColor;

void main(){
   outColor = color;
}

)";
