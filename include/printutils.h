#ifndef PRINTUTILS_H
#define PRINTUTILS_H

#define PRINTFF(FRMT, ...)\
  printf("\e[1m%s.%d:\e[0m ", __FILE__, __LINE__);\
  printf(FRMT, ##__VA_ARGS__);

#endif // PRINTUTILS_H
