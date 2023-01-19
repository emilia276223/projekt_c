#include "auction.h" //w tym struct.h

//potzebne do pokazania ostatniej lewy
card last_trick[4];
void save_last_trick(card karty_na_stole[4]);//wpisanie ostatniej lewy

//tylko do rogzrywki
card choose_card(card *karty, int ile, int gracz, card karty_na_stole[], int n, int atut);//wybranie przez uzytkownika co chce zagrac
card choose_card_bot(card *karty, int ile, card karty_na_stole[4], int n, int atut);//wybranie karty przez bota
int find_winner(card karty_na_stole[4], int atut);//sprawdz kto zbiera dana lewe
void find_winner_test();//test 
void informacje_rozgrywka(int atut);//info dla uzytkownika co wpisywac
bool check_card(card *karty,int ile,card wynik,int poprzedni_kolor);//sprawdz czy mozna zagrac te karte (czy ma w kartach oraz czy jest do koloru lub czy nie ma do koloru)
void show_last_trick();//ostatnia lewa
void remove_card(card *karty, int i);//usuniecie karty nr i



card choose_card(card *karty, int ile, int gracz, card karty_na_stole[], int n, int atut)
{
	//pokazanie kart gracza i co jest na stole
	show_cards(karty, ile, gracz);
	if(n > 0){
		printf("Na stole są: \n");
		for(int i = 0; i < n; i++){
			print(karty_na_stole[i]);
		}
		printf("\n\n");
	}
	else{
		printf("Wistujesz:\n\n");
	}

	//informacje co do wpisywania karty
	informacje_rozgrywka(atut);
	
	//wczytanie jaka katre chce zagrac
	printf("\nWybierz, ktorą kartę chcesz zagrać\n");

	//umozliwienie: 
	//-1 - podejrzyj ostatnia lewe
	//tylko jesli to nie pierwsza runda teraz

	if(ile != 13)//juz nie ma 13 kart, czyli juz po pierwszej rozgrywce
	{
		printf("jesli chcesz podejrzec ostatnia lewe wpisz L\n");
	}

	char numer = 0;
	if(scanf("%c", &numer) < 1) printf(" ");
	if(numer == '\n'){
		if(scanf("%c", &numer) < 1) printf(" ");
	}

	//jesli podejrzenie lewy
	if(numer == 'L' || numer == 'l')
	{
		//odpowiednio wyswietlenie ostatniej lewy lub napisanie ze to niemozliwe
		if(ile == 13) printf("Ta opcja ne jest dostępna w pierwszej rundzie\n");
		else
		{
			printf("Ostatnia lewa:\n");
			show_last_trick();
		}

		//wczytanie jeszcze raz numeru
		printf("\nWybierz, ktorą kartę chcesz zagrać\n");
		if(scanf("%c", &numer) < 1) printf(" ");
		if(numer == '\n'){
			if(scanf("%c", &numer) < 1) printf(" ");
		}
	}
	
	if(DEBUG) printf("Wczytano numer %c\n", numer);
	if(numer == '1') getchar();//zabieram 0

	//wczytanie koloru
	int color = input_color_string();
	while(color == -256 || color == 0)
	{
		printf("Niepoprawnie wpisano kolor, proszę wpisać jeszcze raz:");
		color = input_color_string();
	}
	// char kolor;
	// while(scanf(" %c",&kolor) < 1){
		// printf("\nBłąd przy podaniu karty, proszę spróbować jeszcze raz\n");
	// }
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
	// wynik.color = input_color(kolor);
	wynik.color = color;
	//wyswietlenie wyboru
	// printf("wybrano karte:\n");
	// print(wynik);
	// printf("\n");

	//mozna cofnac jak sie zle wybralo (wybrac jeszcze raz)
	// printf("Potwiedź, czy to tak karta miała zostać wybrana (t - tak, n - nie)");
	// char a;
	// a = getchar();
	// if(a == ' ' || a == '\n') a = getchar();
	// if(a == 'n' || a == 'N') return choose_card(karty, ile, gracz, karty_na_stole, n, atut);

	//sprawdzenie czy taka moze wybrac
	int poprzedni_kolor = -1;
	if(n > 0){//bylo cos wczesniej dane
		poprzedni_kolor = karty_na_stole[0].color;
	}
	if(!check_card(karty, ile, wynik, poprzedni_kolor)){
		printf("Niepoprawnie wybrano karte (na przyklad nie posiadasz takiej lub kolor jest niepoprawny), bedziesz musiał_ podać jeszcze raz\n");
		return choose_card(karty, ile, gracz, karty_na_stole, n, atut);
	}
	
	//zwrocenie wyniku jesli wszystko ok
	return wynik;
}


void remove_card(card *karty, int i)//usuniecie karty nr i
{
	for(int k = i; k < 12; k++)//nadpisanie dalszymi kartami
	{
		(karty + k) -> num = (karty + k + 1) -> num;
		(karty + k) -> color = (karty + k + 1) -> color;
	}
	(karty + 12) -> num = 0;
	(karty + 12) -> color = 0;
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
	printf("Wpisz 4 karty, kolory podawaj jako n, h, d, s lub c");
	card karty[4];
	char kolor;
	for(int i = 0; i < 4; i++){
		if(scanf("%i %c", &karty[i].num, &kolor) < 2) printf(" ");
		karty[i].color = input_color(kolor);
	}
	printf("Podaj atut");
	if(scanf("%c", &kolor) < 1) printf(" ");
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


void informacje_rozgrywka(int atut)
{
	printf("Wpisz odpowiednio jaką kartę chcesz dać: (2 - A) oraz kolor (pik, kier, karo lub trefl)\n");
	printf("Atutem jest");
	print_color(atut);
	printf("\n");
}

card choose_card_bot(card *karty, int ile, card karty_na_stole[4], int n, int atut)
{
	//wyswietlenie kart na stole
	// printf("Na stole są: \n");
	// for(int i = 0; i < n; i++){
		// print(karty_na_stole[i]);
	// }
	// printf("\n");
	
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
		// if(DEBUG) printf("wybrano karte nr %i\n", x);
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
		
		if(DEBUG) printf("x = %i \n", x);
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
		if((karty + i) -> color == atut)
		{
			czy_ma_atut = true;
			if(pierwsza_a == -1) pierwsza_a = i;
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
		if(DEBUG) printf("pierwsza = %i, ostatnia = %i\n", pierwsza_a, ostatnia_a);
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
		remove_card(karty, x);
		return wynik;
	}
	
	if(DEBUG) printf("nie ma w atucie\n");
	
	//jesli nie ma koloru ani atutu to najnizsza z tych, ktore ma
	int najnizsza = 15;
	int miejsce = -1;
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

void show_last_trick()//ostatnia lewa
{
	// printf("Ostatnia lewa to:\n");
	for(int i = 0; i < 4; i++)
	{
		print(last_trick[i]);
		printf("\n");
	}
}


void save_last_trick(card karty_na_stole[4])//zapisanie ostatniej lewy
{
	last_trick[0] = karty_na_stole[0];
	last_trick[1] = karty_na_stole[1];
	last_trick[2] = karty_na_stole[2];
	last_trick[3] = karty_na_stole[3];
}
