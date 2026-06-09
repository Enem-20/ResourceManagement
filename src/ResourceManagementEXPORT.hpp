#pragma once

#if defined _WIN32 || defined __CYGWIN__
  #ifdef RESOURCE_MANAGEMENT_EXPORTS
    #define RESOURCE_MANAGEMENT_EXPORT __declspec(dllexport)
  #else
    #define RESOURCE_MANAGEMENT_EXPORT __declspec(dllimport)
  #endif
#else
  #define RESOURCE_MANAGEMENT_EXPORT
#endif