#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "group.h"

const char *CHARSET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int FACTOR[REPEATED_PATTERN_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

const double EN_FREQ[] = {8.34, 1.54, 2.73, 4.14, 12.60, 2.03, 1.92, 6.11, 6.71, 0.23, 0.87, 4.24, 2.53, 6.80, 7.70, 1.66, 0.09, 5.68, 6.11, 9.37, 2.85, 1.06, 2.34, 0.20, 2.04, 0.06}; 

const double FREQ[] = {9.42, 1.02,	2.64,	3.39, 15.87, 0.95, 1.04, 0.77, 8.41, 0.89, 0.00, 5.34, 3.24, 7.15, 5.14, 2.86, 1.06, 6.46, 7.90, 7.26, 6.24, 2.15, 0.00, 0.30, 0.24, 0.32};

typedef struct group group;

typedef struct groups groups;

void test_prim(int offset, int min_key_length, int max_key_length) {                                   		\
	for (int i = min_key_length; i < max_key_length; i++) {
		printf("\t%c", (offset % FACTOR[i] == 0) ? 'x' : ' ');
	}
	printf("\n");
}

int int_compar(const int *x1, const int *x2) {
  return (*x1 < *x2) - (*x1 > *x2);
}

void print_sep(int min_key_length, int max_key_length) {
	printf("\t\t\t\t\t");
	for (int i = min_key_length; i < max_key_length; i++) {
		printf("\t|");
	}
	printf("\n");
}

void print_sizes(int end, int min_key_length, int max_key_length) {
	if (end == 1) {
		print_sep(min_key_length, max_key_length);
	}
	printf("[+] Possible key size: \t\t\t");
	for (int i = min_key_length; i < max_key_length; i++) {
		printf("\t%d", FACTOR[i]);
	}
	printf("\n");
	if (end == 0) {
		print_sep(min_key_length, max_key_length);
	}
}

void frequency(const char *str) {
	int freq[ALPHABET_SIZE];
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		freq[i] = 0;
	}
	const char *p = str;
	while (*p != '\0') {
		freq[*p - 'A']++;
		p++;
	}
	int c;
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		c = 'A' + i;
		printf("%c %d\n", c, freq[i]);
	}
}

char *read_cipher(const char *filename) {
	FILE *cipher = fopen(filename, "r");
  if (cipher == NULL) {
    perror("[-] fopen ");
    return NULL;
  }

	fseek(cipher, 0, SEEK_END);
	size_t cipher_length = (size_t) ftell(cipher);
	char *buffer = malloc(sizeof(char) * cipher_length);
	if (buffer == NULL) {
		perror("[-] malloc");
		return NULL;
	}
	fseek(cipher, 0, SEEK_SET);
	int index = 0;
	int c;
	while ((c = fgetc(cipher)) != EOF) {
		if (isalpha(c)) {
			if (c >= 97) {
				buffer[index] = (char) c - 32;
			} else {
				buffer[index] = (char) c;
			}
			index = index + 1;
		}
	}
	if (fclose(cipher) != 0) {
		perror("[-] fclose");
		return NULL;
	}
	return buffer;
}

void print_row(const char *str) {
	int index = 0;
	printf("************\n");
	while (str[index] != '\0') {
		printf("%c", str[index]);
		index = index + 1;
		if (index % 120 == 0) {
			printf("\n\n");
		} else if (index % 60 == 0) {
			printf("\n");
		}
	}
	printf("\n************\n\n");
}

int guess_key(const char *str, size_t pattern_length, size_t min_key_length, size_t max_key_length) {
	char pattern[pattern_length + 1];
	strncpy(pattern, str, pattern_length);
	pattern[pattern_length] = '\0';
	str++;

	char *result = strstr(str, pattern);
	if (result == NULL) {
		return -1;
	}
	int offset = (int) (result - str) + 1;
	if ((offset <= 1) || (offset > REPEATED_PATTERN_SIZE)) {
		return -1;
	}
	printf("[+] Repeated pattern %8s \t offset %d", pattern, offset);
	test_prim(offset, min_key_length, max_key_length);
	return offset;
}

void decrypt(char *filename, const char *key) {
	size_t key_length = strlen(key);
	printf("Decrypt with key %s (%lu)\n", key, key_length);

	FILE *cipher = fopen(filename, "r");
  if (cipher == NULL) {
    perror("[-] fopen ");
    return;
  }

	size_t i = 0;
	int c;
	int k;
	int e;
	while ((c = fgetc(cipher)) != EOF) {
		if (isalpha(c)) {
			if (c >= 97) {
				c = c - 32;
			}
			c = c - 'a';
			k = (int) key[i % key_length] - 'a';
			e = (c - k) % 26;
			if (e < 0) {
				e = 26 - (-e);
			}
			e = e + 'a';
			printf("%c", e);
			i++;
		} else {
			printf("%c", c);
		}
	}
	if (fclose(cipher) != 0) {
		perror("[-] fclose");
		return;
	}
}

void print_groups(const groups *nth_letters_group) {
	printf("*********** %d ***********\n", nth_letters_group -> n_group);
	for (int i = 0; i < nth_letters_group -> n_group; i++) {
		group g = nth_letters_group -> list[i];
		printf("%d %s\n", g.index, g.str);
	}
	printf("\n*********** %d ***********\n\n", nth_letters_group -> n_group);
}

double get_score(const char *str) {
	double score = 0;
	int len = (int) strlen(str);
	char *p = (char *) str;

	while (*p != '\0') {
		score = score + FREQ[*p - 'A'];
		p++;
	}
	return (score / (double) len);
}

char *test_decrypt_groups(groups *nth_letters_group) {
	groups *s = nth_letters_group;
	char *MKEY = malloc(sizeof(char) * (size_t) s -> n_group);
	int i;
	for (i = 0; i < s -> n_group; i++) {
		group g = s -> list[i];
		printf("Working on group %d: \n%s\n", g.index, g.str);
		printf("***************\n");
		char SUBKEY;
		double max_score = 0;
		for (int j = 0; j < ALPHABET_SIZE; j++) {
			char key = CHARSET[j];
			printf("Working with subkey %c:\n", key);
			char *p = g.str;
			char *q = malloc(strlen(p) * sizeof(char));
			size_t x = 0;
			int c;
			int k;
			int e;
			while (*p != '\0') {
				c = (int) *p - 'A';
				k = (int) key - 'A';
				e = (c - k) % 26;
				if (e < 0) {
					e = 26 - (-e);
				}
				e = e + 'A';
				q[x] = (char) e;
				x++;
				p++;
			}
			double score = get_score(q);
			if (score > max_score) {
				max_score = score;
				SUBKEY = key;
			}
			printf("%s %lf\n", q, score);
		}
		MKEY[i] = SUBKEY;
	}
	MKEY[i] = '\0';
	return MKEY;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("[-] Usage : %s <file> [--min-key-length] [--max-key-length]\n", argv[0]);
    return EXIT_FAILURE;
  } 
	char *key = "";
	char *kfile = "";
	int max_key_length = KEY_SIZE;
	int min_key_length = 1;
	for (int i = 0; i < argc; i++) {
		char *arg = argv[i];
		if (strcmp(arg, "--key") == 0) {
			key = argv[i + 1];
		}
		if (strcmp(arg, "--kfile") == 0) {
			kfile = argv[i + 1];
		}
		if (strcmp(arg, "--max-key-length") == 0) {
			max_key_length = atoi(argv[i + 1]);
		}
		if (strcmp(arg, "--min-key-length") == 0) {
			min_key_length = atoi(argv[i + 1]) - 1;
		}
	}
	if (strcmp(key, "") != 0) {
		decrypt(argv[1], argv[3]);
	} else if (strcmp(kfile, "") != 0) {
		const char *key = read_cipher(argv[3]);
		decrypt(argv[1], key);
	} else {
		const char *cipher = read_cipher(argv[1]);
		print_row(cipher);
		print_sizes(0, min_key_length, max_key_length);
		const char *p = cipher;
		size_t pattern_length = PATTERN_LENGTH;
		while (pattern_length < PATTERN_LENGTH_MAX) {
			p = cipher;
			while (*p != '\0') {
				guess_key(p, pattern_length, min_key_length, max_key_length);
				p++;
			}
			pattern_length += 1;
		}
		print_sizes(1, min_key_length, max_key_length);

		int key_length = 0;
		printf("[+] Key length to try: ");
		scanf("%d", &key_length);

		groups *nth_letters_group = split_in_groups_of_length(key_length, cipher);
		print_groups(nth_letters_group);

		const char *key = test_decrypt_groups(nth_letters_group);
		printf("[+] Possible key: %s\n", key);
		decrypt(argv[1], key);
	}
}
