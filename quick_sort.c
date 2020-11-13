#include "quick_sort.h"

void mem_swap(void *s1, void *s2, size_t size) {
  char *p1 = s1;
  char *p2 = s2;
  char p;
  if ((p2 >= p1 + size) || (p1 >= p2 + size)) {
    for (size_t i = 0; i < size; ++i) {
      p = *(p1 + i);
      *(p1 + i) = *(p2 + i);
      *(p2 + i) = p;
    }
  }
}

void *partition(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
  if (nmemb == 0 || size == 0) {
    return NULL;
  }
  char *p = base;
  char *curr = p;
  char *avancement = p;
  char *pivot = p + (nmemb * size) - size;
  while (curr < pivot) {
    if (compar(curr, pivot) == -1) {
      mem_swap(curr, avancement, size);
      avancement += size;
    }
    curr += size;
  }
  mem_swap(pivot, avancement, size);
  return (void *) (((size_t) avancement - (size_t) p) / size);
}

void quick_sort(void *base, size_t nmemb, size_t size,
    int (*compar)(const void *, const void *)) {
  char *p = base;
  char *e = p + nmemb*size;
  if (p < e) {
    size_t pivot = (size_t) partition(base, nmemb, size, compar);
    quick_sort(p, pivot, size, compar);
    quick_sort(p + pivot*size + size, nmemb - pivot - 1, size, compar);
  }
}
