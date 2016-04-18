#include "stdafx.h"
#include "FSIO.h"
#include "File.h"

FSIO::~FSIO()
{
    if (diskHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(diskHandle);
        diskHandle = INVALID_HANDLE_VALUE;
    }
}

FSIO::FSIO(string drive)
{
    eof = false;
    c = 0;
    driveLetter = drive[0];
    wstring stemp = wstring(drive.begin(), drive.end());
    //LPCWSTR mount = stemp.c_str();
    LPWSTR volumeNameBuffer = new TCHAR[200];
    DWORD maxLength = 100;
    BOOL success = GetVolumeNameForVolumeMountPoint(stemp.c_str(), volumeNameBuffer, maxLength);

    if(!success)
    {
        wcout << L"Error retreiving volume information!!!" << endl;
        lastError = GetLastError();
        return;
    }
    
    int i;
    for (i = 0; volumeNameBuffer[i] != '\0'; i++);
        volumeNameBuffer[--i] = '\0';
    drivePath = wstring(volumeNameBuffer);
    //cout << "GUID Path of drive: " << CW2A(volumeNameBuffer) << endl;
    
    diskHandle = CreateFile(
        volumeNameBuffer,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING,
        FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH,
        NULL);

    if (diskHandle == INVALID_HANDLE_VALUE)
    {
        cout << "CreateFile failed on " << CW2A(volumeNameBuffer) << GetLastError();
        lastError = GetLastError();
        return;
    }

    cout << "\nSuccess opening drive: " << drive << endl;    
}

File* FSIO::Read(string filename)
{
    string _filename;
    File *file = nullptr;
    SetFilePointer(diskHandle, 0, NULL, FILE_BEGIN);
    eof = false;
    do
    {
        file = ReadNext();
        if (eof)
        {
            break;
        }
        if (file == nullptr)
        {
            continue;
        }
        _filename = file->getFileName();
    } while (filename != _filename);

    if (file != nullptr)
    {
        SetFilePointer(diskHandle, -4096, NULL, FILE_CURRENT);
        file->setAccessionTime(time(0));
        WriteFile(
            diskHandle,
            file->getSerializedFile(),
            4096,
            NULL,
            NULL);
    }
    
    return file;    
}

File* FSIO::ReadNext()
{
    LPVOID readBuffer = new char[4096];
    BOOL readResult = FALSE;
    LPDWORD readBytes;
    readBytes = new DWORD;
    *readBytes = 0;
    
    c++;
    File *file;

    if (c == 4096)
    {
        c = 0;
        eof = true;
        return nullptr;
    }

    readResult = ReadFile(
        diskHandle,
        readBuffer,
        4096,
        readBytes,
        NULL);

    if (readResult == FALSE)
    {
        cout << "Failure reading." << GetLastError();
        return nullptr;
    }

    if (readResult && *readBytes == 0)
    {
        eof = true;
        return nullptr;
    }
    
    file = new File(readBuffer);

    //cout << "Success reading.";
    return file;
}

bool FSIO::Create(File file)
{
    if (!(Read(file.getFileName()) == nullptr && eof))
    {
        cout << "File exists already.";
        return false;
    }
    string _filename;
    SetFilePointer(diskHandle, 0, NULL, FILE_BEGIN);
    eof = false;
    do
    {
        if (eof)
        {
            cout << "Disk full.";
            return false;
        }
        _filename = ReadNext()->getFileName();

    } while (_filename != "");
    SetFilePointer(diskHandle, -4096, NULL, FILE_CURRENT);
    BOOL writeResult = WriteFile(
        diskHandle,
        file.getSerializedFile(),
        4096,
        NULL,
        NULL);

    if (writeResult == FALSE)
    {
        cout << "Failure writing.";
        lastError = GetLastError();
    }

    return writeResult;
}

bool FSIO::Modify(string name, string data)
{
    string _filename;
    SetFilePointer(diskHandle, 0, NULL, FILE_BEGIN);
    eof = false;
    File *file;
    do
    {
        file = ReadNext();
        if (eof)
        {
            cout << "File not found.";
            return false;
        }
        _filename = file->getFileName();

    } while (_filename != name);
    SetFilePointer(diskHandle, -4096, NULL, FILE_CURRENT);
    file->setData(data);
    file->setModificationTime(time(0));
    file->setAccessionTime(time(0));

    BOOL modifyResult = WriteFile(
        diskHandle,
        file->getSerializedFile(),
        4096,
        NULL,
        NULL);

    if (modifyResult == FALSE)
    {
        cout << "Failure renaming.";
        lastError = GetLastError();
    }

    return modifyResult;
}

bool FSIO::Delete(string filename)
{
    string _filename;
    SetFilePointer(diskHandle, 0, NULL, FILE_BEGIN);
    eof = false;
    do
    {
        if (eof)
        {
            cout << "File not found.";
            return false;
        }
        _filename = ReadNext()->getFileName();

    } while (_filename != filename);
    SetFilePointer(diskHandle, -4096, NULL, FILE_CURRENT);

    byte *emptyBuffer;
    emptyBuffer = new byte[4096];
    for (size_t i = 0; i < 4096; i++)
    {
        emptyBuffer[i] = '\0';
    }
    BOOL deleteResult = WriteFile(
        diskHandle,
        emptyBuffer,
        4096,
        NULL,
        NULL);

    if (deleteResult == FALSE)
    {
        cout << "Failure deleting.";
        lastError = GetLastError();
    }

    return deleteResult;
}

vector<File> FSIO::List()
{
    vector<File> files;
    File *file = nullptr;
    SetFilePointer(diskHandle, 0, NULL, FILE_BEGIN);
    eof = false;
    do
    {
        file = ReadNext();
        if (file == nullptr)
        {
            break;
        }
        if (file->getFileName().length() != 0)
        {
            files.push_back(*file);
        }
    } while (!eof);

    return files;
}

bool FSIO::Rename(string oldName, string newName)
{
    string _filename;
    SetFilePointer(diskHandle, 0, NULL, FILE_BEGIN);
    eof = false;
    File *file;
    do
    {
        file = ReadNext();
        if (eof)
        {
            cout << "File not found.";
            return false;
        }
        _filename = file->getFileName();

    } while (_filename != oldName);
    SetFilePointer(diskHandle, -4096, NULL, FILE_CURRENT);
    file->setFileName(newName);
    file->setModificationTime(time(0));
    file->setAccessionTime(time(0));

    BOOL renameResult = WriteFile(
        diskHandle,
        file->getSerializedFile(),
        4096,
        NULL,
        NULL);

    if (renameResult == FALSE)
    {
        cout << "Failure renaming.";
        lastError = GetLastError();
    }

    return renameResult;
}
