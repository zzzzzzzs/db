#include <stdarg.h>
#include <stdio.h>
#include "sds.h"
// https://stackoverflow.com/questions/67629921/print-combine-generic-and-variadic-functions
// compiler gcc
#define COUNT(...)                                  \
  ({                                                \
    register int i;                                 \
    int c = 1;                                      \
    for (i = 0; i <= (strlen(#__VA_ARGS__)); i++) { \
      if (#__VA_ARGS__[i] == ',') {                 \
        c++;                                        \
      }                                             \
    };                                              \
    c;                                              \
  })

#define _print(object) \
  printf( _Generic((object),  \
    _Bool: "bool%d",        \
    unsigned char: "%hhu",                  \
    char: "%c",       \
    signed char: "%hhd",                    \
    short int: "%d",        \
    unsigned short int:"%d",            \
    int: "%d",        \
    unsigned int: "%u",                   \
    long int: "%lu",        \
    unsigned long int: "%lu",             \
    long long int: "%llu",\        
    unsigned long long int: "%llu",         \
    float: "%f",        \
    double: "%f",                         \
    long double: "%Lf",       \
    char *: "%s",               \
    void *: "%p",               \
    int *: "%p",                \
    default: "<unknow object at %p>") , object)

#define print(...) print_(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
#define print_(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, CNT, ...) \
  do {                                                           \
    if (CNT > 0) _print(a0);                                     \
    if (CNT > 1) _print(a1);                                     \
    if (CNT > 2) _print(a2);                                     \
    if (CNT > 3) _print(a3);                                     \
    if (CNT > 4) _print(a4);                                     \
    if (CNT > 5) _print(a5);                                     \
    if (CNT > 6) _print(a6);                                     \
    if (CNT > 7) _print(a7);                                     \
    if (CNT > 8) _print(a8);                                     \
    if (CNT > 9) _print(a9);                                     \
    puts("");                                                    \
  } while (0)
// #define print(...) pyprint(COUNT(__VA_ARGS__), __VA_ARGS__)
// print('A', 10, -10);  ->6510-10
// but print only receive int object because va_arg(ap, int)

// TODO The solution can handle up to 8 arguments but this limitation
// can be add argument.


// https://mp.weixin.qq.com/s/K3yUcTiSTiqbVt5fwVM3UQ
#define  DEBUG   1

#if DEBUG
  #define DBG_PRINTF(fmt, args...)  \
  do\
  {\
      printf("<<File:%s  Line:%d  Function:%s>> ", __FILE__, __LINE__, __FUNCTION__);\
      print(fmt, ##args);\
  }while(0)
#else
  #define DBG_PRINTF(args...)
#endif