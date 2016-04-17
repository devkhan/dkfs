#pragma once

#include "stdafx.h"

using namespace std;

class File
{
    string fileName;
    USHORT size;
    LONG32 creationTime;
    LONG32 accessionTime;
    LONG32 modificationTime;

    string data;
    
    public:

        File(void* _data);

        File(string name = "", string data = "");

        bool setFileName(string name);
        
        string getFileName();

        USHORT getSize();

        LONG32 getCreationTime();

        bool setModificationTime(LONG32 time);

        LONG32 getModificationTime();

        bool setAccessionTime(LONG32 time);

        LONG32 getAccessionTime();

        bool setData(string);

        string getData();

        byte* getSerializedFile();
};
