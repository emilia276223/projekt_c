#include "wyswietlanie_i_karty.h"

int main(){
	srand(time(NULL));
	
	new_game();
	
	while(czy_nowa()){
		new_game();
	}
//	ustal_wygrana_test();
	return 0;
}

