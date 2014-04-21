#ifndef ATTRIBUTES_H_
#define ATTRIBUTES_H_

#include <string>

enum class Attribute {
   FIRST_ATTRIBUTE,
   VERTEX = FIRST_ATTRIBUTE,
   NORMAL,
   TEX_COORD,
   LAST_ATTRIBUTE
};

inline std::string attribute_name(Attribute attribute) {
   switch (attribute) {
      case Attribute::VERTEX:
         return "aPosition";
      case Attribute::NORMAL:
         return "aNormal";
      case Attribute::TEX_COORD:
         return "aTexCoord";
      default:
         throw "Unknown attribute";
   }
}

#endif // ATTRIBUTES_H_
