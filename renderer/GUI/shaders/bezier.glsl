#version 330 core
layout(origin_upper_left) in vec4 gl_FragCoord;
out vec4 FragColor;

vec2 p1 = vec2(50,300);
vec2 p2 = vec2(50,50);
vec2 p3 = vec2(300,50);

#define MARKER_COLOR vec4(0.75, 0.0, 0.0, 1.0)
#define BEZIER_CURVE_COLOR vec3(0.0, 0.75, 0.0)
vec4 pointColor = vec4(0.5f,0.5f,0.9f,1.0f);
float pointRadius = 10.0f;




float circleSdf(vec2 center,vec2 Point,float Radius){ 
 return distance(center,Point) - Radius; 
};
/*
float bezierCoord(vec2 p1,vec2 p2,vec2 p3,vec2 pO){
*/
float bezierCoord(float p1x,float p1y,float p2x,float p2y,float p3x,float p3y,float pOx,float pOy){
        float a = p3x - 2 * p2x + p1x;
        float b = 2 * (p2x - p1x);
        float c = p1x - pOx;
        float dx = b * b - 4.0f * a * c;
        float bez = 0.0f;
        if (dx >= 0.0f) {
            float t1 = (-b + sqrt(dx)) / (2 * a);
            float t2 = (-b - sqrt(dx)) / (2 * a);
            float y1 = p1y + 2 * t1 * (p2y - p1y) + t1 * t1 * (p3y - 2 * p2y + p1y);
            float y2 = p1y + 2 * t2 * (p2y - p1y) + t2 * t2 * (p3y - 2 * p2y + p1y);
            float abspOy1 = abs(pOy - y1);
            float abspOy2 = abs(pOy - y2);
          
            
            float smoothedt1 = mix(0.0f,smoothstep(0.0f,10.0f,abspOy1),t1);
            float smoothedt2 = mix(0.0f,smoothstep(0.0f,10.0f,abspOy2),t2);


            //float mixedAlpha = smoothstep(0.0f,10.0f,abspOy1);
            //float maxt = max(t1,t2);
            
            float smoothedAlpha = min(smoothedt1,smoothedt2);
            
          /* 
            if ((0.0f <= t1 && t1 <= 1.0f && abs(pOy - y1) < 10.0f) ||
                (0.0f <= t2 && t2 <= 1.0f && abs(pOy - y2) < 10.0f))
                */
                
                    return smoothedAlpha;
                           
        
        }

  return bez;
}


void main(){
  vec2 position = gl_FragCoord.xy;
  if(circleSdf(p1,position,pointRadius) < 0 || circleSdf(p2,position,pointRadius) < 0 || circleSdf(p3,position,pointRadius) < 0){
     FragColor = pointColor;
  }
  else{
    float bezy = bezierCoord(p1.x,p1.y,p2.x,p2.y,p3.x,p3.y,position.x,position.y);
    float bezx = bezierCoord(p1.y,p1.x,p2.y,p2.x,p3.y,p3.x,position.y,position.x);
    
    float alpha = smoothstep(0.0f,1.0f,max(bezy,bezx));
    FragColor = mix(vec4(1.0f),vec4(0.0f,1.0f,0.0f,alpha),alpha);
  }
}


