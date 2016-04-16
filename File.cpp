#include "stdafx.h"
#include "File.h"

File::File(string name, void* _data)
{
    fileName = name;
    data = (byte*)_data;
    size = string((char*)data).length();
    creationTime = time(0);
    accessionTime = creationTime;
    modificationTime = creationTime;
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

    size = tempData[i++];
    size = (size << 8)  + (tempData[i]);
    i++;

    creationTime = (tempData[i] << 24) | (tempData[i + 1] << 16) | (tempData[i + 2] << 8) | (tempData[i + 3]);
    i += 4;

    modificationTime = (tempData[i] << 24) | (tempData[i + 1] << 16) | (tempData[i + 2] << 8) | (tempData[i + 3]);
    i += 4;

    accessionTime = (tempData[i] << 24) | (tempData[i + 1] << 16) | (tempData[i + 2] << 8) | (tempData[i + 3]);
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

USHORT File::getSize()
{
    return size;
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
    return true;
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

    temp[i++] = ((byte*)&size)[1];
    temp[i++] = ((byte*)&size)[0];

    temp[i++] = ((byte*)&creationTime)[3];
    temp[i++] = ((byte*)&creationTime)[2];
    temp[i++] = ((byte*)&creationTime)[1];
    temp[i++] = ((byte*)&creationTime)[0];

    temp[i++] = ((byte*)&modificationTime)[3];
    temp[i++] = ((byte*)&modificationTime)[2];
    temp[i++] = ((byte*)&modificationTime)[1];
    temp[i++] = ((byte*)&modificationTime)[0];

    temp[i++] = ((byte*)&accessionTime)[3];
    temp[i++] = ((byte*)&accessionTime)[2];
    temp[i++] = ((byte*)&accessionTime)[1];
    temp[i++] = ((byte*)&accessionTime)[0];

    while(i < 4096)
    {
        temp[i] = data[i-30];
        i++;
    }

    return temp;
}
