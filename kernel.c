#include <efi.h>
#include <efilib.h>
#define VERSION "0.1.0"
#define LVERSION L"0.1.0"

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  InitializeLib(ImageHandle, SystemTable);
  Print(L"Hello, world!\n");
  Print(LVERSION);
  while(1);
  return EFI_SUCCESS;
}