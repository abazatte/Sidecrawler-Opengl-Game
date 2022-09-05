#version 400

#define PARTICLE_COUNT 200
layout (location = 0) out vec4 FragColor;

in flat int instanceID;


//Immer das gleiche für jeden Pixel
uniform vec4 Color;
uniform vec4 Colors[PARTICLE_COUNT];

void main()
{
    FragColor = Color;
}