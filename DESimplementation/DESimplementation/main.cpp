#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int KEY_SIZE = 64;
const int PERM_SIZE = 56;
const int PERM_SIZE2 = 48;
const int KEYS = 16;

const string fileName = "keyFile.bin";
char key[KEY_SIZE];
char perm1[PERM_SIZE] = {57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,44,36,63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4};
char perm2[PERM_SIZE2] = {14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32};
char permute1[PERM_SIZE];
char keys[KEYS][PERM_SIZE];
char keysPerm[KEYS][PERM_SIZE2];
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

int main()
{
	fstream fileStream(fileName, fstream::in);
	int index = 0;
	while(fileStream >> noskipws >> key[index]) { index++;}

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

	system("Pause");
	return 0;
}

