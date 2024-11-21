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
    
    // Suchen nach Block-IO-Handles
    Status = gBS->LocateHandleBuffer(ByProtocol, &gEfiBlockIoProtocolGuid, NULL, &HandleCount, &HandleBuffer);
    if (EFI_ERROR(Status)) {
        Print(L"Fehler beim Finden von Block-Handles: %r\n", Status);
        return Status;
    }

    // Block-Handles durchgehen und das SimpleFileSystem-Protokoll suchen
    SimpleFileSystem = NULL;
    for (UINTN i = 0; i < HandleCount; i++) {
        EFI_BLOCK_IO_PROTOCOL *BlockIo;
        
        // Block-IO-Protokoll auf dem aktuellen Handle öffnen
        Status = gBS->HandleProtocol(HandleBuffer[i], &gEfiBlockIoProtocolGuid, (VOID **)&BlockIo);
        if (EFI_ERROR(Status)) {
            continue; // Wenn Block-IO-Protokoll nicht verfügbar, zum nächsten Handle
        }

        // Überprüfen, ob das SimpleFileSystem-Protokoll auf diesem Block-IO-Handle verfügbar ist
        Status = gBS->HandleProtocol(HandleBuffer[i], &gEfiSimpleFileSystemProtocolGuid, (VOID **)&SimpleFileSystem);
        if (!EFI_ERROR(Status)) {
            // SimpleFileSystem gefunden, breche die Schleife ab
            break;
        }
    }

    // HandleBuffer freigeben
    FreePool(HandleBuffer);

    if (SimpleFileSystem == NULL) {
        Print(L"Kein Simple File System Protokoll gefunden.\n");
        return EFI_NOT_FOUND;
    }

    // Öffne das Root-Verzeichnis
    Status = SimpleFileSystem->OpenVolume(SimpleFileSystem, &RootDir);
    if (EFI_ERROR(Status)) {
        Print(L"Fehler beim Öffnen des Root-Verzeichnisses: %r\n", Status);
        return Status;
    }

    // Speicher für Dateiinformationen anfordern
    BufferSize = sizeof(EFI_FILE_INFO) + 1024;
    FileInfo = AllocateZeroPool(BufferSize);
    if (FileInfo == NULL) {
        Print(L"Speicherfehler: %r\n", Status);
        return EFI_OUT_OF_RESOURCES;
    }

    // Alle Dateien im Root-Verzeichnis lesen
    Status = RootDir->Read(RootDir, &BufferSize, FileInfo);
    while (Status == EFI_SUCCESS) {
        if (FileInfo->FileName[0] != L'.') { // "." und ".." überspringen
            Print(L"Gefundene Datei: %s\n", FileInfo->FileName);
        }

        Status = RootDir->Read(RootDir, &BufferSize, FileInfo);
    }

    if (Status != EFI_END_OF_FILE) {
        Print(L"Fehler beim Auflisten der Dateien: %r\n", Status);
    }

    // Speicher freigeben
    FreePool(FileInfo);

    return EFI_SUCCESS;
}

#endif