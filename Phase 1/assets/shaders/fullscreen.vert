#version 330 core

void main(){
    const vec3 TRIANGLE[3] = vec3[3](
        vec3(-1.0, -1.0, 0.0),
        vec3( 3.0, -1.0, 0.0),
        vec3(-1.0,  3.0, 0.0)
    );

    gl_Position = vec4(TRIANGLE[gl_VertexID], 1.0);
}