#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define DEBUG 0
//to pewnie potrzebuje
struct card{
	int num;
	int color;
};
typedef struct card card;


//it will always be: N is dealer, S is waiting, game starts from 

//card deal;//first - number (1 - 7), second - color
//colors:
//0 - no trump
//1 - hearts
//2 - diamonds
//3 - spades
//4 - clubs

//tylko dla print
char znaczek[][6] = {"BA", "pik", "kier", "karo", "trefl"};
char wartosc[][16] = {"0", "1 ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ", "8 ", "9 ", "10", "J ", "Q ", "K ", "A "};

//potrzebne wszedzie:
void print(card karta);
void clear_screen();
int input_color(char c);
void show_cards(card *karty, int ile);//wyswietl karty gracza
void print_color(int k);


void print_color(int k)
{
	printf(" %s ", znaczek[k]);
}

void print(card karta)
{
	if(karta.num == -1){//pass
		printf("pass");
		return;
	}
	if(karta.num == -2){//nic
		return;
	}
	if(karta.num < 0 || karta.num > 15 || karta.color < 0 || karta.color > 4){
		printf("Karta nie pasuje, wartosci to : numer = %i, kolor = %i", karta.num, karta.color);
		return;
	}
	if(karta.num == 0) return;
	printf("(%s %s)",wartosc[karta.num], znaczek[karta.color]);
}

void clear_screen()
{//bedzie w wyswietlanieNa ten moment
	if(DEBUG) printf("\n\n\n\n\n\n\n");
	else if(system("clear") == 0) return;
}

int number_input()
{
	char number[] = "                   ";
	if(scanf("%s", &number[0]) < 1) return -256;

	if(number[1] == '\0')//tylko jeden znak
	{
		if(number[0] >= '0' && number[0] <= '9') return number[0] - '0';
		if(number[0] == 'A' || number[0] == 'a') return 14;
		if(number[0] == 'K' || number[0] == 'k') return 13;
		if(number[0] == 'Q' || number[0] == 'q') return 12;
		if(number[0] == 'J' || number[0] == 'j') return 11;
		if(number[0] == 'L' || number[0] == 'l') return -2;

		else return -256;
	}
	else{
		if(strcmp(number, "10") == 0) return 10;
		if(strcmp(number, "-1") == 0) return -1;
		else return -256;
	}
}

int input_color_string()
{
	char color[] = "                   ";
	if(scanf("%s", &color[0]) < 1) return -256;
	if(DEBUG)printf("Wczytano: <%s>", color);

	// if(color[0] == 't' || color[0] == 'T') return 4;//trefl
	// if(color[0] == 'b' || color[0] == 'B') return 0;//BA
	// if(color[0] == 'p' || color[0] == 'P') return 1;//pik
	// if(color[0] == 'k' || color[0] == 'K')//karo lub kier
	
	if(strcmp(color, "Trefl") == 0 || strcmp(color, "trefl") == 0) return 4;//trefl
	if(strcmp(color, "BA") == 0 || strcmp(color, "ba") == 0) return 0;//BA
	if(strcmp(color, "Pik") == 0 || strcmp(color, "pik") == 0) return 1;//pik
	if(strcmp(color, "Karo") == 0 || strcmp(color, "karo") == 0) return 3;//karo lub kier
	if(strcmp(color, "Kier") == 0 || strcmp(color, "kier") == 0) return 2;
	// {
		// if(color[1] == 'a' || color[1] == 'A') return 3;
		// if(color[1] == 'i' || color[1] == 'I') return 2;
	// }	
	return -256;
}

void input_color_string_test()
{
	while(1)
	{
		printf("podaj nazwe koloru, jaki chcesz wczytac\n");
		printf("wczytano kolor: %i\n", input_color_string());
	}
}

int input_color(char c)//stara wersja
{
	int kolor = -1;
	if(c == 'n' || c == 'N') kolor = 0;
	if(c == 'h' || c == 'H') kolor = 2;
	if(c == 'd' || c == 'D') kolor = 3;
	if(c == 's' || c == 'S') kolor = 1;
	if(c == 'c' || c == 'C') kolor = 4;
	if(kolor == -1){
		printf("\nBłędnie podany kolor, proszę podać jeszcze raz (sam kolor):\n");
		c = getchar();
		kolor = input_color(c);
	}
	return kolor;
}

void show_cards(card *karty, int ile)
{
	// printf("\nKarty gracza %i:\n", numer_gracza + 1);
	//chce wyswietlic w 4 rzedach
	card kier[13];
	card karo[13];
	card pik[13];
	card trefl[13];
	int kie = 0, kar = 0, pi = 0, tref = 0;
	
	for(int i = 0; i < ile; i++){
		kier[i].num = 0;
		kier[i].color = 0;
		karo[i].num = 0;
		karo[i].color = 0;
		pik[i].num = 0;
		pik[i].color = 0;
		trefl[i].num = 0;
		trefl[i].color = 0;
	}
	for(int i = 0; i < ile; i++){
		if((karty + i) -> color == 1){//pik
			pik[pi] = *(karty + i);
			pi++;
		}
		if((karty + i) -> color == 2){//kier
			kier[kie] = *(karty + i);
			kie++;
		}
		if((karty + i) -> color == 3){//karo
			karo[kar] = *(karty + i);
			kar++;
		}
		if((karty + i) -> color == 4){//trefl
			trefl[tref] = *(karty + i);
			tref++;
		}
	}
	int max = kie;
	if(kar > max) max = kar;
	if(pi > max) max = pi;
	if(tref > max) max = tref;
	for(int i = 0; i < max; i++){
		if(i < pi) print(pik[i]);
		else{
			printf("        ");
		}
		printf(" ");
		if(i < kie) print(kier[i]);
		else{
			printf("         ");
		}
		printf(" ");
		if(i < kar) print(karo[i]);
		else{
			printf("         ");
		}
		printf(" ");
		if(i < tref) print(trefl[i]);
		printf("\n");
	}
	printf("\n");
}


