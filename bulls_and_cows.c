/*
Bulls and Cows game

Copyright (C) 2022-2023 S.V.I 'Vilcrow', <svi@vilcrow.net>
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

enum { key_escape = 'q' };

enum { max_attempts  = 500 };
enum { digits_count = 4 };

enum { false = 0 };
enum { true  = 1 };

int rand_digit();
int *create_number();
int has_repeat(int *input);
void count(int *bulls, int *cows, const int
                *input, const int *number);
void show_rules();

int main()
{
	int *number = create_number();
	int input[digits_count];
	int bulls = 0, cows = 0;
	int c;
	int i = 0;
	int attempt = 1;

	setlocale(LC_CTYPE, "");
	setlocale(LC_MESSAGES, "");
	bindtextdomain(TEXTDOMAIN, LOCALEBASEDIR);
	textdomain(TEXTDOMAIN);

	show_rules();

	do {
		printf("%4d\t", attempt);
		while((c = getchar()) != '\n') {
			if(c >= '0' && c <= '9' && i < digits_count) {
				input[i] = c - '0';	
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
		if(i != digits_count) {
			printf(_("Incorrect input!\n"));
			i = 0;	
		}
		else {
			if(!has_repeat(input)) {
				count(&bulls, &cows, input, number);
				printf(_("\t%d[Bulls] %d[Cows]\n"), bulls, cows);
				if(bulls == digits_count) {
					printf(_("You win! Total attempts - %d.\n"), attempt);
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

int rand_digit()
{
	return (int)(10.0*rand()/RAND_MAX);
}

int *create_number()
{
	srand(time(NULL));	
	int *result = malloc(digits_count * sizeof(int));
	int i, j;
	for(i = 0; i < digits_count; ++i) {
		result[i] = rand_digit();
		/*prevent the repetition of digits*/
		for(j = 0; j < i; ++j) {
			if(result[i] == result[j]) {
				--i;
				break;
			}
		}
	}
	return result;
}

int has_repeat(int *input)
{
	int i, j;
	for(i = 0; i < digits_count; ++i) {
		for(j = i+1; j < digits_count; ++j) {
			if(input[i] == input[j])
				return true;
		}
	}
	return false;
}

void count(int *bulls, int *cows, const int
                *input, const int *number)
{
	int i, j;
	for(i = 0; i < digits_count; ++i) {
		for(j = 0; j < digits_count; ++j) {
			if(i == j && input[i] == number[j])
				*bulls += 1;
			else if(input[i] == number[j])
				*cows += 1;
		}
	}	
}

void show_rules()
{
	printf("----------------------------------------\n");
	printf(_("The computer has generated a sequence of four non-repeating\n"));
	printf(_("digits from 0 to 9. Your goal is to guess these digits.\n"));
	printf(_("If the matching digits are in their right positions, they are\n"));
	printf(_("'bulls', if in different positions, they are 'cows'.\n"));
	printf("----------------------------------------\n");
	printf(_("Enter '%c' or use Ctrl-D(i.e. EOF) to quit the game.\n"),
															key_escape);
	printf(_("The number is hidden. Enter your answer:\n"));
}
