#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define DEBUG 1

//to pewnie potrzebuje
struct card{
	int num;
	int color;
};
typedef struct card card;

char znaczek[][5] = {"BA", "kier", "karo", "pik", "trefl"};
char wartosc[][16] = {"0", "1 ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ", "8 ", "9 ", "10", "J ", "Q ", "K ", "A "};

void print(card karta){
	if(karta.num < 0 || karta.num > 15 || karta.color < 0 || karta.color > 4){
		printf("Karta nie pasuje, wartosci to : numer = %i, kolor = %i", karta.num, karta.color);
//		printf("      ");
		return;
	}
	printf("(%s %s)",wartosc[karta.num], znaczek[karta.color]);
}

void clear_screen(){//bedzie w wyswietlanie
	system("clear");
}

int wczytaj_kolor(char c){
	int kolor = -1;
	if(c == 'n' || c == 'N') kolor = 0;
	if(c == 'h' || c == 'H') kolor = 1;
	if(c == 'd' || c == 'D') kolor = 2;
	if(c == 's' || c == 'S') kolor = 3;
	if(c == 'c' || c == 'C') kolor = 4;
	if(kolor == -1){
		printf("\nBłędnie podany kolor, proszę podać jeszcze raz (sam kolor):\n");
		c = getchar();
		kolor = wczytaj_kolor(c);
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
		if((karty + i) -> color == 1){//kier
			kier[kie] = *(karty + i);
			kie++;
		}
		if((karty + i) -> color == 2){//karo
			karo[kar] = *(karty + i);
			kar++;
		}
		if((karty + i) -> color == 3){//pik
			pik[pi] = *(karty + i);
			pi++;
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
		if(i < pi) print(pik[i]);
		else{
			printf("        ");
		}
		printf(" ");
		if(i < tref) print(trefl[i]);
//		else{
//			printf("       ");
//		}
		printf("\n");
//		printf("%i %i %i %i \n ", kier[i], karo[i], pik[i], trefl[i]);
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
	printf("Liczba twoich punktow to: %i\n", policz_punkty(karty));
	printf("Na ten moment zalicytowane jest:\n");
	print(deal);
	printf("\n");
	printf("Jesli chcesz uzyskać podpowiedź do licytacji napisz -1");
	printf("\n");
}

extern void zalicytowano(card lic);

card wczytanie_licytacja(int number, card deal){
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
		return wczytanie_licytacja(number, deal);
	}
	//nie pass
	int color = -1;
	char c;
	if(scanf(" %c", &c) < 1){//if someone enters wrong thing
		printf("Niepoprawnie wpisany kolor, proszę podać jeszcze raz (h, d, s, c lub n)");
		scanf("%i", &number);
		return wczytanie_licytacja(number, deal);
	}
	color = wczytaj_kolor(c);
	if(color == -1){//blad wczytywania
		printf("Niepoprawnie wpisany kolor, proszę podać jeszcze raz (h, d, s, c lub n)");
		scanf("%i", &number);
		return wczytanie_licytacja(number, deal);
	}
	if(number < deal.num || (number == deal.num && color > deal.color)){
		printf("To jest mniej niż poprzednio, wiec nie mozna tego zalicytowac, sprobuj jeszcze raz: \n");
		scanf("%i", &number);
		return wczytanie_licytacja(number, deal);
	}
	wynik.num = number;
	wynik.color = color;
	zalicytowano(wynik);
	return wynik;
}

bool czy_nowa(){
	clear_screen();
	printf("Gra zakonczona\n");
	printf("Jeśli chcesz rozpocząć kolejną grę wpisz T, w przeciwnym wypadku wpisz inny znak\n");
	char z;
	scanf("%c",&z);
	if(z == '\n'){
		scanf("%c",&z);
	}
	return (z == 't' || z == 'T') ? true : false; 
}

extern bool sprawdz_karte(card *karty,int ile,card wynik,int poprzedni_kolor);

card wybor_karty(card *karty, int ile, int gracz, card karty_na_stole[], int n){
	printf("Kolej gracza %i\n", gracz + 1);
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
	wynik.color = wczytaj_kolor(kolor);
	if(DEBUG){
		printf("wybrano karte:\n");
		print(wynik);
		printf("\n");
	}
	int poprzedni_kolor = -1;
	if(n > 0){//bylo cos wczesniej dane
		poprzedni_kolor = karty_na_stole[0].color;
	}
	if(!sprawdz_karte(karty, ile, wynik, poprzedni_kolor)){
		printf("Niepoprawnie wybrano karte (na przyklad nie posiadasz takiej lub kolor jest niepoprawny), bedziesz musiał_ podać jeszcze raz\n");
		return wybor_karty(karty, ile, gracz, karty_na_stole, n);
	}
	return wynik;
}

