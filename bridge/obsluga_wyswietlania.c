#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//to pewnie potrzebuje
struct card{
	int num;
	int color;
};
typedef struct card card;

char znaczek[5] = {'N', 'h', 'd', 's', 'c'};
char wartosc[][13] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

void print(card karta){
	printf("(%s %c)",wartosc[karta.num], znaczek[karta.color]);
}

void clear_screen(){//bedzie w wyswietlanie
	printf("\n\n\n\n\n\n\n\n\n\n\n");
}

void show_cards(card *karty, int ile, int numer_gracza){
	printf("\nkarty gracza %i:\n", numer_gracza + 1);
	for(int i = 0; i < 13; i++){//wpisuje graczowi 0
		print(*(karty + i));
		printf(" ");
	}
	printf("\n");
}
