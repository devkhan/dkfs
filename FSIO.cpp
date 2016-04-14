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
    driveLetter = drive[0];
    wstring stemp = wstring(drive.begin(), drive.end());
    //LPCWSTR mount = stemp.c_str();
    LPWSTR volumeNameBuffer = new TCHAR[1000];
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
    cout << endl << "GUID Path of drive: " << CW2A(volumeNameBuffer) << endl;
    
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

    cout << "\nSuccess opening file.";    
}

File FSIO::Read(string filename)
{
    LPVOID readBuffer = new char[4096];
    BOOL readResult = FALSE;
    string _filename;
    File *file;
    do
    {
        readResult = ReadFile(
            diskHandle,
            readBuffer,
            512,
            NULL,
            NULL);

        if (readResult == FALSE)
        {
            cout << "Failure reading." << GetLastError();
            return NULL;
        }

        file = new File(readBuffer);
        _filename = (*file).getFileName();
    } while (filename != _filename);
    
    cout << "Success reading.";
    return *file;
}
