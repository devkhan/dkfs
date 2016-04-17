// dkfs.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "File.h"
#include "FSIO.h"

using namespace std;

int main(int argc, char **argv)
{
    wcout << "dkfs: dead stark file system. Enter h for help." << endl;

    string drive = "D:\\";
    FSIO fsio(drive);
    char command;
    string input;
    do
    {
        cout << ">> ";
        cin >> command;
        switch (command)
        {
            case 'h':                
                externalFile.open("help.txt", ios::in);
                while (!externalFile.eof())
                {
                    cout << static_cast<char>(externalFile.get());
                }
                break;

        case 'x':
            cout << "Namaste _/\\_";
            break;
            case 'k':
                cout << "Enter filename to print: ";
                cin >> input;
                file = fsio->Read(input);
                if (file == nullptr)
                {
                    cout << "File not found!";
                }
                else
                {
                    cout << "File: " << file->getFileName() << endl;
                    cout << "Size: " << file->getSize() << endl;
                    cout << "Created: " << file->getCreationTime() << endl;
                    cout << "Last accessed: " << file->getAccessionTime() << endl;
                    cout << "Last modified: " << file->getModificationTime() << endl;
                    cout << file->getData();
                }
                break;

        default:
            cout << "Unknown command. We told you its dead stark, so don't greedy.";
            break;
        }
        cout << endl;
    } while (command != 'x');

    return 0;
}

