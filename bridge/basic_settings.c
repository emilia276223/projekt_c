#include <stdio.h>
#include <string.h>

struct pronouns_info{
	char they[];
	char their[];
	char themself[];
	char them[];
};

typedef struct pronouns pronouns;

pronouns_info pronouns;
char name[];

void pronoun_change_custom(char they[], char their[], char themself[], char them[]){
	pronouns.they[] = they[];
	pronouns.their[] = their[];
	pronouns.themself[] = themself[];
	pronouns.them[] = them[];
}

void pronoun_change_simple(int p){
	if(p == 1){
		pronouns.they[] = "they"; 
		pronouns.their[] = "their"
		pronouns.themself[] = "themself";
		pronouns.them[] = "them";
	}
	if(p == 2){
		pronouns.they[] = "she"; 
		pronouns.their[] = "her"
		pronouns.themself[] = "herself";
		pronouns.them[] = "her";
	}
	if(p == 3){
		pronouns.they[] = "he"; 
		pronouns.their[] = "his"
		pronouns.themself[] = "himself";
		pronouns.them[] = "him";
	}
}

void start(){
	//introduction	
	
	//name check
	
	//pronoun check
	int pron;
	//sprawdzenie
	while(scanf("%i", pron)<1){
		//prosze powtorzyc
	}
	if(pron == 0){
		//pobrac custom
		char they[];
		char their[];
		char themself[];
		char them[];
		//wczytaj
		
		//zapisz
		pronoun_change_custom(char they[], char their[], char themself[], char them[])
	}
}


