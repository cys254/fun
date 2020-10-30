#include <stdio.h>
#include <stdlib.h>

#ifndef N
#define N 16
#endif

// #define COUNT_OPERATIONS
// #define PRINT_RESULT

extern int permute(int a[], int j, int n, int c, int conflictMap[N][N]);

int permute_count = 0;
long rotate_count = 0;

int main()
{
    int i, a[N], j;
    int conflictMap[N][N];
    for (i=0;i<N;i++) for (j=0;j<N;j++) conflictMap[i][j] = 0;
    for (i=0;i<N;i++) a[i] = i;
    
    printf("Count: %d\n", permute(a, 0, N, 0, conflictMap));
#ifdef COUNT_OPERATIONS
    printf("permute: %d\n", permute_count);
    printf("rotate: %ld\n", rotate_count);
#endif
}

void print_result(int a[], int n, int c) {
    int i;
    printf("%d> ", c);
    for (i=0;i<n;i++) printf("%d ", a[i]+1);
    printf("\n");
}

int permute(int a[], int j, int n, int c, int conflictMap[N][N]) {
#ifdef COUNT_OPERATIONS
    permute_count++;
#endif
    if (j == n - 1) {
        if (conflictMap[a[j]][j] == 0 && a[j] != a[j-1]+1 && a[j] != a[j-1]-1) {
            c += 1;
#ifdef PRINT_RESULT
            print_result(a, n, c);
#endif
        }
	return c;
    } else {
        int i,t;
        if (j > 0) for (i=j;i<n;i++) {int k=a[j-1]-i+j-1; if (k>=0) conflictMap[k][i]++; k=a[j-1]+i-j+1; if (k < n) conflictMap[k][i]++;}
//        if (j > 0) {int k=a[j-1]+j-1; int m=k<n?k+1:n; for (i=j;i<m;i++) conflictMap[k-i][i]++; k=a[j-1]-j+1; m=k>0?n-k:n; for (i=j;i<m;i++) conflictMap[k+i][i]++;}
 
        if (conflictMap[a[j]][j]==0) c = permute(a, j+1, n, c, conflictMap);
	for (i=j+1;i<n;i++) {
#ifdef COUNT_OPERATIONS
            rotate_count++;
#endif
            t = a[j];
            a[j] = a[i];
	    a[i] = t;
            if (conflictMap[a[j]][j]==0) c = permute(a, j+1, n, c, conflictMap);
        }
        t = a[j];
	for (i=j;i<n-1;i++) a[i] = a[i+1];
	a[n-1] = t;
        if (j > 0) for (i=j;i<n;i++) {int k=a[j-1]-i+j-1; if (k>=0) conflictMap[k][i]--; k=a[j-1]+i-j+1; if (k < n) conflictMap[k][i]--;}
//        if (j > 0) {int k=a[j-1]+j-1; int m=k<n?k+1:n; for (i=j;i<m;i++) conflictMap[k-i][i]--; k=a[j-1]-j+1; m=k>0?n-k:n; for (i=j;i<m;i++) conflictMap[k+i][i]--;}
	return c;
    }
}
