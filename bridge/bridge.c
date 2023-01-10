#include "wyswietlanie_i_karty.h"

int main(){
	srand(time(NULL));
	
	new_game();
	
	while(czy_nowa() == true){
		new_game();
	}
	
	return 0;
}

