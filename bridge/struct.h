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
char znaczek[][5] = {"BA", "pik", "kier", "karo", "trefl"};
char wartosc[][16] = {"0", "1 ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ", "8 ", "9 ", "10", "J ", "Q ", "K ", "A "};

//potrzebne wszedzie:
void print(card karta);
void clear_screen();
int input_color(char c);
void show_cards(card *karty, int ile, int numer_gracza);//wyswietl karty gracza


void print(card karta){
	if(karta.num == -1){
		printf("pass");
		return;
	}
	if(karta.num < 0 || karta.num > 15 || karta.color < 0 || karta.color > 4){
		printf("Karta nie pasuje, wartosci to : numer = %i, kolor = %i", karta.num, karta.color);
		return;
	}
	printf("(%s %s)",wartosc[karta.num], znaczek[karta.color]);
}

void clear_screen(){//bedzie w wyswietlanie
	if(DEBUG) printf("\n\n\n\n\n\n\n");
	else system("clear");
}

int input_color(char c){
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

void show_cards(card *karty, int ile, int numer_gracza){
	printf("\nKarty gracza %i:\n", numer_gracza + 1);
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


