//File : restoran.h
/* merupakan tipe data restoran */

#ifndef MEJA_H
#define MEJA_H

#include "../point/point.h"

typedef struct {
  int bangku; //jumlah bangku
  Point posisi; //x,y menyatakan posisi di matriks
  int status; //[0,1,2]
  /*
    0 : kosong
    1 : ada pelanggan belum order
    2 : ada pelanggan sudah order
  */
} Meja;

#define Bangku(M) (M).bangku
#define Meja_Posisi(M) (M).posisi
#define Status(M) (M).status

#endif
