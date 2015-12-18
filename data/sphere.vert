#version 120
  
attribute vec4 position;

uniform vec3 offset;
uniform mat4 perspectiveMatrix;
uniform float pointsize;

void main()
{ 
  vec4 cameraPos = position + vec4(offset.xyz, 0.0);

  gl_PointSize = pointsize;
  gl_Position = perspectiveMatrix * cameraPos;
}
