#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

static struct termios original_settings, new_settings;

static inline void startup(void) {
    // terminal non-canonical stuff to read keypresses correctly
    tcgetattr(STDIN_FILENO, &original_settings);

    new_settings = original_settings;

    new_settings.c_lflag &= (tcflag_t) ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

    // seeding the rng with curr time
    srand((unsigned int) time(NULL));
}

static inline void shutdown(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_settings);
}

#define WORDS_LEN 1024
#define WORD_LEN 1024
static char words[WORDS_LEN][WORD_LEN];

int main(void) {
    startup();
    printf("Hello world!\n");

    FILE* file_ptr = fopen("./Happy.TXT", "r");

    unsigned int n = 0;
    while (fgets(words[n], WORD_LEN, file_ptr)) {
        ++n;
    }

    char input;
    int running = 1;

    while (running) {
        read(STDIN_FILENO, &input, 1);

        switch (input) {
            case ' ':
                printf("%s", words[(unsigned) rand() % n]);
                break;
            default:
                running = 0;
                break;
        }
    }

    shutdown();

    return 0;
}
