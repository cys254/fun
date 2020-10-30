#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

#ifndef N
#define N 16
#endif

// #define COUNT_OPERATIONS
// #define PRINT_RESULT

extern void *permute0_func(void *arg);
extern void permute0(int a[], int n, int conflictMap[N][N]);
extern void permute(int a[], int j, int n, int conflictMap[N][N]);

int permute_count = 0;
long rotate_count = 0;
int solution_count = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t done = PTHREAD_COND_INITIALIZER;
struct permute_data {
    int a[N];
    int n;
    int conflictMap[N][N];
};

#ifdef MT
extern pthread_t permute_mt(int a[], int n, int conflictMap[N][N]);
extern void *permute_func(void *arg);
#endif

int main()
{
    int i, a[N], j, err;
    struct permute_data data;
    pthread_t thread;
    struct timespec ts;

    data.n = N;
    for (i=0;i<N;i++) data.a[i] = i;
    memset(data.conflictMap, 0, sizeof(data.conflictMap));
    
    
    pthread_create(&thread, 0, permute0_func, &data);
    clock_gettime(CLOCK_REALTIME, &ts);
    for (;;) {
        ts.tv_sec++;
        pthread_mutex_lock(&lock);
        err = pthread_cond_timedwait(&done, &lock, &ts);
        pthread_mutex_unlock(&lock);
        if (err != ETIMEDOUT) break;
        printf("%-6.2f%%\n", solution_count/14772512.0*100);
    }
    pthread_join(thread, 0);

    printf("Count: %d\n", solution_count);
#ifdef COUNT_OPERATIONS
    printf("permute: %d\n", permute_count);
    printf("rotate: %ld\n", rotate_count);
#endif
}

void print_result(int a[], int n) {
    int i;
    printf("%d> ", solution_count);
    for (i=0;i<n;i++) printf("%d ", a[i]+1);
    printf("\n");
}

void *permute0_func(void *arg) {
    struct permute_data *data = (struct permute_data *)arg;
    permute0(data->a, data->n, data->conflictMap);
    pthread_mutex_lock(&lock);
    pthread_cond_broadcast(&done);
    pthread_mutex_unlock(&lock);
    return 0;
}

void permute0(int a[], int n, int conflictMap[N][N]) {
    int i, t;
#ifdef MT
    pthread_t threads[N-1];
#endif
#ifdef COUNT_OPERATIONS
    permute_count++;
#endif

 
#ifdef MT
    threads[0] = permute_mt(a, n, conflictMap);
#else
    permute(a, 1, n, conflictMap);
#endif
    for (i=1;i<n;i++) {
#ifdef COUNT_OPERATIONS
        rotate_count++;
#endif
        t = a[0]; a[0] = a[i]; a[i] = t;
#ifdef MT
        if (i != n-1) threads[i] = permute_mt(a, n, conflictMap);
        else permute(a, 1, n, conflictMap);
#else
        permute(a, 1, n, conflictMap);
#endif
    }
#ifdef MT
    for (i=0;i<n-1;i++) pthread_join(threads[i], 0);
#endif
}

#ifdef MT
pthread_t permute_mt(int a[], int n, int conflictMap[N-1][N]) {
    int i, j, err;
    pthread_t thread;
    struct permute_data *data = (struct permute_data*)malloc(sizeof(struct permute_data));
    data->n = n;
    for (i = 0; i < n; i++) data->a[i] = a[i];
    memset(data->conflictMap, 0, sizeof(data->conflictMap));
    err = pthread_create(&thread, 0, permute_func, data);
    if (err < 0) {
        printf("pthread_create error\n");
    }
    return thread;
}

void *permute_func(void *arg) {
    struct permute_data *data = (struct permute_data *)arg;
    permute(data->a, 1, data->n, data->conflictMap);
    return 0;
}
#endif

void permute(int a[], int j, int n, int conflictMap[N][N]) {
    int j1 = j-1;
#ifdef COUNT_OPERATIONS
    permute_count++;
#endif
    if (j == n - 1) {
        if (conflictMap[j1][a[j]] == 0 && a[j] != a[j1]+1 && a[j] != a[j1]-1) {
#ifdef MT
            pthread_mutex_lock(&lock);
#endif
            solution_count++;
#ifdef PRINT_RESULT
            print_result(a, n);
#endif
#ifdef MT
            pthread_mutex_unlock(&lock);
#endif
        }
    } else {
        int i,t;
        for (i=j;i<n;i++) {int k=a[j1]-i+j1; if (k>=0) conflictMap[i-1][k]++; k=a[j1]+i-j1; if (k < n) conflictMap[i-1][k]++;}
 
        if (conflictMap[j1][a[j]]==0) permute(a, j+1, n, conflictMap);
	for (i=j+1;i<n;i++) {
#ifdef COUNT_OPERATIONS
            rotate_count++;
#endif
            t = a[j];
            a[j] = a[i];
	    a[i] = t;
            if (conflictMap[j1][a[j]]==0) permute(a, j+1, n, conflictMap);
        }
        t = a[j];
	for (i=j;i<n-1;i++) a[i] = a[i+1];
	a[n-1] = t;
        for (i=j;i<n;i++) {int k=a[j1]-i+j1; if (k>=0) conflictMap[i-1][k]--; k=a[j1]+i-j1; if (k < n) conflictMap[i-1][k]--;}
    }
}
