#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>


struct card{
	int num;
	int color;
};
typedef struct card card;

//it will always be: N is dealer, S is waiting, game starts from 
card cards[4][13];//[i][j][k] - i-ty gracz, [j][0] - numer karty, j[1] - kolor
int score[4];//score of players


card deal;//first - number (1 - 7), second - color
//colors:
//0 - no trump
//1 - hearts
//2 - diamonds
//3 - spades
//4 - clubs

void dealing_cards(){
	//randomised cards
}

void sort_by_num(card *tab, int ile){//od najmniejszej do najwiekszej
	//bubble sort bo czemu nie skoro i tak malo
	int count = -1;
	for(int i = 0; i < ile; i ++){
		count = 0;
		for(int j = 1; j < ile - i; j ++){
//			if(tab[j-1] -> num > tab[j] -> num || (tab[j-1] -> num > tab[j] -> num && tab[j-1] -> color < tab[j] -> color)){
//				card temp = *tab[j-1];
//				*tab[j-1] = *tab[j];
//				*tab[j] = temp;
//			}
			if((tab + j - 1) -> num > (tab + j) -> num || ((tab + j- 1) -> num > (tab + j) -> num && (tab + j - 1) -> color < (tab + j) -> color)){
				card temp = *(tab + j - 1);
				*(tab + j - 1) = *(tab + j);
				*(tab + j) = temp;
			}
		}
	}
}

void sort_by_color(card *tab, int ile){
	//bubble sort
	
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
	//
}

void show_last_trick(){//ostatnia lewa
	
}

void auction(){
	//starts from player A
	printf("LICYTACJA");
	int count = 0;//how many times pass
	int player = 0;//start at N
	int number;
	char c;
	int color;
	while(count < 3){
		printf("Kolej gracza %i, wpisz odpowiednio jak wysoko chcesz zalicytować (1 - 7) oraz kolor (h - kiery, d - karo, s - pik, c - trefl, n - bez atutu), albo p jesli pass\n", player + 1);
		number = getchar();
		if(number == 'p'){
			count ++;
			player = (player + 1)%4;
		}
		else{
			while(scanf("%c", &c) < 1){//if someone enters wrong thing
				printf("niepoprawnie wpisany kolor, proszę podać jeszcze raz (h, d, s, c lub n)");
			}
			if(c == 'n') color = 0;
			if(c == 'h') color = 1;
			if(c == 'd') color = 2;
			if(c == 's') color = 3;
			if(c == 'c') color = 4;
			if(number < deal.num || (number == deal.num && color > deal.color)) printf("to jest mniej niż poprzednio, wiec nie mozna tego zalicytowac");
			else{
				deal.num = number;
				deal.color = color;
				player = (player + 1)%4;
			}
		}
	}
}

void runda(){
	//
}

void new_game(){
	deal.num = 0;
	deal.color = 0;
	dealing_cards();
}

int main(){
	auction();
}
