#include<iostream>
#include<cstdlib>
#include<iomanip>
#include<cmath>
#include<string>
#include<array>
#include<algorithm>
#include<cctype>
#include<ctime>
#include<limits>
#include<vector>
#include<fstream>
#include<cstdio>
#include<stdexcept>
#include<filesystem>
#include<regex>
#include "ABODAstd.h"

using namespace std;
using namespace filesystem;

enum MainMenuChoices { EncryptFiles = 1, DecryptFilesManually = 2, DecryptFilesAutomatically = 3, Exit = 4 };

struct EncryptionData
{
	string FileName;
	short EncryptionKey;
	bool DeleteMark = false;
};

string ReadFileName(string Message)
{
	string FileName = ReadStringLine(Message);

	while (!filesystem::exists(FileName))
	{
		FileName = ReadStringLine("FILE IS NOT EXIST , " + Message);
	}

	return FileName;
}

MainMenuChoices ReadMainMenuChoice()
{
	return (MainMenuChoices)(ValidateNumberInRange(1, 4, "CHOOSE WHAT DO YOU WANT TO DO ? [1 TO 4]"));
}

void PrintMainMenuScreen()
{
	cout << "====================================================================\n\n";
	cout << Tab(3) << "CIPHER-F MAIN MENU SCREEN\n\n";
	cout << "====================================================================\n";
	cout << Tab(3) << "[1]  ENCRYPT FILE." << "\n";
	cout << Tab(3) << "[2]  DECRYPT FILE MANUALLY." << "\n";
	cout << Tab(3) << "[3]  DECRYPT FILE AUTOMATICALLY." << "\n";
	cout << Tab(3) << "[4]  EXIT SYSTEM." << "\n";
	cout << "====================================================================\n\n";
}

void AddNewDataLineToFile(string FileName, string DataLine)
{
	fstream File;
	File.open(FileName, ios::out | ios::app);

	if (File.is_open())
	{
		File << DataLine << endl;

		File.close();
	}
}

string EncryptString(string& String, short EncryptionKey);

string DecryptString(string& String, short DecryptionKey);

string GetEncryptionDataRecord(string FileName, short EncryptionKey, string Separator = "#//#")
{
	string EncryptionDataRecord = "";

	string EncryptionKeyString = to_string(EncryptionKey);

	EncryptionDataRecord += EncryptString(FileName, 12) + Separator;
	EncryptionDataRecord += EncryptString(EncryptionKeyString, 12);

	return EncryptionDataRecord;

}

EncryptionData GetEncryptionDataStructure(string EncryptionDataRecord, string Separator = "#//#")
{
	vector<string>EncryptionDataVector = SplitStringWordsToVector(EncryptionDataRecord, Separator);

	EncryptionData EncryptionDataStructure;

	EncryptionDataStructure.FileName = DecryptString(EncryptionDataVector[0], 12);
	EncryptionDataStructure.EncryptionKey = stoi(DecryptString(EncryptionDataVector[1], 12));

	return EncryptionDataStructure;
}

vector<EncryptionData> LoadEncryptionDataFromFileToVector()
{
	vector<EncryptionData> EncryptionDataVector;

	fstream File;
	File.open("ENCRYPTION_DATA.txt", ios::in);

	if (File.is_open())
	{
		string EncryptionDataRecord;

		while (getline(File, EncryptionDataRecord))
		{
			EncryptionDataVector.push_back(GetEncryptionDataStructure(EncryptionDataRecord));
		}

		File.close();
	}

	return EncryptionDataVector;
}

void SaveEncryptionDataVectorToFile(vector<EncryptionData>& EncryptionDataVector)
{
	fstream File;
	File.open("ENCRYPTION_DATA.txt", ios::out);

	if (File.is_open())
	{
		for (EncryptionData& EncryptionDataLine : EncryptionDataVector)
		{
			if (EncryptionDataLine.DeleteMark == false)
			{
				File << GetEncryptionDataRecord(EncryptionDataLine.FileName, EncryptionDataLine.EncryptionKey) << endl;
			}
		}

		File.close();
	}
}

void SaveEncryptionDataToFile(string EncryptedFileName, short EncryptionKey)
{
	AddNewDataLineToFile("ENCRYPTION_DATA.txt", GetEncryptionDataRecord(EncryptedFileName, EncryptionKey));
}

void DeleteEncryptionDataInFile(string FileName)
{
	vector<EncryptionData> EncryptionDataVector = LoadEncryptionDataFromFileToVector();

	for (EncryptionData& EncryptionDataLine : EncryptionDataVector)
	{
		if (FileName == EncryptionDataLine.FileName)
		{
			EncryptionDataLine.DeleteMark = true;
			break;
		}
	}

	SaveEncryptionDataVectorToFile(EncryptionDataVector);
}

bool IsFileEncrypted(string EncryptedFileName)
{
	vector<EncryptionData> EncryptionDataVector = LoadEncryptionDataFromFileToVector();

	for (EncryptionData& EncryptionDataLine : EncryptionDataVector)
	{
		if (EncryptedFileName == EncryptionDataLine.FileName)
		{
			return true;
		}
	}

	return false;
}

short GetFileEncryptionKey(string EncryptedFileName)
{
	vector<EncryptionData> EncryptionDataVector = LoadEncryptionDataFromFileToVector();

	for (EncryptionData& EncryptionDataLine : EncryptionDataVector)
	{
		if (EncryptedFileName == EncryptionDataLine.FileName)
		{
			return EncryptionDataLine.EncryptionKey;
		}
	}

	return 0;
}

short ReadFileDecryptionKey(string FileName)
{
	short DecryptionKey = ReadIntegerPositiveNumber("ENTER DECRYPTION KEY");

	if (IsFileEncrypted(FileName))
	{
		short FileEncryptionKey = GetFileEncryptionKey(FileName);

		while (DecryptionKey != FileEncryptionKey)
		{
			DecryptionKey = ReadIntegerPositiveNumber("INVALID DECRYPTION KEY , ENTER ANOTHER DECRYPTION KEY");
		}
	}

	return DecryptionKey;
}

vector<string> LoadFileDataFromFileToVector(string FileName)
{
	vector<string> FileData;

	fstream File;
	File.open(FileName, ios::in);

	if (File.is_open())
	{
		string DataLine;

		while (getline(File, DataLine))
		{
			FileData.push_back(DataLine);
		}

		File.close();
	}

	return FileData;
}

void SaveFileDataFromVectorToFile(string FileName, vector<string>& FileDataVector)
{
	fstream File;
	File.open(FileName, ios::out);

	if (File.is_open())
	{
		for (string& DataLine : FileDataVector)
		{
			File << DataLine << "\n";
		}

		File.close();
	}
}

string EncryptString(string& String, short EncryptionKey)
{
	for (int i = 0; i < String.length(); i++)
	{
		String[i] += EncryptionKey;
	}

	return String;
}

string DecryptString(string& String, short DecryptionKey)
{
	for (int i = 0; i < String.length(); i++)
	{
		String[i] -= DecryptionKey;
	}

	return String;
}

void EncryptFile(string FileName, short EncryptionKey)
{
	vector<string> FileDataVector = LoadFileDataFromFileToVector(FileName);

	for (string& DataLine : FileDataVector)
	{
		EncryptString(DataLine, EncryptionKey);
	}

	SaveFileDataFromVectorToFile(FileName, FileDataVector);
}

void DecryptFile(string FileName, short DecryptionKey)
{
	vector<string>FileDataVector = LoadFileDataFromFileToVector(FileName);

	for (string& DataLine : FileDataVector)
	{
		DecryptString(DataLine, DecryptionKey);
	}

	SaveFileDataFromVectorToFile(FileName, FileDataVector);
}

void EncryptFileScreen()
{
	SectionBanner("ENCRYPT FILE SCREEN");

	string FileName = ReadFileName("ENTER FILE NAME YOU WANT TO ENCRYPT\n(WRITE IT WITH ITS EXTENSION)\nAS example.txt");

	short EncryptionKey = ReadIntegerPositiveNumber("ENTER ENCRYPTION KEY");

	EncryptFile(FileName, EncryptionKey);

	SaveEncryptionDataToFile(FileName, EncryptionKey);

	printf("\nFILE IS ENCRYPTED SUCCESSFULLY :-)\n");
}

void DecryptFileAutomaticallyScreen()
{
	SectionBanner("DECRYPT FILE AUTOMATICALLY SCREEN");

	string FileName = ReadFileName("ENTER FILE NAME YOU WANT TO DECRYPT\n(WRITE IT WITH ITS EXTENSION)\nAS example.txt");

	if (IsFileEncrypted(FileName))
	{
		short DecryptionKey = GetFileEncryptionKey(FileName);

		DecryptFile(FileName, DecryptionKey);

		DeleteEncryptionDataInFile(FileName);

		printf("\nFILE IS DECRYPTED SUCCESSFULLY :-)\n");
	}

	else
	{
		printf("\nFILE IS ALREADY NOT ENCRYPTED :-|\n");
	}
}

void DecryptFileManuallyScreen()
{
	SectionBanner("DECRYPT FILE MANUALLY SCREEN");

	string FileName = ReadFileName("ENTER FILE NAME YOU WANT TO DECRYPT\n(WRITE IT WITH ITS EXTENSION)\nAS example.txt");

	short DecryptionKey = ReadFileDecryptionKey(FileName);

	DecryptFile(FileName, DecryptionKey);

	if (IsFileEncrypted(FileName))
	{
		DeleteEncryptionDataInFile(FileName);
	}

	printf("\nFILE IS DECRYPTED SUCCESSFULLY :-)\n");
}

void CIPHER_F()
{
	StartProgram("WELCOME TO CIPHER-F");

	while (true)
	{
		PrintMainMenuScreen();
		MainMenuChoices MainMenuChoice = ReadMainMenuChoice();

		switch (MainMenuChoice)
		{
		case EncryptFiles:
			ClearScreen();
			EncryptFileScreen();
			GoToPauseMessage("\nPRESS ANY KEY TO GO BACK TO MAIN MENU SCREEN...");
			break;
		case DecryptFilesManually:
			ClearScreen();
			DecryptFileManuallyScreen();
			GoToPauseMessage("\nPRESS ANY KEY TO GO BACK TO MAIN MENU SCREEN...");
			break;
		case DecryptFilesAutomatically:
			ClearScreen();
			DecryptFileAutomaticallyScreen();
			GoToPauseMessage("\nPRESS ANY KEY TO GO BACK TO MAIN MENU SCREEN...");
			break;
		case Exit:
			ClearScreen();
			ExitProgram("GOOD BYE , CIPHER-F :-)");
			break;
		}
	}
}

int main()
{
	srand((unsigned)time(NULL));

	CIPHER_F();

	return 0;
}