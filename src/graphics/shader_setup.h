#ifndef SHADER_SETUP_H_
#define SHADER_SETUP_H_

#include <glm/glm.hpp>
#include "graphics/location_maps.h"
struct Shader;
struct Shaders;

void setupModelView(Shader& shader, const UniformLocationMap& locations,
                    const glm::mat4& modelMatrix, const glm::mat4& viewMatrix,
                    bool needsNormal);
void setupProjection(Shader& shader, const UniformLocationMap& locations);
void setupView(Shader& shader, const UniformLocationMap& locations,
                            const glm::mat4& viewMatrix);
void setupSunShader(Shader& shader, const UniformLocationMap& locations,
                    float sunIntensity, glm::vec3 sunDir);
void setupTextureShader(Shader& shader, const UniformLocationMap& locations,
                        GLTextureID texture_id);
void setupWireframeShader(Shader& shader, const UniformLocationMap& locations,
                          glm::vec4 color);
void setupShadowShader(Shader& shader, const UniformLocationMap& locations,
                          glm::vec3 lightDir, glm::mat4 modelMatrix);

#endif
