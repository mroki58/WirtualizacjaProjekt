#!/usr/bin/env python3
import os 
import sys
import struct

PAGE_SIZE = os.sysconf("SC_PAGE_SIZE")

def va2pa(pid, va):
    pagemap_path = f"/proc/{pid}/pagemap"
    page_index = va // PAGE_SIZE
    offset_within_page = va % PAGE_SIZE

    try:
        with open(pagemap_path, "rb") as f:
            f.seek(page_index * 8)
            entry_bytes = f.read(8)
            if len(entry_bytes) != 8:
                raise ValueError("Nie można odczytać wpisu z pagemapu.")
            entry = struct.unpack("Q", entry_bytes)[0]
    except PermissionError:
        raise PermissionError("Brak uprawnień do odczytu pagemapu. Uruchom jako root.")

    present = (entry >> 63) & 1
    if not present:
        raise RuntimeError("Strona nie jest obecna w pamięci fizycznej.")
    
    PFN_MASK = (1 << 55) - 1
    pfn = entry & PFN_MASK
    pa = (pfn * PAGE_SIZE) + offset_within_page
    return pa

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f"Użycie: {sys.argv[0]} <PID> <adres_wirtualny_hex>")
        sys.exit(1)

    pid = int(sys.argv[1])
    va_str = sys.argv[2]
    va = int(str(va_str), 16) if va_str.startswith("0x") else int(va_str)

    try:
        pa = va2pa(pid, va)
        print(f"VA: 0x{va:x} -> PA: 0x{pa:x}")
    except Exception as e:
        print(f"Błąd: {e}")