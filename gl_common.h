#ifndef GL_COMMON_H
#define GL_COMMON_H

#include <GLES2/gl2.h>
#include "engine.h"

#ifdef ANDROID_NDK
#define  LOG_TAG    "shapegl"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#define  LOGI(...)  printf(__VA_ARGS__)
#define  LOGE(...)  printf(__VA_ARGS__)
#endif


class GL_common
{
public:
    static GLuint createTexture(int w, int h, unsigned char *img);
    static GLuint createProgram(const char *pVertexSource, const char *pFragmentSource);
    static GLuint loadShader(GLenum shaderType, const char *pSource);
};

#endif // GL_COMMON_H
