#include <efi.h>
#include <efilib.h>
#define VERSION "0.1.4"


const int CHAR_REPLACEMENTS[] = {'\0', '^A', '^B', '^C', '^D', '^E'};



char replaceChar(CHAR16 *c) {
  return CHAR_REPLACEMENTS[*c];
}

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  InitializeLib(ImageHandle, SystemTable);
  EFI_INPUT_KEY Key;
  EFI_STATUS Status;

  Print(L"Hello, world!\n");
  Print(L"Version %s", VERSION);

  Print(L"\nPress ^C key to exit");

  while (1) {
    // Warte auf Tastatureingabe
    Status = uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, &Key);
    if (Status == EFI_SUCCESS) {
    
      // Gebe das Zeichen auf dem Bildschirm aus
      Print(L"You pressed: %c (Raw: %c, Unicode: %d)\n", replaceChar(&Key.UnicodeChar), Key.UnicodeChar, Key.UnicodeChar);

      if (Key.UnicodeChar == 3) { // ^C pressed
        break;
      }
    }
  }
  
  Print(L"Exit");

  return EFI_SUCCESS;
}