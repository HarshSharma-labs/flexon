#include "./shaderwrapper.hpp"

GLuint SHADER::Viewindices[6] = {0, 1, 3, 1, 2, 3};

const char *SHADER::viewUniforms[10] = {"backgroundColorPrimary_cpu",
                                        "backgroundColorSecoundary_cpu",
                                        "boxSize_cpu",
                                        "borderWidth_cpu",
                                        "borderRadius_cpu",
                                        "borderColorPrimary_cpu",
                                        "borderColorSecoundary_cpu",
                                        "borderGradientPosition_cpu",
                                        "backgroundGradientPosition_cpu",
                                        nullptr};
