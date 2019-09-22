#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>

#define FPTYPE double
#define N 1024

FPTYPE src[N], dst[N];

void init()
{
    int i;
    for (i=0;i<N;i++) src[i]=1.0/N*(i+1);
}

void test_add()
{
    int i;
    for (i=0;i<N;i++) dst[i] = src[i] + src[i];
}

void test_mul()
{
    int i;
    for (i=0;i<N;i++) dst[i] = src[i] * src[i];
}

void test_div()
{
    int i;
    for (i=0;i<N;i++) dst[i] = 1.0 / src[i];
}

void test_sqrt()
{
    int i;
    for (i=0;i<N;i++) dst[i]=sqrt(src[i]);
}

void test_exp()
{
    int i;
    for (i=0;i<N;i++) dst[i]=exp(src[i]);
}

void test_log()
{
    int i;
    for (i=0;i<N;i++) dst[i]=log(src[i]);
}

void test_sin()
{
    int i;
    for (i=0;i<N;i++) dst[i]=sin(src[i]);
}

void test_asin()
{
    int i;
    for (i=0;i<N;i++) dst[i]=asin(src[i]);
}


typedef void test_func();
struct test_item {
    char *test_name;
    test_func *test;
} tests[] = {
  { "add",  test_add  },
  { "mul",  test_mul  },
  { "div",  test_div  },
  { "sqrt", test_sqrt },
  { "exp",  test_exp  },
  { "log",  test_log  },
  { "sin",  test_sin  },
  { "asin", test_asin },
};

#define NUM_OF_TESTS (sizeof(tests)/sizeof(struct test_item))

void do_test()
{
    int i,j;
    for (i=0;i<NUM_OF_TESTS;i++) {
        struct timeval begin, end;
        struct rusage be,en;
        gettimeofday(&begin, 0);
        for (j=0;j<10000;j++) {
            tests[i].test();
            gettimeofday(&end, 0);
            int d=(end.tv_sec-begin.tv_sec)*1000000+(end.tv_usec-begin.tv_usec);
            if (d > 1000000) break;
        }
        int count=j*10;
        getrusage(RUSAGE_SELF, &be);
        for (j=0;j<count;j++) tests[i].test();
        getrusage(RUSAGE_SELF, &en);
        float diffu=(en.ru_utime.tv_sec-be.ru_utime.tv_sec)+1e-6*(en.ru_utime.tv_usec-be.ru_utime.tv_usec);
        float diffs=(en.ru_stime.tv_sec-be.ru_stime.tv_sec)+1e-6*(en.ru_stime.tv_usec-be.ru_stime.tv_usec);
        printf("%4s: %10d %.3e %.3e~%.3e\n", tests[i].test_name,count,N*count/(diffu+diffs),diffu/N/count,diffs/N/count);
    }
}

int main(int argc, char **argv)
{
    init();
    do_test();
}
