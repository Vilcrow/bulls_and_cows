/*
Bulls and Cows game

Copyright (C) 2022 S.V.I 'Vilcrow', <svi@vilcrow.net>
--------------------------------------------------------------------------------
LICENCE:
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <libintl.h>

#define LOCALEBASEDIR "/usr/share/locale/"
#define TEXTDOMAIN "bulls_and_cows"

#define _(STR) gettext(STR)
#define N_(STR) (STR)

enum {key_escape = 'q'};
enum {num_of_digits = 4};
enum {max_attempts = 500};
enum {false = 0};
enum {true = 1};

unsigned char create_digit();
void create_num(unsigned char *hidden_num);
unsigned char is_correctness(unsigned char *input_num);
void bc_counter(unsigned char *bulls, unsigned char *cows, const unsigned char
                *input_num, const unsigned char *hidden_num);
void show_help();

int main()
{
	unsigned char hidden_num[num_of_digits];
	unsigned char input_num[num_of_digits];
	unsigned char corr_inp, i;
	unsigned char bulls = 0, cows = 0;
	int c;
	int attempt = 1;
	setlocale(LC_CTYPE, "");
	setlocale(LC_MESSAGES, "");
	bindtextdomain(TEXTDOMAIN, LOCALEBASEDIR);
	textdomain(TEXTDOMAIN);
	show_help();
	srand(time(NULL));	
	create_num(hidden_num);
	do {
		printf("%4d\t", attempt);
		while((c = getchar()) != '\n') {
			if(c >= '0' && c <= '9' && i < num_of_digits) {
				input_num[i] = c - '0';	
				++i;
			}
			else if(c == '\b' && i > 0) {
				--i;	
			}
			else if(c == key_escape)
				return 0;
			else if(c == EOF) {
				printf("\n");
				return 0;
			}
		}
		if(i != num_of_digits) {
			printf(_("Incorrect input!\n"));
			i = 0;	
		}
		else {
			corr_inp = is_correctness(input_num);	
			if(corr_inp == true) {
				bc_counter(&bulls, &cows, input_num, hidden_num);
				printf(_("\t%d[Bulls] %d[Cows]\n"), bulls, cows);
				if(bulls == num_of_digits) {
					printf(_("You win. Total attempts - %d.\n"), attempt);
					return 0;
				}
				i = 0;
				bulls = 0;
				cows = 0;
				++attempt;
			}
			else {
				printf(_("Reapiting digits!\n"));
				i = 0;
			}
		}
	} while(attempt <= max_attempts);
	printf(_("Too many attempts.\n"));
	return 0;
}

unsigned char create_digit()
{
	return (unsigned char)(10.0*rand()/RAND_MAX);
}

void create_num(unsigned char *hidden_num)
{
	unsigned char i, j;
	for(i = 0; i < num_of_digits; ++i) {
		hidden_num[i] = create_digit();
		for(j = 0; j < i; ++j) {
			if(hidden_num[i] == hidden_num[j]) {
				--i;
				break;
			}
		}
	}
}

unsigned char is_correctness(unsigned char *input_num)
{
	unsigned char i, j;
	for(i = 0; i < num_of_digits; ++i) {
		for(j = i+1; j < num_of_digits; ++j) {
			if(input_num[i] == input_num[j])
				return false;
		}
	}
	return true;
}

void bc_counter(unsigned char *bulls, unsigned char *cows, const unsigned char
                *input_num, const unsigned char *hidden_num)
{
	unsigned char i, j;
	for(i = 0; i < num_of_digits; ++i) {
		for(j = 0; j < num_of_digits; ++j) {
			if(i == j && input_num[i] == hidden_num[j])
				*bulls += 1;
			else if(input_num[i] == hidden_num[j])
				*cows += 1;
		}
	}	
}

void show_help()
{
	printf("----------------------------------------\n");
	printf(_("The computer has generated a sequence of four non-repeating\n"));
	printf(_("digits from 0 to 9. Your goal is to guess these digits.\n"));
	printf(_("If the matching digits are in their right positions, they are\n"));
	printf(_("'bulls', if in different positions, they are 'cows'.\n"));
	printf("----------------------------------------\n");
	printf(_("Enter '%c' to quit the game.\n"), key_escape);
	printf(_("The number is hidden. Enter your answer:\n"));
}
