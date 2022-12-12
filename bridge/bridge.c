#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define DEBUG 1

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


extern void show_cards(card *karty, int ile, int numer_gracza);


//karty
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
	wyswietl_rozdanie(&wszystkie_karty[0]);
	clear_screen();
//	show_cards(&cards[0][0], 13, 0);
//	show_cards(&cards[1][0], 13, 1);
//	show_cards(&cards[2][0], 13, 2);
//	show_cards(&cards[3][0], 13, 3);
}

void sort_by_num(card *tab, int ile);

void sort_by_color(card *tab, int ile);

card system_z_grubsza(int player){
	card wynik;
	int czy_otwarcie = 1;
	//trzeba to sprawdzic, bede w stanie jak bedzie zapis licytacji
	if(czy_otwarcie == 1){
		//policzenie ilosci figur
		int figury = 0;
		card kolory[5];
		for(int i = 0; i < 13; i ++){
			if(cards[player][i].num > 10){//jesli figura
				figury++;
			}
			kolory[cards[player][i].color].num ++;
		}
		sort_by_num(&kolory[1], 4);
		//jesli 2-3 renonse
		if(kolory[3].num <= 2 || kolory[2].num <2){
			wynik.num = 2;
			wynik.color = 0;
			return wynik;
		}
		//jesli ciekawa reka bez koloru
//		if(kolory)
		//jesli malo figur
		if(figury < 3){
			//jesli zrownowazony
			if(kolory[4].num < 5){
				wynik.num = 0;
				return wynik;
			}
			//jesli niezrownowazony
			wynik.num = 1;
			wynik.color = 4;
			return wynik;
		}
		//nie malo figur
		if(figury <= 5){//srednio fugur
			if(kolory[4].num < 5){//sklad zrownowazony
				wynik.color = 2;
				wynik.num = 1;
				return wynik;
			}
			//niezrownowazony
			wynik.num = 1;
			wynik.color = 1;
			return wynik;
		}
	}
		//duzo figur
	//nie otwarcie:
	
	//policz kolory
	//czy_kolory mowi ktora to "moja" odzywka, odpowiednio (po posortowaniu ) mowie kolor[i]
	
}

void show_last_trick(){//ostatnia lewa
	//
}

void auction(){
	//starts from player A
	printf("\nLICYTACJA \n");
	int count = 0;//how many times pass
	int player = 0;//start at N
	int number;
	char c;
	int color;
	while(count < 3){
		clear_screen;
		printf("Kolej gracza %i\n", player + 1);
		printf("Wpisz odpowiednio jak wysoko chcesz zalicytować: (1 - 7) \noraz kolor (h - kiery, d - karo, s - pik, c - trefl, n - bez atutu), albo 0 jesli pass\n");
		printf("a to twoje karty :");
		show_cards(&cards[player][0], 13, player);
		printf("na ten moment zalicytowane jest:\n");
		print(deal);
		printf("\n");
		printf("jesli chcesz uzyskać podpowiedź do licytacji napisz -1");
		printf("\n");
		scanf("%i", &number);
		if(number == -1){
			printf("\nTwoja podpowiedz do licytacji w sysyemie 'z grubsza' to:\n");
			print(system_z_grubsza(player));
			printf("\n");
			printf("Co chcesz zalicytować?\n");
		}
		if(number == 0){
			count ++;
			player = (player + 1)%4;
		}
		else{	
			if(scanf(" %c", &c) < 1){//if someone enters wrong thing
				printf("niepoprawnie wpisany kolor, proszę podać jeszcze raz (h, d, s, c lub n)");
			}
//			scanf(" %c", &c);
			else{
				count = 0;
				if(c == 'n' || c == 'N') color = 0;
				if(c == 'h' || c == 'H') color = 1;
				if(c == 'd' || c == 'D') color = 2;
				if(c == 's' || c == 'S') color = 3;
				if(c == 'c' || c == 'C') color = 4;
				if(number < deal.num || (number == deal.num && color > deal.color)) printf("to jest mniej niż poprzednio, wiec nie mozna tego zalicytowac \n");
				else{
					deal.num = number;
					deal.color = color;
					player = (player + 1)%4;
				}
			}
		}
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
	clear_screen();
}

int main(){
	srand(time(NULL));

//	auction();
	new_game();
	auction();
	//do licytacji dopisac zapamietywanie ostatnich czterech (czyli dla kazdego z graczy), bo to trzeba pokazywac i przyda sie do z grubsza
}
