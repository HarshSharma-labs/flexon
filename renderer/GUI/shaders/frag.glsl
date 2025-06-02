#version 330 core
layout(origin_upper_left) in vec4 gl_FragCoord;
out vec4 FragColor;
uniform vec4 backgroundColorPrimary_cpu;
uniform vec4 backgroundColorSecoundary_cpu;
uniform vec4 borderColorPrimary_cpu;
uniform vec4 borderColorSecoundary_cpu;
uniform vec2 borderGradientPosition_cpu;
uniform vec2 backgroundGradientPosition_cpu;
uniform vec4 boxSize_cpu;
uniform vec4 borderWidth_cpu;
uniform vec4 borderRadius_cpu;


float borderWidth = 10.0f;
float borderRadius = 10.0f;
vec4 borderColor = vec4(1.0f);
vec4 boxColor = vec4(0.0f);

float RoundedBoxSDF(vec2 center,vec2 Size){

  
          vec2 ab = Size  - borderRadius ;
          float len =  length(max(abs(center)-ab,0.0));
          float offset =  borderRadius;
          float ret = len - offset;
         return ret;
         
   //vec2 p = abs(center) - Size ;

//  return max(p.x,p.y);

}

vec4 gradient(vec4 color1,vec4 color2,float fragDirection,float direction,bool enabled){
  if(!enabled)
    return color1;

  vec4 finalColor = vec4(1.0f);
  float interpolatedQuad = smoothstep(0.0f,direction,fragDirection);
  finalColor = mix(color1,color2,interpolatedQuad);
  return finalColor;
}

void setup(vec2 center,vec2 Size,vec2 fragCoord){
 
  /*
      if(center.x > 0 && center.y > 0)
          borderRadius = borderRadius_cpu.x;
      if(center.x < 0 && center.y > 0)
         borderRadius = borderRadius_cpu.y;
      if(center.x < 0 && center.y < 0)
         borderRadius = borderRadius_cpu.z;
      if(center.x > 0 && center.y < 0)
         borderRadius = borderRadius_cpu.w;
*/
       float borderFrag = (borderGradientPosition_cpu.y > 0.0f) ? fragCoord.x : fragCoord.y;
       float boxFrag = (backgroundGradientPosition_cpu.y > 0.0f) ? fragCoord.x : fragCoord.y;
        borderColor = gradient(vec4(1.0f,0.0f,0.0f,1.0f),borderColorSecoundary_cpu,borderFrag,borderGradientPosition_cpu.x,false);
        boxColor = gradient(backgroundColorPrimary_cpu,vec4(1.0f,0.0f,0.0f,1.0f),boxFrag,backgroundGradientPosition_cpu.x,false);

    
  };



void main(){
  vec4 fragCoord = gl_FragCoord.xyzw;
  vec2 Size = boxSize_cpu.xy; // must be calculated on cpu and passed
  vec2 location = vec2(boxSize_cpu.zw); // must be calculated on scpu before passing
  vec2 centered = fragCoord.xy - location;
  vec2 sizeNormal = Size.xy/2.0f;
  setup(centered,Size.xy,vec2(fragCoord.xy));
  float dist = RoundedBoxSDF(centered,sizeNormal);
  float smoothedAlpha = 1.0f - smoothstep(0.0f,-borderWidth , dist);
  float smoothedDist = smoothstep(-1.0f,1.0f,dist);
  vec4 globleColor =  mix(mix(boxColor,borderColor,smoothedAlpha),vec4(0.0f),smoothedDist);
  FragColor = globleColor;

} 

