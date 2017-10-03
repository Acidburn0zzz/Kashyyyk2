/* Any copyright of this file is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#ifndef YYY_TEST_H
#define YYY_TEST_H
#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int(*YYY_TestFunction)(void);

struct YYY_Test{
    YYY_TestFunction function;
    const char * const name;
    unsigned enabled;
};

#define YYY_EXPECT_TRUE(WHAT)\
    do{\
        if(!(WHAT)) {\
            printf("%s:%i in function %s: Expected %s to be true\n",\
                __FILE__, __LINE__, __FUNCTION__, #WHAT);\
            SUCCESS_INDICATOR = 0;\
        }\
    }while(0)

#define YYY_EXPECT_FALSE(WHAT)\
    do{\
        if((WHAT)) {\
            printf("%s:%i in function %s: Expected %s to be false\n",\
                __FILE__, __LINE__, __FUNCTION__, #WHAT);\
            SUCCESS_INDICATOR = 0;\
        }\
    }while(0)

#define YYY_EXPECT_INT_EQ(WHAT, EXPECTED)\
    do{\
        const int what_ = (int)WHAT, expected_ = (int)EXPECTED;\
        if(what_ != expected_) {\
            printf("%s:%i in function %s: Expected %s (%i) to be equal to %s (%i)\n",\
                __FILE__, __LINE__, __FUNCTION__, #WHAT, what_, #EXPECTED, expected_);\
            SUCCESS_INDICATOR = 0;\
        }\
    } while(0)

#define YYY_EXPECT_INT_NOT_EQ(WHAT, EXPECTED)\
    do{\
        const int what_ = (int)WHAT, expected_ = (int)EXPECTED;\
        if(what_ == expected_) {\
            printf("%s:%i in function %s: Expected %s (%i) to not be equal to %s (%i)\n",\
                __FILE__, __LINE__, __FUNCTION__, #WHAT, what_, #EXPECTED, expected_);\
            SUCCESS_INDICATOR = 0;\
        }\
    } while(0)

#define YYY_ASSERT_INT_EQ(WHAT, EXPECTED)\
    do{ \
        const int what_ = (int)(WHAT), expected_ = (int)(EXPECTED);\
        if(what_ != expected_) {\
            printf("%s:%i in function %s: Failed assert %s (%i) to be equal to %s (%i)\n",\
                __FILE__, __LINE__, __FUNCTION__, #WHAT, what_, #EXPECTED, expected_);\
            return 0;\
        }\
    } while(0)

#define YYY_ASSERT_YYY_STR_EQ_LITERAL(STR, LITERAL)\
    YYY_ASSERT_YYY_STR_EQ_LITERAL_N(STR, strnlen(STR, sizeof(LITERAL)), LITERAL)

#define YYY_ASSERT_YYY_STR_EQ_LITERAL_N(STR, LEN, LITERAL)\
    do{\
        const unsigned STR_LEN = sizeof(LITERAL) - 1;\
        YYY_ASSERT_INT_EQ(LEN, STR_LEN);\
        if(STR == NULL){\
            printf("%s:%i in function %s: Expected %s <NULL> to equal %s\n",\
                __FILE__, __LINE__, __FUNCTION__, #STR, LITERAL);\
            return 0;\
        }\
        else if(memcmp(STR, LITERAL, STR_LEN) != 0){\
            printf("%s:%i in function %s: Expected %s <",\
                __FILE__, __LINE__, __FUNCTION__, #STR);\
            fwrite(STR, LEN, 1, stdout);\
            printf("> to equal %s\n", LITERAL);\
            return 0;\
        }\
    }while(0)

#define YYY_EXPECT_YYY_STR_EQ_LITERAL_N(STR, LEN, LITERAL)\
    do{\
        const unsigned STR_LEN = sizeof(LITERAL) - 1;\
        YYY_ASSERT_INT_EQ(LEN, STR_LEN);\
        if(STR == NULL){\
            printf("%s:%i in function %s: Expected %s <NULL> to equal %s\n",\
                __FILE__, __LINE__, __FUNCTION__, #STR , LITERAL);\
            SUCCESS_INDICATOR = 0;\
        }\
        else if(memcmp(STR, LITERAL, STR_LEN) != 0){\
            printf("%s:%i in function %s: Expected %s <",\
                __FILE__, __LINE__, __FUNCTION__, #STR );\
            fwrite(STR, LEN, 1, stdout);\
            printf("> to equal %s\n", LITERAL);\
            SUCCESS_INDICATOR = 0;\
        }\
    }while(0)

#define YYY_EXPECT_YYY_STR_EQ_N(STR0, STR1, LEN)\
    do{\
        if(STR0 == NULL){\
            printf("%s:%i in function %s: Expected %s <NULL> to equal %s<",\
                __FILE__, __LINE__, __FUNCTION__, #STR0 , #STR1 );\
            fwrite(STR1, LEN, 1, stdout);putchar('>');putchar('\n');\
            SUCCESS_INDICATOR = 0;\
        }\
        else if(STR1 == NULL){\
            printf("%s:%i in function %s: Expected %s <",\
                __FILE__, __LINE__, __FUNCTION__, #STR0 );\
            fwrite(STR0, LEN, 1, stdout);\
            printf("> to equal %s <NULL>\n", #STR1 );\
            SUCCESS_INDICATOR = 0;\
        }\
        else if(memcmp(STR0, STR1, LEN) != 0){\
            printf("%s:%i in function %s: Expected %s <%s> to be equal to %s <%s>\n",\
                __FILE__, __LINE__, __FUNCTION__, #STR0 , STR0, #STR1 , STR1);\
            SUCCESS_INDICATOR = 0;\
        }\
    }while(0)

#define YYY_TEST(FUNC)\
{FUNC, #FUNC, 1}

#define YYY_DISABLED_TEST(FUNC)\
{FUNC, #FUNC, 0}

#define YYY_RUN_TEST(T, SUCESSES, NAME)\
if(!T.enabled){\
    puts("[" NAME "]Disabled test:");\
    putchar('\t');\
    puts(T.name);\
}\
else if(!T.function()){\
    puts("[" NAME "]Failed test:");\
    putchar('\t');\
    puts(T.name);\
}\
else do{\
    fputs("[" NAME "]Passed:\t", stdout);\
    puts(T.name);\
    SUCESSES++;\
}while(0)

#define YYY_TEST_FUNCTION(FUNC_NAME, TEST_ARRAY, NAME)\
int FUNC_NAME(){\
    unsigned i;\
    union {\
        unsigned u;\
        unsigned short s[2];\
    }results;\
    results.s[0] = 0;\
    results.s[1] = sizeof(TEST_ARRAY) / sizeof(TEST_ARRAY[0]);\
    for(i = 0; i<sizeof(TEST_ARRAY) / sizeof(TEST_ARRAY[0]); i++)\
        YYY_RUN_TEST(TEST_ARRAY[i], results.s[0], NAME);\
    return results.u;\
}

#define YYY_RUN_TEST_SUITE(R, I, NAME)\
{\
    union {\
        unsigned u;\
        unsigned short s[2];\
    }results;\
    putchar('\n');\
    puts("========== " #R " ==========\n[" NAME "]Beginnning " #R);\
    results.u = R();\
    printf("[" NAME "]" #R " Results: %i/%i\n========== " #R\
        " ==========\n", results.s[0], results.s[1]);\
    I |= (results.s[0] != results.s[1]);\
    putchar('\n');\
}

/*

An example of how to use these tests:

*//* Write a few tests. Returning 1 is a success, 0 is a failure. *//*
int YYY_TestWorks(){
    return 1;
}

int YYY_TestFails(){
    return 0;
}

*//* Create an array of the tests. *//*

static struct YYY_Test athena_tests[] = {
    YYY_TEST(YYY_TestWorks),
    YYY_TEST(YYY_TestFails)
};

*//* This creates the test function for this module. *//*
YYY_TEST_FUNCTION(YYY_Test, athena_tests)

*//* Call that function somewhere (The first argument to 
    YYY_TEST_FUNCTION is the name of the function). That runs the tests.*//*

#define ENABLE_TESTS
int main(){

#ifdef ENABLE_TESTS
*//* This should show one working test and one successful test. *//*
    YYY_Test();
#endif

}

*/

#endif /* YYY_TEST_H */
