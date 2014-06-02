uniform mat4 uModelViewProjectionMatrix;
attribute vec3 aPosition;

#ifdef USE_BONES
uniform int uHasBones;
uniform mat4 uBones[40];
#endif

#ifdef USE_BONES
attribute float aBoneID0;
attribute float aBoneWeight0;
attribute float aBoneID1;
attribute float aBoneWeight1;
attribute float aBoneID2;
attribute float aBoneWeight2;
attribute float aBoneID3;
attribute float aBoneWeight3;
attribute float aBoneID4;
attribute float aBoneWeight4;
#endif

mat4 calculateBones() {
   mat4 bone = mat4(1.0);
#ifdef USE_BONES
   if (uHasBones != 0) {
      if (int(aBoneID0) != -1) {
         bone = uBones[int(aBoneID0)] * aBoneWeight0;
      }
      if (int(aBoneID1) != -1) {
         bone += uBones[int(aBoneID1)] * aBoneWeight1;
      }
      if (int(aBoneID2) != -1) {
         bone += uBones[int(aBoneID2)] * aBoneWeight2;
      }
      if (int(aBoneID3) != -1) {
         bone += uBones[int(aBoneID3)] * aBoneWeight3;
      }
      if (int(aBoneID4) != -1) {
         bone += uBones[int(aBoneID4)] * aBoneWeight4;
      }
   }
#endif
   return bone;
}

void main() {
   gl_Position = uModelViewProjectionMatrix * calculateBones() * vec4(aPosition, 1.0);
}
