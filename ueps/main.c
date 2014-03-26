//
//  main.c
//  ueps
//
//  Created by Maximino Salazar-Lechuga on 3/25/14.
//  Copyright (c) 2014 Evomatik. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <ctype.h>

int LINESIZE = 4096;

struct Compra {
  int fecha;
  int id;
};

void quick_sort (int *a, int n) {
  if (n < 2)
    return;
  int p = a[n / 2];
  int *l = a;
  int *r = a + n - 1;
  while (l <= r) {
    if (*l > p) {
      l++;
      continue;
    }
    if (*r < p) {
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

void extract_element_from_line(char *line, char *element, int column) {
  int i = 0, count = 0, count_column = 0, count_column_separator = 0;
  char ch = line[count];
  
  while (ch != '\n' && count < LINESIZE && count_column <= column) {
    if (ch == ';') {
      count_column_separator++;
    }
    else {
      count_column_separator = 0;
    }
    if (count_column_separator >= 5) {
      count_column++;
    }
    if (count_column == column && count_column_separator < 5) {
      element[i++] = ch;
    }
    count++;
    ch = line[count];
  }
  // Strip the last four ';'
  for (int j = 0; j <= 4; j++) {
    element[i-j]= '\0';
  }
  printf ( "%s\n", element);
}

int convert_to_int(char *element) {
  int i = 0, j = 0;
  char temp[LINESIZE], ch = element[i++];

  
  while (ch != '\0') {
    if(isdigit(ch)) {
      temp[j++]=ch;
    }
    ch = element[i++];
  }
  printf ( "%s\n", temp);
  return (int)(strtoimax(temp, NULL, 10));
}

int main(int argc, const char * argv[])
{
  const char * file_compras = "/Users/max/Documents/Work/Code/xcode/lifo/ueps/compras.csv";
  FILE *iofile_compras;
  int ch;
  char line[LINESIZE];
  char element[LINESIZE];
  int dates[10000] = {0};
  int index = 0, i = 0;
  
  if ((iofile_compras = fopen(file_compras, "r")) == NULL) {
    fprintf(stderr, "Could not open '%s' for input.\n", file_compras);
  }
  else {
    while ((ch = getc(iofile_compras)) != EOF) {
      if ( ch != '\n'){
        line[index++] = ch;
      }
      else {
        line[index] = '\0';
        index = 0;
        extract_element_from_line(line, element, 1);
        dates[i++] = convert_to_int(element);
      }
    }
    fclose(iofile_compras);
  }
  
  printf("Sort.\n");

  int n = sizeof dates / sizeof dates[0];
  quick_sort(dates, n);
  
  // insert code here...
  printf("Ending.\n");
  return 0;
}

