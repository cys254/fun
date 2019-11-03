#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

struct Ran {
    unsigned long long u, v, w;
};

void rand_iter(struct Ran *ran) {
    ran->u = ran->u * 2862933555777941757LL + 7046029254386353087LL;
    ran->v ^= ran->v >> 17;
    ran->v ^= ran->v << 31;
    ran->v ^= ran->v >> 8;
    ran->w = 4294957665U*(ran->w & 0xffffffff) + (ran->w >> 32);
}

void rand_init(struct Ran *ran, unsigned long long j) {
    ran->v = 4101842887655102017LL;
    ran->w = 1;
    ran->u = j ^ ran->v;
    rand_iter(ran);
    ran->v = ran->u;
    rand_iter(ran);
    ran->w = ran->v;
    rand_iter(ran);
}

unsigned long long rand_int64(struct Ran *ran) {
    unsigned long long x = ran->u ^ (ran->u << 21);
    x ^= x >> 35;
    x ^= x << 4;
    return (x + ran->v) ^ ran->w;
}

int main(int argc, char** argv)
{
    int cnt = 2, i;
    struct Ran r;
    rand_init(&r, 0);
    printf("u=0x%016llx\n", r.u);
    printf("v=0x%016llx\n", r.v);
    printf("w=0x%016llx\n", r.w);
    unsigned long long s = rand_int64(&r);
    printf("x=0x%016llx\n", s);
    if (argc > 1) {
        cnt = atoi(argv[1]);
    }
    struct timeval begin, end;
    gettimeofday(&begin, 0);
    for (i=0;i<cnt;i++) {
        rand_iter(&r);
        s ^= rand_int64(&r);
    }
    gettimeofday(&end, 0);
    float elapsed = end.tv_sec - begin.tv_sec + (end.tv_usec - begin.tv_usec)*1e-6;
    printf("Elapsed = %f seconds, throughput = %15.5g\n", elapsed, cnt/elapsed);
    printf("s=0x%016llx\n", s);
}
