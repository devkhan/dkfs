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

    byte *data;
    
    public:

        File(void* _data);

        File(string name = "", void* _data = nullptr);

        bool setFileName(string name);
        
        string getFileName();

        bool setModificationTime(LONG32 time);

        LONG32 getModificationTime();

        bool setAccessionTime(LONG32 time);

        LONG32 getAccessionTime();

        bool setData(void * data);

        void* getData();

        byte* getSerializedFile();
};
