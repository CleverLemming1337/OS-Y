#include "efi.h"

#define print(string) SystemTable->ConOut->OutputString(SystemTable->ConOut, (string))
#define clear() SystemTable->ConOut->ClearScreen(SystemTable->ConOut)
#define readKey(keypointer) SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, keypointer)

// EFI_STATUS SplitArgs(CHAR16 *Input, CHAR16 **Command, CHAR16 **Args) {
//     UINTN Index = 0;
//     UINTN CmdLength = 0;

//     // Find the space character to separate the command and arguments
//     while (Input[Index] != L'\0') {
//         if (Input[Index] == L' ') {
//             break;
//         }
//         Index++;
//     }

//     CmdLength = Index;

//     // Allocate memory for the command
//     *Command = (CHAR16 *)AllocatePool((CmdLength + 1) * sizeof(CHAR16));
//     if (*Command == NULL) {
//         return EFI_OUT_OF_RESOURCES;
//     }

//     // Copy the command into allocated memory
//     StrnCpyS(*Command, CmdLength + 1, Input, CmdLength);
//     (*Command)[CmdLength] = L'\0';  // Null-terminate the command string

//     // Allocate memory for the arguments and copy them
//     *Args = (CHAR16 *)AllocatePool((StrLen(Input) - CmdLength) * sizeof(CHAR16));
//     if (*Args == NULL) {
//         FreePool(*Command);
//         return EFI_OUT_OF_RESOURCES;
//     }

//     // Copy the arguments starting from the space after the command
//     StrCpyS(*Args, StrLen(Input) - CmdLength, Input + CmdLength + 1);

//     return EFI_SUCCESS;
// }

// EFI Image Entry Point
EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    (void)ImageHandle;	// Prevent compiler warning

    // Set text to yellow fg/ green bg
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, 
            EFI_TEXT_ATTR(EFI_YELLOW,EFI_GREEN)); 

    // Clear screen to bg color 
    clear();

    print(u"Hello, world!\r\n\r\n");
    
    // Set text to red fg/ black bg 
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, 
            EFI_TEXT_ATTR(EFI_RED,EFI_BLACK));

    print(u"Press any key to shut down...");
    
    // Wait until keypress, then return
    EFI_INPUT_KEY key;
    while (SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key) != EFI_SUCCESS)
        ;
        
    // Shutdown, does not return
    SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);

    // Should never get here
    return EFI_SUCCESS;
}

