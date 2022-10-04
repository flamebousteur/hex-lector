#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

// selector
void SetCursorUp(SHORT x, SHORT y) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD consoleMode;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(console, &csbi);
	COORD Pos;
	Pos.X = x;
	Pos.Y = csbi.dwCursorPosition.Y - y;
	SetConsoleCursorPosition(console, Pos);
}
 
int selector(const char* ch[], int taille) {
	int i,curs = 0;
	SetCursorUp(0, 0);
	for (i = 0; i < taille; i++)
		if (i == curs) printf("\033[30;47m# %s #\n\033[37;40m", ch[i]);
		else printf("[ %s ]\n", ch[i]);
	SetCursorUp(0, 0);

	while (1) {
		int touche = _getch();
		if (touche == 0x50 && curs < taille-1)
			curs++;
		if (touche == 0x48 && curs > 0)
			curs--;
		if (touche == 0x0D)
			return curs + 1;
		if (touche == 0x1B) // echap
			return 0;
		SetCursorUp(0, taille);
		for (i = 0; i < taille; i++) if (i == curs) printf("\033[30;47m[ %s ]\n\033[37;40m", ch[i]); else printf("[ %s ]\n", ch[i]);
	}	return 0;
}

// input
int input(const char* message, char* buffer, int size) {
	int i, j = 0;
	printf("%s", message);
	for (i = 0; i < size; i++) buffer[i] = 0;
	while (1) {
		int touche = _getch();
		if (touche == 0x0D) {
			printf("\n");
			return 1;
		}
		if (touche == 0x08 && j > 0) {
			j--;
			buffer[j] = 0;
			printf("\b \b");
		}
		if (touche >= 0x20 && touche <= 0x7E && j < size) {
			buffer[j] = touche;
			j++;
			printf("%c", touche);
		}
	}	return 0;
}