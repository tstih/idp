/*
 * test.h
 *
 * Minimal test framework for TDD on CP/M.
 * 
 * NOTES:
 *  Based on https://eradman.com/posts/tdd-in-c.html 
 *  and http://www.jera.com/techinfo/jtns/jtn002.html
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 22.05.2021   tstih
 *
 */
#ifndef __TEST_H__
#define __TEST_H__

#include <stdio.h>

/* These three lines are the entire test framework. */
#define FAIL() printf("\nfailure in %s() line %d\n", __func__, __LINE__)
#define ASSERT(test) do { if (!(test)) { FAIL(); return 1; } } while(0)
#define VERIFY(test) do { int r=test(); tests_run++; if(r) return r; } while(0)

#endif /* __TEST_H__ */