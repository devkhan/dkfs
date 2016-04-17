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
    FSIO *fsio;
    File *file;
    fsio = new FSIO(drive);
    fstream externalFile;
    char command;
    string input, input2;
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

            case 'd':
                cout << "Enter filename to delete: ";
                cin >> input;
                if (input.length() > 16 || input.length() < 0)
                {
                    cout << "Invalid filename!";
                    break;
                }
                if (fsio->Delete(input))
                {
                    cout << "success deleting '" << input << "'...";
                    break;
                }
                cout << "No such file exists.";
                break;


            case 'e':
                cout << "Enter file to edit: ";
                cin >> input;
                cout << "Enter data to put into file: ";
                cin >> input2;
                if (fsio->Modify(input, input2))
                {
                    cout << "Unable to modify file!";
                }
                else
                {
                    cout << "Success modified file " << input << ".";
                }
                break;

            case 'r':
                cout << "Enter file to rename: ";
                cin >> input;
                cout << "Enter new name: ";
                cin >> input2;
                if (fsio->Rename(input, input2))
                {
                    cout << "success renaming " << input << "-->" << input2;
                }
                else
                {
                    cout << "Unable to rename!";
                }
                break;

            case 'x':
                delete fsio;
                cout << "Namaste _/\\_";
                break;

            default:
                cout << "Unknown command. We told you its dead stark, so don't be greedy.";
                break;
        }
        cout << endl;
    } while (command != 'x');

    return 0;
}

