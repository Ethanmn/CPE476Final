#ifndef ATTRIBUTES_H_
#define ATTRIBUTES_H_

#include <string>

extern const size_t kMaxBonesPerVertex;

enum class Attribute {
   FIRST_ATTRIBUTE,
   VERTEX = FIRST_ATTRIBUTE,
   NORMAL,
   TEX_COORD,

   BONE_ID0,
   BONE_ID1,
   BONE_ID2,
   BONE_ID3,
   BONE_ID4,

   BONE_WEIGHT0,
   BONE_WEIGHT1,
   BONE_WEIGHT2,
   BONE_WEIGHT3,
   BONE_WEIGHT4,
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
      case Attribute::BONE_ID0:
         return "aBoneID0";
      case Attribute::BONE_ID1:
         return "aBoneID1";
      case Attribute::BONE_ID2:
         return "aBoneID2";
      case Attribute::BONE_ID3:
         return "aBoneID3";
      case Attribute::BONE_ID4:
         return "aBoneID4";

      case Attribute::BONE_WEIGHT0:
         return "aBoneWeight0";
      case Attribute::BONE_WEIGHT1:
         return "aBoneWeight1";
      case Attribute::BONE_WEIGHT2:
         return "aBoneWeight2";
      case Attribute::BONE_WEIGHT3:
         return "aBoneWeight3";
      case Attribute::BONE_WEIGHT4:
         return "aBoneWeight4";
      default:
         throw "Unknown attribute";
   }
}

#endif // ATTRIBUTES_H_
