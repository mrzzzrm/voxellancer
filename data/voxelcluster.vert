#version 130
#extension GL_ARB_draw_instanced : require

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 viewProjection;

// cube data
in vec3 v_vertex;
in vec3 v_normal;

// instance data
in vec3 v_position;
in vec4 v_color;

flat out vec3 f_normal;
out vec3 f_color;
out vec3 modelposition;

void main() {
    modelposition = v_vertex;

    f_color = v_color.xyz;

    gl_Position = viewProjection * model * (vec4(v_vertex + v_position, 1.0));

    f_normal = (model * vec4(v_normal, 0.0)).xyz;
}