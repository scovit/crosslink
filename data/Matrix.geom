#version 150

layout(points) in;
layout(points, max_vertices = 2) out;

uniform vec3 offset;
uniform mat4 perspectiveMatrix;

void main()
{
    gl_Position = gl_in[0].gl_Position; // + vec4(-0.1, 0.0, 0.0, 0.0);
    gl_PointSize = gl_in[0].gl_PointSize;
    gl_Position.y = -gl_Position.y;

    gl_Position = gl_Position + vec4(offset.xyz, 0.0f);
    gl_Position = perspectiveMatrix * gl_Position;
    EmitVertex();
    EndPrimitive();

    gl_Position = gl_in[0].gl_Position; // + vec4(-0.1, 0.0, 0.0, 0.0);
    gl_PointSize = gl_in[0].gl_PointSize;
    float tmp = gl_Position.x;
    gl_Position.x = gl_Position.y;
    gl_Position.y = -tmp;

    gl_Position = gl_Position + vec4(offset.xyz, 0.0f);
    gl_Position = perspectiveMatrix * gl_Position;
    EmitVertex();

    EndPrimitive();
}
