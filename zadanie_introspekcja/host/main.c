#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <libvmi/libvmi.h>

#define PROCESS_NAME "a.out"
#define VIRT_MEM_ADDR 0x55557fd5f008
#define PHYS_MEM_ADDR 0x731a008

int main(int argc, char **argv) {
    char *vm_name = "alpine";
    vmi_instance_t vmi = NULL;
    vmi_init_error_t error;

    if (vmi_init_complete(&vmi, vm_name, VMI_INIT_DOMAINNAME, NULL,
                          VMI_CONFIG_GLOBAL_FILE_ENTRY, NULL, &error) == VMI_FAILURE) {
        fprintf(stderr, "Nie udało się zainicjalizować LibVMI: %d\n", error);
        return 1;
    }
    printf("Połączono z VM: %s\n", vm_name);

    printf("Tryb adresowania: %d\n", vmi_get_page_mode(vmi, 0));

    uint32_t vcpus = vmi_get_num_vcpus(vmi);
    printf("Ilość vCPU: %u\n", vcpus);

    uint64_t memsize = vmi_get_memsize(vmi);
    printf("Ilość pamięci fizycznej RAM: %lu bajtów\n", memsize);

    addr_t offset_tasks, offset_pid, offset_comm, offset_pgd;
    addr_t list_head, list_entry, next_list_entry, task_addr;
    uint32_t pid;
    vmi_pid_t target_pid = 0;
    addr_t target_addr = 0;
    addr_t target_cr3 = 0;
    char procname[256] = {0};

    vmi_translate_ksym2v(vmi, "init_task", &list_head);
    vmi_get_offset(vmi, "linux_tasks", &offset_tasks);
    vmi_get_offset(vmi, "linux_pid", &offset_pid);
    vmi_get_offset(vmi, "linux_name", &offset_comm);
    vmi_get_offset(vmi, "linux_pgd", &offset_pgd);

    list_entry = list_head + offset_tasks;
    do {
        task_addr = list_entry - offset_tasks;
        vmi_read_32_va(vmi, task_addr + offset_pid, 0, &pid);
        strcpy(procname, vmi_read_str_va(vmi, task_addr + offset_comm, 0));

        printf("PID: %d  NAME: %s\n", pid, procname);

        if (strncmp(procname, PROCESS_NAME, strlen(PROCESS_NAME)) == 0) {
            target_pid = pid;
            target_addr = task_addr;
            vmi_read_addr_va(vmi, target_addr + offset_pgd, 0, &target_cr3);
            printf("Znaleziono proces docelowy: PID=%d CR3=0x%"PRIx64"\n", target_pid, target_cr3);
        }

        vmi_read_addr_va(vmi, list_entry, 0, &next_list_entry);
        list_entry = next_list_entry;
    } while(list_entry != list_head + offset_tasks);

    if(target_pid == 0) {
        printf("Nie znaleziono procesu %s\n", PROCESS_NAME);
        vmi_destroy(vmi);
        return 1;
    }

    uint32_t value;
    if (vmi_read_32_va(vmi, VIRT_MEM_ADDR, target_pid, &value) == VMI_SUCCESS) {
        printf("Wartość spod 0x%"PRIx64": 0x%"PRIx32"\n", VIRT_MEM_ADDR, value);
    } else {
        printf("Nie udało się odczytać pamięci procesu\n");
    }

    uint32_t value_phys;
    if(vmi_read_32_pa(vmi, PHYS_MEM_ADDR, &value_phys) == VMI_SUCCESS) {
        printf("Wartość spod fizycznego 0x%"PRIx64": 0x%"PRIx32"\n", PHYS_MEM_ADDR, value_phys);
    } else {
        printf("Nie udało się odczytać pamięci fizycznej\n");
    }


    vmi_destroy(vmi);
    printf("Zamknięto połączenie.\n");
    return 0;
}
