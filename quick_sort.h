#include <stddef.h>

void quick_sort(void *base, size_t nmemb, size_t size,
    int (*compar)(const void *, const void *));
