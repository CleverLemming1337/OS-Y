#include <efi.h>
#include <efilib.h>
#define VERSION "0.1.3"

void echo_cmd(CHAR16* str, int n) {
  for(int i = 0; i < n; str++) {
    Print(L"%c", *str);
  }
}

EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  InitializeLib(ImageHandle, SystemTable);
  EFI_INPUT_KEY Key;
  EFI_STATUS Status;
  CHAR16 InputBuffer[100];
  int InputIndex = 0;

  Print(L"Hello, world!\n");
  Print(L"Version %s\n", VERSION);
  Print(L"Press ^C key to exit\n");

  while (1) {
    Print(L"Shell> ");
    InputIndex = 0;

    while (1) {
      Status = uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, &Key);
      if (Status == EFI_SUCCESS) {
        if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
          InputBuffer[InputIndex] = '\0';
          break;
        } else if (Key.UnicodeChar == CHAR_BACKSPACE && InputIndex > 0) {
          InputIndex--;
          Print(L"\b \b");
        } else if (Key.UnicodeChar >= 32 && Key.UnicodeChar <= 126) {
          InputBuffer[InputIndex++] = Key.UnicodeChar;
          Print(L"%c", Key.UnicodeChar);
        } else if (Key.UnicodeChar == 3) { // ^C
          Print(L"^C\n");
          return EFI_SUCCESS;
        }
      }
    }

    if (StrCmp(InputBuffer, L"echo") == 0) {
      Print(L"\n");
      echo_cmd(InputBuffer + 5, InputIndex - 5);
      Print(L"\n");
    } else {
      Print(L"\nBefehl nicht gefunden\n");
    }
  }

  return EFI_SUCCESS;
}