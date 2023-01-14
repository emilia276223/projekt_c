#include "start.h"

int main(){
	srand(time(NULL));
	
	new_game();
	
	while(if_new_game()){
		new_game();
	}
	return 0;
}

