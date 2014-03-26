//
//  main.c
//  ueps
//
//  Created by Maximino Salazar-Lechuga on 3/25/14.
//  Copyright (c) 2014 Evomatik. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

int LINESIZE = 4096;

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

int extract_date (int * date) {
    int year = 1000 * date[0] + 100 * date[1] + 10 * date[2] + date[3];
    int month = 10 * date[5] + date[6];
    int day = 10 * date[8] + date[9];
    return 10000 * year + 100 * month + day;
}

void extract_element_from_line(char *line, int element) {
    char ele_char [128];
    int count = 0;
    
    while (count < LINESIZE) {
        ele_char[count] = line[count];
    }
    
}


int main(int argc, const char * argv[])
{
    const char * compras = "/Users/max/Development/lifo/ueps/compras.csv";
    FILE *iofile_compras;
    int ch;
//    long count, count_div;
    char line[LINESIZE];
    int index = 0;
    
    if ((iofile_compras = fopen(compras, "r")) == NULL) {
        fprintf(stderr, "Could not open '%s' for input.\n", compras);
    }
    
//    count_div = 0;
    while ((ch = getc(iofile_compras)) != EOF) {
        if ( ch != '\n'){
            line[index++] = ch;
        }
        else {
            line[index] = '\0';
            index = 0;
            printf ( "%s\n", line );
            extract_element_from_line(line, 22);
        }
    }
    
//    if (ch == ';') {
//        count_div++;
//    }
//    else {
//        count_div = 0;
//    }
//    // if count_div = 5 means column separator
//    if (count_div == 5) {
//        
//    }
    
    fclose(iofile_compras);
//    printf("File '%s'has %ld characters\n.", compras, count);
    
    
    
    int a[] = {4, 65, 2, -31, 0, 99, 2, 83, 782, 1};
    int n = sizeof a / sizeof a[0];
    quick_sort(a, n);
    
    
    
    // insert code here...
    printf("Ending.\n");
    return 0;
}

