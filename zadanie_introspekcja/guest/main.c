#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>

volatile uint32_t word = 0xdeadbeef;

int main() {

    system("clear");
    printf("adres wirtualny zmiennej word: 0x%p\n", &word);
    printf("wartosc zmiennej word: 0x%08" PRIx32 "\n", word);
    fflush(stdout);
    while(1) {
        sleep(1);
    }

}