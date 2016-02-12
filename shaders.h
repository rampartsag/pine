#include <string>

#ifndef SHADERS
#define SHADERS

class Shaders {
public:
    std::string frame_vsh, frame_fsh;
    std::string axis_vsh, axis_fsh;
    std::string menu_vsh, menu_fsh;
    std::string sides_vsh, sides_fsh;
    std::string refl_vsh, refl_fsh;
    std::string tmp_vsh, tmp_fsh;
    Shaders();
    ~Shaders();
};

#endif // SHADERS

