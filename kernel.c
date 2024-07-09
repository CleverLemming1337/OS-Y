#include <efi.h>
#include <efilib.h>
#define VERSION "0.1.2"
#define LVERSION L"0.1.2\n"

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  InitializeLib(ImageHandle, SystemTable);
  EFI_INPUT_KEY Key;
  EFI_STATUS Status;

  Print(L"Hello, world!\n");
  Print(LVERSION);

  Print(L"\nPress any key to exit");

  while (1) {
        // Warte auf Tastatureingabe
        Status = uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, &Key);
        if (Status == EFI_SUCCESS) {
            // Gebe das Zeichen auf dem Bildschirm aus
            Print(L"%c", Key.UnicodeChar);
        }
    }
  Print(L"Exit");

  return EFI_SUCCESS;
}