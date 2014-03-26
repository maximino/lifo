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
#include <string.h>

int LINESIZE = 4096;

struct CompraVale {
  int fecha;
  int id;
  char kind;
};

void quick_sort (struct CompraVale *a, int n) {
  if (n < 2)
    return;
  int p = a[n / 2].fecha;
  struct CompraVale *l = a;
  struct CompraVale *r = a + n - 1;
  while (l <= r) {
    if (l->fecha > p) {
      l++;
      continue;
    }
    if (r->fecha < p) {
      r--;
      continue; // we need to check the condition (l <= r) every time we change the value of l or r
    }
    struct CompraVale t = *l;
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

void extract_element_from_line(char * line, char * element, int column) {
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
}

int convert_to_int(char *element) {
  int j = 0, i = 0;
  char ch = element[i++];
  char temp[LINESIZE];
  
  while (ch != '\0') {
    if(isdigit(ch)) {
      temp[j++]=ch;
    }
    ch = element[i++];
  }
  temp[j] = '\0';
  return (int)(strtoimax(temp, NULL, 10));
}

void initialize(char *element) {
  for (int i = 0; i < LINESIZE; i++) {
    element[i] = '\0';
  }
}

int main(int argc, const char * argv[])
{
  const char * file_compras = "/Users/max/Documents/Work/Code/xcode/lifo/ueps/compras.csv";
  const char * file_vales = "/Users/max/Documents/Work/Code/xcode/lifo/ueps/vales.csv";
  FILE *iofile_compras;
  FILE *iofile_vales;
  int ch;
  char line[LINESIZE];
  char element[LINESIZE];
  struct CompraVale compras_vales[32768] = {0};
  int index = 0, i = 0;
  
  if ((iofile_compras = fopen(file_compras, "r")) == NULL) {
    fprintf(stderr, "Could not open '%s' for input.\n", file_compras);
  }
  else if ((iofile_vales = fopen(file_vales, "r")) == NULL) {
    fprintf(stderr, "Could not open '%s' for input.\n", file_vales);
  }
  else {
    while ((ch = getc(iofile_compras)) != EOF) {
      if ( ch != '\n'){
        line[index++] = ch;
      }
      else {
        line[index] = '\0';
        index = 0;
        compras_vales[i].kind = 'c';
        extract_element_from_line(line, element, 0);
        compras_vales[i].id = convert_to_int(element);
        extract_element_from_line(line, element, 1);
        compras_vales[i++].fecha = convert_to_int(element);
      }
      
    }
    while ((ch = getc(iofile_vales)) != EOF) {
      if ( ch != '\n'){
        line[index++] = ch;
      }
      else {
        line[index] = '\0';
        index = 0;
        compras_vales[i].kind = 'v';
        extract_element_from_line(line, element, 0);
        compras_vales[i].id = convert_to_int(element);
        extract_element_from_line(line, element, 1);
        compras_vales[i++].fecha = convert_to_int(element);
      }
    }
    fclose(iofile_compras);
    fclose(iofile_vales);
  }
  
  printf("Sort.\n");

  int n = sizeof compras_vales / sizeof compras_vales[0];
  quick_sort(compras_vales, n);
  
  printf("Ending.\n");
  return 0;
}

