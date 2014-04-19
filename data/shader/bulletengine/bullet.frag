#version 330

uniform float time;
uniform float withBorder;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 normalz;
layout(location = 2) out vec4 emissiveness;

in vec4 f_color;
flat in vec3 f_normal;
in float f_emissiveness;
in vec3 f_modelposition;
flat in float f_active;
flat in float f_deathTime;

vec4 voxelFragmentColor(vec3 color, float emissiveness, vec3 normal, vec3 positionInVoxel);
vec4 voxelFragmentEmissiveness(vec3 color, float emissiveness);
vec4 voxelFragmenNormalZ(vec3 normal);


void main() {
    if (f_active < 0.5 ) {
        discard;
    }

    fragColor = vec4(0, 1, 0, 1);//voxelFragmentColor(f_color.xyz, f_emissiveness, f_normal, f_modelposition);
    emissiveness = 0.5;//voxelFragmentEmissiveness(f_color.xyz, f_emissiveness);
    normalz = voxelFragmenNormalZ(f_normal);
}
