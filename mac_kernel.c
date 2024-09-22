#include <efi.h>
#include <efilib.h>

EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;

EFI_STATUS LocateGop(EFI_SYSTEM_TABLE *SystemTable) {
    EFI_STATUS Status;
    Status = uefi_call_wrapper(
        SystemTable->BootServices->LocateProtocol,
        3,
        &gEfiGraphicsOutputProtocolGuid,
        NULL,
        (VOID **)&Gop
    );

    if (EFI_ERROR(Status)) {
        Print(L"Failed to locate GOP: %r\n", Status);
    }

    return Status;
}

void SetVideoMode(UINT32 Width, UINT32 Height) {
    for (UINTN i = 0; i < Gop->Mode->MaxMode; i++) {
        EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
        UINTN SizeOfInfo;
        EFI_STATUS Status = Gop->QueryMode(Gop, i, &SizeOfInfo, &Info);
        if (Status == EFI_SUCCESS && Info->HorizontalResolution == Width && Info->VerticalResolution == Height) {
            Gop->SetMode(Gop, i);
            break;
        }
    }
}

void SetBackgroundColor(UINT32 Color) {
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL PixelColor;
    PixelColor.Red = (Color >> 16) & 0xFF;
    PixelColor.Green = (Color >> 8) & 0xFF;
    PixelColor.Blue = Color & 0xFF;
    PixelColor.Reserved = 0x00; // Unused for most cases.

    // Use Blt (Block Transfer) to fill the screen with the color.
    Gop->Blt(
        Gop,
        &PixelColor,
        EfiBltVideoFill,
        0, 0,
        0, 0,
        Gop->Mode->Info->HorizontalResolution,
        Gop->Mode->Info->VerticalResolution,
        0
    );
}

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);

    // Locate the GOP
    EFI_STATUS Status = LocateGop(SystemTable);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    // Optionally set a specific video mode, e.g., 1024x768
    // SetVideoMode(1024, 768);

    // Set the background color to blue (0x0000FF)
    SetBackgroundColor(0x0000FF);

    // Wait for key press before exiting
    EFI_INPUT_KEY Key;
    SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
    SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);
    
    return EFI_SUCCESS;
}
