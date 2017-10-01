#include "pl_wchar.h"
#include <stddef.h>
#include <ctype.h>

/* Bits for code point */
#define CPBITS_2 (((CHAR32_T)1)<<12)-1
#define CPBITS_3 (((CHAR32_T)1)<<17)-1
#define CPBITS_4 (((CHAR32_T)1)<<22)-1
#define CPBITS_5 (((CHAR32_T)1)<<27)-1

/*-------------------------------------------------------------------------*
 * contains_wchar                                                          *
 *                                                                         *
 *-------------------------------------------------------------------------*/
int
contains_wchar(const char* s, int slen) {
  int i;
  int c;
  if (s == NULL || slen < 1) {
    return 0;
  }
  for(i = 0;i < slen && s[i] != '\0';i++) {
    c = s[i] & 0xff;
    if (c < 0x80) {
      continue;
    }
    return 1;
  }
  return 0;
}
int normalize_pos(int pos, const char* s) {
  int i;
  int c;
  if (s == NULL) {
    return 0;
  }
  for(i = 0;i < pos && s[i] != '\0';i++) {
    c = s[i] & 0xff;
    if (c < 0x80) {
      continue;
    }
    if (s[i+1] == '\0') {
      break;
    }
    if (c < 0xE0) {
      i++;
      continue;
    }
    if (s[i+2] == '\0') {
      break;
    }
    if (c < 0xF0) {
      i += 2;
      continue;
    }
    if (s[i+3] == '\0') {
      break;
    }
    i += 3;
    continue;
  }
  return i;
}
/*-------------------------------------------------------------------------*
 * count_wchar_bytes                                                       *
 *                                                                         *
 *-------------------------------------------------------------------------*/
int
count_wchar_bytes(const char* s, int slen) {
  int i;
  int c;
  if (s == NULL || slen < 1|| s[0] == '\0') {
    return 0;
  }
  c = s[0] & 0xff;
  if ((c & 0xc0) == 0x80) {
    /* detected broken character sequence */
    for(i = 1;i < slen && (s[i] & 0xc0) == 0x80;i++);
    return i;
  }
  if (c < 0x80 || slen < 2 || s[1] == '\0') {
    return 1;
  }
  if (c < 0xE0 || slen < 3 || s[2] == '\0') {
    return 2;
  }
  if (c < 0xF0 || slen < 4 || s[3] == '\0') {
    return 3;
  }
  return 4;
}
int
count_wchar_bytes_without_slen(const char* s) {
  int i;
  int c;
  if (s == NULL || s[0] == '\0') {
    return 0;
  }
  c = s[0] & 0xff;
  if ((c & 0xc0) == 0x80) {
    /* detected broken character sequence */
    for(i = 1;(s[i] & 0xc0) == 0x80;i++);
    return i;
  }
  if (c < 0x80 || s[1] == '\0') {
    return 1;
  }
  if (c < 0xE0 || s[2] == '\0') {
    return 2;
  }
  if (c < 0xF0 || s[3] == '\0') {
    return 3;
  }
  return 4;
}

int
count_wchar_bytes_back(const char* s) {
  int i;
  for (i = 1; (s[-i] & 0xc0) == 0x80;i++) {}
  return i;
}

int get_wchar_bytes(CHAR32_T c) {
  if (c < 0x80) {
    return 1;
  }
  if (c < CPBITS_2+1) {
    return 2;
  } else if (c < CPBITS_3+1) {
    return 3;
  } else if (c < CPBITS_4+1) {
    return 4;
  }
  return 4;
}

int
count_wchars(char* s, int slen) {
  int count;
  int i;
  for(count = 0;slen > 0;count++) {
    i = count_wchar_bytes(s, slen);
    if (i < 1) {
      break;
    }
    s += i;
    slen -= i;
  }
  return count;
}

CHAR32_T
get_wchar(const char* s, int slen) {
  int i;
  CHAR32_T c0, c;
  if (s == NULL || slen < 1 || s[0] == '\0') {
    return 0;
  }
  c0 = s[0] & 0xff;
  c = c0;
  if ((c0 & 0xc0) == 0x80) {
    /* detected broken character sequence */
    for(i = 1;i < slen && (s[i] & 0xc0) == 0x80;i++) {
      c = (c << 6) | (s[i] & 0x3f);
    }
    return c;
  }
  if (c0 < 0x80 || slen < 2 || s[1] == '\0') {
    return c0;
  }
  c = (c << 6) | (s[1] & 0x3f);
  if (c0 < 0xE0 || slen < 3 || s[2] == '\0') {
    return c & CPBITS_2;
  }
  c = (c << 6) | (s[2] & 0x3f);
  if (c0 < 0xF0 || slen < 4 || s[3] == '\0') {
    return c & CPBITS_3;
  }
  c = (c << 6) | (s[3] & 0x3f);
  return c & CPBITS_4;
}

CHAR32_T
get_wchar_without_slen(const char* s) {
  int i;
  CHAR32_T c0, c;
  if (s == NULL || s[0] == '\0') {
    return 0;
  }
  c0 = s[0] & 0xff;
  c = c0;
  if ((c0 & 0xc0) == 0x80) {
    /* detected broken character sequence */
    for(i = 1;(s[i] & 0xc0) == 0x80;i++) {
      c = (c << 6) | (s[i] & 0x3f);
    }
    return c;
  }
  if (c0 < 0x80 || s[1] == '\0') {
    return c;
  }
  c = (c << 6) | (s[1] & 0x3f);
  if (c0 < 0xE0 || s[2] == '\0') {
    return c & CPBITS_2;
  }
  c = (c << 6) | (s[2] & 0x3f);
  if (c0 < 0xF0 || s[3] == '\0') {
    return c & CPBITS_3;
  }
  c = (c << 6) | (s[3] & 0x3f);
  return c & CPBITS_4;
}

int put_wchar(char* s, int slen, CHAR32_T c) {
  if (slen > 0 && c < 0x80) {
    s[0] = c;
    return 1;
  }
  if (slen > 1 && c < CPBITS_2+1) {
    s[0] = (0x1f & (c >> 6)) | 0xc0;
    s[1] = (0x3f &  c      ) | 0x80;
    return 2;
  } else if (slen > 2 && c < CPBITS_3+1) {
    s[0] = (0x0f & (c >> 12)) | 0xe0;
    s[1] = (0x3f & (c >>  6)) | 0x80;
    s[2] = (0x3f &  c       ) | 0x80;
    return 3;
  } else if (slen > 3 && c < CPBITS_4+1) {
    s[0] = (0x07 & (c >> 18)) | 0xf0;
    s[1] = (0x3f & (c >> 12)) | 0x80;
    s[2] = (0x3f & (c >>  6)) | 0x80;
    s[3] = (0x3f &  c       ) | 0x80;
    return 4;
  }
  return 0;
}

int put_wchar_without_slen(char* s, CHAR32_T c) {
  if (c < 0x80) {
    s[0] = c;
    return 1;
  }
  if (c < CPBITS_2+1) {
    s[0] = (0x1f & (c >> 6)) | 0xc0;
    s[1] = (0x3f &  c      ) | 0x80;
    return 2;
  } else if (c < CPBITS_3+1) {
    s[0] = (0x0f & (c >> 12)) | 0xe0;
    s[1] = (0x3f & (c >>  6)) | 0x80;
    s[2] = (0x3f &  c       ) | 0x80;
    return 3;
  } else if (c < CPBITS_4+1) {
    s[0] = (0x07 & (c >> 18)) | 0xf0;
    s[1] = (0x3f & (c >> 12)) | 0x80;
    s[2] = (0x3f & (c >>  6)) | 0x80;
    s[3] = (0x3f &  c       ) | 0x80;
    return 4;
  }
  return 0;
}

int put_wchar_eof(char* s, int slen, CHAR32_T c) {
  int i;
  i = put_wchar(s, slen, c);
  s[i] = '\0';
  return i;
}

int fill_wchar(CHAR32_T* cp, int* modep, CHAR32_T c0) {
  if (*modep == FILL_WCHAR_MODE_INIT) {
    if ((c0 & 0xc0) == 0x80) {
      *cp = c0;
      return 1;
    }
    if (c0 < 0x80) {
      *cp = c0;
      return 1; // done  (single byte character)
    } else if (c0 < 0xE0) {
      *cp = c0 & 0x1f;
      *modep = 1; // conrinue ( one more character)
    } else if (c0 < 0xF0) {
      *cp = c0 & 0x0f;
      *modep = 2; // continue ( two more characters)
    } else if (c0 < 0xF8) {
      *cp = c0 & 0x07;
      *modep = 3; // continue ( three more characters)
    } else {
      return 1; // done (key code with modifiers or escape)
    }
    return 0;
  }
  *cp = (*cp << 6) | (c0 & 0x3f);
  --(*modep);
  if (*modep <= 0) {
    *modep = FILL_WCHAR_MODE_INIT;
    return 1;
  }
  return 0;
}

int
wchar_bytes_of_chars(const char* s, int slen, int chars) {
  int i;
  int bytes;
  bytes = 0;
  while(chars > 0 && slen > 0) {
    i = count_wchar_bytes(s, slen);
    if (i < 1) {
      break;
    }
    chars--;
    bytes += i;
    s += i;
    slen -= i;
  }
  if (slen < 0 || chars > 0) {
    return -1;
  }
  return bytes;
}

int
iswprint(CHAR32_T c) {
  if ((c & 0xfffff000) != 0) {
    return 1;
  }
  return isprint(c);
}
