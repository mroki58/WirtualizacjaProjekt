#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <x86intrin.h>
#ifdef __linux__
#include <sys/io.h>
#endif

static inline uint64_t rdtsc_now() {
    unsigned int lo, hi;
    __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

typedef void (*fn_t)(void);

static void nop(void) { 
    __asm__ volatile("nop"); 
}
static void add(void) { 
    __asm__ volatile("add $1, %%eax" ::: "eax"); 
}
static void mov(void) { 
    __asm__ volatile("mov %%eax, %%ebx" ::: "eax","ebx"); 
}

static void cpuid(void) {
    unsigned int a=0, b, c, d;
    __asm__ volatile("cpuid":"=a"(a),"=b"(b),"=c"(c),"=d"(d):"a"(0));
}

static int have_ioperm = 0;
static void in80(void) {
#if defined(__linux__) && defined(__x86_64__)
    if (!have_ioperm) return;
    unsigned char val;
    __asm__ volatile("inb $0x80, %0" : "=a"(val));
#endif
}

static double measure_avg(fn_t fn, int reps) {
    double sum = 0.0;
    for (int r = 0; r < reps; ++r) {
        uint64_t t0 = rdtsc_now();
        fn();
        uint64_t t1 = rdtsc_now();
        sum += (double)(t1 - t0);
    }
    return sum / reps;
}

int main() {
    const int reps = 100;

#if defined(__linux__) && defined(__x86_64__)
    if (ioperm(0x80, 1, 1) == 0) have_ioperm = 1;
#endif

    printf("reps: %d\n", reps);
    printf("NOP avg: %.2f\n", measure_avg(nop, reps));
    printf("ADD avg: %.2f\n", measure_avg(add, reps));
    printf("MOV avg: %.2f\n", measure_avg(mov, reps));
    printf("CPUID avg: %.2f\n", measure_avg(cpuid, reps));
    if (have_ioperm)
        printf("IN 0x80 avg: %.2f\n", measure_avg(in80, reps));
    else
        printf("IN 0x80 brak uprawnien.\n");
    return 0;
}

// gcc -O2 zad2.c -o zad2
// sudo ./zad2
