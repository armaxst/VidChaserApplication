#pragma once

#if (defined WIN32) && defined MAXSTAR_EXPORTS
#  define MAXSTAR_API __declspec(dllexport)
#else
#  define MAXSTAR_API
#endif