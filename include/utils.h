#ifndef UTILS_H
#define UTILS_H

#include <time.h>

/**
 * @fn datetime(const struct tm *timeptr)
 * @brief Formate une structure struct tm en une date française (inspiré du code source trouvé sur http://www.cplusplus.com/reference/ctime/asctime/)
 * @author Romain FAILLA
 * @param struct tm *timeptr : la structure à formater
 * @return char*
 */
char *datetime(const struct tm *timeptr);

#endif
