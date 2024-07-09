#include <efi.h>
#include <efilib.h>
#define VERSION "0.1.3"

void echo_cmd(char* str, int n) {
  for(int i = 0; i<n; str++) {
    Print(L"%c", *str);
  }
}

EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  InitializeLib(ImageHandle, SystemTable);
  EFI_INPUT_KEY Key;
  EFI_STATUS Status;

  Print(L"Hello, world!\n");
  Print(L"Version %s", VERSION);

  Print(L"\nPress ^C key to exit");

  while (1) {
    Print(L"Shell> ");
    while(Key.UnicodeChar != 33) {
      
      Status = uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, &Key);
      if (Status == EFI_SUCCESS) {
        if (Key.UnicodeChar == 9) {
          Print(L"\t");
        }
        else if (Key.UnicodeChar == 33) {
          Print(L"\n");
        }
        else if (Key.UnicodeChar == 3) {
          Print(L"^C");
          return EFI_SUCCESS;
        }
        Print(L"%c", Key.UnicodeChar);
      }
    }
    
  }
  
  Print(L"Exit");

  return EFI_SUCCESS;
}