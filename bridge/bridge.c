#include "wyswietlanie_i_karty.h"

int main(){
	srand(time(NULL));
	
	new_game();
	
	while(if_new_game()){
		new_game();
	}
	return 0;
}
//dorobic:

//opcja gry z 3 komputerami (moga byc losowe ruchy) 
