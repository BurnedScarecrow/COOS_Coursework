#include <iostream>
#include <conio.h>
#include <iomanip>
#include <string>
#include <Windows.h>
using namespace std;

// Want more counting systems?
// Just add letters ("0123456789abcdefghigklmnopqrstuvwxyzABCDEFJHIJKLMNOPQRSTUVWXYZ" 62 system)
string alf = "0123456789abcdef";

// Returns index of the element in alf, if wasn't find returns -1; 
int indexOf(char c) {
	for (int i = 0; i < alf.size(); i++)
		if (c == alf[i]) return i;
	return -1;
}

// Returns decimal value in T* res (in type, that was given when function was called) 
template <typename T> // T var - variable that can be in any type
int getDecimal(string number, int sys, char type, T* res)
{
	long double  ideal = 0; // ideal decimal'll be here to compare for oversize
	int minus = 0; // trigger for minus
	int dot = number.size(); // position of point (initially in the end if number doesn't have a point)
	*res = 0;

	if (number[0] == '-') minus = 1;
	if (type == 's' || type == 'i' || type == 'l' || type == 'c') {
		for (int i = minus; i < number.size(); i++) {
			if (minus) *res += indexOf(number[number.size() - i]) * pow(sys, i - minus);	// [-123] & i = 1 : number[4-1] * pow(sys, 1-1)
			else *res += indexOf(number[number.size() - i - 1]) * pow(sys, i - minus);	// [123] & i = 0 : number[3-0-1] (size =2)
			if (minus) ideal += indexOf(number[number.size() - i]) * pow(sys, i - minus);	// smae for ideal
			else ideal += indexOf(number[number.size() - i - 1]) * pow(sys, i - minus);	// smae for ideal
		}
	}
	if (type == 'f' || type == 'd') {
		for (int i = 0; i < number.size(); i++)
			if (number[i] == '.') dot = i;
		for (int i = minus; i < dot; i++) {
			if (minus) *res += indexOf(number[dot - i]) * pow(sys, i - minus);
			else *res += indexOf(number[dot - i - 1]) * pow(sys, i - minus);
			if (minus) ideal += indexOf(number[dot - i]) * pow(sys, i - minus);
			else ideal += indexOf(number[dot - i - 1]) * pow(sys, i - minus);
		}
		int count = -1; // power digit bases after point. (__.123 1*base^(-1) + 2*base^(-2) + ... )
		for (int i = dot + 1; i < number.size(); i++) {
			*res += indexOf(number[i]) * pow(sys, count);
			ideal += indexOf(number[i]) * pow(sys, count);
			count--;
		}
	}
	// check for oversize
	if (ideal - *res > 0.001 || *res - ideal > 0.001) return 1;  // 0.001 - acceptable calculation error
	if (minus) *res *= -1;
	return 0;
}


int checkNumber(string number, int sys, char type) {
	if (number.size() == 0) {
		cout << "\n[Error] -> '[Error] -> Empty number\n";
		return 7;
	}
	int dot = 0;
	for (int i = 0; i < number.size(); i++)
	{
		if (number[i] == '-' && i > 0) {
			cout << "\n[Error] -> '-' is on the wrong place\n";
			return 1;
		}
		else if (number[i] == '.' && type != 'd' && type != 'f') {
			cout << "\n[Error] -> '.' can't be in integer number\n";
			return 2;
		}
		else if (number[i] == '.' && (i == 0 || (number[0] == '-' && i == 1) || dot == 1)) {
			cout << "\n[Error] -> '.' is on the wrong place\n";
			return 3;
		}
		else if ((indexOf(number[i]) < 0 || indexOf(number[i]) >= sys) && number[i] != '-' && number[i] != '.') {
			cout << "\n[Error] -> Forbidden symbol '" << number[i] << "'\n";
			return 4;
		}
		if (number[i] == '.') dot = 1; // make trigger for point on
	}
	// call getDecimal to check for oversize for all types
	int err = 0;
	if (type == 'i') {
		int p = 0; if (getDecimal(number.substr(number[0] == '-' ? 1 : 0, number.size()), sys, type, &p)) err = 1;
	}
	else if (type == 'f') {
		float p = 0; if (getDecimal(number.substr(number[0] == '-' ? 1 : 0, number.size()), sys, type, &p)) err = 1;
	}
	else if (type == 'd') {
		double p = 0; if (getDecimal(number.substr(number[0] == '-' ? 1 : 0, number.size()), sys, type, &p)) err = 1;
	}
	else if (type == 's') {
		short p = 0; if (getDecimal(number.substr(number[0] == '-' ? 1 : 0, number.size()), sys, type, &p)) err = 1;
	}
	else if (type == 'c') {
		char p = 0;	if (getDecimal(number.substr(number[0] == '-' ? 1 : 0, number.size()), sys, type, &p)) err = 1;
	}
	else if (type == 'l') { long p = 0; if (getDecimal(number.substr(number[0] == '-' ? 1 : 0, number.size()), sys, type, &p)) err = 1; }
	if (err) {
		cout << "\n[Error] ->  Number is too big\n"; //oversize has happened
		return 6;
	}
	return 0;
}

template <typename T>
string getBinary(T val) {
	string res = "";
	char arr[sizeof(val)];
	int counter = 0;
	memcpy(arr, &val, sizeof(val)); //copy bytes to array
	for (int i = 8 * sizeof(val) - 1; i >= 0; i--) {
		res += (arr[i / 8] & (1 << (i % 8))) ? '1' : '0'; // copy bits from each byte in right order
	}
	return res;
}

string insertionSort(string arr, int left, int right) {
	for (int i = right; i >= left + 1; i--) {
		int current = i;
		int compareable = current - 1;
		while (current > 0) {
			if (arr[current] > arr[compareable]) {
				char temp = arr[current];
				arr[current] = arr[compareable];
				arr[compareable] = temp;
				current++;
				compareable++;
			}
			else break;
		}
	}
	return arr;
}

string change(string initial) {
	int left = -1;
	int right = -1;
	cout << "1 limit -> ";
	while (cin.fail() || left < 0 || left > initial.size() - 1)
	{
		cin.clear();
		cin.ignore();
		cin >> left;
	}
	cout << "2 limit -> ";
	while (cin.fail() || right < 0 || right > initial.size() - 1)
	{
		cin.clear();
		cin.ignore();
		cin >> right;
	}
	if (left < right) {
		right += left;
		left = right - left;
		right -= left;
	}
	return insertionSort(initial, initial.size() - left - 1, initial.size() - right - 1);
}

void colorBits(string before, string after, int color, char type) {
	CONSOLE_SCREEN_BUFFER_INFO Info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
	WORD Attributes = Info.wAttributes;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attributes);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 17,3 });
	if (type == 'c') SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 17,4 });
	for (int i = 0; i <= before.size() - 1; i++) {
		if (before[i] != after[i]) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
			cout << after[i];
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attributes);
		}
		else cout << after[i];
		Sleep(75);
	}
}

void showColors() {
	CONSOLE_SCREEN_BUFFER_INFO Info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
	WORD Attributes = Info.wAttributes;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attributes);
	for (int i = 1; i <= 15; i++) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
		cout << i << " ";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attributes);
}

void paint(string num, bool enter, int ampl)
{
	HANDLE con_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO con_wrapper;
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleScreenBufferInfo(con_handle, &con_wrapper);
	GetConsoleCursorInfo(con_handle, &cursor);
	float Y;
	Y = con_wrapper.dwCursorPosition.Y - 1;
	if (enter) Y = con_wrapper.dwCursorPosition.Y + enter;
	Y *= 21.7;
	POINT op;
	HWND window = GetConsoleWindow();
	HDC hdc = GetDC(window);
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB(00, 0xcc, 0xcc));
	int y = Y;
	short x = 0;
	x = 16 * 17;
	for (int i = 0; i < num.size() - 1; i++)
	{
		Sleep(40);
		x = 16 * 17 + 16 * i;
		(num[i] == '0') ? y = Y + ampl : y = Y;
		MoveToEx(hdc, x, y, &op);
		LineTo(hdc, x + 16, y);
		MoveToEx(hdc, x + 16, y, &op);
		if (num[i + 1] != num[i]) {
			num[i] == '1' ? LineTo(hdc, x + 16, y += ampl) : LineTo(hdc, x + 16, y -= ampl);
		}
	}
	MoveToEx(hdc, x + 16, y, &op);
	LineTo(hdc, x + 32, y);
	ReleaseDC(window, hdc);
}

double stringToDouble(string s)
{
	int power = 0;
	int exp = 0;
	int p = 0;
	for (int i = 11; i > 0; --i)
	{
		exp += (s[i] - 48) * pow(2, power);
		power++;
	}
	p = -1023 + exp;
	long long mant = 0;
	power = 0;
	for (int i = 63; i > 11; i--)
	{
		mant += (s[i] - 48) * pow(2, power);
		power++;
	}
	return pow(-1, s[0] - 48) * ((mant / pow(2, 52)) * (pow(2, p + 1) - pow(2, p)) + pow(2, p));
}

float stringToFloat(string s)
{
	int power = 0;
	int exp = 0;
	int p = 0;
	for (int i = 8; i > 0; --i)
	{
		exp += (s[i] - 48) * pow(2, power);
		power++;
	}
	p = -127 + exp;
	int mant = 0;
	power = 0;
	for (int i = 31; i > 8; i--)
	{
		mant += (s[i] - 48) * pow(2, power);
		power++;
	}
	return pow(-1, s[0] - 48) * ((mant / pow(2, 23)) * (pow(2, p + 1) - pow(2, p)) + pow(2, p));
}

// Shows any info that you want (decimal/binary) graphics and etc. should be added
template <typename T>
void show(string number, int sys, char type, T* p) {
	char cmd = 0;
	getDecimal(number, sys, type, p);
	string binary = getBinary(*p);
	string _binary = binary;
	int iterrations = 0;
	int color = -1;
	float fl = 0;
	double dbl = 0;
	do {
		if (iterrations > 0) {

			if (type == 'f') fl = stringToFloat(binary);
			else if (type == 'd') dbl = stringToDouble(binary);
			else getDecimal(binary, 2, type, p);
		}
		cout << "\n\tDecimal: ";
		if (type == 'd' && iterrations > 0) cout << dbl << endl;
		else if (type == 'f' && iterrations > 0) cout << fl << endl;
		else if (type == 'c') {
			cout << (int)*p << endl << "\tSymbol:  " << *p << endl;
		}
		else cout << *p << endl;
		if (iterrations > 0)
		{
			cout << "\tOld bin: " << _binary << endl;
			cout << "\tBinary:  ";
			colorBits(_binary, binary, color, type);
		}
		else cout << "\tBinary:  " << binary;
		cout << "\n\tSignal:  \n";
		paint(binary, 1, 20);
		cout << endl;
		cout << "\nEnter 'y' to change some bits.\nEnter 'n' to continue.\n[Change]-> ";
		cin >> cmd;
		if (cmd == 'y') {
			_binary = binary;
			binary = change(binary);
			cout << endl << endl << "Color (1-15)\n";
			showColors();
			cout << "\n-> ";
			do
			{
				cin.clear();
				cin.ignore();
				cin >> color;
			} while (cin.fail() || color < 1 || color > 15);
			iterrations++;
		}
		system("cls");
	} while (cmd != 'n');
}

void setFont(short width, short height, LPCWSTR faceName)
{
	CONSOLE_FONT_INFOEX font = {};
	GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &font);
	font.dwFontSize = { width, height };
	font.FontWeight = 1000;
	font.cbSize = sizeof(font);
	lstrcpyW(font.FaceName, faceName);
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &font);
}

int main() {
	setFont(13, 24, L"Courier New");
	char type = 0;	// variable for type of data
	char cmd = 0;	// trigger to exit
	int sys = 0;	// variable for counting system
	string number;  // here is the correct initial number (-123.1fa) 
	cout << setprecision(12); // accuracy in cout of float number is ~ 12 symbols

	do {
		system("cls");
		// Choosing data type ////////////////////////////////////
		cout << "Choose type:\n\n"
			"\tc\t-\tCHAR\n"
			"\ts\t-\tSHORT\n"
			"\ti\t-\tINT\n"
			"\tl\t-\tLONG\n"
			"\tf\t-\tFLOAT\n"
			"\td\t-\tDOUBLE\n\n"
			"[Type] -> ";
		do {
			type = getch();
		} while (type != 'c' && type != 's' && type != 'i' && type != 'l' && type != 'f' && type != 'd');
		cout << type << endl;
		//////////////////////////////////////////////////////////

		// Choosing number system ////////////////////////////////
		do {
			system("cls");
			cout << "Choose number system [2 - 16]\n"
				"[System] -> ";
			cin >> sys;
			while (cin.fail())
			{
				cin.clear();
				cin.ignore();
			}
		} while (sys < 2 || sys > alf.size());
		//////////////////////////////////////////////////////////

		// Enter number //////////////////////////////////////////
		int check = 0; // error trigger
		char symbol = 0; // every new symbol
		number = ""; // clean from garbage before using string
		do {
			system("cls");
			cout << "Enter number in " << sys << " number system.\nPossible symbols: ";
			for (short i = 0; i < sys; i++) cout << alf[i] << ", ";
			(type == 'd' || type == 'f') ? cout << "'-', '.'\n[Number] -> " : cout << "'-'\n[Number] -> ";
			cout << number;

			symbol = getch();
			if (symbol == 13) { // ENTER pressed
				if (number == "-") { // '-' is the only symbol in number
					cout << "\n[Error] ->  '-' is not a number\n";
					system("pause");
					continue;
				}
				else if (number == "") continue; // Empty string
				else break; // end of while (number is correct)
			}
			if (symbol == '\b') { // backspace pressed
				if (number.size() > 0)
					number = number.substr(0, number.size() - 1); // delete last symbol from number
				continue;
			}
			else if (symbol != '\n') { // any other symbol
				cout << symbol;
				number += symbol;
				check = checkNumber(number, sys, type);
				if (check != 0) { // if error it'll be printed
					system("pause");
					number = number.substr(0, number.size() - 1); // delete last symbol from number
				}
				check = 0; // now we do not have errors
			}
		} while (1);
		system("cls");
		//////////////////////////////////////////////////////////

		// Show decimal, binary, etc. ////////////////////////////
		if (type != 'c') cout << endl;
		cout << "\n\tInitial: " << number << " (" << sys << "-based)";
		if (type == 'c') {
			char p;
			show(number, sys, type, &p);
		}

		if (type == 's') {
			short p;
			show(number, sys, type, &p);
		}

		if (type == 'i') {
			int p;
			show(number, sys, type, &p);
		}

		if (type == 'l') {
			long p;
			show(number, sys, type, &p);
		}

		if (type == 'f') {
			float p;
			show(number, sys, type, &p);
		}

		if (type == 'd') {
			double p;
			show(number, sys, type, &p);
		}
		//////////////////////////////////////////////////////////

		cout << "\nPress 'q' to terminate program.\nAny other key other to restart. ";
		cmd = getch();
	} while (cmd != 'q');
}