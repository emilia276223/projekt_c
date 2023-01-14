#include "struct.h"

//potrzebne globalne
int rozgrywajacy = 0;
int dziadek;

card auction_history[200];//maksymalna mozliwa liczba ruchow w licytacji
//pass == -1
int nr_odzywki = 0;//na nr nastepnej


//potrzebne tylko dla licytacji
int count_points(card *karty);//policz punkty w danych kartach
void print_last_4_auction(int gracz);//wyswietlenie ostatich 4 w licytacji
void zalicytowano(card lic);//zapisanie tego, co zostalo zalicytowane
card auction_user_input(int number, card deal);//wybranie przez uzytkownika co chce zalicytowac
void naturalny_odpowiedz_bot(card *karty, int *punkty_p, int *suma_p, int *max_z);//odpowiedz w systemie naturalnym dla bota (zeby wiedziec +-co zalicytowac)
void naturalny_odpowiedz(card *karty);//odpowiedz w naturalnym dla uzytkownika (czyli ladny opis + odp dla bota)
card naturalny_otwarcie(card *karty, card deal);//podpowiedz w naturalnym dla uzytkownika na otwarcie
int naturalny_odpowiedz_kolor(card *karty);//dla bota zeby wiedzial w jaki kolor grac
void podpowiedz_naturalny(card *karty,card deal);//odpowiednio wywola odpowiednia rzecz
card podpowiedz_naturalny_bot(card *karty, card deal);//odpowiednio wywola odpowiednia rzecz
card auction_for_4_players();//licytacja gdy jest 4 graczy
card auction_with_bot();//gdy 1 gracz i 3 boty
void auction_clear();//wyczyszczenie danych
void auction_information(int gracz, card *karty, card deal);
int ustaw_rozgrywajacego(int zaczynajacy_licytacje);


int count_points(card *karty){
	int suma = 0;
	for(int i = 0; i < 13; i++){
		if((karty + i) -> num == 14){//ace
			suma += 4;
		}
		if((karty + i) -> num == 13){//krol
			suma += 3;
		}
		if((karty + i) -> num == 12){//dama
			suma += 2;
		}
		if((karty + i) -> num == 11){//walet
			suma += 1;
		}
	}
	return suma;
}

void auction_information(int gracz, card *karty, card deal){
	printf("Kolej gracza %i\n", gracz + 1);
	printf("Wpisz odpowiednio jak wysoko chcesz zalicytować: (1 - 7) \noraz kolor (h - kiery, d - karo, s - pik, c - trefl, n - bez atutu), albo 0 jesli pass\n");
	printf("a to twoje karty :");
	show_cards(karty, 13, gracz);
	printf("Liczba twoich punktow to: %i\n", count_points(karty));
	printf("Na ten moment zalicytowane jest:\n");
	print(deal);
	printf("\n");
	printf("ostatnio zalicytowano:\n");
	print_last_4_auction(gracz);
	printf("Jesli chcesz uzyskać podpowiedź do licytacji napisz -1");
	printf("\n");
}

void print_last_4_auction(int gracz){
	//teraz kolej gracza
	for(int i = 1; i < 4; i++){
		if(nr_odzywki + i - 4 >= 0){
			printf("Gracz %i: ",(gracz + i) % 4 + 1);
			print(auction_history[nr_odzywki + i - 4]);
			printf("\n");
		}
	}
}

void zalicytowano(card lic){
	auction_history[nr_odzywki].num = lic.num;
	auction_history[nr_odzywki].color = lic.color;
	nr_odzywki++;
}

void auction_clear(){
	nr_odzywki = 0;
}

int ustaw_rozgrywajacego(int zaczynajacy_licytacje){//cos nie tak
	//wyswietlenie licytacji
	if(DEBUG) for(int i = 0; i < nr_odzywki; i++) printf("Zalicytowano: (%i, %i)\n", auction_history[i].num, auction_history[i].color);
	//odpalam tylko gdy deal jest, czyli jest cos zalicytowane
	printf("Nr_odzywki = %i \n", nr_odzywki);
	int rozgrywajacy = nr_odzywki - 1;//ostatnia
	
	//znajduje ostatnia zalicytowana rzecz (powinna byc 3 od konca ale lepiej sprawdzic)
	while(auction_history[rozgrywajacy].num == -1){
		rozgrywajacy--;
	}
	
	//ustalam co jest zalicytowane (kolor)
	int kolor = auction_history[rozgrywajacy].color;
	
	//znajduje kto z pary pierwszy go powiedzial
	int gracz = rozgrywajacy - 2;//bede przechodzic co 2 bo to sa co w tej parze sie zadzialo
	while(gracz >= 0){
		if(auction_history[gracz].color == kolor || auction_history[gracz].num > 0){//zalicytowal ten kolor (i to nie byl pass)
			rozgrywajacy = gracz;
		}
		gracz -= 2;
	}
	//teraz gracz jest ustawiony w dobrym miejscu (moment pierwszego zalicytowania danego koloru)
	//ustalam ktory to gracz
	return (rozgrywajacy - zaczynajacy_licytacje) % 4;
}

card naturalny_otwarcie(card *karty, card deal)
{
	//policzenie liczby punktow
	int liczba_punktow = count_points(karty);
	
	//policzenie ilosci kart w kolorach
	int kolory[5]={0};
	card wynik;
	for(int i = 0; i < 13; i++){
		kolory[(karty + i) -> color]++;
	}
	
	//znalezienie najdluzszego koloru
	card maksymalny_kolor;
	maksymalny_kolor.num = kolory[1];//0 to by bylo dla BA
	maksymalny_kolor.color = 1;
	for(int i = 2; i < 5; i++){
		if(maksymalny_kolor.num < kolory[i]){
			maksymalny_kolor.num = kolory[i];
			maksymalny_kolor.color = i;
		}
	}
	
	//sprawdzenie poprawnej odpowiedzi
	if(liczba_punktow <= 17 && liczba_punktow >= 12 && maksymalny_kolor.num >=5){
		wynik.num = 1;
		wynik.color = maksymalny_kolor.color;
	}
	else if(liczba_punktow <= 17 && liczba_punktow >= 12){//gdy zrownowazony rozklad
		wynik.num = 1;
		wynik.color = 4;
	}
	else if(liczba_punktow >= 18 && maksymalny_kolor.num >=5){
		wynik.num = 2;
		wynik.color = maksymalny_kolor.color;
	}
	else if(liczba_punktow >= 18){
		wynik.num = 2;
		wynik.color = 0;
	}
	else if(maksymalny_kolor.num >= 7){
		wynik.num = 3;
		wynik.color = maksymalny_kolor.color;
	}
	else{
		wynik.num = -1;//pass
		wynik.color = 0;
	}
	
	//sprawdzenie czy jest wieksza od poprzednio zalicytowanego
	if(deal.num < wynik.num || (deal.num == wynik.num && deal.color > wynik.color)) return wynik;
	
	//jesli nie to pass
	wynik.num = -1;
	wynik.color = 0;
	return wynik;
}

int naturalny_odpowiedz_kolor(card *karty)
{
	//sprawdzenie, w jaki kolor zalicytowal partner i ile ma w tym kolorze
	
	//podliczenie sumy w tym kolorze
	
	//podliczenie ile kart w max kolorze (jesli inny) - liczymy ze partner ma 2 (lub 3 jesli BA lub 1c)
	
	//zwrocenie tego, w ktorym jest wiecej
	return 0;
}

void naturalny_odpowiedz_bot(card *karty, int *punkty_p, int *suma_p, int *max_z)
{
	//policzenie punktow partnera (na podstawie pierwszej odzywki, najmniejsze mozliwe)
	int pierwsza_p = (nr_odzywki - 2) % 4;
	int punkty_partnera;
	if(auction_history[pierwsza_p].num == 1){
		if(auction_history[pierwsza_p].color != 0){//1 w kolor
			punkty_partnera = 12;
		}
		else{
			punkty_partnera = 15;
		}
	}
	if(auction_history[pierwsza_p].num == -1){//jesli pas
		punkty_partnera = 0;
	}
	if(auction_history[pierwsza_p].num == 2){//jesli 2
		punkty_partnera = 12;
	}
	if(auction_history[pierwsza_p].num == 3){
		if(auction_history[pierwsza_p].color != 0){//3 w kolor
			punkty_partnera = 7;
		}
		else{
			punkty_partnera = 26;
		}
	}
	
	//policzenie sumy punktow
	int suma_punktow;
	suma_punktow = punkty_partnera + count_points(karty);
	
	//okreslenie wysokosci docelowej na podstawie ilosci punktow
	int max_zagranie;
	if(suma_punktow < 22){//nie warto grac
		max_zagranie = 0;
	}
	if(suma_punktow >= 22 && suma_punktow < 24){//max 1
		max_zagranie = 1;
	}
	if(suma_punktow >= 24 && suma_punktow < 26){//max 2
		max_zagranie = 2;
	}
	if(suma_punktow >= 24 && suma_punktow < 26){//max 3
		max_zagranie = 3;
	}
	if(suma_punktow >= 26){//warto grac 4 lub wiecej
		max_zagranie = 4;
	}
	
	//zwrocenie wynikow
	*punkty_p = punkty_partnera;
	*suma_p = suma_punktow;
	*max_z = max_zagranie;
}

void naturalny_odpowiedz(card *karty){
	int punkty_p, suma_punktow, max_zagranie;
	
	naturalny_odpowiedz_bot(karty, &punkty_p, &suma_punktow, &max_zagranie);
	
	printf("punkty partnera to co najmniej %i punktow\n", punkty_p);
	printf("zatem razem macie co najmniej %i punktow\n", suma_punktow);
	if(max_zagranie == 0){
		printf("Z licytacji i Twoich kart wynika, że nie warto grać");
		return;
	}
	if(max_zagranie < 4){
		printf("Z licytacji i Twoich kart wynika, że możecie zagrać na wysokości %i.\n", max_zagranie);
		return;
	}
	printf("Z licytacji i Twoich kart wynika, że możecie zagrać na wysokości 4 lub więcej\n");
}

void podpowiedz_naturalny(card *karty, card deal)
{
	if(nr_odzywki <= 1){//nie bylo jeszcze zadnej informacji od gracza z pary
		card odp;
		odp = naturalny_otwarcie(karty, deal);
		print(odp);
	}
	else{
		naturalny_odpowiedz(karty);
	}
}


card podpowiedz_naturalny_bot(card *karty, card deal)//odpowiednio wywola odpowiednia rzecz
{	
	if(nr_odzywki <= 1){//nie bylo jeszcze zadnej informacji od gracza z pary
		card odp;
		odp = naturalny_otwarcie(karty, deal);
		print(odp);
		return odp;

	}
	else{ 
		int num;
		int a;//tych rzeczy nie potrzebuje, wiec moga sie nadpisac
		naturalny_odpowiedz_bot(karty, &a, &a, &num);//chce zagrac na wysokosci max_zagranie bo skoro mozna to czemu nie
		
		//sprawdzam w jaki kolor chce grac
		int color = naturalny_odpowiedz_kolor(karty);
		card odp;
		odp.num = num;
		odp.color = color;
		
		//sprawdzam czy odp wieksza niz do tej pory zalicytowane
		if(odp.num > deal.num || (odp.num == deal.num && odp.color < deal.color))
		{
			print(odp);
			return odp;
		}
		
		//w przeciwnym przypadku
		odp.num = -1;//pass
		print(odp);
		return odp;
	}
}

card auction_for_4_players()
{
	card deal;
	deal.num = 0;
	deal.color = 0;
	printf("\nLICYTACJA \n");
	int player = 0;//startujemu od 1
	int number;
	int count = 0;
	while(count < 3 || (count < 4 && deal.num == 0)){
		clear_screen();
		auction_information(player, &cards[player][0], deal);
		scanf("%i", &number);
		if(number == -1){
			printf("\nTwoja podpowiedz do licytacji w systemie naturalnym to:\n");
			podpowiedz_naturalny(&cards[player][0], deal);
			printf("\nCo chcesz zalicytować?\n");
			scanf("%i", &number);
		}
		card zalicytowane = auction_user_input(number, deal);
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
	return deal;
}

card auction_with_bot()
{
	card deal;
	deal.num = 0;
	deal.color = 0;
	int player = 0;//startujemu od 1
	int number;
	int count = 0;
	while(count < 3 || (count < 4 && deal.num == 0))//dopoki nie ma 3 pasow z rzedu lub 4 jesli nic nie bylo zalicytowane
	{
		clear_screen();
		if(player == 0)//kiedy kolej gracza
		{
			auction_information(player, &cards[player][0], deal);
			scanf("%i", &number);
			if(number == -1)//podpowiedz
			{
				printf("\nTwoja podpowiedz do licytacji w systemie naturalnym to:\n");
				podpowiedz_naturalny(&cards[player][0], deal);
				printf("\nCo chcesz zalicytować?\n");
				scanf("%i", &number);
			}
			card zalicytowane = auction_user_input(number, deal);
			if(zalicytowane.num == -1){//pass
				count++;
			}
			else{
				count = 0;
				deal = zalicytowane;
			}
		}
		
		else//jesli kolej bota
		{
			printf("Gracz %i: ", player + 1);
			card odp = podpowiedz_naturalny_bot(&cards[player][0], deal);
			if(odp.num == -1){//pass
				count++;
			}
			else{
				count = 0;
				deal = odp;
			}
			zalicytowano(odp);
		}
		
//		zalicytowano(zalicytowane);
		
		player = (player + 1)%4;
		clear_screen();
		getchar();
	}
	return deal;
}

card auction_user_input(int number, card deal){
	card wynik;
	if(number == 0){//pass
		wynik.num = -1;
		wynik.color = -1;
		zalicytowano(wynik);
		return wynik;
	}
	//jesli niepoprawny numer
	if(number < 0 || number > 7){
		printf("Niepoprawnie wpisana liczba, proszę podać jeszcze raz ( 0 - pass lub liczba z zakresu 1 - 7)");
		scanf("%i", &number);
		return auction_user_input(number, deal);
	}
	//nie pass
	int color = -1;
	char c;
	if(scanf(" %c", &c) < 1){//if someone enters wrong thing
		printf("Niepoprawnie wpisany kolor, proszę podać jeszcze raz (h, d, s, c lub n)");
		scanf("%i", &number);
		return auction_user_input(number, deal);
	}
	color = input_color(c);
	if(color == -1){//blad wczytywania
		printf("Niepoprawnie wpisany kolor, proszę podać jeszcze raz (h, d, s, c lub n)");
		scanf("%i", &number);
		return auction_user_input(number, deal);
	}
	if(number < deal.num || (number == deal.num && color > deal.color)){
		printf("To jest mniej niż poprzednio, wiec nie mozna tego zalicytowac, sprobuj jeszcze raz: \n");
		scanf("%i", &number);
		return auction_user_input(number, deal);
	}
	wynik.num = number;
	wynik.color = color;
	zalicytowano(wynik);
	return wynik;
}

