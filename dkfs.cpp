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

        default:
            cout << "Unknown command. We told you its dead stark, so don't greedy.";
            break;
        }
        cout << endl;
    } while (command != 'x');

    return 0;
}

