# Wypełniona tabela - przykładowe rozwiązanie:

# | Element VMCS | Opis |

# | **VM-exit reason** | Kody powodu wyjścia do hypervisora |

# | **Exit qualification** | Szczegóły zdarzenia wymagane przez VMM | `IN` z portu 0x60 -> kwalifikacja niesie numer portu, rozmiar, kierunek |

# | **VM-entry interruption info** | Informuje jakie przerwanie (jeśli) ma zostać wstrzyknięte przy powrocie |

# | **Guest-state area** | Bufor stanu gościa (CR0/3/4, RIP, RSP, segmenty, MSR…) |

# | **Host-state area** | Stan hosta (RIP/RSP hypervisora, selektory, MSR…) |
