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