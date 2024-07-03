#include <efi.h>
#include <efilib.h>

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);

    // Terminal initialisieren
    ST->ConOut->ClearScreen(ST->ConOut);
    ST->ConOut->SetAttribute(ST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
    ST->ConOut->OutputString(ST->ConOut, L"Hello, EFI World!\n");

    // Tastaturprotokoll abrufen
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn = ST->ConIn;
    EFI_INPUT_KEY Key;

    // Tastatureingaben verarbeiten
    while (1) {
        // Warten auf Tastatureingabe
        while (ConIn->ReadKeyStroke(ConIn, &Key) == EFI_NOT_READY);

        // Zeichen ausgeben
        CHAR16 Str[2] = { Key.UnicodeChar, L'\0' };
        ST->ConOut->OutputString(ST->ConOut, Str);
    }

    return EFI_SUCCESS;
}