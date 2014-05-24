#include "globals.h"

const int kScreenWidth = 640;
const int kScreenHeight = 480;
const float kScreenWidthf = kScreenWidth;
const float kScreenHeightf = kScreenHeight;

#ifdef __APPLE__
const bool kDebugUseDeerModels = true; //For Katelyn
#else
const bool kDebugUseDeerModels = false;
#endif
