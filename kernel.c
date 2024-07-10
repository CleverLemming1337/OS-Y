#include <efi.h>
#include <efilib.h>

const char version[] = "0.1.4";

void echo_cmd(CHAR16* str, int n) {
  /*
  Executes the ECHO command.

  Args:

  * str: CHAR16*: The string to print (pointer to start address)
  * n: int:       The length of the string
  */
  for(int i = 0; i < n; str++) {
    Print(L"%c", *str);
  }
}

int splitArgs(CHAR16* input, int n) {
  /*
  Splits the command and the args.

  Args:
  * input: CHAR16*: Start address of input
  * n: int:         Length of input
  
  Returns:
  * cmdLengths: int:  Length of command

  Example:
  * Input: Pointer to "echo Hello, world!"
  * Searching for ' ', when found, return i
  * Returns: 4 (offset from input to end of command)
  */

  for(int i = 0; i<n; i++, input++) {
    if (*input == ' ') {
      return i;
    }
  }
}

void sliceString(CHAR16 *source, CHAR16 *dest, int start, int end) {
    int j = 0;
    for (int i = start; i < end && source[i] != '\0'; i++) {
        dest[j++] = source[i];
    }
    dest[j] = '\0';
}

EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  InitializeLib(ImageHandle, SystemTable);
  EFI_INPUT_KEY Key;
  EFI_STATUS Status;
  CHAR16 InputBuffer[100];
  int InputIndex = 0;

  int cmdLength;
  CHAR16 command[64];

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
          Print(L"%c", Key.UnicodeChar);
        } else if (Key.UnicodeChar == 3) { // ^C
          Print(L"^C\n");
          return EFI_SUCCESS;
        }
      }
    }
    Print(L"\n");

    cmdLength = splitArgs(InputBuffer, StrLen(InputBuffer));
    sliceString(InputBuffer, command, 0, 5);
    Print(L"Command: %s \n", command);
    if (StrCmp(command, L"echo") == 0) {
      echo_cmd(InputBuffer + cmdLength+1, InputIndex - 5);
      Print(L"\n");
    } else if (StrCmp(command, L"exit") == 0) {
      Print(L"Exiting...\n");
      break;
    } else if (StrCmp(command, L"sysinfo") == 0) {
      Print(L"System Information:\n");
      Print(L"EFI Specification Version: %d.%d\n", EFI_SPECIFICATION_MAJOR_REVISION, EFI_SPECIFICATION_MINOR_REVISION);
    } else if (StrCmp(command, L"time") == 0) {
      // Get current time (a custom implementation)
      EFI_TIME currentTime;
      SystemTable->RuntimeServices->GetTime(&currentTime, NULL);

      Print(L"Current time: %02d:%02d:%02d\n", currentTime.Hour, currentTime.Minute, currentTime.Second);
    } else if (StrCmp(command, L"color a") == 0) {
      // Set text color to green
      ST->ConOut->SetAttribute(ST->ConOut, EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK);
    } else if (StrCmp(InputBuffer, L"") == 0) {
      break;
    }
    else {
      Print(L"\nCommand not found.\n");
    }
  }

  return EFI_SUCCESS;
}