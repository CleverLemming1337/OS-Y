#include <efi.h>
#include <efilib.h>
#define VERSION "0.1.1"
#define LVERSION L"0.1.1\n"

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  InitializeLib(ImageHandle, SystemTable);
  Print(L"Hello, world!\n");
  Print(LVERSION);

  Print(L"\nPress any key to exit");
  EFI_INPUT_KEY Key;
  while (SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key) == EFI_NOT_READY);
  Print(L"Exit");

  return EFI_SUCCESS;
}