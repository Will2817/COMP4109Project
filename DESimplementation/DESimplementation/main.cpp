#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>


using namespace std;

const int KEY_SIZE = 64;
const int PERM_SIZE = 56;
const int PERM_SIZE2 = 48;
const int KEYS = 16;

const string fileName = "keyFile.txt";
const string messageName = "binMessage.txt";

//char key[KEY_SIZE] = {0,1,1,1,1,0,0,0,0,0,1,0,1,1,1,0,1,0,0,0,1,1,0,1,0,1,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,1,0,1,1,1,0,1,0,0,0,1,1,0,1,0,1,0,1,1,0,0,1};
char key[KEY_SIZE] = {0,0,0,1,0,0,1,1,0,0,1,1,0,1,0,0,0,1,0,1,0,1,1,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,1,1,1,0,1,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,1,1,0,0,0,1};

char perm1[PERM_SIZE] = {57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,44,36,63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4};
char perm2[PERM_SIZE2] = {14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32};
char IP[KEY_SIZE] = {58,50,42,34,26,18,10,2,
					 60,52,44,36,28,20,12,4,
					 62,54,46,38,30,22,14,6,
					 64,56,48,40,32,24,16,8,
					 57,49,41,33,25,17,9,1,
					 59,51,43,35,27,19,11,3,
					 61,53,45,37,29,21,13,5,
					 63,55,47,39,31,23,15,7};

char IPI[KEY_SIZE] = {40,8,48,16,56,24,64,32,
					  39,7,47,15,55,23,63,31,
					  38,6,46,14,54,22,62,30,
					  37,5,45,13,53,21,61,29,
					  36,4,44,12,52,20,60,28,
					  35,3,43,11,51,19,59,27,
					  34,2,42,10,50,18,58,26,
					  33,1,41, 9,49,17,57,25};

char messageExpansion[PERM_SIZE2] = {32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,12,13,14,15,16,17,16,17,18,19,20,21,20,21,22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1};
char message[KEY_SIZE] = {0,1,0,0,1,0,0,0,0,1,1,0,0,1,0,1,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,1,1,0,0,1,0,0,0,0,0,0,1,1,1,0,1,1,1,0,1,1,0,1,1,1,1};
//char message[KEY_SIZE] = {0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,1,0,1,0,0,0,1,0,1,0,1,1,0,0,1,1,1,1,0,0,0,1,0,0,1,1,0,1,0,1,0,1,1,1,1,0,0,1,1,0,1,1,1,1,0,1,1,1,1};
//char ciphertext[KEY_SIZE] = {1,0,0,0,0,1,0,1,1,1,1,0,1,0,0,0,0,0,0,1,0,0,1,1,0,1,0,1,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,0,1,0,1,0,1,1,0,1,0,0,0,0,0,0,0,1,0,1};
char ciphertext[KEY_SIZE] = {1,1,1,1,1,0,1,1,1,1,1,0,1,1,0,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,0,1,0,0,1,1,0,0,0,1,0,1,0,1,1,1,1,1,1,0,0,1,1,0,1,0,1,1,0,0,1,0,0,1};

char messageP1[KEY_SIZE];
char permute1[PERM_SIZE];
char keys[KEYS][PERM_SIZE];
char keysPerm[KEYS][PERM_SIZE2];
char messagePermL[KEYS][KEY_SIZE/2];
char messagePermR[KEYS][KEY_SIZE/2];

union {
	long long x;
	unsigned char a[8];
} q;

char S[8][4][16] = {{{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
					 {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
					 {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
					 {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
					{{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
					 {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
					 {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
					 {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},
					{{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
					 {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
					 {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
					 {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},
					{{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
					 {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
					 {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
					 {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},
					{{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
					 {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
					 {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
					 {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
					{{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
					 {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
					 {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
					 {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},
					{{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
					 {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
					 {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
					 {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},
					{{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
					 {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
					 {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
					 {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}
					};

char P[] = {16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25};

char shifts[KEYS] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

/******* 
	get bit at nth position
************/
unsigned char getBit(unsigned char c, int n){
	return (c&(1<<n))>>n;
}

/*******
	set bit at nth position to 1
*********/
unsigned char setBit(unsigned char c, int n){
	return c|(1<<n);
}

/********
	set bit at nth position to 0
************/
unsigned char clearBit(unsigned char c, int n){
	return c&(~(1<<n));
}


void printAsBinary(char *input, int size)
{
	for (int i = 0; i < size; i++)
	{
		cout << (int) input[i];
	}
	cout << endl;
}

void printAsChar(char *input, int size)
{
	for (int i = 0; i < size; i+=8)
	{
		char letter = 0;
		for (int j=0; j<8; j++)
		{
			if (input[i+j])
				letter = setBit(letter,(7-j));
		}
		cout << letter;
	}
	cout << endl;
}

string convertToString(char *input, int size)
{
	string result;
	for (int i = 0; i < size; i+=8)
	{
		char letter = 0;
		for (int j=0; j<8; j++)
		{
			if (input[i+j])
				letter = setBit(letter,(7-j));
		}
		result += letter;
	}
	return result;
}

void convertToChar(char* output, char *input)
{
	for (int i=0; i<64; i+=8)
	{
		for (int j=0; j <8; j++)
		{
			output[i+j] = getBit(input[i/8],7-j);
		}
	}
}

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

void function(char *output, char *right, char *key, int size)
{
	char expand[PERM_SIZE2];
	char f[PERM_SIZE2];
	char lookUp;
	int row=0,col=0;
	permute(expand,right,messageExpansion,PERM_SIZE2);
	for (int i=0; i < PERM_SIZE2; i++)
	{
		expand[i] = expand[i] ^ key[i];
	}
	for (int i=0; i<PERM_SIZE2; i+=6)
	{
		row = expand[i]*2 + expand[i+5];
		col = expand[i+1]*8+expand[i+2]*4+expand[i+3]*2+expand[i+4];
		lookUp = S[i/6][row][col];
		for (int j = 0; j < 4; j++)
		{
			f[i/6*4+j] = (lookUp >> (3-j)) & 1;
		}
	}
	permute(output,f,P,32);
}

void xorWith(char *output, char *input, int size)
{
	for (int i=0; i<size; i++)
	{
		output[i] = output[i] ^ input[i];
	}
}

void encrypt(char *message1, char *key1, char *output)
{
	//cout << "Read key:" << endl;
	//printAsBinary(key1, KEY_SIZE);

	//cout << endl << endl;
	
	//cout << "Permute once:" << endl;
	permute(permute1,key1,perm1,PERM_SIZE);
/*	for (int i = 0; i < PERM_SIZE; i++)
	{
		cout << (int)permute1[i];
	}
	cout << endl << endl;*/

	
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
	/*
	cout << "KEYS :" << endl;
	for (int i = 0; i < KEYS; i++){
		for (int j = 0; j < PERM_SIZE; j++){
			cout << keys[i][j];
		}
		cout << endl;
	}

	cout << endl;*/
	
	for (int i = 0; i < KEYS; i++)
	{
		permute(keysPerm[i], keys[i], perm2, PERM_SIZE2);
	}

/*	cout << "Permuted KEYS :" << endl;
	for (int i = 0; i < KEYS; i++){
		for (int j = 0; j < PERM_SIZE2; j++){
			cout << keysPerm[i][j];
		}
		cout << endl;
	}

	cout << endl;*/

	
	//printAsBinary(message1,KEY_SIZE);
	//cout << endl << endl;

	permute(messageP1,message1,IP,KEY_SIZE);

	/*
	cout << "IP message:" << endl;
	for (int i = 0; i < KEY_SIZE; i++)
	{
		cout << (int)messageP1[i];
	}
	cout << endl << endl;*/
	
	char left[KEY_SIZE/2];
	char right[KEY_SIZE/2];
	for (int i = 0; i < KEY_SIZE; i++)
	{
		if (i>=KEY_SIZE/2)
			right[i-KEY_SIZE/2] = messageP1[i];
		else
			left[i] = messageP1[i];
	}

	copyArray(messagePermL[0],right,KEY_SIZE/2);
	function(messagePermR[0],right,keysPerm[0],PERM_SIZE2);
//	cout << "after P" << endl;
//	printAsBinary(messagePermR[0],KEY_SIZE/2);
	
	xorWith(messagePermR[0],left, KEY_SIZE/2);
//	printAsBinary(messagePermR[0],KEY_SIZE/2);

	cout <<endl<<endl; 
	
	for (int i = 1; i < KEYS; i++)
	{
		copyArray(messagePermL[i],messagePermR[i-1],KEY_SIZE/2);
		function(messagePermR[i],messagePermR[i-1],keysPerm[i],PERM_SIZE2);
		xorWith(messagePermR[i],messagePermL[i-1], KEY_SIZE/2);
	}

	/*
	for (int i = 0; i < KEYS; i++)
	{
		cout << "Position " << i << endl;
		printAsBinary(messagePermL[i],KEY_SIZE/2);
		printAsBinary(messagePermR[i],KEY_SIZE/2);
	}*/




	char temp[KEY_SIZE];
	char result[KEY_SIZE];

	for (int i=0; i < KEY_SIZE; i++)
	{
		if (i >= KEY_SIZE/2)
		{
			temp[i] = messagePermL[15][i-KEY_SIZE/2];
		}
		else
		{
			temp[i] = messagePermR[15][i];
		}
	}
	
	//cout<< "R16 + L16" <<endl;
	//printAsBinary(temp,KEY_SIZE);


	permute(output, temp, IPI, KEY_SIZE);
	//cout << "cipertext:" << endl;
	//printAsBinary(output,KEY_SIZE);
}


void decrypt(char *ciphertext1, char *key1, char *output)
{
	permute(permute1,key1,perm1,PERM_SIZE);
	
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
	
	for (int i = 0; i < KEYS; i++)
	{
		permute(keysPerm[i], keys[i], perm2, PERM_SIZE2);
	}
	
	permute(messageP1,ciphertext1,IP,KEY_SIZE);

	
	char left[KEY_SIZE/2];
	char right[KEY_SIZE/2];
	for (int i = 0; i < KEY_SIZE; i++)
	{
		if (i>=KEY_SIZE/2)
			right[i-KEY_SIZE/2] = messageP1[i];
		else
			left[i] = messageP1[i];
	}

	copyArray(messagePermL[0],right,KEY_SIZE/2);
	function(messagePermR[0],right,keysPerm[15],PERM_SIZE2);
	xorWith(messagePermR[0],left, KEY_SIZE/2);
	
	for (int i = 1; i < KEYS; i++)
	{
		copyArray(messagePermL[i],messagePermR[i-1],KEY_SIZE/2);
		function(messagePermR[i],messagePermR[i-1],keysPerm[15-i],PERM_SIZE2);
		xorWith(messagePermR[i],messagePermL[i-1], KEY_SIZE/2);
	}

	char temp[KEY_SIZE];
	char result[KEY_SIZE];

	for (int i=0; i < KEY_SIZE; i++)
	{
		if (i >= KEY_SIZE/2)
		{
			temp[i] = messagePermL[15][i-KEY_SIZE/2];
		}
		else
		{
			temp[i] = messagePermR[15][i];
		}
	}

	permute(output, temp, IPI, KEY_SIZE);
}

void convertToBinary(char *output, char * input)
{
	for (int i=0; i < 8; i++)
	{
		for (int j=0; j < 8; j++)
		{
			output[i] = (input[i] >> (7-j) & 1);
		}
	}
}

void encryption()
{
	char messageRead[8];
	char message[64];
	char output[64];
	int index = 0;
	string file;
	fstream messageStream;

	while(true)
	{
		cout << "Input filename of message to be encrypted or 0 to quit" << endl;
		cin >> file;
		if (file == "0") return;
		messageStream.open(file);
		if (!messageStream.fail()) break;
		cout << "Could not open file" << endl;
	}

	
	fstream outStream("ct.txt", fstream::out);
	index = 0;
	while(true) 
	{ 
		if (messageStream.eof())
		{
			convertToChar(message,messageRead);
			encrypt(message,key,output);
			outStream << convertToString(output,64);
			break;
		}
		messageRead[index] = messageStream.get();
		index++;
		if (index == 8)
		{
			convertToChar(message,messageRead);
			encrypt(message,key,output);
			outStream << convertToString(output,64);
			index = 0;
			memset( messageRead, 0, sizeof(char) * 8);
		}
	}
	messageStream.close();	
	outStream.close();
	cout << "file has been encrypted, cipthertext has been saved to ct.txt" << endl;
}

void decryption()
{
	char messageRead[8];
	char message[64];
	char output[64];
	int index = 0;
	string file;
	fstream cipherStream;

	while(true)
	{
		cout << "Input filename of message to be decrypted or 0 to quit" << endl;
		cin >> file;
		if (file == "0") return;
		cipherStream.open(file);
		if (!cipherStream.fail()) break;
		cout << "Could not open file" << endl;
	}

	cout << "Message is:" << endl;
	while (!cipherStream.eof())
	{
		messageRead[index] = cipherStream.get();
		index++;
		if (index == 8)
		{
			convertToChar(message,messageRead);
			decrypt(message,key,output);
			cout << convertToString(output,64);
			index = 0;
			memset( messageRead, 0, sizeof(char) * 8);
		}
	}
	cipherStream.close();
	cout << endl;
}

int main()
{
	int input;
	while(true)
	{
		cout << endl << "Menu" << endl << "0. Exit" << endl << "1.Encrypt" << endl << "2.Decrypt" << endl;
		cin >> input;
		if (input == 1)
		{
			encryption();
		}
		else if (input == 2)
		{
			decryption();
		}
		else
			break;
	}

	/*


	char messageRead[8];
	char message[64];
	char output[64];


	//fstream fileStream(fileName, fstream::in);
	int index = 0;
	
	fstream messageStream("message.txt", fstream::in);
	fstream outStream("ct.txt", fstream::out);
	index = 0;
	while(true) 
	{ 
		if (messageStream.eof())
		{
			convertToChar(message,messageRead);
			encrypt(message,key,output);
			outStream << convertToString(output,64);
			break;
		}
		messageRead[index] = messageStream.get();
		index++;
		if (index == 8)
		{
			convertToChar(message,messageRead);
			encrypt(message,key,output);
			outStream << convertToString(output,64);
			index = 0;
			memset( messageRead, 0, sizeof(char) * 8);
		}
	}
	messageStream.close();	
	outStream.close();

	fstream cipherStream("ct.txt", fstream::in);
	index = 0;
	cout << "Message is:" << endl;
	while (!cipherStream.eof())
	{
		messageRead[index] = cipherStream.get();
		index++;
		if (index == 8)
		{
			convertToChar(message,messageRead);
			decrypt(message,key,output);
			cout << convertToString(output,64);
			index = 0;
			memset( messageRead, 0, sizeof(char) * 8);
		}
	}	
	cipherStream.close();
	cout << endl;
	*/
	system("Pause");
	return 0;
}

