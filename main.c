#include "gen_vector.h"
#include "my_string.h"
#include "AssociativeArray.h"
#include <stdio.h>
#include <stdlib.h>

void destroy_vector_of_strings(VECTOR* hVoStr);
void sort_dictionary(VECTOR* phVofStr, FILE* fp);
void print_vector(VECTOR hVector);
Boolean PlayGame(VECTOR hDictionary, int guessMax, Boolean showWords);
//Precondition:current_word_family, new_key and word are all handles to valid
//   MY_STRING opaque objects.  guess is an alphabetical character that can be either
//   upper or lower case.
//Postcondition:  Returns SUCCESS after replacing the string in new_key with the key
//   value formed by considering the current word family, the word and the guess.
//   Returns failure in the case of a resizing problem with the new_key string.
Status get_word_key_value(MY_STRING current_word_family, MY_STRING* new_key, MY_STRING word, char guess);
char get_guess(MY_STRING guessed);
Boolean winGame(MY_STRING key);
void clear_keyboard_buffer(void);
void copy_word_list(VECTOR original, VECTOR* copy);

int main(int argc, char *argv[]) {
	int input = 0;
	int guess_amount = 0;
	Boolean play_game = TRUE;
	Boolean show_words = FALSE;
	char input_letter;
	FILE* fp;
	VECTOR hVofStr = vector_init_default();  //vector of vectors of strings
	VECTOR currentV = NULL; //vector being worked with

	for (int i = 0; i < 30; ++i) {  //initialize the 30 vectors used to store strings
		currentV = vector_init_default();
		vector_push_back(hVofStr, currentV);
	}
	currentV = NULL; //set to null when no longer being used

	fp = fopen("dictionary.txt", "r");
	sort_dictionary(&hVofStr, fp);
	fclose(fp);// close file

	//print_vector(vector_get_data(hVofStr, 5));		//DEBUG

	while (play_game == TRUE) {
		//----get length of words to be used----
		printf("Enter length of words to use (between 1 and 30): ");
		scanf("%d", &input);
		clear_keyboard_buffer();
		while (input > 30 || input < 1 ||
			vector_get_size(vector_get_data(hVofStr, input)) == 0) {//validate input
			printf("No words of that length in my dictionary, "
				"please enter a different number: ");
			scanf("%d", &input);
			clear_keyboard_buffer();
		}
		printf("Using words of length %d\n", input); //brief-back
		currentV = vector_get_data(hVofStr, input);//select vector with requested string length
		input = 0;  //reset input


		//----prompt for # of guesses----
		printf("Enter number of guesses you want: ");
		scanf("%d", &input);
		clear_keyboard_buffer();
		while (input < 1 || input > 26) {//input validation
			printf("cannot have less than 1 guess or more than 26, please enter a new number: ");
			scanf("%d", &input);
			clear_keyboard_buffer();
		}
		printf("You will get %d guesses\n", input); //brief-back
		guess_amount = input; //enter input # of guesses
		input = 0;//reset input


		//---display number of word left(y/n)---
		printf("Show number of remaining possible words?(y/n): ");
		scanf("%c", &input_letter);
		clear_keyboard_buffer();
		//input validation
		while (input_letter != 'n' && input_letter != 'N' && input_letter != 'y' && input_letter != 'Y') {
			printf("Invalid input, please enter \'y\' or \'n\': ");
			scanf("%c", &input_letter);
			clear_keyboard_buffer();
		}
		if (input_letter == 'y' || input_letter == 'Y')
			show_words = 1;
		else
			show_words = 0;

		play_game = PlayGame(currentV, guess_amount, show_words);
	}
	destroy_vector_of_strings(&hVofStr);
	printf("\nENDED\n");
	return 0;
}


void destroy_vector_of_strings(VECTOR* phVofStr) {
	VECTOR hVofStr = *phVofStr;
	VECTOR hVtemp = NULL;
	MY_STRING hStemp = NULL;
	for (int i = 0; i < 30; ++i) {
		hVtemp = vector_get_data(hVofStr, i);
		for (int j = 0; j < vector_get_size(hVtemp); ++j) {
			hStemp = vector_get_data(hVtemp, j);
			my_string_destroy(&hStemp);
		}
		vector_destroy(&hVtemp);
	}
	vector_destroy(&hVofStr);
}

void sort_dictionary(VECTOR* phVofStr, FILE* fp) {
	VECTOR hVofStr = *phVofStr;
	MY_STRING hString = NULL;
	while (!feof(fp)) {  //copy dictionary into vector of vector of strings
		hString = my_string_init_default();
		my_string_extraction(hString, fp);

		if (my_string_get_size(hString) > 0) { //filter out any 0 length "words"
			vector_push_back(vector_get_data(hVofStr, (my_string_get_size(hString))), hString);
		}
		else
			my_string_destroy(&hString);  //destroy invalid strings
	}
}

void print_vector(VECTOR hVector) {
	for (int i = 0; i < vector_get_size(hVector); ++i) {
		printf("%s\n", my_string_c_str(vector_get_data(hVector, i)));
	}
}

Boolean PlayGame(VECTOR hDictionary, int guessMax, Boolean showWords) {
	MY_STRING guessed = my_string_init_default(); //string of all letters guessed
	MY_STRING currentKey = my_string_init_default();
	MY_STRING newKey = NULL;
	VECTOR wordList = hDictionary;
	A_ARRAY wordFamilies = NULL;
	char guess;

	//copy_word_list(hDictionary, &wordList); //create starting wordlist

	//vector_get_size(wordList);		//DEBUG

	for (int i = 0; i < my_string_get_size(vector_get_data(hDictionary, 0)); ++i) { //get starting empty key
		my_string_push_back(currentKey, '-');
	}
	while (guessMax > 0) {
		//print data for user
		printf("You have %d guesses left.\n", guessMax); //# of guesses left
		printf("Used letters:  %s\n", my_string_c_str(guessed)); //letters already guessed
		printf("word: %s\n", my_string_c_str(currentKey)); //current key
		if (showWords == TRUE)  //if user wants to see # of possible words left
			printf("Possible words left: %d\n", vector_get_size(wordList));

		printf("Enter guess: ");// get guess
		guess = get_guess(guessed);

		if (array_create_families(wordList, &wordFamilies, guess) == FAILURE) //create new word families
			printf("ERROR: problem occured while sorting subfamilies");

		if (wordList == hDictionary)
			wordList = NULL;

		copy_word_list(array_max_family(wordFamilies), &wordList); //update wordlist with new list
		get_word_key_value(currentKey, &newKey, vector_get_data(wordList, 0), guess); //update key

		if (my_string_compare(newKey, currentKey) == 0) { //if key is unchanged (bad guess)
			guessMax--; //decrement number of guesses left
			printf("I'm sorry, there were no %c's in this word\n", guess);
		}
		else
			printf("Good Guess\n");

		my_string_destroy(&currentKey);//reset keys for next round
		currentKey = newKey;
		newKey = NULL;

		if (winGame(currentKey)) {
			printf("\n------------\n | You Won! |\n------------\n");
			break;
		}
		if (guessMax == 0) {
			printf("\n--------------\n | You Lose |\n--------------\n");
			printf("The word was: %s\n\n", my_string_c_str(vector_get_data(wordList, 1)));
			break;
		}



		array_list_keys(wordFamilies);              //****DEBUG****

	}
	my_string_destroy(&guessed);
	my_string_destroy(&currentKey);
	vector_destroy(&wordList);
	array_destroy(&wordFamilies);

	printf("Play Again? (y/n): ");
	scanf("%c", &guess);
	clear_keyboard_buffer();
	//input validation
	while (guess != 'n' && guess != 'N' && guess != 'y' && guess != 'Y') {
		printf("Invalid input, please enter \'y\' or \'n\': ");
		scanf("%c", &guess);
		clear_keyboard_buffer();
	}
	if (guess == 'y' || guess == 'Y')
		return TRUE;
	else
		return FALSE;
}

Status get_word_key_value(MY_STRING current_word_family, MY_STRING* new_key, MY_STRING word, char guess) {
	MY_STRING temp = NULL;
	char* key_str = my_string_c_str(current_word_family);

	if (*new_key != NULL)
		my_string_destroy(new_key);
	
	for (int i = 0; i < my_string_get_size(word); ++i) {
		if (*(my_string_at(word, i)) == guess)
			key_str[i] = guess;
	}
	*new_key = my_string_init_c_string(key_str);
	return SUCCESS;
}

char get_guess(MY_STRING guessed) { //clean
	char guess;
	Boolean isValid = FALSE;
	while (isValid == FALSE) {

		isValid = TRUE;
		scanf("%c", &guess);
		clear_keyboard_buffer();
		if (guess < 97)
			guess += 32;
		if (guess > 122 || guess < 97) {
			printf("Invalid letter, try again: ");
			isValid = FALSE;
		}
		else {
			for (int i = 0; i < my_string_get_size(guessed); ++i) {
				if (*(my_string_at(guessed, i)) == guess) {
					printf("You have already guessed that letter, try again: ");
					isValid = FALSE;
				}
			}
		}
	}
	my_string_push_back(guessed, guess);
	my_string_push_back(guessed, ' ');
	return guess;
}

Boolean winGame(MY_STRING key) {
	for (int i = 0; i < my_string_get_size(key); ++i) {
		if (*(my_string_at(key, i)) == '-')
			return FALSE;
	}
	return TRUE;
}

void clear_keyboard_buffer(void) {
	char c;
	scanf("%c", &c);
	while (c != '\n') {
		scanf("%c", &c);
	}
}

void copy_word_list(VECTOR original, VECTOR* copy) {
	if (*copy != NULL)
		vector_destroy(copy);
	*copy = vector_init_default();
	for (int i = 0; i < vector_get_size(original); ++i){
		my_string_get_size(vector_get_data(original, i));
		vector_push_back(*copy, vector_get_data(original, i));
	}
}
