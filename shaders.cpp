#include "shaders.h"

Shaders::Shaders() {
    frame_vsh =
                    "attribute vec4 a_position;\n"
                    "attribute vec4 a_color;\n"
                    "uniform bool BW;\n"
                    "uniform mat4 u_mvpMatrix;\n"
                    "varying vec4 v_color;\n"
                    "void main() {\n"
                    "if(!BW)\n"
                    "	v_color = vec4(0.9, 0.9, 1.0, 0.07);\n"
                    "else v_color = a_color;\n"
                    "  gl_Position = u_mvpMatrix * a_position;\n"
                    "}\n";
    frame_fsh =
                    "precision mediump float;\n"
                    "varying vec4 v_color;\n"
                    "void main() {\n"
                    "  gl_FragColor = vec4(v_color.rgb, 1.0);\n"
                    "}\n";

    axis_vsh = frame_vsh;
    axis_fsh = frame_vsh;

    sides_vsh = frame_vsh;
    sides_fsh =
                    "precision mediump float;\n"
                    "varying vec4 v_color;\n"
                    "void main() {\n"
                    "  gl_FragColor = vec4(0.0, 0.6, 1.0, 0.1);\n"
                    "}\n";

    menu_vsh =
                    "uniform mat4 u_mvpMatrix;\n"
                    "attribute vec4 a_position;\n"
                    "attribute vec2 a_texCoord;\n"
                    "varying vec2 v_texCoord;\n"
                    "void main() {\n"
                    "	v_texCoord = a_texCoord;\n"
                    "	gl_Position = a_position;\n"
                    "}\n";
    menu_fsh =
                    "precision mediump float;\n"
                    "varying vec2 v_texCoord;\n"
                    "uniform sampler2D s_texture;\n"
                    "void main() {\n"
                    "	vec4 f_color = texture2D(s_texture, v_texCoord);\n"
                    "	gl_FragColor = f_color;\n"
                    "}\n";

    refl_vsh =
                    "uniform mat4 MVPMatrix;\n"
                    "uniform mat3 NormalMatrix;\n"
                    "uniform bool BW;\n"
                    "attribute vec4 VertexColor;\n"
                    "attribute vec3 VertexNormal;\n"
                    "attribute vec4 VertexPosition;\n"
                    "varying vec4 Color;\n"
                    "varying vec3 Normal;\n"
                    "\n"
                    "void main() {\n"
                    "\n"
                    "if(!BW)\n"
                    "	Color = vec4(0.9, 0.9, 1.0, 0.07);\n"
                    "else Color = VertexColor;\n"
                    "Normal = normalize(NormalMatrix * VertexNormal);\n"
                    "gl_Position = MVPMatrix * VertexPosition;\n"
                    "\n"
                    "}\n";

    refl_fsh =
                    "precision mediump float;\n"
                    "uniform vec4 Ambient;\n"
                    "uniform vec4 LightColor;\n"
                    "uniform vec3 LightDirection;\n"
                    "uniform vec3 HalfVector;\n"
                    "uniform float Shininess;\n"
                    "uniform float Strength;\n"
                    "varying vec4 Color;\n"
                    "varying vec3 Normal;\n"
                    "\n"
                    "void main() {\n"
                    "\n"
                    "vec3 ld = vec3(0.707, 0, 0.707);\n"
                    "vec3 zero = vec3(0.0, 0, 1.0);\n"
                    "float diffuse = abs(dot(Normal, LightDirection));\n"
                    "float specular = abs(dot(Normal, HalfVector));\n"
                    "\n"
                    "if(diffuse == 0.0)\n"
                    "specular = 0.0;\n"
                    "else\n"
                    "specular = pow(specular, Shininess);\n"
                    "\n"
                    "vec4 scatteredLight = Ambient + LightColor * diffuse;\n"
                    "vec4 reflectedLight = LightColor * specular * Strength;\n"
                    "\n"
                    "gl_FragColor = min(Color*scatteredLight + reflectedLight, vec4(1.0));\n"
                    "\n"
                    "}\n";

}

Shaders::~Shaders() {

}
