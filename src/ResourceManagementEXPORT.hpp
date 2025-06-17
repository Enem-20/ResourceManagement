#ifdef RESOURCE_MANAGEMENT_EXPORTS
#define RESOURCE_MANAGEMENT_EXPORT __declspec(dllexport)
#else
#define RESOURCE_MANAGEMENT_EXPORT __declspec(dllimport)
#endif