# Introspekcja VM
___

Na samym dole pliku znajdziesz pomocne informacje do rozwiązania zadań.
Przed zajęciami polecamy wykonać zadania z sekcji Przygotowanie, przynajmniej jeśli - chodzi o git clone oraz przeczytanie i zrozumienie

### Cel

Zapoznanie się z funkcjonalnościami biblioteki do introspekcji maszyn wirtualnych LibVMI. Zrozumienie, jak działa tłumaczenie adresów wirtualnych procesów na fizyczne oraz jak można monitorować pamięć procesów VM przy pomocy narzędzia do introspekcji. Pokazanie pozostałych możliwości libvmi.

### Zadanie

Utwórz prosty program w C po stronie gościa, który tworzy zmienną volatile oraz wypisuje jej adres oraz wartość. Następnie z użyciem LibVMI - zapauzuj wirtualną maszynę, wylistuj aktualnie działające na maszynie procesy, znajdź uruchomiony proces, użyj funkcji do odczytywania pamięci wirtualnej oraz funkcji do odczytywania pamięci fizycznej dla naszej zmiennej. Sprawdź czy wartości się zgadzają - adres pamięci fizycznej pobierz przy pomocy skryptu python czytającego plik /proc/{pid}/pagemap.


## 1. Przygotowanie

- Utwórz VM przy pomocy dostarczonego pliku .qcow - pliki qcow pozwalają na szybkie przenoszenie maszyn wirtualnych oraz tworzenie ich kopii.

- Vm-ka ma pobrane wszystie potrzebne moduły - jest dostępny kompilator gcc, python, narzędzie make. W katalogu domowym znajduje się narzędzie linux-offset-finder jest to rozszerzenie jądra które pozwoli na znalezienie potrzebnych offsetów. Po wykonaniu operacji insmod użyj poniższego kodu, aby wyświetlić:


```bash 
dmesg | tail -n 10 >> offsets.txt
```

- Znaczenie tych offsetów jest bardzo ważne w introspekcji pamięci wirtualnej jądra, ponieważ pozwalają one na znalezienie odpowiednich struktur w pamięci, które odpowiadają poszczególnym procesom.

- Skopiuj plik offsets.txt na maszynę hosta - możesz do tego użyć komendy `scp` - do hosta skopiuj również plik `/boot/System.map*`, który przechowuje przechowuje mapę symboli jądra Linuksa — czyli listę wszystkich symboli (adresów funkcji, zmiennych globalnych, struktur danych itp.) zbudowanych w danej wersji jądra.

- Przejdź na maszynę hosta i w wybranym katalogu wykonaj:

```bash 
git clone https://github.com/libvmi/libvmi.git
```

- Przejdź do katalogu i pobierz zależności o których mowa w README - flex, bison, libjson-c etc.

- W katalogu repozytorium libvmi uruchom:

```bash 
mkdir build; cd build
cmake .. -DENABLE_KVM_LEGACY=ON -DENABLE_XEN=OFF 
make
sudo make install
```

- Kompilujemy LibVMI z wykorzystaniem KVM_LEGACY, które jest okrojone - jednakże nie wymaga od nas instalowania dodatkowych modułów do jądra. Wersja KVM jest ciekawsza pozwala np. na rejestrowanie eventów oraz odczytywanie rejestrów vCPU.

- Wykorzystaj przesłane offsety oraz plik System.map i zapisz je w formacie takim jaki jest przedstawiony w README libvmi.

- Jesteśmy gotowi do pracy :)

## 2. Na maszynie wirtualnej

- Utwórz prosty program, który posiada zmienną globalną volatile dowolnego typu (volatile, aby kompilator nie optymalizował). Wypisz zmienną wraz z jej adresem na ekran. (wskazówka: spłukaj bufor, bo może się nic nie wypisywać - fflush)

- (To proponuje wykonać na końcu, na razie nie twórz skryptu tylko skorzystaj z odpowiedniej metody LibVMI (poniżej), potem możesz utworzyć jeszcze skrypt lub wykorzystać gotowy) Utwórz skrypt Python tłumaczący adres wirtualny zmiennej na fizyczny. Wykorzystamy do tego celu plik binarny /proc/{pid}/pagemap, który przechowuje przechowuje mapowanie stron pamięci wirtualnej procesu na fizyczne ramki pamięci (PFN – Page Frame Number).


## 3. Na hoście

Introspekcja maszyn wirtualnych (VMI – Virtual Machine Introspection) to technika polegająca na obserwowaniu i analizowaniu stanu maszyny wirtualnej „z zewnątrz”, bez konieczności instalowania oprogramowania monitorującego wewnątrz tej maszyny. Można ją wykorzystać monitorowania procesów, podglądania, czy też debugowania działania systemów operacyjnych czy też jądra, oferuję możliwość testowania malware'u itp.

- Zainicjalizuj LibVMI przy pomocy nazwy VM i odczytaj informacje o maszynie wirtualnej takie jak tryb adresowania, ilość vCPU oraz ilość pamięci fizycznej, w dodatku możesz przetestować pauzowanie i wznawianie maszyny wirtualnej.


- Stwórz program, który wylistuje wszystkie procesy uruchomione na wirtualnej maszynie, następnie znajdź proces, który uruchomiłeś w ramach poprzedniego zadania i spróbuj odczytać dane z pamięci przy pomocy adresu wirtualnego oraz fizycznego (dodaj je jako stałe w programie). LibVMI daje możliwość tłumaczenia adresów jednak dla przetestowania działania skryptu czytającego pagemap odczytaj je w ten sposób.

- Zamknij połączenie z VM

### Uwaga!
Zadanie wcześniej zakładało również użycie funkcji do odczytywania rejestrów wirtualnego procesora niestety okazały się one nie zaimplementowane w ramach użytej wersji LibVMI, co pokazał debug biblioteki - więc niestety musieliśmy zrezygnować z tej części zadania choć wydawało się to całkiem ciekawe, bo chcieliśmy uchwycić dokładny moment w którym procesor wykonywał instrukcje w ramach naszego procesu na vm-ce.

## POMOCE

- Przydatne funkcje w ramach LibVMI (ich nazwy sugerują dość mocno do czego ich używać) - ich dokładnych interfejsów szukaj w `/usr/local/include/libvmi/libvmi.h`

```c
vmi_init()
vmi_init_complete() - użyj tej :)
vmi_get_page_mode()
vmi_get_num_vcpus()
vmi_get_memsize()
vmi_translate_ksym2v()
vmi_get_offset()
vmi_read_32_va()
vmi_read_str_va()
vmi_read_addr_va()
vmi_read_32_pa()
vmi_destroy()
vmi_pause_vm()
vmi_resume_vm()
vmi_get_vcpureg()
vmi_get_vcpuregs()
```

- Odczytywanie plików binarnych w python

```python
PAGE_SIZE = os.sysconf("SC_PAGE_SIZE") # pobierz wielkość strony danych ze zmiennej systemowej
with open(nazwa_pliku, "rb") as f # otwiera plik w trybie binarnym do odczytu
f.seek(offset) # przesuwa wskaźnik odczytywania pliku
f.read(number_of_bytes)
struct.unpack("Q", entry_bytes)[0]
import struct
entry = struct.unpack("Q", entry_bytes)[0] #zamienia odczytane bity na wartość 64 bitową (unsigned long long) i wypakowuje tylko wartość (w entry już znajduję się odpowiednia wartość)
```

- Odczytywanie pliku pagemap

Musimy znaleźć dokładną stronę i offset w ramach strony, aby znaleźć nasz adres fizyczny.

Indeks strony oraz offset łatwo znaleźć przy pomocy adresu wirtualnego ponieważ numer strony będzie częścią stałą z dzielenia adresu przez PAGE_SIZE, a offset będzie modulo z tego dzielenia. Następnie wystarczy odczytać 8 bajtów z indeksu strony w pliku pagemap, aby otrzymać pewną strukturę w której najbardziej znaczący bit oznacza czy strona jest wogóle dostępna w pamięci, a bity 55-0 przechowują fizyczny numer strony.

Następnie pomnóż numer strony przez jej wielkość i dodaj offset :).

Normalnie tłumaczenie adresów wirtualnych na fizyczne w systemach operacyjnych przechodzi przez wiele systemów map, gdzie odczytuję się cześci adresu i na ich podstawie przechodzi się z mapy do mapy, aby w końcu otrzymać fizyczną ramkę i offset. Ważne jest, że każdy proces ma inną początkową mapę tzw. root page table i jej wartość jest przechowywana w strukturze task_struct (nie na jej początku tylko na początku + pewien offset), która przechowuje informację o procesach w pamięci procesu 0(kernel). Przy przełączaniu kontekstu kernel pobiera odpowiedni wskaźnik (adres bazy tablicy stron) z mm_struct procesu i ładuje do CR3 fizyczny adres ramki bazowej page-table. Pagemap jedynie odczytuje i prezentuje dane, które jądro przechowuje o mapowaniach. W środowisku VM pojawia się dodatkowa warstwa: nested page tables (EPT w Intel / NPT w AMD). Wtedy translacja VA (Adres Wirtualny Procesu) -> GPA(Adres Fizyczny Gościa) -> HPA (Adres Fizyczny Hosta) wymaga dodatkowego kroku. (takie struktury przechowywane są hardware'owo i nie da się ich odczytać z poziomu systemu operacyjnego)(dlatego zadanie nie jest o nich dokładnie, ale warto wiedzieć :)).

- Listowanie zadań na VM-ce

W zadaniu w którym mamy wylistować procesy VM-ki i je odpowiednio nazwać nie korzystamy z żadnych komend w stylu `ps aux` tylko przechodzimy strukturę zapętlonej listy, która przechowuje informacje o zadaniach.

W Linuxie każdy proces jest reprezentowany przez strukturę task_struct, a wszystkie task_struct są połączone w coś w rodzaju dwukierunkowej, zapętlonej listy, dzięki czemu możemy przechodzić od jednego procesu do następnego i wrócić do początku listy bez utraty informacji o jakimkolwiek procesie. Punktem wyjścia jest specjalny proces init_task, który jest pierwszym procesem jądra i od którego zaczyna się cała lista procesów. W System.map znajdują się symbole jądra, czyli nazwy funkcji i struktur powiązane z ich adresami fizycznymi lub wirtualnymi w pamięci jądra, co pozwala nam znaleźć np. adres init_task.

Offsety, które pobieramy z funkcji vmi_get_offset, to nic innego jak przesunięcia w strukturze task_struct do konkretnych pól. offset_tasks wskazuje na pole w task_struct, które przechowuje wskaźnik do kolejnego elementu w liście procesów. offset_pid wskazuje na pole z identyfikatorem PID procesu. offset_comm wskazuje na pole, w którym przechowywana jest nazwa procesu. offset_pgd wskazuje na pole zawierające bazę tablicy stron (CR3) procesu. Dzięki tym przesunięciom możemy odczytać z pamięci wirtualnej maszyny wszystkie potrzebne informacje o każdym procesie. Pseudokod tego zadania wygląda mniej więcej następująco:

```
ustaw listę początkową = przetłumacz symbol "init_task" na adres
pobierz offsety dla pól tasks, pid, comm i pgd
ustaw bieżący element listy = lista początkowa + offset_tasks

dopóki bieżący element listy nie wrócił do początku:
    oblicz adres task_struct = bieżący element - offset_tasks
    odczytaj pid procesu = odczytaj 32-bitową wartość z pamięci wirtualnej na task_struct + offset_pid
    odczytaj nazwę procesu = odczytaj ciąg znaków z pamięci wirtualnej na task_struct + offset_comm
    wypisz PID i nazwę procesu
    odczytaj następny element listy = odczytaj wskaźnik z pamięci wirtualnej na bieżący element
    ustaw bieżący element listy = następny elemen
```


## NA KONIEC
Jeśli życzysz sobie odinstalować LibVMI to skorzystaj ze skryptu remove_libvmi - upewnij się, jednak czy nie odinstalowuje Ci czegoś turbo ważnego :) (nie powinienem)