#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <cpuid.h>

int main() {
    unsigned int eax, ebx, ecx, edx;
    char vendor[13] = {0};

    // Leaf 0x1: ECX bit 31 - Hypervisor Present
    __cpuid(0x1, eax, ebx, ecx, edx);
    int hv = (ecx >> 31) & 1;

    if (!hv) {
        printf("Brak hypervisora. - host fizyczny.\n");
        return 0;
    }

    // Leaf 0x40000000 -  nazwa hypervisora
    __cpuid(0x40000000, eax, ebx, ecx, edx);
    memcpy(&hypervendor[0],  &ebx, 4);
    memcpy(&hypervendor[4],  &ecx, 4);
    memcpy(&hypervendor[8],  &edx, 4);

    printf("Wykryto srodowisko wirtualne.\n");
    printf("Hypervisor: %s\n", hypervendor);
    printf("Maksymalny leaf hypervisora: 0x%x\n", eax);
    return 0;
}

// gcc -O2 zad1.c -o zad1
// ./zad1
