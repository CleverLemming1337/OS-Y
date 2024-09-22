#include <efi.h>
#include <efilib.h>

// Function to open the root directory of the EFI system partition
EFI_FILE_PROTOCOL* OpenRoot(EFI_HANDLE ImageHandle) {
    EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
    EFI_FILE_PROTOCOL* Root;

    // Get the Loaded Image Protocol for this image
    EFI_STATUS Status = uefi_call_wrapper(
        BS->HandleProtocol, 3, ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage
    );
    if (EFI_ERROR(Status)) {
        Print(L"Failed to get LoadedImage: %r\n", Status);
        return NULL;
    }

    // Get the Simple File System Protocol from the device handle
    Status = uefi_call_wrapper(
        BS->HandleProtocol, 3, LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID**)&FileSystem
    );
    if (EFI_ERROR(Status)) {
        Print(L"Failed to get FileSystem: %r\n", Status);
        return NULL;
    }

    // Open the root directory of the file system
    Status = uefi_call_wrapper(FileSystem->OpenVolume, 2, FileSystem, &Root);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to open root volume: %r\n", Status);
        return NULL;
    }

    return Root;
}

// Function to read the contents of a file and print them
EFI_STATUS ReadFile(EFI_FILE_PROTOCOL* Root, CHAR16* FilePath) {
    EFI_FILE_PROTOCOL* File;
    EFI_STATUS Status;
    UINTN BufferSize = 1024;  // Adjust the size if needed
    CHAR8 Buffer[1024];       // Buffer to hold file content

    // Open the file for reading
    Status = uefi_call_wrapper(Root->Open, 5, Root, &File, FilePath, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to open file %s: %r\n", FilePath, Status);
        return Status;
    }

    // Read the contents of the file
    Status = uefi_call_wrapper(File->Read, 3, File, &BufferSize, Buffer);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to read file: %r\n", Status);
        File->Close(File);
        return Status;
    }

    // Null-terminate the buffer and print the file contents
    Buffer[BufferSize] = '\0';
    Print(L"File contents:\n%a\n", Buffer);  // %a for ASCII string

    // Close the file
    uefi_call_wrapper(File->Close, 1, File);

    return EFI_SUCCESS;
}

// Main entry point for the UEFI application
EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    // Initialize the library (sets up SystemTable and other globals)
    InitializeLib(ImageHandle, SystemTable);

    Print(L"Reading file from EFI partition...\n");

    // Open the root directory of the EFI partition
    EFI_FILE_PROTOCOL* Root = OpenRoot(ImageHandle);
    if (Root == NULL) {
        Print(L"Error opening root.");
    }

    // Path to the file you want to read (must be on the EFI partition)
    CHAR16* FilePath = L"\\EFI\\BOOT\\test.txt";

    // Read and print the file contents
    EFI_STATUS Status = ReadFile(Root, FilePath);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to read file: %r\n", Status);
    }
}
