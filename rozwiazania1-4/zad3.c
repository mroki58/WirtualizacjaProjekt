#include <stdio.h>
#include <cpuid.h>

int main() {
    unsigned int eax, ebx, ecx, edx;
    unsigned int max_basic, max_ext;

    __cpuid(0x0, max_basic, ebx, ecx, edx);
    printf("Maksymalny leaf podstawowy: 0x%x\n", max_basic);

    __cpuid(0x80000000, max_ext, ebx, ecx, edx);
    printf("Maksymalny leaf rozszerzony: 0x%x\n\n", max_ext);

    printf("--- Podstawowe leafy ---\n");
    for (unsigned int i = 0; i <= max_basic; ++i) {
        __cpuid(i, eax, ebx, ecx, edx);
        printf("0x%08x: EAX=%08x EBX=%08x ECX=%08x EDX=%08x\n", i, eax, ebx, ecx, edx);
    }

    printf("\n--- Rozszerzone leafy ---\n");
    for (unsigned int i = 0x80000000; i <= max_ext; ++i) {
        __cpuid(i, eax, ebx, ecx, edx);
        printf("0x%08x: EAX=%08x EBX=%08x ECX=%08x EDX=%08x\n", i, eax, ebx, ecx, edx);
    }

    return 0;
}

// gcc -O2 zad3.c -o zad3
// ./zad3 | less
