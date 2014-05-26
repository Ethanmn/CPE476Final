#ifndef CULLING_H_
#define CULLING_H_

enum class CullType {
   REFLECT_CULLING,
   VIEW_CULLING
};

typedef std::set<CullType> CullSet;

struct CulledDrawable {
   Drawable drawable;
   CullSet cullFlag;
};

#endif