#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "group.h"

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
