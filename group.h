#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "const.h"

struct group {
	int index;
	int length;
	char *str;
};

typedef struct group group;

struct groups {
	int n_group;
	group list[PATTERN_LENGTH_MAX];
};

typedef struct groups groups;

extern group *group_init(int i);

extern groups *split_in_groups_of_length(int length, const char *str);
