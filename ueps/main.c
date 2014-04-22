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
#include <sys/time.h>
#include <sys/resource.h>

int LINESIZE = 4096;
const int DATASIZE = 65536;

struct Consolidado {
  char kind;
  int fecha;
  int id;
  int division_id;
  int articulo_id;
  int cantidad;
  int cv_id;
};

struct CompraVale {
  int id;
  int fecha;
};

struct Articulo {
  int id;
  int equivalencia;
};

int compare(const void *arg1, const void *arg2)
{
  struct Consolidado a, b;
  a = *(struct Consolidado*)arg1;
  b = *(struct Consolidado*)arg2;
  
  if(a.fecha > b.fecha)
    return 1;
  else if(a.fecha < b.fecha)
    return -1;
  else
  {
    if(a.kind > b.kind)
      return 1;
    else if(a.kind < b.kind)
      return -1;
    else
      return 0;
  }
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

double get_time()
{
  struct timeval t;
  struct timezone tzp;
  gettimeofday(&t, &tzp);
  return t.tv_sec + t.tv_usec*1e-6;
}

int search_date(int id, struct CompraVale * compras, int count_compras) {
  for (int i = 0; i < count_compras; i++) {
    if (id == compras[i].id)
      return compras[i].fecha;
  }
  return 0;
}

int get_equivalencia(int id, struct Articulo * articulos, int count_articulos) {
  for (int i = 0; i < count_articulos; i++) {
    if (id == articulos[i].id)
      return articulos[i].equivalencia;
  }
  return 0;
}


int main(int argc, const char * argv[])
{
  float startTime = (float)clock()/CLOCKS_PER_SEC;
  double start = get_time();
  const char * file_compras = "/Users/max/Documents/Work/Code/xcode/lifo/ueps/compras.csv";
  const char * file_mov_compras = "/Users/max/Documents/Work/Code/xcode/lifo/ueps/movimientos_compras.csv";
  const char * file_vales = "/Users/max/Documents/Work/Code/xcode/lifo/ueps/vales.csv";
  const char * file_art_vales = "/Users/max/Documents/Work/Code/xcode/lifo/ueps/articulo_vales.csv";
  const char * file_articulos = "/Users/max/Documents/Work/Code/xcode/lifo/ueps/articulos.csv";
  FILE *iofile_compras;
  FILE *iofile_mov_compras;
  FILE *iofile_vales;
  FILE *iofile_art_vales;
  FILE *iofile_articulos;
  int ch;
  char line[LINESIZE];
  char element[LINESIZE];
  struct Consolidado consolidado[DATASIZE] = {};
  struct CompraVale compras[DATASIZE] = {};
  struct CompraVale vales[DATASIZE] = {};
  struct Articulo articulos[DATASIZE] = {};
  int index = 0, insert_count = 0, count_articulos = 0, count_vales = 0, count_compras = 0;
  
  if ((iofile_compras = fopen(file_compras, "r")) == NULL) {
    fprintf(stderr, "Could not open '%s' for input.\n", file_compras);
  }
  else if ((iofile_mov_compras = fopen(file_mov_compras, "r")) == NULL) {
    fprintf(stderr, "Could not open '%s' for input.\n", file_mov_compras);
  }
  else if ((iofile_vales = fopen(file_vales, "r")) == NULL) {
    fprintf(stderr, "Could not open '%s' for input.\n", file_vales);
  }
  else if ((iofile_art_vales = fopen(file_art_vales, "r")) == NULL){
    fprintf(stderr, "Could not open '%s' for input.\n", file_art_vales);
  }
  else if ((iofile_articulos = fopen(file_articulos, "r")) == NULL){
    fprintf(stderr, "Could not open '%s' for input.\n", file_articulos);
  }
  else {
    while ((ch = getc(iofile_mov_compras)) != EOF) {
      if ( ch != '\n'){
        line[index++] = ch;
      }
      else {
        line[index] = '\0';
        index = 0;
        consolidado[insert_count].kind = 'c';
        extract_element_from_line(line, element, 0);
        consolidado[insert_count].cv_id = convert_to_int(element);
        extract_element_from_line(line, element, 1);
        consolidado[insert_count].id = convert_to_int(element);
        extract_element_from_line(line, element, 2);
        consolidado[insert_count].articulo_id = convert_to_int(element);
        extract_element_from_line(line, element, 6);
        consolidado[insert_count].cantidad = convert_to_int(element);
        extract_element_from_line(line, element, 8);
        consolidado[insert_count++].division_id = convert_to_int(element);
      }
      
    }
    while ((ch = getc(iofile_art_vales)) != EOF) {
      if ( ch != '\n'){
        line[index++] = ch;
      }
      else {
        line[index] = '\0';
        index = 0;
        consolidado[insert_count].kind = 'v';
        extract_element_from_line(line, element, 0);
        consolidado[insert_count].cv_id = convert_to_int(element);
        extract_element_from_line(line, element, 1);
        consolidado[insert_count].id = convert_to_int(element);
        extract_element_from_line(line, element, 2);
        consolidado[insert_count].articulo_id = convert_to_int(element);
        extract_element_from_line(line, element, 5);
        consolidado[insert_count].cantidad = convert_to_int(element);
        extract_element_from_line(line, element, 6);
        consolidado[insert_count++].division_id = convert_to_int(element);
      }
    }
    while ((ch = getc(iofile_compras)) != EOF) {
      if ( ch != '\n'){
        line[index++] = ch;
      }
      else {
        line[index] = '\0';
        index = 0;
        extract_element_from_line(line, element, 0);
        compras[count_compras].id = convert_to_int(element);
        extract_element_from_line(line, element, 1);
        compras[count_compras++].fecha = convert_to_int(element);
      }
    }
    while ((ch = getc(iofile_vales)) != EOF) {
      if ( ch != '\n'){
        line[index++] = ch;
      }
      else {
        line[index] = '\0';
        index = 0;
        extract_element_from_line(line, element, 0);
        vales[count_vales].id = convert_to_int(element);
        extract_element_from_line(line, element, 1);
        vales[count_vales++].fecha = convert_to_int(element);
      }
    }
    while ((ch = getc(iofile_articulos)) != EOF) {
      if ( ch != '\n'){
        line[index++] = ch;
      }
      else {
        line[index] = '\0';
        index = 0;
        extract_element_from_line(line, element, 0);
        articulos[count_articulos].id = convert_to_int(element);
        extract_element_from_line(line, element, 11);
        articulos[count_articulos++].equivalencia = convert_to_int(element);
      }
    }

    fclose(iofile_compras);
    fclose(iofile_mov_compras);
    fclose(iofile_vales);
    fclose(iofile_art_vales);
  }
  
  int id, fecha, equivalencia;
  char kind;
  
  int rec;
  for (rec = 0; rec < insert_count; rec++) {
    kind = consolidado[rec].kind;
    if (kind == 'c') {
      id = consolidado[rec].id;
      fecha = search_date(id, compras, count_compras);
      equivalencia = get_equivalencia(id, articulos, count_articulos);
      consolidado[rec].fecha = fecha;
      consolidado[rec].cantidad = consolidado[rec].cantidad * equivalencia;
    }
    else {
      id = consolidado[rec].id;
      fecha = search_date(id, vales, count_vales);
      consolidado[rec].fecha = fecha;
    }
  }
  
  printf("Sort.\n");
  
  qsort(consolidado, rec, sizeof(consolidado[0]), compare);
  
  printf("Ending.\n");
  
  double finish = get_time();
  float endTime = (float)clock()/CLOCKS_PER_SEC;
  
  for (int i = 0; i < rec + 1; i++) {
    printf("%d. id:%d fecha:%d tipo:%c cantidad:%d articulo:%d\n", i, consolidado[i].id, consolidado[i].fecha, consolidado[i].kind, consolidado[i].cantidad, consolidado[i].articulo_id);
  }
  
  printf("Registros: %d.\n", rec);
  
  printf("Time Elapsed 1: %f \n", finish - start);
  printf("Time Elapsed 2: %f \n", endTime - startTime);
  
  return 0;
}

