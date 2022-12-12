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

char znaczek[][5] = {"BA", "kier", "karo", "pik", "trefl"};
char wartosc[][13] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

void print(card karta){
	printf("(%s %s)",wartosc[karta.num], znaczek[karta.color]);
}

void clear_screen(){//bedzie w wyswietlanie
	printf("\n\n\n\n\n\n\n\n\n\n\n");
}

void show_cards(card *karty, int ile, int numer_gracza){
	printf("\nkarty gracza %i:\n", numer_gracza + 1);
	for(int i = 0; i < ile; i++){//wpisuje graczowi 0
//		printf("a");
		print(*(karty + i));
		printf(" ");
	printf("\n");
	}
	printf("\n");
}

void wyswietl_rozdanie(card *karty){
	show_cards(karty, 13, 0);
	show_cards(karty + 13, 13, 1);
	show_cards(karty + 26, 13, 2);
	show_cards(karty + 39, 13, 3);
}

//daję zaproszenia na zosię:
//Bbe
//Zagórski
//Szykuła
//Ako
