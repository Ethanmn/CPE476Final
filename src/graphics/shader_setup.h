#ifndef SHADER_SETUP_H_
#define SHADER_SETUP_H_

#include <glm/glm.hpp>
#include "graphics/location_maps.h"

struct Shader;
struct Shaders;
struct Texture;

void setupModelView(Shader& shader, const UniformLocationMap& locations,
                    const glm::mat4& modelMatrix, const glm::mat4& viewMatrix,
                    bool needsNormal);
void setupProjection(Shader& shader, const UniformLocationMap& locations);
void setupView(Shader& shader, const UniformLocationMap& locations,
                            const glm::mat4& viewMatrix);
void setupSunShader(Shader& shader, const UniformLocationMap& locations,
                    float sunIntensity, glm::vec3 sunDir);

void setupTextureShader(Shader& shader, const UniformLocationMap& locations, const Texture& texture);
void setupHeightMap(Shader& shader, const UniformLocationMap& locations, const Texture& height_map);

void setupWireframeShader(Shader& shader, const UniformLocationMap& locations,
                          glm::vec4 color);

#endif
