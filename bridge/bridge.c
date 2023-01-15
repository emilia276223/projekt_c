#include "start.h"

int main(){
	srand(time(NULL));
	
	// input_color_string_test();
	new_game();
	
	while(if_new_game()){
		new_game();
	}
	return 0;
}

