#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aOffset;

out vec2 tex;
out vec3 crntPos;
out vec3 normal;

uniform mat4 camMatrix;
uniform bool instanced;
uniform mat4 model;


void main()
{
    tex = aTex;
    normal = aNormal;

    if(instanced) {
        crntPos = vec3(model * vec4(aPos + aOffset, 1.0f));
        gl_Position = camMatrix * model * vec4(aPos + aOffset, 1.0f);
    }
    else {
        crntPos = vec3(model * vec4(aPos, 1.0f));
        gl_Position = camMatrix * model*  vec4(aPos, 1.0f);
    }

    //gl_Position = vec4(aPos, 1.0f);
}