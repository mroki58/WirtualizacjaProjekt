Zadania Projektowe
Gr. 1 Zespół 3 - Wirtualizacja
Maksymilian Walicki, Igor Sala
25.10.2025 r.

## Zadanie 1. Detekcja środowiska wirtualnego i identyfikacja Hypervisora

**Cel**: Napisz program w języku C, który sprawdza, czy system działa wewnątrz środowiska wirtualnego oraz jaki hypervisor zarządza maszyną.

**Wskazówka**: Ta komenda była używana w filmie projektowym. Poszukaj w dokumentacjach, który leaf odpowiada za znalezienie informacji o środowisku i hypervisorze.

Źródła do wykorzystania:

- Film projektowy
- [https://en.wikipedia.org/wiki/CPUID](https://en.wikipedia.org/wiki/CPUID)
- [https://wiki.osdev.org/CPUID](https://wiki.osdev.org/CPUID)

## Zadanie 2. Pomiar czasów instrukcji uprzywilejowanych

**Cel**: Napisz program, który porównuje koszt wykonania dwóch zwykłych instrukcji i dwóch instrukcji uprzywilejowanych, które mogą powodować VM-exit. Wywołaj program na wirtualnej maszynie oraz w systemie hosta. Dla lepszych pomiarów skorzystaj ze średniej z wielu uruchomień tych samych komend.

**Źródła do wykorzystania**:

- film projektowy
- [https://xem.github.io/minix86/manual/intel-x86-and-64-manual-vol3/o_fe12b1e2a880e0ce-1075.html](https://xem.github.io/minix86/manual/intel-x86-and-64-manual-vol3/o_fe12b1e2a880e0ce-1075.html)

## Zadanie 3. Enumeracja funkcji CPU (rozszerzenia CPUID)

**Cel**: Napisz program, który:

1. Wywołuje CPUID(0) i CPUID(0x80000000) aby znaleźć zakres leaf-ów,
2. Iteruje po wszystkich leaf’ach w tym zakresie i wypisuje wartości rejestrów EAX/EDX.
3. Uruchom program na hoście i w VM.
4. Porównaj, czy hypervisor maskuje niektóre rozszerzenia (np. VMX, AVX, SGX).
5. Wskaż różnice w liczbie dostępnych leaf-ów.

**Wprowadzenie**: Procesory mają wiele leaf’ów CPUID, które opisują na przykład: producenta, model CPU, obsługiwane rozszerzenia (SSE, AVX, VMX), topologię rdzeni, cache, a także dodatkowe leaf’y definiowane przez hypervisor. Enumeracja pozwala zrozumieć, które funkcje są “widoczne” w środowisku wirtualnym, a które z nich są maskowane przez hypervisora.

Źródła do wykorzystania:

- [https://wiki.osdev.org/CPUID](https://wiki.osdev.org/CPUID)

## Zadanie 4 (opcjonalne). Analiza pól związanych z VM-exit i VM-entry

**Cel**:  
Dzięki temu zadaniu zrozumiesz strukturę i działanie pól kontrolnych VMCS odpowiedzialnych za przełączanie między trybami VMX root i non-root oraz zidentyfikujesz, które instrukcje powodują VM-exit oraz w jaki sposób hypervisor decyduje, kiedy je przechwycić.

**Wprowadzenie**:

Podczas działania w trybie wirtualizacji, procesor korzysta ze struktury VMCS (Virtual Machine Control Structure), zawierającej:

- VM-exit controls - co powoduje wyjście do hypervisora
- VM-entry controls - jak CPU wraca do gościa
- VM-exit reason - kod przyczyny przełączenia
- Guest-state area - stan CPU w momencie wyjścia,
- Host-state area - stan CPU po wejściu do hypervisora.

Instrukcje takie jak CPUID,… mogą wywoływać VM-exit w zależności od ustawień VMCS.

**Źródła do wykorzystania**:

- [https://wiki.osdev.org/VMX](https://wiki.osdev.org/VMX)
- [https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&ved=2ahUKEwjOoujTvbqQAxWxAtsEHd1kA9cQFnoECBcQAQ&url=https%3A%2F%2Fcdrdv2-public.intel.com%2F774497%2F326019-sdm-vol-3c.pdf&usg=AOvVaw1WqeUyNOYLRTI2VnC2sKsM&opi=89978449](https://cdrdv2-public.intel.com/774497/326019-sdm-vol-3c.pdf) (Szukaj w pliku - łatwiej znaleźć)

**Polecenie**:

1. Z dokumentacji Intel SDM znajdź i opisz:

|                            |          |
| -------------------------- | -------- |
| **Element VMCS**           | **Opis** |
| VM-exit reason             |          |
| Exit qualification         |          |
| VM-entry interruption info |          |
| Guest-state area           |          |
| Host-state area            |          |

# Przygotowanie środowiska

1. Pobierz środowisko [https://alpinelinux.org/downloads](https://alpinelinux.org/downloads)
2. Uruchom je w maszynie wirtualnej (np. VirtualBox: [https://www.virtualbox.org](https://www.virtualbox.org))
3. Skonfiguruj alpine korzystając z komendy:
4. setup-alpine (pamiętaj o skorzystaniu dysku jako sys)
5. apk update
6. apk add build-base linux-headers nano vim pciuthils ethtool (nano/vim zamiennie w zależności od preferowanego edytora tekstowego)

## Zadanie 5. Introspekcja

Zadanie oraz wymagane przygotowanie znajduje się w podfolderze zadanie_introspekcja. Przy okazji, jeśli nie korzysta się jeszcze z KVM to poniżej znajdują się przydatne linki:

- Instalacja na ubuntu: https://www.cherryservers.com/blog/install-kvm-ubuntu
- Instalacja na fedora: https://docs.fedoraproject.org/en-US/quick-docs/virtualization-getting-started/

Link do VM, którą wykorzystamy w tym zadaniu:
https://drive.google.com/file/d/1XeaRDhbHE7_CVhFU_SKbtMHkgdtyJjaE/view?usp=sharing
