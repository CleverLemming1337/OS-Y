#include <efi.h>
#include <efilib.h>
#include "filesystem.h"

#ifndef PARSER
#define PARSER

EFI_STATUS
EFIAPI
ListFilesInRoot(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    EFI_STATUS Status;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem;
    EFI_FILE_PROTOCOL *RootDir;
    EFI_FILE_PROTOCOL *File;
    EFI_FILE_INFO *FileInfo;
    UINTN BufferSize;
    EFI_HANDLE *HandleBuffer;
    UINTN HandleCount;
    
    // Search for Block IO handles
    Status = gBS->LocateHandleBuffer(ByProtocol, &gEfiBlockIoProtocolGuid, NULL, &HandleCount, &HandleBuffer);
    if (EFI_ERROR(Status)) {
        Print(L"Error finding block handles: %r\n", Status);
        return Status;
    }

    // Search SimpleFileSystem protocol in block handles
    SimpleFileSystem = NULL;
    for (UINTN i = 0; i < HandleCount; i++) {
        EFI_BLOCK_IO_PROTOCOL *BlockIo;
        
        // Open block-IO protocol on handle
        Status = gBS->HandleProtocol(HandleBuffer[i], &gEfiBlockIoProtocolGuid, (VOID **)&BlockIo);
        if (EFI_ERROR(Status)) {
            continue; // Block IO not available, try next handle
        }

        // Check if SimpleFileSystem protocol is available
        Status = gBS->HandleProtocol(HandleBuffer[i], &gEfiSimpleFileSystemProtocolGuid, (VOID **)&SimpleFileSystem);
        if (!EFI_ERROR(Status)) {
            // SimpleFileSystem found, break loop
            break;
        }
    }

    FreePool(HandleBuffer);

    if (SimpleFileSystem == NULL) {
        Print(L"No filesystem found.\n");
        return EFI_NOT_FOUND;
    }

    // Open root directory
    Status = SimpleFileSystem->OpenVolume(SimpleFileSystem, &RootDir);
    if (EFI_ERROR(Status)) {
        Print(L"Error opening root directory: %r\n", Status);
        return Status;
    }

    // Speicher für Dateiinformationen anfordern
    BufferSize = sizeof(EFI_FILE_INFO) + 1024;
    FileInfo = AllocateZeroPool(BufferSize);
    if (FileInfo == NULL) {
        Print(L"Storage error: %r\n", Status);
        return EFI_OUT_OF_RESOURCES;
    }

    // Read all files
    Status = RootDir->Read(RootDir, &BufferSize, FileInfo);
    while (Status == EFI_SUCCESS) {
        if (FileInfo->FileName[0] != L'.') { // Skip ., .. and hidden files
            Print(L"%s\n", FileInfo->FileName);
        }

        Status = RootDir->Read(RootDir, &BufferSize, FileInfo);
    }

    if (Status != EFI_END_OF_FILE) {
        Print(L"Error listing files: %r\n", Status);
    }

    FreePool(FileInfo);

    return EFI_SUCCESS;
}

#endif
