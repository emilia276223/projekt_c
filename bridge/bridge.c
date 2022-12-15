#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define DEBUG 0

struct card{
	int num;
	int color;
};
typedef struct card card;


//it will always be: N is dealer, S is waiting, game starts from 
card cards[4][13];//[i][j][k] - i-ty gracz, [j] j - ta karta
int score[4];//score of players

card wszystkie_karty[52];

extern void print(card karta);

extern void clear_screen();

extern void sort_by_num(card *tab, int ile);

extern void sort_by_color(card *tab, int ile);

extern void show_cards(card *karty, int ile, int numer_gracza);

extern void generate_cards(card *wszystkie_karty);

extern void tasowanie(card *talia);

extern void wyswietl_rozdanie(card *karty);

card deal;//first - number (1 - 7), second - color
//colors:
//0 - no trump
//1 - hearts
//2 - diamonds
//3 - spades
//4 - clubs

void dealing_cards(){
	//randomised cards
	tasowanie(&wszystkie_karty[0]);
	//podzielenie na graczy
	for(int i = 0; i < 52; i ++){
		cards[i/13][i%13] = wszystkie_karty[i];
	}
	//potasowanie kart kolorami
	sort_by_color(&cards[0][0], 13);
	sort_by_color(&cards[1][0], 13);
	sort_by_color(&cards[2][0], 13);
	sort_by_color(&cards[3][0], 13);
	//wyswietlenie
	wyswietl_rozdanie(&cards[0][0]);
}

//card historia_licytacji[200];//maksymalna mozliwa liczba ruchow w licytacji


extern int policz_punkty(card *karty);

card naturalny_otwarcie(card *karty);


card naturalny_odpowiedz(card *karty,int liczba_punktow, card poprzedni){
	int kolory[5];
	card wynik;
	for(int i = 0; i < 13; i++){
		kolory[(karty + i) -> color]++;
	}
	//dokonczyc
}

extern card system_z_grubsza(int cards[4][13], int player);//nie dziala

void show_last_trick(){//ostatnia lewa
	//
}

int count;

card wczytanie_licytacja(int number,card deal);

void info_dla_gracza_licytacja(int gracz, card *karty, card deal);

void auction(){
	printf("\nLICYTACJA \n");
	int player = 0;//startujemu od 1
	int number;
	while(count < 3){
		clear_screen();
		info_dla_gracza_licytacja(player, &cards[player][0], deal);
		scanf("%i", &number);
		if(number == -1){
			printf("\nTwoja podpowiedz do licytacji w systemie naturalnym to:\n");
//			print(system_z_grubsza(player));
			card proponowana_odzywka = naturalny_otwarcie(&cards[player][0]);
			if(proponowana_odzywka.num == 0){
				printf("pass (0)");
			}
			else{
				print(proponowana_odzywka);
			}
			printf("\n");
			printf("Co chcesz zalicytować?\n");
			scanf("%i", &number);
		}
		card zalicytowane = wczytanie_licytacja(number, deal);
		if(zalicytowane.num == -1){//pass
			count++;
		}
		else{
			count = 0;
			deal = zalicytowane;
		}
		player = (player + 1)%4;
//		clear_screen();
	}
	if(DEBUG){
		printf("licytacja zakonczona na ");
		print(deal);
		printf("\n");
	}
}

void runda(){
	//
}

void new_game(){
	deal.num = 0;
	deal.color = 0;
	generate_cards(&wszystkie_karty[0]);
	dealing_cards();
	if(DEBUG){
		printf("ENTER, żeby kontynuować:\n");
		getchar();
	}
	clear_screen();
}

int main(){
	srand(time(NULL));

//	auction();
	new_game();
	auction();
	//do licytacji dopisac zapamietywanie ostatnich czterech (czyli dla kazdego z graczy), bo to trzeba pokazywac i przyda sie do z grubsza
}

