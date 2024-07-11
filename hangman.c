#include <stdio.h>
#include <string.h>
#include "hangman-io.h"
#include "words.h"

// Function prototypes
void initialize_board(const char *word, char *board);
void update_board(const char *word, char *board, char guess);
int is_word_guessed(const char *board);
int is_letter_guessed(char letter, const char *guessed_letters);
char to_upper_case(char ch);
int is_alpha(char ch);

int main() {
    int game_number;
    char play_again_response;
    do {
        // Prompt for and validate game number
        printf(enter_game_number, max_words);
        while (scanf("%d", &game_number) != 1 || game_number < 1 || game_number > max_words) {
            printf("%s", invalid_game_number);
            printf(enter_game_number, max_words);
            while (scanf("%*[^\n]%*1[\n]")); // Clear invalid input
        }

        const char *word = get_word(game_number);
        char board[51]; // Array to store the board representation
        initialize_board(word, board);

        int strikes = 0;
        int word_guessed = 0;
        char guessed_letters[27] = {0}; // Array to track guessed letters
        int guessed_count = 0;

        // Game loop
        while (strikes < max_strikes && !word_guessed) {
            print_hangman(strikes);
            printf("%s", letter_prompt);
            printf("%s\n", board);

            char guess;
            while (scanf(" %c", &guess) != 1) {
                printf(not_a_letter, guess);
                printf("%s", letter_prompt);
                while (scanf("%*[^\n]%*1[\n]")); // Clear invalid input
            }

            guess = to_upper_case(guess); // Convert guess to uppercase
            if (!is_alpha(guess)) {
                printf(not_a_letter, guess);
                continue;
            }

            // Check if letter was already guessed
            if (is_letter_guessed(guess, guessed_letters)) {
                printf(already_guessed_letter, guess);
                continue;
            }

            guessed_letters[guessed_count++] = guess;

            // Check if the guess is in the word
            int found = 0;
            for (int i = 0; word[i] != '\0'; ++i) {
                if (word[i] == guess) {
                    found = 1;
                    break;
                }
            }

            if (found) {
                update_board(word, board, guess);
                if (is_word_guessed(board)) {
                    word_guessed = 1;
                    printf(congratulations, word);
                }
            } else {
                printf(not_in_word, guess);
                strikes++;
                if (strikes < max_strikes) {
                    printf(not_last_guess, max_strikes - strikes);
                } else {
                    print_hangman(strikes);
                    printf(game_over, word);
                }
            }
        }

        // Ask if the user wants to play again
        printf("%s", play_again);
        while (scanf(" %c", &play_again_response) != 1 || (play_again_response != 'Y' && play_again_response != 'y' && play_again_response != 'N' && play_again_response != 'n')) {
            printf("%s", invalid_play_again_response);
            printf("%s", play_again);
            while (scanf("%*[^\n]%*1[\n]")); // Clear invalid input
        }
    } while (play_again_response == 'Y' || play_again_response == 'y');

    return 0;
}

// Function to initialize the board with asterisks and spaces
void initialize_board(const char *word, char *board) {
    int length = strlen(word);
    for (int i = 0; i < length; ++i) {
        if (word[i] == ' ') {
            board[i] = ' ';
        } else {
            board[i] = '*';
        }
    }
    board[length] = '\0';
}

// Function to update the board with correctly guessed letters
void update_board(const char *word, char *board, char guess) {
    int length = strlen(word);
    for (int i = 0; i < length; ++i) {
        if (word[i] == guess) {
            board[i] = guess;
        }
    }
}

// Function to check if the word has been completely guessed
int is_word_guessed(const char *board) {
    while (*board) {
        if (*board == '*') {
            return 0;
        }
        ++board;
    }
    return 1;
}

// Function to check if a letter has been guessed
int is_letter_guessed(char letter, const char *guessed_letters) {
    for (int i = 0; guessed_letters[i] != '\0'; ++i) {
        if (guessed_letters[i] == letter) {
            return 1;
        }
    }
    return 0;
}

// Function to convert a character to uppercase
char to_upper_case(char ch) {
    if (ch >= 'a' && ch <= 'z') {
        return ch - ('a' - 'A');
    }
    return ch;
}

// Function to check if a character is an alphabetic letter
int is_alpha(char ch) {
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}
