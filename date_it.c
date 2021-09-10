/* dleiferives */
/* Fist coded 9-8-2021 */
/* updated 9-9-2021 23:29:23 */
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <ctype.h>
/* lazy definitions to remove magic numbers... which are used anyways in some cases */
#define SEP 0xBD
#define COLON 0xBA

long long times_printed =0;

/* Virtually types given character and wether it is a press or a realease */
void type_char(unsigned int val, DWORD type)
{
		/* create a virual-keyboard event */
		INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = 0;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;

    input.ki.wVk = val; // virtual-key code for the val 
    input.ki.dwFlags = type; // 0 for key press
    SendInput(1, &input, sizeof(INPUT));
}

/* Uses type_char to print out a string */
void type_string(char * buffer)
{
	/* iterates through passed string and writes all chars */
	int iterator =0;
	while(buffer[iterator] != '\0')
	{
		type_char(buffer[iterator],0);
		type_char(buffer[iterator],KEYEVENTF_KEYUP);
		iterator++;
	}
}

/* a hacky implementation to type the colons */
void type_colon(void)
{
	/* Lifts up control, then presses shift semicolon and releases shift */
	type_char(0x11,KEYEVENTF_KEYUP);
	type_char(0x10,0);
	type_char(COLON,0);
	type_char(0x10,KEYEVENTF_KEYUP);
}


/* the core function. quite literally types out the date */
int print_date(int detail)
{

	/* unpresses shift control and alt (in order here as is below) */
	type_char(0x10,KEYEVENTF_KEYUP);
	type_char(0x11,KEYEVENTF_KEYUP);
	type_char(0x12,KEYEVENTF_KEYUP);

	printf("Time Printed %i times \r",times_printed);
	/* accessing the time from the computer */
	SYSTEMTIME t;
	GetLocalTime(&t);
	/* creating the buffer where each value will be stored */
	char buffer[100];

	/* only called if the ctrl+k has been pressed, does the date */
	if(detail > -1)
	{
		/* types the date */
		/* types the month */
		sprintf(buffer,"%d", t.wMonth);  
		type_string(buffer);


		type_char(SEP,0);

		/* types the day */
		sprintf(buffer,"%d", t.wDay);  
		type_string(buffer);

		type_char(SEP,0);


		/* types the year */
		sprintf(buffer,"%d", t.wYear);  
		type_string(buffer);




		/* enable hours minutes and seconds */ 
		if(detail > 0)
		{
		/* presses space when needed */
		type_char(VK_SPACE,0);
		type_char(VK_SPACE,KEYEVENTF_KEYUP);

			sprintf(buffer,"%d", t.wHour);  
			type_string(buffer);

			type_colon();

			sprintf(buffer,"%d", t.wMinute);  
			type_string(buffer);

			type_colon();

			sprintf(buffer,"%d", t.wSecond);
			type_string(buffer);

			/* enable miliseconds */ 
			if(detail >1)
			{

				type_colon();

				sprintf(buffer,"%d", t.wMilliseconds);  
				type_string(buffer);
				type_char(0x12,0); // If alt was pressed repress it
			}
			type_char(0x10,0);// If shift was pressed repress it
		}
	type_char(VK_SPACE,0);
	type_char(VK_SPACE,KEYEVENTF_KEYUP);
	type_char(0x11,0); // If ctrl was presed... represses it
	times_printed++;
	}
}




int main(void)
{
	// Main loop
	while(1)
	{
		/* read in the current key values that are pressed down */
		int v0 = GetKeyState(VK_CONTROL) &8000;
		int v1 = GetKeyState(VK_SHIFT) &8000;
		int v2 = GetKeyState(VK_MENU) &8000;
		int v3 = GetKeyState('K') &8000;

		/* test if ctrl + k are pressed but shift and alt are not */
		if(v0  && v3 && (!v1) && (!v2))
		{
			print_date(0);
			Sleep(100);
		}
		/* test if ctrl + shift + k are pressed but alt is are not */
		if(v0 && v3 && v1 && (!v2))
		{
			print_date(1);
			Sleep(100);
		}
		/* test if ctrl + shift + k + alt are pressed */ 
		if(v0 && v3 && v1 && v2)
		{
			print_date(10);
			Sleep(100);
		}
	}
	return 0;
}


