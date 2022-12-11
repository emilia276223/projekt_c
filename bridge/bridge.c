#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define DEBUG 1

struct card{
	int num;
	int color;
};
typedef struct card card;

//it will always be: N is dealer, S is waiting, game starts from 
card cards[4][13];//[i][j][k] - i-ty gracz, [j] j - ta karta
int score[4];//score of players

char znaczek[5] = {'N', 'h', 'd', 's', 'c'};

card wszystkie_karty[52];

void generate_cards(){
	for(int i = 0; i < 52; i ++){
		wszystkie_karty[i].color = i/13 + 1;
		wszystkie_karty[i].num = i%13 + 2;
	}
}


card deal;//first - number (1 - 7), second - color
//colors:
//0 - no trump
//1 - hearts
//2 - diamonds
//3 - spades
//4 - clubs

void dealing_cards(){
	//randomised cards
	
	
	//na ten moment wpisywane recznie
	/*for(int i = 0; i < 13; i++){//wpisuje graczowi 0
		scanf("%i %i",&cards[0][i].num, &cards[0][i].color);
	}*/
	
	
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

card system_z_grubsza(int player, int czy_otwarcie){
	card wynik;
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

void show_cards(int player){
	printf("\nkarty gracza %i:\n", player + 1);
	for(int i = 0; i < 13; i++){//wpisuje graczowi 0
		printf("(%i %c), ",cards[player][i].num, znaczek[cards[0][i].color]);
	}
	printf("\n");
}

void show_last_trick(){//ostatnia lewa
	
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
		printf("Kolej gracza %i\n wpisz odpowiednio jak wysoko chcesz zalicytować (1 - 7) \noraz kolor (h - kiery, d - karo, s - pik, c - trefl, n - bez atutu), albo 0 jesli pass\n", player + 1);
		scanf("%i", &number);
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
		printf("licytacja zakonczona na (%i, %c)\n", deal.num, znaczek[deal.color]);
	}
}

void runda(){
	//
}

void new_game(){
	deal.num = 0;
	deal.color = 0;
	generate_cards();
	dealing_cards();
	if(DEBUG){
		show_cards(0);
		show_cards(1);
		show_cards(2);
		show_cards(3);
	}
}

int main(){
//	auction();
	new_game();
	auction();
}
