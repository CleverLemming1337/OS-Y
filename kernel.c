#include <efi.h>
#include <efilib.h>

const char version[] = "0.1.4";

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
  Print(L"Version %s\n", version);
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
          Print(L"\b%c_", Key.UnicodeChar);
        } else if (Key.UnicodeChar == 3) { // ^C
          Print(L"^C\n");
          return EFI_SUCCESS;
        }
      }
    }
    Print(L"\n");

    if (StrCmp(InputBuffer, L"echo") == 0) {
      echo_cmd(InputBuffer + 5, InputIndex - 5);
      Print(L"\n");
    } else if (StrCmp(InputBuffer, L"exit") == 0) {
      Print(L"Exiting...\n");
      break;
    } else if (StrCmp(InputBuffer, L"sysinfo") == 0) {
      Print(L"System Information:\n");
      Print(L"EFI Specification Version: %d.%d\n", EFI_SPECIFICATION_MAJOR_REVISION, EFI_SPECIFICATION_MINOR_REVISION);
    } else if (StrCmp(InputBuffer, L"time") == 0) {
      // Get current time (a custom implementation)
      EFI_TIME currentTime;
      SystemTable->RuntimeServices->GetTime(&currentTime, NULL);

      Print(L"Current time: %02d:%02d:%02d\n", currentTime.Hour, currentTime.Minute, currentTime.Second);
    } else if (StrCmp(InputBuffer, L"color a") == 0) {
      // Set text color to green
      ST->ConOut->SetAttribute(ST->ConOut, EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK);
    }
    else {
      Print(L"\nCommand not found.\n");
    }
  }

  return EFI_SUCCESS;
}