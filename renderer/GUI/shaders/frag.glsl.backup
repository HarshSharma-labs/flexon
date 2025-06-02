#version 330 core
layout(origin_upper_left) in vec4 gl_FragCoord;
out vec4 FragColor;
uniform vec3 uColor;
uniform vec4 boxSize;

float borderWidth = 10.0f;
float borderRadius = 60.0f;

float BoxSdf(vec2 p , vec2 Size){
   p = abs(p) - Size;

  return max(p.x,p.y);
}

float RoundedBoxSDF(vec2 center,vec2 Size,float Radius){
    vec2 ab = Size  - (borderWidth + Radius);
    if(Radius > 0)
       return length(max(abs(center)-ab,0.0)) - (Radius+borderWidth);
           return BoxSdf(center,Size);
}

//TODO dropshadow inset and out
vec4 dropShadow(vec4 color,vec4 bgColor,float Radius,float offset,float Current){
  return mix(color,bgColor,smoothstep(Radius,offset,Current));
}

vec4 gradient(vec4 color1,vec4 color2,vec2 start,float angle,float fragDirection,float direction,bool enabled){
  if(!enabled)
    return color1;

  vec4 finalColor = vec4(1.0f);
  float interpolatedQuad = smoothstep(0.0f,direction,fragDirection);
  finalColor = mix(color1,color2,interpolatedQuad);
  return finalColor;
}

void main(){
  vec4 fragCoord = gl_FragCoord.xyzw;
  vec2 Size = boxSize.xy; // must be calculated on cpu and passed
  vec2 location = vec2(boxSize.z,boxSize.w) + boxSize.xy / 2; // must be calculated on scpu before passing
  float dist = RoundedBoxSDF(location - fragCoord.xy ,Size.xy/2,borderRadius);
  float smoothedAlpha = 1.0f - smoothstep(-1.0f,0.0f , abs(dist) -  borderWidth);
  float smoothedDist = smoothstep(0.0f,1.0f,dist); 
  FragColor = mix(mix(gradient(vec4(0.0f,1.0f,0.0f,1.0f),vec4(1.0f,0.0f,0.0f,1.0f),vec2(0.0f,0.0f),20.0f,fragCoord.y,Size.y,true),gradient(vec4(uColor,1.0f),vec4(uColor,smoothedAlpha),vec2(0.0f,0.0f),20.0f,fragCoord.y,Size.y,false),smoothedAlpha),vec4(0.0f),smoothedDist);
  
 }  
  

