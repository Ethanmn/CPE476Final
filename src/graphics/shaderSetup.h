#ifndef SHADER_SETUP_H_
#define SHADER_SETUP_H_

void setupModelView(Shader& shader, const UniformLocationMap& locations,
                    const glm::mat4& viewModelMatrix, bool needsNormal) const;
void setupViewAndProjection(Shader& shader, const UniformLocationMap& locations,
                            const glm::mat4& viewMatrix) const;
void setupSunShader(Shader& shader, const UniformLocationMap& locations,
                    const glm::mat4& modelMatrix, const glm::mat4& viewMatrix) const;
void setupTextureShader(Shader& shader, const UniformLocationMap& locations,
                        const glm::mat4& modelMatrix, const glm::mat4& viewMatrix) const;
void setupWireframeShader(Shader& shader, const UniformLocationMap& locations,
                          const glm::mat4& modelMatrix, const glm::mat4& viewMatrix) const;

#endif
