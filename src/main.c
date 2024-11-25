#include <efi.h>
#include <efilib.h>
#include "parser.h"
#include "filesystem.h"
#define VERSION L"0.1.8"

void echo_cmd(CHAR16* str, int n) {
  /*
  Executes the ECHO command.

  Args:

  * str: CHAR16*: The string to print (pointer to start address)
  * n: int:       The length of the string
  */

 Print(L"str: %c", *str);
 Print(L"n: %d\n", n);

  for(int i = 0; i < n; i++, str++) {
    Print(L"%c", *str);
  }
}

void counter_cmd(CHAR16* arg) {
  static int counter;

  if (*arg == L'+') {
    Print(L"Counter is set from %d ", counter);
    Print(L"to %d.\n", ++counter);
  }
  else if (*arg == L'-') {
    Print(L"Counter is set from %d ", counter);
    Print(L"to %d.\n", --counter);
  }
  else if (*arg == L'0') {
    Print(L"Counter is set from %d to 0.\n", counter);
  }
  else {
    Print(L"The counter is currently at %d.\n", counter);
  }
}

void keyscan_cmd(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  EFI_INPUT_KEY Key;
  EFI_STATUS Status;

  while(1) {
    Status = uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, &Key);
    if (Status == EFI_SUCCESS) {
      Print(L"You pressed %c (Unicode: %d, ScanCode: %d).\n", Key.UnicodeChar, Key.UnicodeChar, Key.ScanCode);

      if (Key.UnicodeChar == 3) { // ^C
        return;
      }
    }
  }
}



EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  InitializeLib(ImageHandle, SystemTable);
  EFI_INPUT_KEY Key;
  EFI_STATUS Status;
  CHAR16 InputBuffer[100];
  int InputIndex = 0;

  int cmdLength;
  CHAR16 command[32];
  CHAR16 args[128];
  
  Print(L"Hello, world!\n");
  Print(L"Version: ");
  Print(VERSION);
  Print(L"\n\nEnter \"exit\" to exit or \"help\" for help.\n");

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
    sliceString(InputBuffer, command, 0, cmdLength);
    sliceString(InputBuffer, args, cmdLength+1, StrLen(InputBuffer));

    // Please sort alphabetically!
    if (StrCmp(command, L"clear") == 0) {
      SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    } else if (StrCmp(command, L"counter") == 0) {
      counter_cmd(args);
    } else if (StrCmp(command, L"echo") == 0) {
      echo_cmd(InputBuffer + cmdLength+1, InputIndex - cmdLength);
      Print(L"\n");
    } else if (StrCmp(command, L"exit") == 0) {
      Print(L"Exiting...\n");
      break;
    } else if (StrCmp(command, L"keyscan") == 0) {
      Print(L"Entering keyscan mode. Exit with ^C.\n");
      keyscan_cmd(ImageHandle, SystemTable);
    } else if (StrCmp(command, L"reboot")==0) {
      Print(L"Shutting down...\n");
      SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
    } else if (StrCmp(command, L"sysinfo") == 0) {
      Print(L"System Information:\n");
      Print(L"Version: %s", VERSION);
      Print(L"EFI Specification Version: %d.%d\n", EFI_SPECIFICATION_MAJOR_REVISION, EFI_SPECIFICATION_MINOR_REVISION);
    } else if (StrCmp(command, L"color a") == 0) {
      // Set text color to green
      ST->ConOut->SetAttribute(ST->ConOut, EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK);
    } else if (StrCmp(command, L"filesystems")==0) {
      ListFilesInRoot(ImageHandle, SystemTable);
    } else if (StrCmp(command, L"help")==0) {
      // Please sort alphabetically!
      Print(L"Available commands:\n\n");
      Print(L"- COUNTER:  Count up/down with +/- and reset with 0.\n");
      Print(L"- CLEAR:    Clear screen.\n");
      Print(L"- ECHO:     Output a string.\n");
      Print(L"- EXIT:     Exit terminal (return to boot picker.\n");
      Print(L"- KEYSCAN:  Press keys and get their unicode- and scan codes.\n");
      Print(L"- REBOOT:   Shut down computer.\n");
      Print(L"- SYSINFO:  Show system information.\n");
    } else if (StrCmp(InputBuffer, L"") == 0) {
    }
    else {
      Print(L"Command not found.\n");
    }
  }

  return EFI_SUCCESS;
}
