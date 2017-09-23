#ifndef PL_WCHAR_T
#define PL_WCHAR_T

#include <stdint.h>

/* #ifdef char32_t */
/* #define CHAR32_T char32_t */
/* #else */
#define CHAR32_T int32_t
/* #endif */

extern int contains_wchar(const char* s, int slen);
extern int normalize_pos(int pos, const char* s);
/* get wchar (or char) byte size at s */
extern int count_wchar_bytes(const char* s, int slen);
extern int count_wchar_bytes_without_slen(const char* s);
/* get wchar (or char) byte size before s */
extern int count_wchar_bytes_back(const char* s);
/* get wchar (or char) byte size of c */
extern int get_wchar_bytes(CHAR32_T c);
/* get wchar (or char) at s */
extern CHAR32_T get_wchar(const char* s, int slen);
/* count wchar(or char) characters from s to s+slen */
extern int count_wchars(char* s, int slen);
extern int put_wchar(char* s, int slen, CHAR32_T c);
extern int put_wchar_without_slen(char* s, CHAR32_T c);
extern int put_wchar_eof(char* s, int slen, CHAR32_T c);

#define FILL_WCHAR_MODE_INIT 0
extern int fill_wchar(CHAR32_T* cp, int* modep, CHAR32_T c0);

#endif
