#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int KEY_SIZE = 64;
const int PERM_SIZE = 56;
const int PERM_SIZE2 = 48;
const int KEYS = 16;

const string fileName = "keyFile.txt";
const string messageName = "message.txt";
char key[KEY_SIZE];
char perm1[PERM_SIZE] = {57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,44,36,63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4};
char perm2[PERM_SIZE2] = {14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32};
char IP[KEY_SIZE] = {58,50,42,34,26,18,10,42,
					 60,52,44,36,28,20,12,4,
					 62,54,46,38,30,22,14,6,
					 64,56,48,40,32,20,16,8,
					 57,49,41,33,25,17,9,1,
					 59,51,43,35,27,19,11,3,
					 61,53,45,37,29,21,13,5,
					 63,55,47,39,31,23,15,7};
char messageExpansion[PERM_SIZE2] = {32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,12,13,14,15,16,17,16,17,18,19,20,21,20,21,22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1};
char message[KEY_SIZE];
char messageP1[KEY_SIZE];
char permute1[PERM_SIZE];
char keys[KEYS][PERM_SIZE];
char keysPerm[KEYS][PERM_SIZE2];
char messagePermL[KEYS][KEY_SIZE/2];
char messagePermR[KEYS][KEY_SIZE/2];

char S[8][4][16] = {{{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
					 {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
					 {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
					 {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
					{{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
					 {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
					 {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
					 {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}}};

char shifts[KEYS] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

void permute(char *output, char *input, char *table, int table_size)
{
	for (int i = 0; i < table_size; i++)
	{
		output[i] = input[table[i]-1];
	}
}

void leftShift(char *output, char *input, int input_size)
{
	char store1 = input[0];
	char store2 = input[28];
	//output[27] = input[0];
	//output[input_size-1] = input[28];

	for (int i = 0; i < input_size - 1; i++)
	{
		if (i == 27) continue;
		output[i] = input[i+1];		
	}
	output[27] = store1;
	output[input_size-1] = store2;
}

void copyArray(char *output, char *input, int input_size)
{
	for (int i = 0; i < input_size; i ++)
	{
		output[i] = input[i];
	}
}

void function(char *output, char *right, char *left, char *key, int size)
{
	char perm[PERM_SIZE2];
	permute(perm,right,messageExpansion,PERM_SIZE2);
	for (int i=0; i < PERM_SIZE2; i++)
	{
		perm[i] = perm[i] ^ key[i];
	}

}

int main()
{
	fstream fileStream(fileName, fstream::in);
	int index = 0;
	while(fileStream >> noskipws >> key[index]) { index++;}
	fileStream.close();

	fstream messageStream(messageName, fstream::in);
	index = 0;
	char input[KEY_SIZE];
	while(messageStream >> noskipws >> input[index]) { index++; if (index == KEY_SIZE) break;}
	fileStream.close();

	for (int i = 0; i < KEY_SIZE; i+=8)
	{
		for (int j = 0; j < 8; j++)
		{
			char temp = input[(int)i/8] >> (7-j);
			message[i+j] = temp & 1;
		}
	}
	
	cout << "Read key:" << endl;
	for (int i = 0; i < KEY_SIZE; i++)
	{
		cout << key[i];
	}
	cout << endl << endl;

	cout << "Permute once:" << endl;
	permute(permute1,key,perm1,PERM_SIZE);
	for (int i = 0; i < PERM_SIZE; i++)
	{
		cout << permute1[i];
	}
	cout << endl << endl;

	leftShift(keys[0], permute1, PERM_SIZE);	
	for (int i = 1; i < KEYS; i++)
	{
		if (shifts[i] == 2)
		{
			char temp[PERM_SIZE];
			leftShift(temp, keys[i-1], PERM_SIZE);
			leftShift(keys[i], temp, PERM_SIZE);
		}
		else 
			leftShift(keys[i], keys[i-1], PERM_SIZE);

	}

	cout << "KEYS :" << endl;
	for (int i = 0; i < KEYS; i++){
		for (int j = 0; j < PERM_SIZE; j++){
			cout << keys[i][j];
		}
		cout << endl;
	}

	cout << endl;

	for (int i = 0; i < KEYS; i++)
	{
		permute(keysPerm[i], keys[i], perm2, PERM_SIZE2);
	}

	cout << "Permuted KEYS :" << endl;
	for (int i = 0; i < KEYS; i++){
		for (int j = 0; j < PERM_SIZE2; j++){
			cout << keysPerm[i][j];
		}
		cout << endl;
	}

	cout << endl;

	cout << "Read message:" << endl;
	for (int i = 0; i < KEY_SIZE; i++)
	{
		cout << input[i];
	}
	cout << endl << endl;

	cout << "binary message:" << endl;
	for (int i = 0; i < KEY_SIZE; i++)
	{
		cout << (unsigned int)message[i];
	}
	cout << endl << endl;

	permute(messageP1,message,IP,KEY_SIZE);

	cout << "IP message:" << endl;
	for (int i = 0; i < KEY_SIZE; i++)
	{
		cout << (unsigned int)messageP1[i];
	}
	cout << endl << endl;

	char left[KEY_SIZE/2];
	char right[KEY_SIZE/2];
	for (int i = 0; i < KEY_SIZE; i++)
	{
		if (i>=KEY_SIZE/2)
			left[i-KEY_SIZE/2] = messageP1[i];
		else
			right[i] = messageP1[i];
	}

	copyArray(messagePermL[0],right,KEY_SIZE/2);
	for (int i = 1; i < KEYS; i++)
	{
		for (int j = 0; j < KEY_SIZE; j++)
		{

		}
	}


	system("Pause");
	return 0;
}

