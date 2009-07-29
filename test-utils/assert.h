#ifndef SMUG_TEST_UTILS_ASSERT_H
#define SMUG_TEST_UTILS_ASSERT_H

void SmugTest_assert(int expression, char* assertion, char* file, int line);

#define assertTrue(expression) SmugTest_assert(expression, #expression, __FILE__, __LINE__);

#endif // SMUG_TEST_UTILS_ASSERT_H
