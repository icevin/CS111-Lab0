#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void segfault();

void seghandler();

void handleErr(int err);

int main(int argc, char **argv) {
    int c;

    char *input_filename  = "";
    char *output_filename = "";

    int catch = 0;
    int seg   = 0;

    for (;;) {
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
                break;

            case 'a':
                input_filename = (char *)malloc(sizeof(optarg));
                strcpy(input_filename, optarg);
                break;

            case 'b':
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
                exit(1);
        }
    }

    if (catch)
        signal(SIGSEGV, seghandler);

    if (seg)
        segfault();

    int input_fd;
    int output_fd;

    if (strcmp(input_filename, "") != 0) {
        input_fd = open(input_filename, O_RDONLY);
        if (input_fd < 0) {
            fprintf(stderr, "Error: Failed to open --input file %s: %s\n", input_filename, strerror(errno));
            exit(2);
        } else {
            close(0);
            dup(input_fd);
            close(input_fd);
        }
    }

    if (strcmp(output_filename, "") != 0) {
        output_fd = open(output_filename, O_CREAT | O_RDWR | O_TRUNC | O_APPEND, 0666);
        if (output_fd < 0) {
            fprintf(stderr, "Error: Failed to open/create --output file %s: %s\n", output_filename, strerror(errno));
            exit(3);
        } else {
            close(1);
            dup(output_fd);
            close(output_fd);
        }
    }

    /* Print any remaining command line arguments (not options). */
    if (optind < argc) {
        printf("Error: non-option argv elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        putchar('\n');
        exit(1);
    }

    int flag     = 0;
    char *buffer = (char *)malloc(sizeof(char));
    do {
        flag = read(0, buffer, 1);
        write(1, buffer, 1);
    } while (flag != 0);

    if (!input_filename)
        free(input_filename);

    if (!output_filename)
        free(output_filename);
    exit(0);
}

void segfault() {
    int *dum = NULL;
    *dum     = 0;
}

void seghandler() {
    fprintf(stderr, "Error: Segmentation fault\n");
    exit(4);
}

void handleErr(int err) {
    switch (err) {
        case 1:
            fprintf(stderr, "Operation not permitted\n");
            break;
        case 2:
            fprintf(stderr, "No such file or directory\n");
            break;
        case 13:
            fprintf(stderr, "Permission denied\n");
            break;
        case 17:
            fprintf(stderr, "File exists\n");
            break;
        case 27:
            fprintf(stderr, "File too large\n");
            break;

        default:
            fprintf(stderr, "No additional information available.\n");
            break;
    }
}