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

void generate_cards(card *wszystkie_karty){//dziala
	for(int i = 0; i < 52; i ++){
		(wszystkie_karty + i) -> color = i/13 + 1;
		(wszystkie_karty + i) -> num = i%13 + 2;
//		if(DEBUG){
//			 printf("karta:");
//			 print(wszystkie_karty[i]);
//			 printf("\n");
//		}
	}
}

void dealing_cards(){
	//randomised cards
	
	//podzielenie na graczy
	for(int i = 0; i < 52; i ++){
		cards[i/13][i%13] = wszystkie_karty[i];
	}
}

void sort_by_num(card *tab, int ile){//od najmniejszej do najwiekszej
	//bubble sort bo czemu nie skoro i tak malo
//	int count = -1;
	for(int i = 0; i < ile; i ++){
//		count = 0;
		for(int j = 1; j < ile - i; j ++){
//			if(tab[j-1] -> num > tab[j] -> num || (tab[j-1] -> num > tab[j] -> num && tab[j-1] -> color < tab[j] -> color)){
//				card temp = *tab[j-1];
//				*tab[j-1] = *tab[j];
//				*tab[j] = temp;
//			}
			if((tab + j - 1) -> num > (tab + j) -> num || ((tab + j- 1) -> num == (tab + j) -> num && (tab + j - 1) -> color < (tab + j) -> color)){
				card temp = *(tab + j - 1);
				*(tab + j - 1) = *(tab + j);
				*(tab + j) = temp;
			}
		}
	}
}

void sort_by_color(card *tab, int ile){
	//bubble sort
	//	int count = -1;
	for(int i = 0; i < ile; i ++){
//		count = 0;
		for(int j = 1; j < ile - i; j ++){
//			if(tab[j-1] -> num > tab[j] -> num || (tab[j-1] -> num > tab[j] -> num && tab[j-1] -> color < tab[j] -> color)){
//				card temp = *tab[j-1];
//				*tab[j-1] = *tab[j];
//				*tab[j] = temp;
//			}
			if((tab + j - 1) -> color > (tab + j) -> color || ((tab + j- 1) -> num < (tab + j) -> num && (tab + j - 1) -> color == (tab + j) -> color)){
				card temp = *(tab + j - 1);
				*(tab + j - 1) = *(tab + j);
				*(tab + j) = temp;
			}
		}
	}
}

