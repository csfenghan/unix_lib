#ifndef HEAP_H
#define HEAP_H

struct heap {
    int* buf;
    int size;
    int _length;

    int is_max;
};

void h_init(struct heap* hp, int is_max);
int h_size(struct heap* hp);
int h_top(struct heap* hp);

int h_empty(struct heap* hp);

int h_push(struct heap* hp, int val);

int h_pop(struct heap* hp);

#endif
