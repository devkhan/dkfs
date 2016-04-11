// dkfs.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <windows.h>
#include <string>
#include <atlstr.h>

#define WIN32_LEAN_AND_MEAN
using namespace std;

int main(int argc, char **argv)
{
    wcout << "Hello World, this binary provides high-level access methods to a dkfs drive." << endl;

    LPWSTR physical = new TCHAR[10000];
    QueryDosDevice(NULL, physical, 10000);
    wcout << "Drives: " << physical << endl;

    DWORD mydrives = 100;// buffer length
    LPWSTR lpBuffer = new TCHAR[100];// buffer for drive string storage
    GetLogicalDriveStrings(mydrives, lpBuffer);
    for (int i = 0; i < 100; i++)
        wcout << lpBuffer[i];

    

    // Below is the code for getting FS info, but since ours is unformatted,
    // its not gonna work. We'll have look for other solutions.

    /* 
    string s = "D:\\";
    wstring stemp = wstring(s.begin(), s.end());
    LPCWSTR drive = stemp.c_str();
    LPTSTR volumeNameBuffer = new TCHAR[100];
    DWORD maxLength;
    DWORD flags;

    BOOL successGettingVolumeInformation = GetVolumeInformation(
        drive,
        volumeNameBuffer,
        1000,
        NULL,
        &maxLength,
        &flags,
        NULL,
        0);

    if (successGettingVolumeInformation == TRUE)
    {
        wcout << "Drive information: " << drive << endl;
        wcout << "Volume Name: " << volumeNameBuffer << endl;
        wcout << "Max component length: " << maxLength << endl;
        wcout << "Flags: " << flags << endl;
    }
    else
    {
        wcout << "Last error code: " << GetLastError() << endl;
    }*/
    
    string s = "D:\\";
    wstring stemp = wstring(s.begin(), s.end());
    LPCWSTR drive = stemp.c_str();
    LPWSTR volumeNameBuffer = new TCHAR[1000];
    DWORD maxLength = 100;
    BOOL success = GetVolumeNameForVolumeMountPoint(drive, volumeNameBuffer, maxLength);
    
    if (success)
    {
        int i;
        for (i = 0; volumeNameBuffer[i] != '\0'; i++);
            volumeNameBuffer[--i] = '\0';
        cout << endl << "GUID Path of drive: " << CW2A(volumeNameBuffer) << endl;
    }
    else
    {
        wcout << L"Error retreiving volume information!!!" << endl;
    }
    
    HRESULT hr = S_OK;
    HANDLE  hDisk = INVALID_HANDLE_VALUE;
    BOOL fResult = FALSE;
    hDisk = CreateFile(volumeNameBuffer,
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, 
        FILE_ATTRIBUTE_NORMAL, NULL);

    if (hDisk == INVALID_HANDLE_VALUE)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
        cout << "CreateFile failed on " << CW2A(volumeNameBuffer) << GetLastError();
        return 1;
    }

    cout << "\nSuccess opening file.";
    cout << "\nCalling DeviceIoControl( FSCTL_QUERY_FILE_SYSTEM_RECOGNITION ) ";
    
    FILE_SYSTEM_RECOGNITION_INFORMATION FsRi = { 0 };
    ULONG BytesReturned = 0;

    fResult = DeviceIoControl(hDisk,
        FSCTL_QUERY_FILE_SYSTEM_RECOGNITION,
        NULL,
        0,
        &FsRi,
        sizeof(FsRi),
        &BytesReturned,
        NULL);

    /*if (!fResult)
    {
        DWORD error = GetLastError();
        hr = HRESULT_FROM_WIN32(error);
        cout << "failed GLE = 0x" << HRESULT_FROM_WIN32( GetLastError());
        return 0;
    }*/
    cout << "succeeded.\n\n" ;
    cout << "FSCTL_QUERY_FILE_SYSTEM_RECOGNITION returned success.\n";
    //cout << "FSCTL_QUERY_FILE_SYSTEM_RECOGNITION retrieved \"%S\".\n" << FsRi.FileSystem;

    LPCVOID writeBuffer = "dkfs: don't know fucking shit";
    LPVOID readBuffer = new char[512];
    LPDWORD writtenBytes = 0, readBytes = 0;

    BOOL readResult = FALSE;
    readResult = ReadFile(
        hDisk,
        readBuffer,
        512,
        readBytes,
        NULL);

    if (readResult == FALSE)
    {
        cout << "Failure reading." << GetLastError();
        return 1;
    }
    else
    {
        cout << "Success reading." << (char *)readBuffer;
    }
    /*BOOL writeResult = WriteFile(
        hDisk,
        lpBuffer,
        512,
        writtenBytes,
        NULL);
*/


    if (hDisk != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hDisk);
        hDisk = INVALID_HANDLE_VALUE;
    }
    return 0;
}

