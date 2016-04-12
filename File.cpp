#include "File.h"

File::File(string name = "", void* data = nullptr)
{
    fileName = name;
    size = 0;
    creationTime = time(0);
    accessionTime = creationTime;
    modificationTime = creationTime;
    if (data == nullptr)
    {

    }
}

File::File(void* data)
{
    size_t i = 0;
    byte *tempData = (byte*)data;
    char tempName[16];
    while(i < 16)
    {
        tempName[i] = (char)tempData[i];
        i++;
    }
    fileName = string(tempName);

    size = (tempData[i + 1] << 8) | (tempData[i]);
    i += 2;

    creationTime = (tempData[i + 3] << 24) | (tempData[i + 2] << 16) | (tempData[i + 1] << 8) | (tempData[i]);
    i += 4;

    modificationTime = (tempData[i + 3] << 24) | (tempData[i + 2] << 16) | (tempData[i + 1] << 8) | (tempData[i]);
    i += 4;

    accessionTime = (tempData[i + 3] << 24) | (tempData[i + 2] << 16) | (tempData[i + 1] << 8) | (tempData[i]);
    i += 4;

    data = &tempData[i];
}

bool File::setFileName(string name)
{
    fileName = name;
    setModificationTime(time(0));
    return true;
}

string File::getFileName()
{
    return fileName;
}

bool File::setModificationTime(LONG32 time)
{
    modificationTime = time;
    setAccessionTime(std::time(0));
    return true;
}

LONG32 File::getModificationTime()
{
    return modificationTime;
}

bool File::setAccessionTime(LONG32 time)
{
    accessionTime = time;
    return true;
}

LONG32 File::getAccessionTime()
{
    return accessionTime;
}

bool File::setData(void* _data)
{
    data = (byte*)_data;
}

void* File::getData()
{
    return (void*)data;
}

byte* File::getSerializedFile()
{
    byte *temp;
    temp = new byte[4096];

    size_t i = 0;
    for (i = 0; fileName.c_str()[i] != '\0'; i++)
    {
        temp[i] = fileName.c_str()[i];
    }

    for (; i < 16;i++)
    {
        temp[i] = '\0';
    }

    temp[i] = size;
    i += 2;

    temp[i] = creationTime;
    i += 4;
    temp[i] = modificationTime;
    i += 4;
    temp[i] = accessionTime;
    i += 4;

    while(i < 4096)
    {
        temp[i] = data[i];
    }

    return temp;
}
