#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>


#define SLEEP_TIME 50000

#define DEBUG 1
//to pewnie potrzebuje
struct card{
	int num;
	int color;
};
typedef struct card card;


//it will always be: N is dealer, S is waiting, game starts from 
card cards[4][13];//[i][j][k] - i-ty gracz, [j] j - ta karta
card all_cards[52];

int score[4];//score of players

//card deal;//first - number (1 - 7), second - color
//colors:
//0 - no trump
//1 - hearts
//2 - diamonds
//3 - spades
//4 - clubs

int rozgrywajacy = 0;
int dziadek;

char znaczek[][5] = {"BA", "pik", "kier", "karo", "trefl"};
char wartosc[][16] = {"0", "1 ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ", "8 ", "9 ", "10", "J ", "Q ", "K ", "A "};

card auction_history[200];//maksymalna mozliwa liczba ruchow w licytacji
//pass == -1
int nr_odzywki = 0;//na nr nastepnej

//zadeklarowanie funkcji

//komunikacja z urzytkownikiem
void print(card karta);
void clear_screen();
int input_color(char c);
void show_cards(card *karty, int ile, int numer_gracza);
void show_deal(card *karty);
void auction_information(int gracz, card *karty, card deal);

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

//obsluga kolejnosci gry
bool if_new_game();
int runda_dla_4_graczy(int dealer, int n, int atut, int *score);//zwracam kto zebral lewe
int runda_z_botem(int dealer, int n, int atut, int *score);//zwracam kto zebral lewe
void game(int atut, int *score, int tryb_gry);
void new_game();
int ustaw_rozgrywajacego(int zaczynajacy_licytacje);

//ustawianie kart itp
void shuffling(card *talia);//potasowanie kart
void dealing_cards();//rozlozenie kart na 4 graczy
void sort_by_num(card *tab, int ile);//posortowanie po wielkosci kart
void sort_by_color(card *tab, int ile);//posortowanie po kolorach
void generate_cards(card *all_cards);//wygenerowanie talii

//tylko do rogzrywki
card choose_card(card *karty, int ile, int gracz, card karty_na_stole[], int n);//wybranie przez uzytkownika co chce zagrac
card choose_card_bot(card *karty, int ile, card karty_na_stole[4], int n, int atut);//wybranie karty przez bota
int find_winner(card karty_na_stole[4], int atut);//sprawdz kto zbiera dana lewe
void find_winner_test();//test 
void informacje_rozgrywka();//info dla uzytkownika co wpisywac
bool check_card(card *karty,int ile,card wynik,int poprzedni_kolor);//sprawdz czy mozna zagrac te karte (czy ma w kartach oraz czy jest do koloru lub czy nie ma do koloru)
void show_last_trick();//ostatnia lewa
void remove_card(card *karty, int i);//usuniecie karty nr i

void print(card karta){
	if(karta.num == -1){
		printf("pass");
		return;
	}
	if(karta.num < 0 || karta.num > 15 || karta.color < 0 || karta.color > 4){
		printf("Karta nie pasuje, wartosci to : numer = %i, kolor = %i", karta.num, karta.color);
		return;
	}
	printf("(%s %s)",wartosc[karta.num], znaczek[karta.color]);
}

void clear_screen(){//bedzie w wyswietlanie
	if(DEBUG) printf("\n\n\n\n\n\n\n");
	else system("clear");
}

int input_color(char c){
	int kolor = -1;
	if(c == 'n' || c == 'N') kolor = 0;
	if(c == 'h' || c == 'H') kolor = 2;
	if(c == 'd' || c == 'D') kolor = 3;
	if(c == 's' || c == 'S') kolor = 1;
	if(c == 'c' || c == 'C') kolor = 4;
	if(kolor == -1){
		printf("\nBłędnie podany kolor, proszę podać jeszcze raz (sam kolor):\n");
		c = getchar();
		kolor = input_color(c);
	}
	return kolor;
}

void show_cards(card *karty, int ile, int numer_gracza){
	printf("\nKarty gracza %i:\n", numer_gracza + 1);
	//chce wyswietlic w 4 rzedach
	card kier[13];
	card karo[13];
	card pik[13];
	card trefl[13];
	int kie = 0, kar = 0, pi = 0, tref = 0;
	
	for(int i = 0; i < ile; i++){
		kier[i].num = 0;
		kier[i].color = 0;
		karo[i].num = 0;
		karo[i].color = 0;
		pik[i].num = 0;
		pik[i].color = 0;
		trefl[i].num = 0;
		trefl[i].color = 0;
	}
	for(int i = 0; i < ile; i++){
		if((karty + i) -> color == 1){//pik
			pik[pi] = *(karty + i);
			pi++;
		}
		if((karty + i) -> color == 2){//kier
			kier[kie] = *(karty + i);
			kie++;
		}
		if((karty + i) -> color == 3){//karo
			karo[kar] = *(karty + i);
			kar++;
		}
		if((karty + i) -> color == 4){//trefl
			trefl[tref] = *(karty + i);
			tref++;
		}
	}
	int max = kie;
	if(kar > max) max = kar;
	if(pi > max) max = pi;
	if(tref > max) max = tref;
	for(int i = 0; i < max; i++){
		if(i < pi) print(pik[i]);
		else{
			printf("        ");
		}
		printf(" ");
		if(i < kie) print(kier[i]);
		else{
			printf("         ");
		}
		printf(" ");
		if(i < kar) print(karo[i]);
		else{
			printf("         ");
		}
		printf(" ");
		if(i < tref) print(trefl[i]);
		printf("\n");
	}
	printf("\n");
}

void show_deal(card *karty){
	show_cards(karty, 13, 0);
	show_cards(karty + 13, 13, 1);
	show_cards(karty + 26, 13, 2);
	show_cards(karty + 39, 13, 3);
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

bool if_new_game(){
	clear_screen();
	printf("Gra zakonczona\n");
	printf("Jeśli chcesz rozpocząć kolejną grę wpisz T, w przeciwnym wypadku wpisz inny znak\n");
	int z;
	z = getchar();
	if(z == '\n' || z == ' '){
		z = getchar();
	}
	getchar();
	return (z == 't' || z == 'T') ? true : false; 
}

card choose_card(card *karty, int ile, int gracz, card karty_na_stole[], int n){

	//pokazanie kart gracza i co jest na stole
	show_cards(karty, ile, gracz);
	if(n > 0){
		printf("Na stole są: \n");
		for(int i = 0; i < n; i++){
			print(karty_na_stole[i]);
		}
	}
	else{
		printf("Wistujesz:\n");
	}
	
	//wczytanie jaka katre chce zagrac
	printf("\nWybierz, ktorą kartę chcesz zagrać\n");
	char numer = 0;
	scanf("%c", &numer);
	if(numer == '\n'){
		scanf("%c", &numer);
	}
	printf("Wczytano numer %c\n", numer);
	if(numer == '1') getchar();//zabieram 0
	char kolor;
	while(scanf(" %c",&kolor) < 1){
		printf("\nBłąd przy podaniu karty, proszę spróbować jeszcze raz\n");
	}
	card wynik;
	if(numer >= '2' && numer <= '9'){//2 - 9
		wynik.num = numer - '0';
	}
	if(numer == '1'){//10
		wynik.num = 10;
	}
	if(numer == 'J' || numer == 'j'){//10
		wynik.num = 11;
	}
	if(numer == 'Q' || numer == 'q'){//10
		wynik.num = 12;
	}
	if(numer == 'K' || numer == 'k'){//10
		wynik.num = 13;
	}
	if(numer == 'A' || numer == 'a'){//10
		wynik.num = 14;
	}
	wynik.color = input_color(kolor);
	
	//wyswietlenie wyboru
	printf("wybrano karte:\n");
	print(wynik);
	printf("\n");
	
	//sprawdzenie czy taka moze wybrac
	int poprzedni_kolor = -1;
	if(n > 0){//bylo cos wczesniej dane
		poprzedni_kolor = karty_na_stole[0].color;
	}
	if(!check_card(karty, ile, wynik, poprzedni_kolor)){
		printf("Niepoprawnie wybrano karte (na przyklad nie posiadasz takiej lub kolor jest niepoprawny), bedziesz musiał_ podać jeszcze raz\n");
		return choose_card(karty, ile, gracz, karty_na_stole, n);
	}
	
	//zwrocenie wyniku jesli wszystko ok
	return wynik;
}

void generate_cards(card *all_cards){//dziala
	for(int i = 0; i < 52; i ++){
		(all_cards + i) -> color = i/13 + 1;
		(all_cards + i) -> num = i%13 + 2;
	}
}

void sort_by_num(card *tab, int ile){//od najmniejszej do najwiekszej
	//bubble sort bo czemu nie skoro i tak malo
	for(int i = 0; i < ile; i ++){
		for(int j = 1; j < ile - i; j ++){
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
	for(int i = 0; i < ile; i ++){
		for(int j = 1; j < ile - i; j ++){
			if((tab + j - 1) -> color > (tab + j) -> color || ((tab + j- 1) -> num < (tab + j) -> num && (tab + j - 1) -> color == (tab + j) -> color)){
				card temp = *(tab + j - 1);
				*(tab + j - 1) = *(tab + j);
				*(tab + j) = temp;
			}
		}
	}
}


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


void shuffling(card *talia){//tasuje 52 karty
	card temp[52];
	for(int i = 0; i < 52; i++){
		temp[i].num = -1;
	}
	int miejsce;
	for(int i = 0; i < 52; i++){
		miejsce = rand() % (52 - i);//liczba od 0 do 52 - i
		int j = 0;
		while(miejsce > 0){
			if(temp[j].num == -1) miejsce--;
			j++;
			j %= 52;
		}
		while(temp[j].num != -1) j++;
		j %= 52;
		temp[j].num = (talia + i) -> num;
		temp[j].color = (talia + i) -> color;
	}
	for(int i = 0; i < 52; i++){
		(talia + i) -> num = temp[i].num;
		(talia + i) -> color = temp[i].color;
	}
	if(DEBUG) printf("Rozdanie :");
	if(DEBUG) show_deal(talia);
}

void remove_card(card *karty, int i)//usuniecie karty nr i
{
	for(int k = i; k < 12; k++)//nadpisanie dalszymi kartami
	{
		(karty + k) -> num = (karty + k + 1) -> num;
		(karty + k) -> color = (karty + k + 1) -> color;
	}
}

bool check_card(card *karty,int ile,card wynik,int poprzedni_kolor){
	//sprawdzam czy ma te karte
	int znalezione = -1;
	for(int i = 0; i < ile; i++){
		if((karty + i) -> num == wynik.num && (karty + i) -> color == wynik.color){//ta karta
			znalezione = i;
		}
	}
	if(znalezione == -1) return false;
	if(wynik.color != poprzedni_kolor){//jesli wynik jest w kartach ale ma inny kolor niz pierwsza w lewie
		bool znalezione_2 = false;
		for(int i = 0; i < ile; i++){
			if((karty + i) -> color == poprzedni_kolor){
				znalezione_2 = true;
				break;
			}
		}
		//jesli ma karte w kolorze pierwszej a dal inna
		if(znalezione_2) return false;
	}
	
	//jesli karta poprawna
	remove_card(karty, znalezione);
	return true;
}

void find_winner_test(){
	printf("Wpisz 4 karty");
	card karty[4];
	char kolor;
	for(int i = 0; i < 4; i++){
		scanf("%i %c", &karty[i].num, &kolor);
		karty[i].color = input_color(kolor);
	}
	printf("Podaj atut");
	scanf("%c", &kolor);
	int atut = input_color(kolor);
	printf(" wygral: %i",find_winner(karty, atut));
}

int find_winner(card karty_na_stole[4], int atut){
	int kolor = karty_na_stole[0].color;
	int wygrywa = 0;//ma zawsze atutowy lub kolor
	for(int i = 1; i < 4; i++){
		if(karty_na_stole[i].num > karty_na_stole[wygrywa].num){
			if(karty_na_stole[wygrywa].color == kolor && karty_na_stole[i].color == kolor){//jesli w tym samym kolorze
				wygrywa = i;
			}
			else{//jesli maja w innym kolorze
				if(karty_na_stole[wygrywa].color == kolor && karty_na_stole[i].color == atut){
					wygrywa = i;
				}
				else{//
					if(karty_na_stole[wygrywa].color == atut && karty_na_stole[i].color == atut){
						wygrywa = i;
					}
					else{
						//poprzedni ma w atutowym a drugi nie wiec zostaje jak bylo
					}
				}
			}
		}
		else{//i nie ma przewagi wysokosci karty
			if(karty_na_stole[wygrywa].color != atut && karty_na_stole[i].color == atut){//ale ma przewage bo atut
				wygrywa = i;
			}
			//w innym pzypadku zostaje jak bylo
		}
	}
	return wygrywa;
}

void auction_clear(){
	nr_odzywki = 0;
}

void zalicytowano(card lic){
	auction_history[nr_odzywki].num = lic.num;
	auction_history[nr_odzywki].color = lic.color;
	nr_odzywki++;
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

void dealing_cards()
{
	//randomised cards
	shuffling(&all_cards[0]);
	
	//podzielenie na graczy
	for(int i = 0; i < 52; i ++){
		cards[i/13][i%13] = all_cards[i];
	}
	
	//potasowanie kart kolorami
	sort_by_color(&cards[0][0], 13);
	sort_by_color(&cards[1][0], 13);
	sort_by_color(&cards[2][0], 13);
	sort_by_color(&cards[3][0], 13);
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

void informacje_rozgrywka()
{
	printf("Wpisz odpowiednio jaką kartę chcesz dać: (2 - A) \noraz kolor (h - kiery, d - karo, s - pik, c - trefl, n - bez atutu), albo 0 jesli pass\n");
}

int runda_dla_4_graczy(int dealer, int n, int atut, int *score)//zwracam kto zebral lewe
{
	//n - n-ta runda
	int player = dealer;
	card karty_na_stole[4];
	for(int i = 0; i < 4; i++){
		clear_screen();
		informacje_rozgrywka();
		if(player == dziadek)//jesli dziadek to pokazuje karty rozgrywajacego i dziadka
		{
			printf("Karty rozgrywajacego:");
			show_cards(&cards[rozgrywajacy][0], 13 - n, rozgrywajacy);
			karty_na_stole[i] = choose_card(&cards[dziadek][0], 13 - n, dziadek, karty_na_stole, i);
		}
		else{
			if(n == 0 && player == dealer)//gracz nie widzie jeszcze kart dziadka bo to 0-wa runda
			{
				karty_na_stole[i] = choose_card(&cards[player][0], 13 - n, player, karty_na_stole, i);
			}
			else
			{
				printf("Karty dziadka:");
				show_cards(&cards[dziadek][0], 13 - n, dziadek);
				karty_na_stole[i] = choose_card(&cards[player][0], 13 - n, player, karty_na_stole, i);	
			}
		}
		//ustawienie kolejnego gracza
		player = (player + 1) % 4;
		
		getchar();
		getchar();
		clear_screen();
		getchar();
		printf("Ruch gracza poprzedniego zakończony, kolejny ruch będzie gracza %i\n", player + 1);
		printf("Wcisnij ENTER, żeby rozpoczac ruch gracza %i:\n", player + 1);
		getchar();
		
	}
	int new_dealer;
	new_dealer = (dealer + find_winner(karty_na_stole, atut))%4;
	*(score + new_dealer) += 1;
	return new_dealer;
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

void new_game()
{
	//wyczyszczenie danych
	int score[4] = {0};
	auction_clear();
	
	//utworzenie nowych kart
	generate_cards(&all_cards[0]);
	dealing_cards();
	clear_screen();
	
	//ustaleniu trybu gry
	int tryb_gry = 0;//4 graczy
	printf("jesli chcesz zagrac z komputerem wpisz K");
	char a = getchar();
	if(a == ' ' || a == '\n') getchar();
	if(a == 'k' || a == 'K') tryb_gry = 1;//z komputerem
	
	//nowa licytacja
	clear_screen();
	card deal;
	if(tryb_gry == 0)//4 graczy
	{
		deal = auction_for_4_players();
	}
	else
	{
		deal = auction_with_bot();
	}
	//sprawdzenie czy bedzie rozgrywka
	if(deal.num == 0){
		printf("Licytacja zakonczona niepowodzeniem\n\n");
		return;
	}
	//ustawienia poczatkowe rozgrywki
	rozgrywajacy = ustaw_rozgrywajacego(0);//bo na razie licytacje zawsze rozpoczyna gracz 0
	dziadek = (rozgrywajacy + 2) % 4;
	
	int atut = deal.color;
	clear_screen();
	
	//rozpoczecie rozgrywki
	game(atut, &score[0], tryb_gry);
	
	//wyswietlenie wynikow
	printf("Gra zakonczona, wyniki to: %i, %i, %i, %i, tryb_gry = %i", score[0], score[1], score[2], score[3], tryb_gry);
	//trzeba zrobic sprawdzenie czy ugrane
}

card choose_card_bot(card *karty, int ile, card karty_na_stole[4], int n, int atut)
{
	//wyswietlenie kart na stole
	printf("Na stole są: \n");
	for(int i = 0; i < n; i++){
		print(karty_na_stole[i]);
	}
	printf("\n");
	
	//wybranie karty do zagrania
	card wynik;
	if(DEBUG) printf("a\n");
	
	//sprawdzenie poprzedniego koloru
	int poprzedni_kolor = -1;
	if(n > 0){//bylo cos wczesniej dane
		poprzedni_kolor = karty_na_stole[0].color;
	}
	if(DEBUG) printf("b\n");
	
	//jesli pierwsza karta w tej turze
	if(n == 0)
	{
		int x = rand() % ile;
		if(DEBUG) printf("wybrano karte nr %i\n", x);
		wynik = *(karty + x);
		remove_card(karty, x);
		return wynik;
	}
	
	
	//jesli juz cos bylo
	
	//sprawdzenie czy ma karte w tym kolorze
	bool czy_ma_w_kolorze = false;
	int pierwsza = -1;//wiem gdzie pierwsza
	int ostatnia;//wiem ile ich jest
	for(int i = 0; i < ile; i++)
	{
		if((karty + i) -> color == poprzedni_kolor)//jesli i-ta karta jest w kolorze
		{
			czy_ma_w_kolorze = true;
			if(pierwsza == -1) pierwsza = i;
			ostatnia = i;
		}
	}
	if(DEBUG)
	{
		printf("sprawdzone czy ma w kolorze:\n");
		if(czy_ma_w_kolorze) printf("TAK\n");
		else printf("NIE\n");
	}
	
	//jesli ma karte w tym kolorze to losuje jedna z nich
	if(czy_ma_w_kolorze)
	{
		int x = rand() % (ostatnia - pierwsza + 1);
		x += pierwsza;
		
		printf("x = %i \n", x);
		//sprawdzenie czy karta istnieje
		if(DEBUG && (x < 0 || x >= ile))
		{
			printf("miejsce karty nie istnieje (ma w kolorze, x = %i) \n", x);
			wynik.num = 0;
			wynik.color = 0;
			return wynik;
		}
		
		wynik = *(karty + x);
		remove_card(karty, x);
		return wynik;
	}
	
	if(DEBUG) printf("nie ma w kolorze\n");
	
	//jesli nie ma karty w tym kolorze to atut jesli ma (i losuje)
	bool czy_ma_atut = false;
	int pierwsza_a = -1;//wiem gdzie pierwsza
	int ostatnia_a;//wiem ile ich jest
	for(int i = 0; i < ile; i++)
	{
		if((karty + i) -> color == poprzedni_kolor)
		{
			czy_ma_atut = true;
			if(pierwsza_a == -1) pierwsza = i;
			ostatnia_a = i;
		}
	}
	
	if(DEBUG)
	{
		printf("sprawdzone czy ma w atucie:\n");
		if(czy_ma_atut) printf("TAK\n");
		else printf("NIE\n");
	}
	
	if(czy_ma_atut)//jesli ma atut to ktoras z nich wybieram
	{
		int x = rand() % (ostatnia_a - pierwsza_a + 1);
		x += pierwsza_a;
		
		//sprawdzenie czy karta istnieje
		if(DEBUG && (x < 0 || x >= ile))
		{
			printf("miejsce karty nie istnieje (atut, x = %i)\n", x);
			wynik.num = 0;
			wynik.color = 0;
			return wynik;
		}
		
		wynik = *(karty + x);
		return wynik;
	}
	
	if(DEBUG) printf("nie ma w atucie\n");
	
	//jesli nie ma koloru ani atutu to najnizsza z tych, ktore ma
	int najnizsza = 15;
	int miejsce;
	for(int i = 0; i < ile; i++)
	{
		if((karty + i) -> num < najnizsza)//jesli najmniejsza do tej pory
		{
			najnizsza = (karty + i) -> num;
			miejsce = i;
		}
	}
	
	//sprawdzenie czy istnieje
	if(DEBUG && (miejsce < 0 || miejsce >= ile))
	{
		printf("miejsce karty nie istnieje (najnizsza, miejsce = %i)\n", miejsce);
		wynik.num = 0;
		wynik.color = 0;
		return wynik;
	}
	
	wynik =  *(karty + miejsce);
	remove_card(karty, miejsce);
	return wynik;
}

int runda_z_botem(int dealer, int n, int atut, int *score)//zwracam kto zebral lewe
{
	//plan na przyszlosc:
	//bot wybiera najnizsza w kolorze jesli nie ma wyzszej a wyzsza (ale losowa) jak bije	
	
	
	//n - n-ta runda
	//gracz 1 to urzytkownik
	int player = dealer;
	card karty_na_stole[4] = {0,0,0,0};
	for(int i = 0; i < 4; i++){
		clear_screen();
		
		//jesli kolej uzytkowina i nie jest on dziadkiem
		if(player == 0 && player != dziadek)//jesli teraz kolej uzytkownika
		{
			informacje_rozgrywka();
			if(n == 0 && player == dealer)//gracz nie widzie jeszcze kart dziadka bo to 0-wa runda
			{
				karty_na_stole[i] = choose_card(&cards[player][0], 13 - n, player, karty_na_stole, i);
			}
			else
			{
				printf("Karty dziadka:\n");
				show_cards(&cards[dziadek][0], 13 - n, dziadek);
				karty_na_stole[i] = choose_card(&cards[player][0], 13 - n, player, karty_na_stole, i);	
			}
		}
		
		//jesli gracz jest dziadkiem
		else if(player == 0 && player == dziadek)
		{
			printf("jestes dziadkiem, wiec rozgrywajacy wybiera twoja karte\n");
			karty_na_stole[i] = choose_card_bot(&cards[dziadek][0], 13 - n, karty_na_stole, i, atut);
//			usleep(SLEEP_TIME);
		}
		
		//jesli gracz jest rozgrywajacym i kolej dziadka
		else if(player == 2 && rozgrywajacy == 0)
		{
			printf("jestes rozgrywajacym wiec mozesz wybrac:\n");
			printf("Twoje karty:");
			show_cards(&cards[0][0], 13 - n, 0);
			karty_na_stole[i] = choose_card(&cards[player][0], 13 - n, player, karty_na_stole, i);
		}
		
		//jesli kolej bota
		else//w pozostalych przypadkach
		{
			//tutaj nie ma znaczenia czy ktos jest dziadkiem czy rozgrywajacym, bo i tak losuje karte
			//bot i tak nie sugeruje sie innymi kartami		
			printf("Ruch gracza %i\n", player + 1);		
			karty_na_stole[i] = choose_card_bot(&cards[player][0], 13 - n, karty_na_stole, i, atut);
			printf("\nGracz %i wybrał kartę :", player + 1);
			print(karty_na_stole[i]);
//			usleep(SLEEP_TIME);
		}
		
		//ustawienie kolejnego gracza
		player = (player + 1) % 4;
		getchar();
		clear_screen();
//		getchar();
//		printf("Ruch gracza poprzedniego zakończony, kolejny ruch będzie gracza %i\n", player + 1);
//		printf("Wcisnij ENTER, żeby rozpoczac ruch gracza %i:\n", player + 1);
		
	}
	int new_dealer;
	new_dealer = (dealer + find_winner(karty_na_stole, atut))%4;
	*(score + new_dealer) += 1;
	return new_dealer;
}

void game(int atut, int *score, int tryb_gry)
{
	//ustawienie dealera
	int dealer = (rozgrywajacy + 1) % 4;
	
	//jesli gra 4 graczy
	if(tryb_gry == 0)
	{
		for(int i = 0; i < 13; i++)
		{
			//dla kazdej rundu rozpoczenie z ustalonym wczesniej dealerem
			dealer = runda_dla_4_graczy(dealer,i, atut, score);
		}
	}
	
	else//1 gracz i 4 razy bot
	{
		for(int i = 0; i < 13; i++){
			dealer = runda_z_botem(dealer,i, atut, score);
		}
//		printf("jeszcze nie ma tej opcji gry");
	}
}

void show_last_trick()//ostatnia lewa
{
	printf("Ta funkcja nie jest jeszcze dostepna");
}
