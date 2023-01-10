#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


//to pewnie potrzebuje
struct card{
	int num;
	int color;
};
typedef struct card card;


//it will always be: N is dealer, S is waiting, game starts from 
card cards[4][13];//[i][j][k] - i-ty gracz, [j] j - ta karta
card wszystkie_karty[52];

int score[4];//score of players

card deal;//first - number (1 - 7), second - color
//colors:
//0 - no trump
//1 - hearts
//2 - diamonds
//3 - spades
//4 - clubs


int rozgrywajacy = 0;
int dziadek;
int atut;

char znaczek[][5] = {"BA", "pik", "kier", "karo", "trefl"};
char wartosc[][16] = {"0", "1 ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ", "8 ", "9 ", "10", "J ", "Q ", "K ", "A "};

card historia_licytacji[200];//maksymalna mozliwa liczba ruchow w licytacji
int nr_odzywki = 0;//na nr nastepnej

//zadeklarowanie funkcji
void print(card karta);
void clear_screen();
int wczytaj_kolor(char c);
void show_cards(card *karty, int ile, int numer_gracza);
void wyswietl_rozdanie(card *karty);
int policz_punkty(card *karty);
void print_4_ostatnie_licytacja(int gracz);
void info_dla_gracza_licytacja(int gracz, card *karty, card deal);
void zalicytowano(card lic);
card wczytanie_licytacja(int number, card deal);
bool czy_nowa();
bool sprawdz_karte(card *karty,int ile,card wynik,int poprzedni_kolor);
card wybor_karty(card *karty, int ile, int gracz, card karty_na_stole[], int n);
void generate_cards(card *wszystkie_karty);
void sort_by_num(card *tab, int ile);
void sort_by_color(card *tab, int ile);
int policz_punkty(card *karty);
void tasowanie(card *talia);
bool sprawdz_karte(card *karty,int ile,card wynik,int poprzedni_kolor);
int ustal_wygrana(card karty_na_stole[4], int atut);
void wyczysc_licytacja();
int ustaw_rozgrywajacego(int zaczynajacy_licytacje);
card naturalny_otwarcie(card *karty);
void dealing_cards();
card naturalny_odpowiedz(card *karty,int liczba_punktow, card poprzedni);//dokonczyc
void auction();
int runda(int dealer, int n);
void new_game();
void gra();

void print(card karta){
	if(karta.num == -1){
		printf("pass");
		return;
	}
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
	if(c == 'h' || c == 'H') kolor = 2;
	if(c == 'd' || c == 'D') kolor = 3;
	if(c == 's' || c == 'S') kolor = 1;
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



void info_dla_gracza_licytacja(int gracz, card *karty, card deal){
	printf("Kolej gracza %i\n", gracz + 1);
	printf("Wpisz odpowiednio jak wysoko chcesz zalicytować: (1 - 7) \noraz kolor (h - kiery, d - karo, s - pik, c - trefl, n - bez atutu), albo 0 jesli pass\n");
	printf("a to twoje karty :");
	show_cards(karty, 13, gracz);
	printf("Liczba twoich punktow to: %i\n", policz_punkty(karty));
	printf("Na ten moment zalicytowane jest:\n");
	print(deal);
	printf("\n");
	printf("ostatnio zalicytowano:\n");
	print_4_ostatnie_licytacja(gracz);
	printf("Jesli chcesz uzyskać podpowiedź do licytacji napisz -1");
	printf("\n");
}

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
	int z;
	z = getchar();
//	scanf("%c",&z);
	if(z == '\n' || z == ' '){
//		scanf("%c",&z);
		z = getchar();
	}
	return (z == 't' || z == 'T') ? true : false; 
}

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
	
		printf("wybrano karte:\n");
		print(wynik);
		printf("\n");
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

void generate_cards(card *wszystkie_karty){//dziala
	for(int i = 0; i < 52; i ++){
		(wszystkie_karty + i) -> color = i/13 + 1;
		(wszystkie_karty + i) -> num = i%13 + 2;
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


int policz_punkty(card *karty){
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




void tasowanie(card *talia){//tasuje 52 karty
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
//	printf("Rozdanie :");
//	wyswietl_rozdanie(talia);
}

bool sprawdz_karte(card *karty,int ile,card wynik,int poprzedni_kolor){
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
	for(int i = znalezione; i < ile; i++){
		//pzepisuje kolejna karte
		(karty + i) -> num = (karty + i + 1) -> num;
		(karty + i) -> color = (karty + i + 1) -> color;
	}
	return true;
}

int ustal_wygrana(card karty_na_stole[4], int atut){
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

void wyczysc_licytacja(){
	nr_odzywki = 0;
}

void zalicytowano(card lic){
	historia_licytacji[nr_odzywki].num = lic.num;
	historia_licytacji[nr_odzywki].color = lic.color;
	nr_odzywki++;
}

int ustaw_rozgrywajacego(int zaczynajacy_licytacje){//cos nie tak
	for(int i = 0; i < nr_odzywki; i++){
		printf("Zalicytowano: (%i, %i)\n", historia_licytacji[i].num, historia_licytacji[i].color);
	}
	//odpalam tylko gdy deal jest, czyli jest cos zalicytowane
	printf("Nr_odzywki = %i \n", nr_odzywki);
	int rozgrywajacy = nr_odzywki - 1;//ostatnia
	//znajduje ostatnia zalicytowana rzecz (powinna byc 3 od konca ale lepiej sprawdzic)
	while(historia_licytacji[rozgrywajacy].num == -1){
		rozgrywajacy--;
	}
	//ustalam co jest zalicytowane (kolor)
	int kolor = historia_licytacji[rozgrywajacy].color;
	//znajduje kto z pary pierwszy go powiedzial
	int gracz = rozgrywajacy - 2;//bede przechodzic co 2 bo to sa co w tej parze sie zadzialo
	while(gracz >= 0){
		if(historia_licytacji[gracz].color == kolor || historia_licytacji[gracz].num > 0){//zalicytowal ten kolor (i to nie byl pass)
			rozgrywajacy = gracz;
		}
		gracz -= 2;
	}
	//teraz gracz jest ustawiony w dobrym miejscu (moment pierwszego zalicytowania danego koloru)
	//ustalam ktory to gracz
	return (rozgrywajacy - zaczynajacy_licytacje) % 4;
}

void print_4_ostatnie_licytacja(int gracz){
	//teraz kolej gracza
	for(int i = 1; i < 4; i++){
		if(nr_odzywki + i - 4 >= 0){
			printf("Gracz %i: ",(gracz + i) % 4 + 1);
			print(historia_licytacji[nr_odzywki + i - 4]);
			printf("\n");
		}
	}
}

card naturalny_otwarcie(card *karty){
	int liczba_punktow = policz_punkty(karty);
//	printf("Liczba punktów to : %i\n",liczba_punktow);
	int kolory[5]={0};
	card wynik;
	for(int i = 0; i < 13; i++){
		kolory[(karty + i) -> color]++;
	}
//	for(int i = 1; i < 5; i++){
//		printf("Jest %i kart w kolorze %i\n",kolory[i],i);
//	}
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
	wynik.num = -1;//pass
	wynik.color = 0;
	return wynik;
}

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


void gra(){
	int dealer = (rozgrywajacy + 1) % 4;
	for(int i = 0; i < 13; i++){
		dealer = runda(dealer,i);
	}
}

void show_last_trick(){//ostatnia lewa
	//
}

