// dkfs.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "File.h"
#include "FSIO.h"

using namespace std;

string getFormattedTime(time_t);

int main(int argc, char **argv)
{
    wcout << "dkfs: deaD starK File System. Enter h for help." << endl;
    wcout << "Sector size on disk: 512 B, dkfs block size: 4096 B." << endl;

    // Declare resources.
    string drive = "D:\\";
    FSIO *fsio;
    File *file;
    fsio = new FSIO(drive);
    fstream externalFile;
    vector<File> files;
    vector<File>::iterator iterator;
    char command;
    string input, input2;

    // Interactive mode.
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
                externalFile.close();
                break;

            case 's':
                cout << "Enter drive: ";
                cin >> input;
                drive = input;
                fsio = new FSIO(drive);
                break;

            case 't':
                cout << "Setting drive to default i.e. D:\\ ...";
                drive = "D:\\";
                fsio = new FSIO(drive);
                break;

            case 'c':
                cout << "Enter filename for new file: ";
                cin >> input;
                if (input.length() > 16 || input.length() < 0)
                {
                    cout << "Invalid filename!";
                    break;
                }
                file = new File(input);
                cout << "Enter data to be put into file: ";
                getline(cin, input);
                getline(cin, input);
                if (input.length() > 4066)
                {
                    cout << "File larger than supported size! Will be truncated.";
                }
                file->setData(input);
                if (fsio->Create(*file))
                {
                    cout << file->getFileName() << " written successfully.";
                }
                else
                {
                    cout << "Unable to write file!";
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
                    cout << "Created: " << getFormattedTime(file->getCreationTime()) << endl;
                    cout << "Last accessed: " << getFormattedTime(file->getAccessionTime()) << endl;
                    cout << "Last modified: " << getFormattedTime(file->getModificationTime()) << endl;
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

            case 'l':
                cout << "Files in " << drive << endl << endl;
                cout << setfill(' ') << setw(16) << "Filename" 
                     << setfill(' ') << setw(16) << "Size"
                     << setfill(' ') << setw(28) << "Created at" << endl << endl;
                files = fsio->List();
                for (iterator = files.begin(); iterator < files.end(); ++iterator)
                {
                    cout << setfill(' ') << setw(16) << iterator->getFileName();
                    cout << setfill(' ') << setw(14) << iterator->getSize() << " B";
                    cout << setfill(' ') << setw(28) << getFormattedTime(iterator->getCreationTime()) << endl;
                }
                break;

            case 'e':
                cout << "Enter file to edit: ";
                cin >> input;
                cout << "Enter data to put into file: ";
                getline(cin, input2);
                getline(cin, input2);
                if (fsio->Modify(input, input2))
                {
                    cout << "Success modified file " << input << ".";
                }
                else
                {
                    cout << "Unable to modify file!";
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

string getFormattedTime(time_t t)
{
    struct tm *tm = localtime(&t);
    char date[20];
    strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm);
    return string(date);
}

