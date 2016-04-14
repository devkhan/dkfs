#include "stdafx.h"
#include "File.h"
using namespace std;

class FSIO
{
    char driveLetter;
    
    string drivePath;

    HANDLE diskHandle;

    DWORD lastError;

    public:

        ~FSIO();

        FSIO(string drivePath);

        File ReadFile(string filename);

        bool CreateFile(File file);

        bool ModifyFile(string name, USHORT size, void* data);

        bool DeleteFile(string filename);

        vector<File> ListFiles();

        bool RenameFile(string oldName, string newName);
};
