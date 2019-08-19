#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern int permute(int a[], int j, int n, int c);

#define N 8
#define REP 10

int main()
{
    int i, a[N], k;
    clock_t begin, end;
    begin = clock();
    for (k=0;k<REP;k++) {
	for (i=0;i<N;i++) a[i] = i+1;
	permute(a, 0, N, 0);
    }
    end = clock();
    printf("%d %ld %ld %f\n", REP, begin, end, CLK_TCK);
    printf("%f\n", (end-begin)/CLK_TCK/REP);
}

int has_conflict(int a[], int j) {
    int i;
    for (i=0;i<j;i++) {
	if (abs(a[j]-a[i])==j-i) {
	    return 1;
	}
    }
    return 0;
}

void print_result(int a[], int n, int c) {
    int i;
    printf("%d> ", c);
    for (i=0;i<n;i++) printf("%d ", a[i]);
    printf("\n");
}

int permute(int a[], int j, int n, int c) {
    int i,t;
    if (has_conflict(a, j-1)) {
	return c;
    }
    if (j == n) {
	c += 1;
	/* print_result(a, n, c); */
	return c;
    } else {
	c = permute(a, j+1, n, c);
	for (i=j+1;i<n;i++) {
            t = a[j];
            a[j] = a[i];
	    a[i] = t;
	    c = permute(a, j+1, n, c);
        }
        t = a[j];
	for (i=j;i<n-1;i++) a[i] = a[i+1];
	a[n-1] = t;
	return c;
    }
}