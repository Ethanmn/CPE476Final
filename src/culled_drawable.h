#ifndef CULLING_H_
#define CULLING_H_

enum class CullType {
   REFLECT_CULLING,
   VIEW_CULLING
};

typedef std::set<CullType> CullSet;

struct CulledTransform {
	glm::mat4 model;
	CullSet cullFlag;
};

struct CulledDrawable {
   DrawTemplate draw_template;
   std::vector<CulledTransform> model_transforms;
};

#endif