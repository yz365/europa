#ifndef _PATHDEFS_H_
#define _PATHDEFS_H_

#include <string>

#ifdef _MSC_VER
static const std::string PATH_SEPARATOR_STR(";");
static const char PATH_SEPARATOR_CHR(';');
static const std::string PATH_STR("\\");
static const char PATH_CHR('\\');
#else
const std::string PATH_SEPARATOR_STR(":");
const char PATH_SEPARATOR_CHR(':'); 
static const std::string PATH_STR("/");
static const char PATH_CHR('/');
#endif

#endif /* _PATHDEFS_H_ */
