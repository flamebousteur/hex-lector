#include <windows.h>
#include <conio.h>

#include <stdio.h>
#include <stdlib.h>

void Locate(SHORT x, SHORT y) {
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
   	// enable ANSI sequences for windows 10:
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD consoleMode;
	GetConsoleMode(console, &consoleMode);
	consoleMode |= 0x0004;
	SetConsoleMode(console, consoleMode);

	int i,curs = 0;
	Locate(0, 0);
	for (i = 0; i < taille; i++)
		if (i == curs) printf("\033[30;47m# %s #\n\033[37;40m", ch[i]);
		else printf("[ %s ]\n", ch[i]);
	Locate(0, 0);

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
		Locate(0, taille);
		for (i = 0; i < taille; i++) if (i == curs) printf("\033[30;47m[ %s ]\n\033[37;40m", ch[i]); else printf("[ %s ]\n", ch[i]);
	}	return 0;
}