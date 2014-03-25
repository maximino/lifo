//
//  main.c
//  ueps
//
//  Created by Maximino Salazar-Lechuga on 3/25/14.
//  Copyright (c) 2014 Evomatik. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>


void quick_sort (int *a, int n) {
    if (n < 2)
        return;
    int p = a[n / 2];
    int *l = a;
    int *r = a + n - 1;
    while (l <= r) {
        if (*l < p) {
            l++;
            continue;
        }
        if (*r > p) {
            r--;
            continue; // we need to check the condition (l <= r) every time we change the value of l or r
        }
        int t = *l;
        *l++ = *r;
        *r-- = t;
    }
    quick_sort(a, (int)(r - a + 1));
    quick_sort(l, (int)(a + n - l));
}

int main(int argc, const char * argv[])
{
    const char * compras = "/Users/max/Documents/Work/Code/xcode/ueps/ueps/ueps/compras.csv";
    FILE *iofile_compras;
    int ch;
    long count;
    
    if ((iofile_compras = fopen(compras, "r")) == NULL) {
        fprintf(stderr, "Could not open '%s' for input.\n", compras);
    }
    
    while ((ch = getc(iofile_compras)) != EOF) {
        putc(ch, stdout);
        count ++;
    }
    fclose(iofile_compras);
    printf("File '%s'has %ld characters\n.", compras, count);
    
    int a[] = {4, 65, 2, -31, 0, 99, 2, 83, 782, 1};
    int n = sizeof a / sizeof a[0];
    quick_sort(a, n);
    
    // 10000 * year + 100 * month + day;
    
    // insert code here...
    printf("Ending.\n");
    return 0;
}

