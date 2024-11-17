#include <luna/core.glsl>

PropertyBlock(Material) {
    vec4 _Color;
};

out vec4 color;

void main() {
    color = _Color;
    gl_Position = _ProjViewMat * _ModelMat * vec4(VertexPosition, 1.0);
}
