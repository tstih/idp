/*
 * stdlib.c
 *
 * idp standard C library stdlib implementation
 * 
 * NOTES:
 *  some functions are implemented in a separate files
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 28.04.2021   tstih
 *
 */

int abs (int i)
{
  return i < 0 ? -i : i;
}

int atoi(const char *str) {
    int res = 0;
    for (int i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';
    return res;
}