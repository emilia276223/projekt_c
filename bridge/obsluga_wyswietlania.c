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
	system("clear");
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

extern int policz_punkty(card *karty);

void info_dla_gracza_licytacja(int gracz, card *karty, card deal){
	printf("Kolej gracza %i\n", gracz + 1);
	printf("Wpisz odpowiednio jak wysoko chcesz zalicytować: (1 - 7) \noraz kolor (h - kiery, d - karo, s - pik, c - trefl, n - bez atutu), albo 0 jesli pass\n");
	printf("a to twoje karty :");
	show_cards(karty, 13, gracz);
	printf("liczba twoich punktow to: %i\n", policz_punkty(karty));
	printf("na ten moment zalicytowane jest:\n");
	print(deal);
	printf("\n");
	printf("jesli chcesz uzyskać podpowiedź do licytacji napisz -1");
	printf("\n");
}

extern void zalicytowano(card lic);

card wczytanie_licytacja(int number, card deal){
	card wynik;
	if(number == 0){//pass
		wynik.num = -1;
		return wynik;
	}
	//jesli niepoprawny numer
	if(number < 0 || number > 7){
		printf("niepoprawnie wpisana liczba, proszę podać jeszcze raz ( 0 - pass lub liczba z zakresu 1 - 7)");
		scanf("%i", &number);
		return wczytanie_licytacja(number, deal);
	}
	//nie pass
	int color = -1;
	char c;
	if(scanf(" %c", &c) < 1){//if someone enters wrong thing
		printf("niepoprawnie wpisany kolor, proszę podać jeszcze raz (h, d, s, c lub n)");
		scanf("%i", &number);
		return wczytanie_licytacja(number, deal);
	}
	if(c == 'n' || c == 'N') color = 0;
	if(c == 'h' || c == 'H') color = 1;
	if(c == 'd' || c == 'D') color = 2;
	if(c == 's' || c == 'S') color = 3;
	if(c == 'c' || c == 'C') color = 4;
	if(color == -1){//blad wczytywania
		printf("niepoprawnie wpisany kolor, proszę podać jeszcze raz (h, d, s, c lub n)");
		scanf("%i", &number);
		return wczytanie_licytacja(number, deal);
	}
	if(number < deal.num || (number == deal.num && color > deal.color)){
		printf("to jest mniej niż poprzednio, wiec nie mozna tego zalicytowac, sprobuj jeszcze raz: \n");
		scanf("%i", &number);
		return wczytanie_licytacja(number, deal);
	}
	wynik.num = number;
	wynik.color = color;
	zalicytowano(wynik);
	return wynik;
}

//daję zaproszenia na zosię:
//Bbe
