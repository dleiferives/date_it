#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <ctype.h>
#define SEP 0xBD
#define COLON 0xBA

void type_char(unsigned int val, DWORD type)
{
		INPUT ip;
// ...
    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    ip.ki.wVk = val; // virtual-key code for the "a" key
    ip.ki.dwFlags = type; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));
}

void type_string(char * buffer)
{
	int iterator =0;
	while(buffer[iterator] != '\0')
	{
		type_char(buffer[iterator],0);
		type_char(buffer[iterator],KEYEVENTF_KEYUP);
		iterator++;
	}
}

void type_colon(void)
{
	type_char(0x11,KEYEVENTF_KEYUP);
	type_char(0x10,0);
	type_char(COLON,0);
	type_char(0x10,KEYEVENTF_KEYUP);
}


int print_date(int detail)
{

	type_char(0x10,KEYEVENTF_KEYUP);
	type_char(0x11,KEYEVENTF_KEYUP);
	type_char(0x12,KEYEVENTF_KEYUP);

	printf("hello");
	SYSTEMTIME t;

	GetLocalTime(&t);
	char buffer[100];
	/* t.wYear, t.wMonth, t.wDay, t.wHour, */ 
	/* t.wMinute, t.wSecond, t.wMilliseconds */


	if(detail > -1)
	{

		sprintf(buffer,"%d", t.wMonth);  
		type_string(buffer);


		type_char(SEP,0);

		sprintf(buffer,"%d", t.wDay);  
		type_string(buffer);

		type_char(SEP,0);


		sprintf(buffer,"%d", t.wYear);  
		type_string(buffer);



		type_char(VK_SPACE,0);
		type_char(VK_SPACE,KEYEVENTF_KEYUP);

		if(detail > 0)
		{
			sprintf(buffer,"%d", t.wHour);  
			type_string(buffer);

			type_colon();

			sprintf(buffer,"%d", t.wMinute);  
			type_string(buffer);

			type_colon();

			sprintf(buffer,"%d", t.wSecond);
			type_string(buffer);

			if(detail >1)
			{

				type_colon();

				sprintf(buffer,"%d", t.wMilliseconds);  
				type_string(buffer);
				type_char(0x12,0);

			}
			type_char(0x10,0);
		}
	}
	type_char(VK_SPACE,0);
	type_char(VK_SPACE,KEYEVENTF_KEYUP);



	/* 	type_char(COLON); */



	type_char(0x11,0);


	puts(buffer);
}




/* int get_key_code */
int main(void)
{

	/* BYTE b[256]; */
	/* BYTE o[256]; // original keyboardstate */


	while(1)
	{
		int v0 = GetKeyState(VK_CONTROL) &8000;
		int v1 = GetKeyState(VK_SHIFT) &8000;
		int v2 = GetKeyState(VK_MENU) &8000;
		int v3 = GetKeyState('K') &8000;

		if(v0  && v3 && (!v1) && (!v2))
		{
			print_date(0);
			Sleep(100);
		}
		if(v0 && v3 && v1 && (!v2))
		{
			print_date(1);
			Sleep(100);
		}
		if(v0 && v3 && v1 && v2)
		{
			print_date(10);
			Sleep(100);
		}
	}
	return 0;
}


