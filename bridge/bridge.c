#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "wyswietlanie_i_karty.h"

#define DEBUG 1

//it will always be: N is dealer, S is waiting, game starts from 
card cards[4][13];//[i][j][k] - i-ty gracz, [j] j - ta karta
int score[4];//score of players

card wszystkie_karty[52];

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

card naturalny_odpowiedz(card *karty,int liczba_punktow, card poprzedni){
	int kolory[5];
	card wynik;
	for(int i = 0; i < 13; i++){
		kolory[(karty + i) -> color]++;
	}
	//dokonczyc
}

void show_last_trick(){//ostatnia lewa
	//
}
int rozgrywajacy = 0;
int dziadek;
int atut;

void auction(){
	printf("\nLICYTACJA \n");
	int player = 0;//startujemu od 1
	int number;
	int count = 0;
	while(count < 3 || (count < 4 && deal.num == 0)){
		clear_screen();
		info_dla_gracza_licytacja(player, &cards[player][0], deal);
		scanf("%i", &number);
		if(number == -1){
			printf("\nTwoja podpowiedz do licytacji w systemie naturalnym to:\n");
			print(naturalny_otwarcie(&cards[player][0]));
			printf("\n");
			printf("Co chcesz zalicytować?\n");
			scanf("%i", &number);
		}
		card zalicytowane = wczytanie_licytacja(number, deal);
//		zalicytowano(zalicytowane);
		if(zalicytowane.num == -1){//pass
			count++;
		}
		else{
			count = 0;
			deal = zalicytowane;
//			rozgrywajacy = player;
		}
		player = (player + 1)%4;
		//strona przejsciowa
		clear_screen();
		getchar();
		printf("Ruch gracza poprzedniego zakończony, kolejny ruch będzie gracza %i\n", player + 1);
		printf("Wcisnij ENTER, żeby rozpoczac ruch gracza %i:\n", player + 1);
		getchar();
		clear_screen();
	}
//	if(DEBUG){
//		printf("Licytacja zakonczona na ");
//		print(deal);
//		printf("\n");
//	}
}


int runda(int dealer, int n){//zwracam kto zebral lewe
	//n - n-ta runda
	int player = dealer;
	card karty_na_stole[4];
	for(int i = 0; i < 4; i++){
		clear_screen();
		if(player == dziadek){//jesli dziadek
			printf("Karty rozgrywajacego:");
			show_cards(&cards[rozgrywajacy][0], 13 - n, rozgrywajacy);
			karty_na_stole[i] = wybor_karty(&cards[dziadek][0], 13 - n, dziadek, karty_na_stole, i);
		}
		else{
			if(n == 0 && player == (dziadek - 1) % 4){
				//gracz nie widzie jeszcze kart dziadka
				karty_na_stole[i] = wybor_karty(&cards[player][0], 13 - n, player, karty_na_stole, i);
			}
			else{
				printf("Karty dziadka:");
				show_cards(&cards[dziadek][0], 13 - n, dziadek);
				karty_na_stole[i] = wybor_karty(&cards[player][0], 13 - n, player, karty_na_stole, i);	
			}
		}
		player = (player + 1) % 4;
		clear_screen();
		getchar();
		printf("Ruch gracza poprzedniego zakończony, kolejny ruch będzie gracza %i\n", player + 1);
		printf("Wcisnij ENTER, żeby rozpoczac ruch gracza %i:\n", player + 1);
		getchar();
	}
	int new_dealer;
	new_dealer = ustal_wygrana(karty_na_stole, atut);
	score[new_dealer]++;
	return new_dealer;
}

void gra(){
	//13 rund
	//deal -> atut
	//znam rozgrywajacego
	//pierwsza runde rozpoczyna rozgrywajacy + 1
	int dealer = (rozgrywajacy + 1) % 4;
	for(int i = 0; i < 13; i++){
		dealer = runda(dealer,i);
	}
}

void new_game(){
	deal.num = 0;
	deal.color = 0;
	score[0] = 0;
	score[1] = 0;
	score[2] = 0;
	score[3] = 0;
	wyczysc_licytacja();
	generate_cards(&wszystkie_karty[0]);
	dealing_cards();
	clear_screen();
	auction();
	if(deal.num == 0){
		printf("Licytacja zakonczona niepowodzeniem\n\n");
		return;
	}
	rozgrywajacy = ustaw_rozgrywajacego(0);//bo na razie licytacje zawsze rozpoczyna gracz 0
	dziadek = (rozgrywajacy + 2) % 4;
	atut = deal.color;
	clear_screen();
	gra();
	//wyswietlenie wynikow
	printf("Gra zakonczona, wyniki to: %i, %i, %i, %i", score[0], score[1], score[2], score[3]);
	//trzeba zrobic sprawdzenie czy ugrane
}

int main(){
	srand(time(NULL));

//	auction();
	new_game();
//	auction();
	while(czy_nowa() == true){
		new_game();
	}
	return 0;
}

