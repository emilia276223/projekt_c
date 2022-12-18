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

card historia_licytacji[200];//maksymalna mozliwa liczba ruchow w licytacji
int nr_odzywki;//na nr nastepnej
//card deal;

void zalicytowano(card lic){
	historia_licytacji[nr_odzywki] = lic;
	nr_odzywki++;
}

extern void sort_by_num(card *tab, int ile);

extern int policz_punkty(card *karty);

int ustaw_rozgrywajacego(int zaczynajacy_licytacje){
	//odpalam tylko gdy deal jest, czyli jest cos zalicytowane
	int rozgrywajacy = nr_odzywki - 1;//ostatnia
	//znajduje ostatnia zalicytowana rzecz (powinna byc 3 od konca ale lepiej sprawdzic)
	while(historia_licytacji[rozgrywajacy].num == 0){
		rozgrywajacy--;
	}
	//ustalam co jest zalicytowane (kolor)
	int kolor = historia_licytacji[rozgrywajacy].color;
	//znajduje kto z pary pierwszy go powiedzial
	int gracz = rozgrywajacy - 2;//bede przechodzic co 2 bo to sa co w tej parze sie zadzialo
	while(gracz > 0){
		if(historia_licytacji[gracz].color == kolor || historia_licytacji[gracz].num > 0){//zalicytowal ten kolor (i to nie byl pass)
			rozgrywajacy = gracz;
		}
		gracz -= 2;
	}
	//teraz gracz jest ustawiony w dobrym miejscu (moment pierwszego zalicytowania danego koloru)
	//ustalam ktory to gracz
	return (gracz - zaczynajacy_licytacje) % 4;
}

card naturalny_otwarcie(card *karty){
	int liczba_punktow = policz_punkty(karty);
	printf("liczba punktów to : %i\n",liczba_punktow);
	int kolory[5]={0};
	card wynik;
	for(int i = 0; i < 13; i++){
		kolory[(karty + i) -> color]++;
	}
	for(int i = 1; i < 5; i++){
		printf("jest %i kart w kolorze %i\n",kolory[i],i);
	}
	card maksymalny_kolor;
	maksymalny_kolor.num = kolory[1];//0 to by bylo dla BA
	maksymalny_kolor.color = 1;
	for(int i = 2; i < 5; i++){
		if(maksymalny_kolor.num < kolory[i]){
			maksymalny_kolor.num = kolory[i];
			maksymalny_kolor.color = i;
		}
	}
	if(liczba_punktow <= 17 && liczba_punktow >= 12 && maksymalny_kolor.num >=5){
		wynik.num = 1;
		wynik.color = maksymalny_kolor.color;
		return wynik;
	}
	if(liczba_punktow <= 17 && liczba_punktow >= 12){//gdy zrownowazony rozklad
		wynik.num = 1;
		wynik.color = 4;
		return wynik;
	}
	if(liczba_punktow >= 18 && maksymalny_kolor.num >=5){
		wynik.num = 2;
		wynik.color = maksymalny_kolor.color;
		return wynik;
	}
	if(liczba_punktow >= 18){
		wynik.num = 2;
		wynik.color = 0;
	}
	if(maksymalny_kolor.num >= 7){
		wynik.num = 3;
		wynik.color = maksymalny_kolor.color;
		return wynik;
	}
	wynik.num = 0;
	wynik.color = 0;
	return wynik;
}

card system_z_grubsza(card cards[4][13],int player){//nie dziala
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

