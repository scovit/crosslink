#version 120

attribute vec4 position;

uniform float wdn;
uniform float dim;
uniform float pointsize;

void main()
{
  vec4 tmppos = position;
  tmppos.xy = tmppos.xy * wdn / dim;

  vec4 cameraPos = tmppos - vec4(wdn / 2, wdn / 2, 0.0f, 0.0f);

  gl_Position  = cameraPos;
  gl_PointSize = pointsize;
}
