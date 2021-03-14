#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_SIZE 16

static int h_resize(struct heap *hp, int nsize) {
        int *new_buf;

        printf("last:%d  new:%d\n", hp->_length, nsize);

        if (nsize <= hp->_length)
                return -1;
        if ((new_buf = realloc(hp->buf, nsize * sizeof(int))) == NULL)
                return -1;

        hp->buf = new_buf;
        hp->_length = nsize;

        return 1;
}

/*
 * is_map：
 * 	1：最大堆
 * 	0：最小堆
 *
 */
void h_init(struct heap *hp, int is_max) {
        int length;

        length = INIT_SIZE;
        hp->buf = malloc(length * sizeof(int));
        hp->_length = length;
        hp->size = 0;
        hp->is_max = is_max;
}

int h_size(struct heap *hp) { return hp->size; }

int h_empty(struct heap *hp) { return hp->size == 0; }

int h_top(struct heap *hp) {

        if (h_empty(hp)) {
                fprintf(stderr, "h_top error:heap is empty!\n");
                exit(1);
        }

        return hp->buf[0];
}

int h_push(struct heap *hp, int val) {
        int curr, prev;

        if (hp->size == hp->_length) {
                if (h_resize(hp, 2 * hp->_length) < 0)
                        return -1;
        }

        curr = hp->size;
        hp->buf[curr] = val;
        while (curr > 0) {
                prev = (curr - 1) / 2;
                if (hp->is_max) { //如果是最大堆
                        if (val > hp->buf[prev]) {
                                hp->buf[curr] = hp->buf[prev];
                                hp->buf[prev] = val;
                        } else {
                                break;
                        }

                } else { //如果是最小堆
                        if (val < hp->buf[prev]) {
                                hp->buf[curr] = hp->buf[prev];
                                hp->buf[prev] = val;
                        } else {
                                break;
                        }
                }
                curr = prev;
        }
        hp->size++;

        return 1;
}

int h_pop(struct heap *hp) {
        int prev,curr,next,result,last_num;

        if (h_empty(hp)) {
                fprintf(stderr, "h_pop error:heap is empty\n");
                exit(1);
        }

	last_num=hp->buf[--hp->size];
        result = hp->buf[0];
        curr = 0;
        next = 2 * curr + 1;
        while (1) {
                if (next >= hp->size)
                        break;
                if (next + 1 >= hp->size) {
                        hp->buf[curr] = hp->buf[next];
                        break;
                }

                if (hp->is_max) { //最大堆
                        if (hp->buf[next] > hp->buf[next + 1]) {
                                hp->buf[curr] = hp->buf[next];
                                curr = next;
                        } else {
                                hp->buf[curr] = hp->buf[next + 1];
                                curr = next + 1;
                        }
                } else { //最小堆
                        if (hp->buf[next] < hp->buf[next + 1]) {
                                hp->buf[curr] = hp->buf[next];
                                curr = next;
                        } else {
                                hp->buf[curr] = hp->buf[next + 1];
                                curr = next + 1;
                        }
                }
                next = 2 * curr + 1;
        }
	hp->buf[curr]=last_num;
	prev=(curr-1)/2;
	if(hp->is_max){
		while(hp->buf[prev]<last_num){
			hp->buf[curr]=hp->buf[prev];
			hp->buf[prev]=last_num;
			curr=prev;
			prev=(curr-1)/2;
		}
	}else{
		while(hp->buf[prev]>last_num){
			hp->buf[curr]=hp->buf[prev];
			hp->buf[prev]=last_num;
			curr=prev;
			prev=(curr-1)/2;
		}
	}


        return result;
}
