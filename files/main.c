#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <getopt.h>


void segfault();

void seghandler();

int main(int argc, char **argv) {
    int c;

    char *input_filename;
    char *output_filename;

    int catch = 0;
    int seg   = 0;

    for(;;) {
        static struct option long_options[] = {
            {"input", required_argument, 0, 'a'},
            {"output", required_argument, 0, 'b'},
            {"segfault", no_argument, 0, 'c'},
            {"catch", no_argument, 0, 'd'},
            {0, 0, 0, 0}};

        int option_index = 0;

        c = getopt_long(argc, argv, "abc:d:f:",
                        long_options, &option_index);

        if (c == -1)
            break;

        switch (c) {
            case 0:
                // /* If this option set a flag, do nothing else now. */
                // if (long_options[option_index].flag != 0)
                //     break;
                // printf("option %s", long_options[option_index].name);
                // if (optarg)
                //     printf(" with arg %s", optarg);
                // printf("\n");
                break;

            case 'a':
                // printf("input length: %d", strlen(optarg));
                input_filename = (char *)malloc(sizeof(optarg));
                strcpy(input_filename, optarg);
                break;

            case 'b':
                // puts("option -b\n");
                output_filename = (char *)malloc(sizeof(optarg));
                strcpy(output_filename, optarg);
                break;

            case 'c':
                seg = 1;
                break;

            case 'd':
                catch = 1;
                break;

            default:
                abort();
        }
    }

    if (catch)
        signal(SIGSEGV, seghandler);

    if (seg)
        segfault();

    if (input_filename) {
        printf(input_filename);
    }

    /* Print any remaining command line arguments (not options). */
    if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        putchar('\n');
    }

    if (!input_filename)
        free(input_filename);

    if (!output_filename)
        free(output_filename);
    exit(0);
}

void segfault() {
    int *dum = NULL;
    *dum = 0;
}

void seghandler() {
    fprintf(stderr, "Error: Segmentation fault\n");
    exit(4);
}