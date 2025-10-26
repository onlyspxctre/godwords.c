#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#include <bits/time.h>
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

#define S_MS(S) (S) * 1000
#define NS_MS(NS) (NS) / 1000000

#define WORDS_LEN 1024
#define WORD_LEN 1024

static char words[WORDS_LEN][WORD_LEN];
static inline unsigned int parse(void) {
    FILE* file_ptr = fopen("./Happy.TXT", "r");

    unsigned int n = 0;
    while (n < WORDS_LEN && fgets(words[n], WORD_LEN, file_ptr)) {
        ++n;
    }

    fclose(file_ptr);

    return n;
}

static int running = 1;
static struct timespec start = {0}, end = {0};
static clock_t elapsed_ms;

static char input;
static int timesGenerated;

int main(void) {
    startup();
    unsigned int n = parse();

    while (running) {
        read(STDIN_FILENO, &input, 1);

        switch (input) {
            case ' ':
                clock_gettime(CLOCK_MONOTONIC, &end);

                elapsed_ms = S_MS(end.tv_sec - start.tv_sec) + NS_MS(end.tv_nsec - start.tv_nsec);
                if (elapsed_ms > 2500) {
                    putc('\n', stdout);
                }

                start = end;

                printf("%s", words[(unsigned) rand() % n]);
                ++timesGenerated;
                break;
            default:
                running = 0;
                break;
        }
    }

    shutdown();

    return 0;
}
