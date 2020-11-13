#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "quick_sort.h"

#define FUN_SUCCESS 0
#define FUN_FAILURE -1

#define KEY_SIZE 	128
#define KEYS 			12
#define PATTERN_LENGTH 3
#define PATTERN_LENGTH_MAX 7

#define BUF_SIZE 256

const char *CHARSET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
#define ALPHABET_SIZE 26

#define FACTOR_LENGTH 8

const double FREQ[] = {8.34, 1.54, 2.73, 4.14, 12.60, 2.03, 1.92, 6.11, 6.71, 0.23, 0.87, 4.24, 2.53, 6.80, 7.70, 1.66, 0.09, 5.68, 6.11, 9.37, 2.85, 1.06, 2.34, 0.20, 2.04, 0.06}; 

int FACTOR[FACTOR_LENGTH] = {2, 3, 4, 5, 6, 7, 8, 9};

struct group {
	int index;
	int length;
	char *str;
};

typedef struct group group;

#define GROUP_SIZE sizeof(int) * 2 + sizeof(char) * BUF_SIZE

struct groups {
	int n_group;
	group list[PATTERN_LENGTH_MAX];
};

typedef struct groups groups;


void test_prim(int offset) {                                   		\
	for (int i = 0; i < FACTOR_LENGTH; i++) {
		printf("\t%c", (offset % FACTOR[i] == 0) ? 'x' : ' ');
	}
	printf("\n");
}

int int_compar(const int *x1, const int *x2) {
  return (*x1 < *x2) - (*x1 > *x2);
}

void print_factors() {
	printf("\t\t\t\t\t\t\t");
	for (int i = 0; i < FACTOR_LENGTH; i++) {
		printf("\t%d", FACTOR[i]);
	}
	printf("\n");
	printf("\t\t\t\t\t\t\t");
	for (int i = 0; i < FACTOR_LENGTH; i++) {
		printf("\t|");
	}
	printf("\n");
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
	//quick_sort(freq, sizeof freq / sizeof *freq, sizeof *freq, (int (*) (const void *, const void *)) int_compar);
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

int guess_key(const char *str, size_t pattern_length) {
	char pattern[pattern_length + 1];
	strncpy(pattern, str, pattern_length);
	pattern[pattern_length] = '\0';
	str++;

	char *result = strstr(str, pattern);
	if (result == NULL) {
		return -1;
	}
	int offset = (int) (result - str) + 1;
	if ((offset <= 1) || (offset > KEY_SIZE)) {
		return -1;
	}
	printf("[+] Found possible keys starting by %s \t of length %d", pattern, offset);
	test_prim(offset);
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

/*
char *decrypt(const char *str, const char *key) {
	size_t key_length = strlen(key);
	printf("Decrypt with key %s (%lu)\n", key, key_length);

	char *p = (char *) str;
	char *q = malloc(strlen(str) * sizeof(char));
	size_t i = 0;
	int c;
	int k;
	int e;
	while (*p != '\0') {
		c = (int) *p - 'A';
		k = (int) key[i % key_length] - 'A';
		e = (c - k) % 26;
		if (e < 0) {
			e = 26 - (-e);
		}
		e = e + 'A';
		q[i] = (char) e;
		i++;
		p++;
	}

	return q;
}
*/

group *group_init(int i) {
	group *g = malloc(sizeof(g));
	if (g == NULL) {
		return NULL;
	}
	g -> index = i;
	g -> length = 0;
	g -> str = malloc(1000 * sizeof(char));
	return g;
}

groups *split_in_groups_of_length(int length, const char *str) {
	size_t groups_size = GROUP_SIZE * (size_t) length + sizeof(int);
	groups *groups = malloc(groups_size);
	groups -> n_group = length;
	for (int i = 0; i < length; i++) {
		group *g = group_init(i);
		groups -> list[i] = *g;
	}

	char *p = (char *) str;
	int i = 0;
	while (*p != '\0') {
		printf("%c", *p);
		int index = (groups -> list[i % length]).length;
		(groups -> list[i % length]).str[index] = *p;
		(groups -> list[i % length]).length++;
		p++;
		i++;
	}
	printf("\n");
	return groups;
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
    printf("[-] Usage : %s <file>\n", argv[0]);
    return EXIT_FAILURE;
  } 
	if ((argc == 4) && (strcmp(argv[2], "--key") == 0)) {
		decrypt(argv[1], argv[3]);
		/*
		const char *plaintext = decrypt(argv[1], argv[3]);
		print_row(plaintext);
		*/
	} else {
		const char *cipher = read_cipher(argv[1]);
		print_row(cipher);
		print_factors();
		const char *p = cipher;
		size_t pattern_length = PATTERN_LENGTH;
		while (pattern_length < PATTERN_LENGTH_MAX) {
			p = cipher;
			while (*p != '\0') {
				guess_key(p, pattern_length);
				p++;
			}
			pattern_length += 1;
		}
		
		//frequency(cipher);

		int key_length = 0;
		printf("[+] Key length to try: ");
		scanf("%d", &key_length);
		groups *nth_letters_group = split_in_groups_of_length(key_length, cipher);
		print_groups(nth_letters_group);
		const char *key = test_decrypt_groups(nth_letters_group);
		printf("[+] Find possible key: %s\n", key);
	}

  return EXIT_SUCCESS;
}
