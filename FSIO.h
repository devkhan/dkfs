#include "stdafx.h"
#include "File.h"
using namespace std;

class FSIO
{
    char driveLetter;
    
    wstring drivePath;

    HANDLE diskHandle;

    DWORD lastError;

    bool eof;

    public:

        FSIO() : FSIO("D:\\"){}

        ~FSIO();

        FSIO(string drive);

        File* Read(string filename);

        File* ReadNext();

        bool Create(File file);

        bool Modify(string name, USHORT size, void* data);

        bool Delete(string filename);

        vector<File> List();

        bool Rename(string oldName, string newName);
};
