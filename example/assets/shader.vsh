#include <luna/core.glsl>

PropertyBlock(Material) {
    vec4 _Color;
};

out vec2 uv;
out vec4 color;

out vec3 pos;
out vec3 normal;

void main() {
    pos = (_ModelMat * vec4(VertexPosition, 1.0)).xyz;
    normal = mat3(_ModelMat) * VertexNormal;
    uv = VertexUv;
    color = _Color;
    gl_Position = _ProjViewMat * vec4(pos, 1.0);
}
