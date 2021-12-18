#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const int MAX = 8;
const int OUTPUT_MAX = 4;
const int INPUT_MAX = 10;

char digit_mapping[INPUT_MAX] = {
    0b1110111,
    0b0010010,
    0b1011101,
    0b1011011,
    0b0111010,
    0b1101011,
    0b1101111,
    0b1010010,
    0b1111111,
    0b1111011
};
char digit_segments_count[INPUT_MAX] = {
    6,
    2,
    5,
    5,
    4,
    5,
    6,
    3,
    7,
    6
};

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

void print_options(char *options) {
    char pow_2 = 1;
    for (char c = 'a'; c <= 'g'; c += 1) {
        printf("%c - "BYTE_TO_BINARY_PATTERN" ", c, BYTE_TO_BINARY(options[ c - 'a' ]));
        pow_2 = 1;
        for (char decoded = 'g'; decoded >= 'a'; decoded -= 1) {
            if (options[c-'a'] & pow_2) {
                printf("%c ", decoded);
            }
            pow_2 *= 2;
        }
        printf("\n");
    }
    printf("\n\n");
}

int decode(char *to_orig_segment, char *signal) {
    char decoded = 0;
    for (int i = 0; i < strlen(signal); i += 1) {
        decoded += (char)pow(2, 6 - (to_orig_segment[ signal[i] - 'a' ] - 'a'));
    }
    for (int i = 0; i < 10; i += 1) {
        if (digit_mapping[i] == decoded) {
            return i;
        }
    }
    return -1;
}

// cfgabde

int validate(char *to_orig_segment, char signals[10][8]) {
    for (int i = 0; i < 10; i += 1) {
        if (decode(to_orig_segment, signals[i]) < 0) {
            return 0;
        }
    }
    return 1;
}

int process(char *options, char *taken_segment, char *to_orig_segment, char encoded_segment, char signals[10][8]) {
    char pow_2 = 1;
    pow_2 = 1;
    int result = 0;
    for (char orig_segment = 'g'; orig_segment >= 'a'; orig_segment -= 1) {
        if ( options[ encoded_segment - 'a' ] & pow_2 && !taken_segment[orig_segment - 'a'] ) {
            to_orig_segment[encoded_segment - 'a'] = orig_segment;
            taken_segment[orig_segment - 'a'] = 1;
            if (encoded_segment == 'g') {
                taken_segment[orig_segment - 'a'] = 0;
                if (!validate(to_orig_segment, signals)) {
                    to_orig_segment[encoded_segment - 'a'] = 0;
                    return 0;
                } else {
                    return 1;
                }
            } else {
                if (process(options, taken_segment, to_orig_segment, encoded_segment + 1, signals)) {
                    return 1;
                }
            }
            taken_segment[orig_segment - 'a'] = 0;
            to_orig_segment[encoded_segment - 'a'] = 0;
        }
        pow_2 *= 2;
    }
    return 0;
}

int main() {
    char signals[INPUT_MAX][MAX], outputs[OUTPUT_MAX][MAX], exit = 0, len = 0, del;
    int result = 0, sum = 0, number = 0;
    char options[INPUT_MAX], current_options[INPUT_MAX];
    char taken_segments[INPUT_MAX], to_orig_segment[INPUT_MAX];
    
    FILE *f = fopen("input.txt", "r");
    while ( !exit ) {
        memset(options, 0b11111111, sizeof(char) * INPUT_MAX);
        for (int i = 0; i < 10; i += 1) {
            if ( fscanf(f, "%s", signals[i]) != 1 ) {
                exit = 1;
                break;
            }
            len = strlen(signals[i]);
            memset(current_options, 0, sizeof(char) * INPUT_MAX);
            for (int ch_idx = 0; ch_idx < len; ch_idx += 1) {
                for (int digit = 0; digit < INPUT_MAX; digit += 1) {
                    if (len == digit_segments_count[digit]) {
                        current_options[ signals[i][ch_idx] - 'a' ] |= digit_mapping[digit];
                    }
                }
//                printf("Current options:\n");
//                print_options(current_options);
//                printf("Options:\n");
//                print_options(options);
                options[ signals[i][ch_idx] - 'a' ] &= current_options[ signals[i][ch_idx] - 'a' ];
            }
        }
        
        memset(taken_segments, 0, sizeof(char) * INPUT_MAX);
//        printf("Starting solution with:\n");
//        print_options(options);
        if (!process(options, taken_segments, to_orig_segment, 'a', signals)) {
            printf("No solutions\n");
        }
        
        if (fscanf(f, " %c ", &del) != 1 || del != '|') {
            break;
        }
        number = 0;
        for (int i = 0; i < 4; i += 1) {
            if (fscanf(f, "%s", outputs[i]) != 1) {
                exit = 1;
                break;
            }
            number = number * 10 + decode(to_orig_segment, outputs[i]);
        }
        printf("Number is %d\n", number);
        sum += number;
        
//        print_options(options);
    }
    fclose(f);
    
    printf("%d\n", sum);
}
