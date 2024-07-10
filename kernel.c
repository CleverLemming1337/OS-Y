#include <efi.h>
#include <efilib.h>
#define VERSION L"0.2.0.1"

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



  EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput;
  EFI_FILE_IO_INTERFACE *FileSystem;
  EFI_FILE_HANDLE Root, File;
  EFI_STATUS Status;
  UINTN BufferSize;
  VOID *Buffer;


  // Hole das Graphics Output Protocol
  Status = uefi_call_wrapper(BS->LocateProtocol, 3, &GraphicsOutputProtocol, NULL, (void **)&GraphicsOutput);
  if (EFI_ERROR(Status)) {
      Print(L"Unable to locate Graphics Output Protocol\n");
      while(1);
      return Status;
  }

  // Hole das Simple File System Protocol
  Status = uefi_call_wrapper(BS->LocateProtocol, 3, &FileSystemProtocol, NULL, (void **)&FileSystem);
  if (EFI_ERROR(Status)) {
      Print(L"Unable to locate File System Protocol\n");
      while(1);
      return Status;
  }

  // Öffne das Root-Verzeichnis
  Status = uefi_call_wrapper(FileSystem->OpenVolume, 2, FileSystem, &Root);
  if (EFI_ERROR(Status)) {
      Print(L"Unable to open root volume\n");
      while(1);
      return Status;
  }

  // Öffne die Bilddatei
  Status = uefi_call_wrapper(Root->Open, 5, Root, &File, L"Logo.bmp", EFI_FILE_MODE_READ, 0);
  if (EFI_ERROR(Status)) {
      Print(L"Unable to open file\n");
      while(1);
      return Status;
  }

  // Bestimme die Größe der Datei
  BufferSize = 0;
  Status = uefi_call_wrapper(File->GetInfo, 4, File, &GenericFileInfo, &BufferSize, NULL);
  if (Status != EFI_BUFFER_TOO_SMALL) {
      Print(L"Unable to get file size\n");
      while(1);
      return Status;
  }

  // Allokiere Speicher für die Datei
  Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, BufferSize, (void **)&Buffer);
  if (EFI_ERROR(Status)) {
      Print(L"Unable to allocate memory\n");
      while(1);
      return Status;
  }

  // Lese die Datei in den Speicher
  Status = uefi_call_wrapper(File->Read, 3, File, &BufferSize, Buffer);
  if (EFI_ERROR(Status)) {
      Print(L"Unable to read file\n");
      while(1);
      return Status;
  }

  // Schließe die Datei
  uefi_call_wrapper(File->Close, 1, File);

  // Berechne die Position, um das Bild mittig anzuzeigen
  UINTN ScreenWidth = GraphicsOutput->Mode->Info->HorizontalResolution;
  UINTN ScreenHeight = GraphicsOutput->Mode->Info->VerticalResolution;
  UINTN ImageWidth = 100; // Setze hier die tatsächliche Breite des Bildes ein
  UINTN ImageHeight = 100; // Setze hier die tatsächliche Höhe des Bildes ein
  UINTN X = (ScreenWidth - ImageWidth) / 2;
  UINTN Y = (ScreenHeight - ImageHeight) / 2;

  // Zeige das Bild an (dieser Teil muss an die spezifische Bildverarbeitung angepasst werden)
  // Hier wird angenommen, dass das Bild im BMP-Format vorliegt und direkt angezeigt werden kann
  Status = uefi_call_wrapper(GraphicsOutput->Blt, 10, GraphicsOutput, Buffer, EfiBltBufferToVideo, 0, 0, X, Y, ImageWidth, ImageHeight, 0);
  if (EFI_ERROR(Status)) {
      Print(L"Unable to display image\n");
      return Status;
  }

  // Befreie den Speicher
  uefi_call_wrapper(BS->FreePool, 1, Buffer);



  CHAR16 InputBuffer[100];
  int InputIndex = 0;

  int cmdLength;
  CHAR16 command[64];

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
    
    // Please sort alphabetically!
    if (StrCmp(command, L"clear") == 0) {
      SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    } else if (StrCmp(command, L"echo") == 0) {
      echo_cmd(InputBuffer + cmdLength+1, InputIndex - cmdLength);
      Print(L"\n");
    } else if (StrCmp(command, L"exit") == 0) {
      Print(L"Exiting...\n");
      break;
    } else if (StrCmp(command, L"reboot")==0) {
      Print(L"Shutting down...\n");
      SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
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
    } else if (StrCmp(command, L"help")==0) {
      // Please sort alphabetically!
      Print(L"Available commands:\n\n");
      Print(L"- CLEAR:    Clear screen.\n");
      Print(L"- ECHO:     Output a string.\n");
      Print(L"- EXIT:     Exit terminal (return to boot picker.\n");
      Print(L"- REBOOT:   Shut down computer.\n");
      Print(L"- SYSINFO:  Show system information.\n");
      Print(L"- TIME:     Show current time.\n");
    } else if (StrCmp(InputBuffer, L"") == 0) {
    }
    else {
      Print(L"\nCommand not found.\n");
    }
  }

  return EFI_SUCCESS;
}