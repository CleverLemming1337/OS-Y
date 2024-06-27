#include <efi.h>
#include <efilib.h>

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);

    // Terminal initialisieren
    ST->ConOut->ClearScreen(ST->ConOut);
    ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
    ST->ConOut->OutputString(ST->ConOut, L"Hello, EFI World!\n");

    // Halte den Kernel an
    while (1);
    return EFI_SUCCESS;
}