#include "rozgrywka.h"


//obsluga kolejnosci gry
bool if_new_game();
int runda_dla_4_graczy(int dealer, int n, int atut, int *score, int rozgrywajacy);//zwracam kto zebral lewe
int runda_z_botem(int dealer, int n, int atut, int *score, int rozgrywajacy, bool *wylozone_karty);//zwracam kto zebral lewe
void game(int atut, int *score, int tryb_gry, int rozgrywajacy);
void new_game();
void czy_ugrane(card deal,int *score, int rozgrywajacy);

//ogarniecie kart (potasowanie i podzielenie)
void shuffling(card *talia);//potasowanie kart
void dealing_cards();//rozlozenie kart na 4 graczy
void sort_by_num(card *tab, int ile);//posortowanie po wielkosci kart
void sort_by_color(card *tab, int ile);//posortowanie po kolorach
void generate_cards(card *all_cards);//wygenerowanie talii
void wylozone_karty_reset(bool *wylozone_karty);//wyczyszczenie miejsca na wpisywanie wulozonych kart

//debugging
void print_wylozone_karty(bool *wylozone_karty);
void show_deal(card *karty); // tylko do debugowania

void wylozone_karty_reset(bool *wylozone_karty)//ustawienie wszystkich na false
{
	for(int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			*(wylozone_karty + i*13 + j) = false;
		}
		
	}
}

void game(int atut, int *score, int tryb_gry, int rozgrywajacy)
{
	//ustawienie dealera
	int dealer = (rozgrywajacy + 1) % 4;

	//tu beda trzymane wszystkie wylozone karty (zeby zrobic upgrade bota)
	bool wylozone_karty[5][13];//false - nie wylozona, true - juz wylozona
	//[1] - pik
	//[2] - kier ...
	wylozone_karty_reset(&wylozone_karty[1][0]);

	
	//jesli gra 4 graczy
	if(tryb_gry == 0)
	{
		for(int i = 0; i < 13; i++)
		{
			//dla kazdej rundu rozpoczenie z ustalonym wczesniej dealerem
			dealer = runda_dla_4_graczy(dealer,i, atut, score, rozgrywajacy);
		}
	}
	
	else//1 gracz i 4 razy bot
	{
		for(int i = 0; i < 13; i++){
			dealer = runda_z_botem(dealer,i, atut, score,rozgrywajacy, &wylozone_karty[0][0]);
		}
//		printf("jeszcze nie ma tej opcji gry");
	}
}

bool if_new_game(){
	// clear_screen();
	printf("Gra zakonczona\n");
	printf("Jeśli chcesz rozpocząć kolejną grę wpisz T, w przeciwnym wypadku wpisz inny znak:\n");
	int z;
	z = getchar();
	if(z == '\n' || z == ' '){
		z = getchar();
	}
	getchar();
	return (z == 't' || z == 'T') ? true : false; 
}

int runda_dla_4_graczy(int dealer, int n, int atut, int *score, int rozgrywajacy)//zwracam kto zebral lewe
{
	//n - n-ta runda
	int player = dealer;
	card karty_na_stole[4];
	for(int i = 0; i < 4; i++){
		clear_screen();
		if(player == dziadek)//jesli dziadek to pokazuje karty rozgrywajacego i dziadka
		{
			printf("Karty rozgrywajacego:\n");
			show_cards(&cards[rozgrywajacy][0], 13 - n);
			printf("\nTwoje karty:\n");
			karty_na_stole[i] = choose_card(&cards[dziadek][0], 13 - n, dziadek, karty_na_stole, i, atut);
		}
		else{
			if(n == 0 && player == dealer)//gracz nie widzie jeszcze kart dziadka bo to 0-wa runda
			{
				printf("Twoje karty:\n");
				karty_na_stole[i] = choose_card(&cards[player][0], 13 - n, player, karty_na_stole, i, atut);
			}
			else
			{
				printf("Karty dziadka (gracz %i):\n", dziadek + 1);
				show_cards(&cards[dziadek][0], 13 - n);
				printf("\nTwoje karty:\n");
				karty_na_stole[i] = choose_card(&cards[player][0], 13 - n, player, karty_na_stole, i, atut);	
			}
		}
		//ustawienie kolejnego gracza
		player = (player + 1) % 4;
		
		getchar();
		getchar();
		clear_screen();
		printf("Ruch gracza poprzedniego zakończony, kolejny ruch będzie gracza %i\n", player + 1);
		printf("Wcisnij ENTER, żeby rozpoczac ruch gracza %i:\n", player + 1);
		getchar();
		
	}
	int new_dealer;
	new_dealer = (dealer + find_winner(karty_na_stole, atut))%4;
	*(score + new_dealer) += 1;
	return new_dealer;
}

int runda_z_botem(int dealer, int n, int atut, int *score, int rozgrywajacy, bool *wylozone_karty)//zwracam kto zebral lewe
{
	//plan na przyszlosc:
	//bot wybiera najnizsza w kolorze jesli nie ma wyzszej a wyzsza (ale losowa) jak bije	

	//n - n-ta runda
	//gracz 1 to urzytkownik
	int player = dealer;
	card karty_na_stole[4] = {{0,0},{0,0},{0,0},{0,0}};
	for(int i = 0; i < 4; i++){
		clear_screen();
		
		//jesli kolej uzytkowina i nie jest on dziadkiem
		if(player == 0 && player != dziadek)//jesli teraz kolej uzytkownika
		{
			//wyswietlenie co sie zadzialo w ostatniej runddzie
			if(n > 0)//to nie pierwsza runda
			{
				printf("Poprzednia tura:\n");
				show_last_trick();
				printf("Więc zebrał gracz %i\n", dealer + 1);
			}

			//wybranie karty
			if(n == 0 && player == dealer)//gracz nie widzie jeszcze kart dziadka bo to 0-wa runda
			{
				printf("Twoje karty:\n");
				karty_na_stole[i] = choose_card(&cards[player][0], 13 - n, player, karty_na_stole, i, atut);
			}
			else
			{
				printf("Karty dziadka (gracz %i):\n", dziadek + 1);
				show_cards(&cards[dziadek][0], 13 - n);//poprawic bo jest zle bo sie czasem o 1 za duzo wyswietla
				printf("Twoje karty:\n");
				karty_na_stole[i] = choose_card(&cards[player][0], 13 - n, player, karty_na_stole, i, atut);	
			}
			getchar();
			clear_screen();
		}
		
		//jesli gracz jest dziadkiem
		else if(player == 0 && player == dziadek)
		{
			//wyswietlenie co sie zadzialo w ostatniej runddzie
			if(n > 0)//to nie pierwsza runda
			{
				printf("Poprzednia tura:\n");
				show_last_trick();
				printf("Więc zebrał gracz %i\n", dealer + 1);
			}

			printf("Jestes dziadkiem, wiec rozgrywajacy wybiera twoja karte\n");
			karty_na_stole[i] = choose_card_bot_the_worst(&cards[dziadek][0], 13 - n, karty_na_stole, i, atut);
			// printf("\nGracz %i wybrał kartę :", rozgrywajacy + 1);
			// print(karty_na_stole[i]);
			printf("Wpisz enter: ");
			
			int x;
			x = getchar();
			while(x != '\n') x = getchar(); //Leon-proof
			clear_screen();
		}
		
		//jesli gracz jest rozgrywajacym i kolej dziadka
		else if(player == 2 && rozgrywajacy == 0)
		{
			if(n > 0)//to nie pierwsza runda
			{
				printf("Poprzednia tura:\n");
				show_last_trick();
				printf("Więc zebrał gracz %i\n\n", dealer + 1);
			}

			printf("Jestes rozgrywajacym wiec wybierasz, jaką kartę na położyć dziadek:\n\n");
			printf("Dla przypomnienia Twoje karty:\n");
			show_cards(&cards[0][0], 13 - n);
			printf("\n A to karty dziadka (gracz %i), wybierz jedną z nich:\n", dziadek + 1);
			karty_na_stole[i] = choose_card(&cards[player][0], 13 - n, player, karty_na_stole, i, atut);
		getchar();
		clear_screen();
		}
		
		//jesli kolej bota
		else//w pozostalych przypadkach
		{
			//tutaj nie ma znaczenia czy ktos jest dziadkiem czy rozgrywajacym, bo i tak losuje karte
			//bot i tak nie sugeruje sie innymi kartami		
			if(player == 2) karty_na_stole[i] = choose_card_bot_the_worst(&cards[player][0], 13 - n, karty_na_stole, i, atut);
			else karty_na_stole[i] = choose_card_bot(&cards[player][0], 13 - n, karty_na_stole, i, atut);
			// printf("\nGracz %i wybrał kartę :", player + 1);
			// print(karty_na_stole[i]);
		}

		//dodanie wybranej karty do juz wylozonych
		*(wylozone_karty + (karty_na_stole[i].num - 2) + karty_na_stole[i].color * 13) = true;
		if(DEBUG) print_wylozone_karty(wylozone_karty);
		
		//ustawienie kolejnego gracza
		player = (player + 1) % 4;
//		getchar();
//		printf("Ruch gracza poprzedniego zakończony, kolejny ruch będzie gracza %i\n", player + 1);
//		printf("Wcisnij ENTER, żeby rozpoczac ruch gracza %i:\n", player + 1);
		
	}
	save_last_trick(karty_na_stole);
	int new_dealer;
	new_dealer = (dealer + find_winner(karty_na_stole, atut))%4;
	*(score + new_dealer) += 1;
	return new_dealer;
}

void new_game()
{
	//wyczyszczenie danych
	int score[4] = {0};
	auction_clear();
	
	//utworzenie nowych kart
	dealing_cards();
	clear_screen();
	
	//ustaleniu trybu gry
	int tryb_gry = 0;//4 graczy
	printf("Jesli chcesz zagrac z komputerem wpisz K:\n");
	char a = getchar();
	if(a == ' ' || a == '\n') getchar();
	if(a == 'k' || a == 'K') tryb_gry = 1;//z komputerem
	getchar();//"zjedzenie entera"
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
		// getchar();
		return;
	}
	//ustawienia poczatkowe rozgrywki
	int rozgrywajacy = ustaw_rozgrywajacego(0);//bo na razie licytacje zawsze rozpoczyna gracz 0
	dziadek = (rozgrywajacy + 2) % 4;
	
	int atut = deal.color;
	clear_screen();

	//wyswietlenie wynikow licytacji
	printf("Został zalicytowany kontrakt: ");
	print(deal);
	getchar();
	clear_screen();
	
	//rozpoczecie rozgrywki
	game(atut, &score[0], tryb_gry, rozgrywajacy);
	
	//wyswietlenie wynikow
	printf("Gra zakonczona, wyniki to: %i, %i, %i, %i, tryb_gry = %i\n", score[0], score[1], score[2], score[3], tryb_gry);
	printf("Para 1-3 zebrala %i lew, a para 2-4 - %i\n", score[0] + score[2], score[1] + score[3]);
	//trzeba zrobic sprawdzenie czy ugrane
	czy_ugrane(deal, &score[0], rozgrywajacy);
	getchar();
}

void czy_ugrane(card deal,int *score, int rozgrywajacy)
{
	int lewy_w_parze;
	if(rozgrywajacy == 0 || rozgrywajacy == 2)
	{
		lewy_w_parze = *(score) + *(score + 2);
	}
	else
	{
		lewy_w_parze = *(score + 1) + *(score + 3);
	}

	if(lewy_w_parze >= deal.num + 6)
	{
		printf("Zalicytowany kontrakt został ugrany!!!");
	}
	else
	{
		printf("Zalicytowany kontrakt nie został ugrany :(\n");
	}
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

void dealing_cards()
{
	//create and shuffle cards
	card all_cards[52];
	generate_cards(&all_cards[0]);
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

void print_wylozone_karty(bool *wylozone_karty)
{
	printf("Do tej pory wylozone karty:\n");
	card karta;
	for(int i = 1; i < 5; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			if(*(wylozone_karty + i*13 + j) == true)
			{
				karta.num = j + 2;
				karta.color = i;
				print(karta);
				printf(" , ");
			}
		}
		
	}
}

void show_deal(card *karty) // tylko przy debugowaniu
{
	printf("Karty gracza 1: \n");
	show_cards(karty, 13);
	printf("Karty gracza 2: \n");
	show_cards(karty + 13, 13);
	printf("Karty gracza 3: \n");
	show_cards(karty + 26, 13);
	printf("Karty gracza 4: \n");
	show_cards(karty + 39, 13);
}

