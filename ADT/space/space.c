/* Kelompok  : UAS
   Nama file : space.c
   Topik     : Engi's Kitchen Game
   Tanggal   : 1 November 2018
   Deskripsi : ADT untuk tipe space*/

#include "space.h"
#include "../../std.h"
#include "../point/point.h"
#include "matTile/matTile.h"
#include "arrMeja/arrMeja.h"
#include "grafRuangan/grafRuangan.h"
#include "pelayan.h"
#include "restoran.h"

// *** KONSTRUKTOR ***
void InitPelayan(Pelayan *P)
/* membuat Pelayan baru
  I.S. Sembarang
  F.S. terbentuk Pelayan dengan posisi di 0,0
      semua karakter bernilai CharUndeff
      semua value bernilai ValUndeff
      semua deskripsi berniali ""
*/
{
  //ALGORITMA
  //posisi
  P_SetXY(&Pelayan_Posisi(*P),5,5);
  //up
  Up(*P) = MT_CreateTile(CharUndeff,ValUndeff);
  //down
  Down(*P) = MT_CreateTile(CharUndeff,ValUndeff);
  //left
  Left(*P) = MT_CreateTile(CharUndeff,ValUndeff);
  //right
  Right(*P) = MT_CreateTile(CharUndeff,ValUndeff);
}
void PlacePelayan(Pelayan *P,int x,int y, MatTile M)
/*
  menaruh Pelayan di posisi (x,y) di ruangan di Restoran r
  I.S. Pelayan,x,y,R terdefinisi
  F.S. Pelayan terletak di posisi x,y di ruangan di R Tile bersampingan terupdate
*/
{
  //ALGORITMA
  P_SetXY(&Pelayan_Posisi(*P),x,y);
  SetUpTile(P,M);
  SetDownTile(P,M);
  SetLeftTile(P,M);
  SetRightTile(P,M);
}
void InitRestoran(Restoran *R)
/*
  menyiapkan ruangan kosong dan menghubungkan semua ruangan
  I.S. R Sembarang
  F.S. setiap room.karakter terisi "",
      room.value berisi ValUndeff
      room.deskripsi berisi ""
      lokasi pusat meja sudah ada nomornya,
      ruangan berniali 0,
      hubungan telah menghubungkan semua ruangan.
*/
{
  //ALGORITMA
  // for (int i = AR_GetFirstIdx(Ruangan(*R));i <= AR_GetLastIdx(Ruangan(*R));i++) {
  //   MT_MakeMatriks(8,8,&Room(AR_Elmt(GR_Info(Ruangan(*R)),i)));
  // }
  GR_CreateEmpty(&Ruangan(*R));
  RoomNow(*R) = 0;
}

// *** PINDAH ***
void Move(Pelayan *P,Restoran *R, int code, boolean *status)
/*
  I.S. Pelayan dan Restoran terdifinsi, Pelayang dapat bergerak ke arah code
  F.S. Pelayan pindah ke posisi sesuai code dan update semua karakter
  code: 1 = Up
        2 = Right
        3 = Down
        4 = Left
*/
{
  //KAMUS
  int x,y;
  int i,j;
  boolean found;
  GR_address p,p1;
  GRD_address pt,door;
  Door pintu;
  Point tujuan;
  int arah,nomorRuang;
  MatTile M;
  //ALGORITMA
  //P_GetXY(Pelayan_Posisi(P),&x,&y);
  found = false;
  p = GR_First(Ruangan(*R));
  while (p!=Nil && !found) {
    pt = GR_Doors(p);
    door = GRD_Search(pt,Pelayan_Posisi(*P));
    if (door != Nil) {
      found = true;
    } else {
      p = GR_Next(p);
    }
  } //found == true || p == nil

  if (found) {
    pintu = GRD_Info(door);
    tujuan = DoorLocation(GRD_Info(GRD_To(door)));
    P_GetXY(tujuan,&i,&j);
    arah = DoorDirection(pintu);
    nomorRuang = DoorRoomID(GRD_Info(GRD_To(door)));
  } else {
    nomorRuang = RoomNow(*R);
  }
  p1 = GR_Search(Ruangan(*R),nomorRuang);
  M = Room(GR_Info(p1));

  *status = false;
  switch (code) {
    case 1:
      if (Karakter(Up(*P))==' ') {
        *status =true;
        P_Geser(&Pelayan_Posisi(*P),-1,0);
        SetUpTile(P,M);
      } else if (found) { //di pintu bisa naik
        if (arah==code) {
          *status =true;
          PlacePelayan(P,i,j,M);
          RoomNow(*R) = nomorRuang;
        }
      }
      break;
    case 2:
      if (Karakter(Right(*P))==' ') {
        *status =true;
        P_Geser(&Pelayan_Posisi(*P),0,1);
        SetRightTile(P,M);
      } else if (found) { //di pintu bisa naik
        if (arah==code) {
          *status =true;
          PlacePelayan(P,i,j,M);
          RoomNow(*R) = nomorRuang;
        }
      }
      break;
    case 3:
      if (Karakter(Down(*P))==' ') {
        *status =true;
        P_Geser(&Pelayan_Posisi(*P),1,0);
        SetDownTile(P,M);
      } else if (found) { //di pintu bisa naik
        if (arah==code) {
          *status =true;
          PlacePelayan(P,i,j,M);
          RoomNow(*R) = nomorRuang;
        }
      }
      break;
    case 4:
      if (Karakter(Left(*P))==' ') {
        *status =true;
        P_Geser(&Pelayan_Posisi(*P),0,-1);
        SetLeftTile(P,M);
      } else if (found) { //di pintu bisa naik
        if (arah==code) {
          *status =true;
          PlacePelayan(P,i,j,M);
          RoomNow(*R) = nomorRuang;
        }
      }
      break;
  };
}
// void MoveUp(Pelayan *P,MatTile M)
// /*
//   I.S. Pelayan dan Restoran terdifinsi, Pelayan masih dapat naik
//   F.S. Pelayan pindah ke posisi di atasnya dan update semua karakter
// */
// {
//   //ALGORITMA
//   P_Geser(&Pelayan_Posisi(*P),-1,0);
//   SetUpTile(P,M);
// }
// void MoveDown(Pelayan *P,MatTile M)
// /*
//   I.S. Pelayan dan Restoran terdifinsi, Pelayan masih dapat turun
//   F.S. Pelayan pindah ke posisi di bawahnya dan update semua karakter
// */
// {
//   //ALGORITMA
//   P_Geser(&Pelayan_Posisi(*P),1,0);
//   SetDownTile(P,M);
// }
// void MoveLeft(Pelayan *P,MatTile M)
// /*
//   I.S. Pelayan dan Restoran terdifinsi, Pelayan masih dapat bergerak ke kiri
//   F.S. Pelayan pindah ke posisi di kirinya dan update semua karakter
// */
// {
//   //ALGORITMA
//   P_Geser(&Pelayan_Posisi(*P),0,-1);
//   SetLeftTile(P,M);
// }
// void MoveRight(Pelayan *P,MatTile M)
// /*
//   I.S. Pelayan dan Restoran terdifinsi, Pelayan masih dapat bergerak ke kanan
//   F.S. Pelayan pindah ke posisi di kanannya dan update semua karakter
// */
// {
//   //ALGORITMA
//   P_Geser(&Pelayan_Posisi(*P),0,1);
//   SetRightTile(P,M);
// }

// *** BOOLEAN PELAYAN ***
// boolean CanMove(Pelayan P, Restoran R, int code)
// /*
//   fungsi bernilai true jika pelayan dapat Move sesuai code
//   termasuk saat di pintu
// */
// {
//   //KAMUS
//   boolean flag;
//   //ALGORITMA
//   flag = false;
//   switch (code) {
//     case 1:
//       if (Karakter(Up(P))==' ') {
//         flag = true;
//       } else if () { //di pintu bisa naik
//         flag = true;
//       }
//     case 2:
//       if (Karakter(Right(P))==' ') {
//         flag = true;
//       } else if () { //di pintu bisa naik
//         flag = true;
//       }
//     case 3:
//       if (Karakter(Down(P))==' ') {
//         flag = true;
//       } else if () { //di pintu bisa naik
//         flag = true;
//       }
//     case 4:
//       if (Karakter(left(P))==' ') {
//         flag = true;
//       } else if () { //di pintu bisa naik
//         flag = true;
//       }
//   };
//   return flag;
// }
// boolean CanMoveUp(Pelayan P,MatTile M)
// /*
//   fungsi bernilai true jika pelayan dapat MoveUp dan P bukan di pintu
// */
// {
//   //KAMUS
//   boolean flag;
//   //ALGORITMA
//   flag = false;
//   if (Karakter(Up(P))==' ') {
//     flag = true;
//   }
//   return flag;
// }
// boolean CanMoveDown(Pelayan P,MatTile M)
// /*
//   fungsi bernilai true jika pelayan dapat MoveDown dan P bukan di pintu
// */
// {
//   //KAMUS
//   boolean flag;
//   //ALGORITMA
//   flag = false;
//   if (Karakter(Down(P))==' ') {
//     flag = true;
//   }
//   return flag;
// }
// boolean CanMoveLeft(Pelayan P,MatTile M)
// /*
//   fungsi bernilai true jika pelayan dapat MoveLeft dan P bukan di pintu
// */
// {
//   //KAMUS
//   boolean flag;
//   //ALGORITMA
//   flag = false;
//   if (Karakter(Left(P))==' ') {
//     flag = true;
//   }
//   return flag;
// }
// boolean CanMoveRight(Pelayan P,MatTile M)
// /*
//   fungsi bernilai true jika pelayan dapat MoveRight dan P bukan di pintu
// */
// {
//   //KAMUS
//   boolean flag;
//   //ALGORITMA
//   flag = false;
//   if (Karakter(Right(P))==' ') {
//     flag = true;
//   }
//   return flag;
// }
// boolean IsOnDoor(Pelayan P, Restoran R)
// /*
//   fungsi mengambalikan true jika P berada di pintu
// */
// {
//   //KAMUS
//   int x,y;
//   boolean found;
//   GR_address p;
//   GRD_address pt,door;
//   //ALGORITMA
//   P_GetXY(Pelayan_Posisi(P),&x,&y);
//   found = false;
//   p = GR_First(Ruangan(R));
//   while (p!=Nil && !found) {
//     pt = GR_Doors(p);
//     door = GRD_Search(pt,Pelayan_Posisi(P));
//     if (door != Nil) {
//       found = true;
//     } else {
//       p = GR_Next(p);
//     }
//   } //found == true || p == nil
//   return found;
// }
boolean CanOrder(Pelayan P, Ruangan R)
/*
  fungsi akan bernialai true jika meja dekat p dapat memesan
*/
{
  //KAMUS
  int nomorMeja;
  //ALGORITMA
  nomorMeja = GetTableNumber(P,R);
  return Status(AM_Elmt(Meja(R),nomorMeja)) ==  1;
}
boolean CanGive(Pelayan P, Ruangan R, int nomorMeja)
/*
  fungsi akan bernilai true jika nomorMeja sama dengan nomor dari
  meja di dekat P
*/
{
  //KAMUS
  Tile lantai;
  //ALGORITMA
  lantai = GetTableTile(P,R);
  return nomorMeja == Value(lantai);
}

// *** Tile Set ***
void SetUpTile(Pelayan *P,MatTile M)
/*
  I.S. P dan M terdefinisi
  F.S. Tile Up pada pelayan diisi dengan elemt yang sesuai dengan M
*/
{
  //KAMUS
  int x,y;
  Tile tmp;
  //ALGORITMA
  P_GetXY(Pelayan_Posisi(*P),&x,&y);
  x -= 1;
  if (MT_IsIdxValid(x,y)) {
    tmp = MT_Elmt(M,x,y);
    Up(*P) = MT_CreateTile(Karakter(tmp),Value(tmp));
  } else {
    Up(*P) = MT_CreateTile(CharUndeff,ValUndeff);
  }
}
void SetDownTile(Pelayan *P,MatTile M)
/*
  I.S. P dan M terdefinisi
  F.S. Tile Down pada pelayan diisi dengan elemt yang sesuai dengan M
*/
{
  //KAMUS
  int x,y;
  Tile tmp;
  //ALGORITMA
  P_GetXY(Pelayan_Posisi(*P),&x,&y);
  x += 1;
  if (MT_IsIdxValid(x,y)) {
    tmp = MT_Elmt(M,x,y);
    Down(*P) = MT_CreateTile(Karakter(tmp),Value(tmp));
  } else {
    Down(*P) = MT_CreateTile(CharUndeff,ValUndeff);
  }
}
void SetLeftTile(Pelayan *P,MatTile M)
/*
  I.S. P dan M terdefinisi
  F.S. Tile Left pada pelayan diisi dengan elemt yang sesuai dengan M
*/
{
  //KAMUS
  int x,y;
  Tile tmp;
  //ALGORITMA
  P_GetXY(Pelayan_Posisi(*P),&x,&y);
  y -= 1;
  if (MT_IsIdxValid(x,y)) {
    tmp = MT_Elmt(M,x,y);
    Left(*P) = MT_CreateTile(Karakter(tmp),Value(tmp));
  } else {
    Left(*P) = MT_CreateTile(CharUndeff,ValUndeff);
  }
}
void SetRightTile(Pelayan *P,MatTile M)
/*
  I.S. P dan M terdefinisi
  F.S. Tile Right pada pelayan diisi dengan elemt yang sesuai dengan M
*/
{
  //KAMUS
  int x,y;
  Tile tmp;
  //ALGORITMA
  P_GetXY(Pelayan_Posisi(*P),&x,&y);
  y += 1;
  if (MT_IsIdxValid(x,y)) {
    tmp = MT_Elmt(M,x,y);
    Right(*P) = MT_CreateTile(Karakter(tmp),Value(tmp));
  } else {
    Right(*P) = MT_CreateTile(CharUndeff,ValUndeff);
  }
}

// // *** RUANGAN ***
// void IsiRuang(Restoran *R, int ruangan, Ruangan Ru)
// /*
//   I.S. R sudah terdeinisi, ruangan dan M juga terdefinisi
//   F.S. room ke-(ruangan) di set menjadi seperti M
// */
// {
//   //ALGORITMA
//
//   //MT_CopyMatriks(M,&Room(AR_Elmt(GR_Info(Ruangan(*R)),ruangan)));
// }

// *** BOOLEAN RUANGAN***
boolean IsTableEmpty(int nomorMeja, Ruangan R)
/*
  mengirimkan true jika meja dengan nomor meja nomor adalah kosong dan bisa
  di duduki, mengirimkan false jika tidak
*/
{
  //ALGORITMA
  return Status(AM_Elmt(Meja(R),nomorMeja)) ==  0;
}
boolean CanPlace(int pelanggan,Pelayan P, Ruangan R)
/*
  fungsi menghasilkan true jika meja di dekat P dapat diduduki oleh Pelanggan
  yaitu jumlah bangkunya <= dan sedang kosong
  P harus sebelah bangku ('x')
*/
{
  //KAMUS
  boolean empty;
  Tile table;
  int jumlahBangku;
  //ALGORITMA
  table = GetTableTile(P,R);
  jumlahBangku = Bangku(AM_Elmt(Meja(R),Value(table)));
  empty = IsTableEmpty(Value(table),R);
  return empty && (jumlahBangku<=pelanggan);
}

// *** ACTION ***
void Ordering(Pelayan P,Ruangan *R, int *idMakanan,int *nomorMeja)
/*
  I.S. P dan *R terdefinisi, dilakukan jika sudah dipastikan dapat order dan P
    dekat Meja
  F.S.
*/
{
  //KAMUS
  Tile table;
  time_t t;
  //ALGORITMA
  srand((unsigned) time(&t)); //inisialisasi rnd()
  *idMakanan = (rand()%8+17); //[17..24]
  *nomorMeja = GetTableNumber(P,*R);
  Status(AM_Elmt(Meja(*R),*nomorMeja)) = 2;
}
int Taking(Pelayan P)
/*
  menghasilkan id makanan yang diambil dengan syarat di dekat P hanya ada
  satu meja bahan ('m')
*/
{
  //KAMUS
  int makanan;
  //ALGORITMA
  if (Karakter(Up(P)) == 'm'){ // 'm' ada di atas
    makanan = Value(Up(P));
  } else if (Karakter(Down(P)) == 'm'){ // 'm' ada di bawah
    makanan = Value(Down(P));
  } else if (Karakter(Left(P)) == 'm'){ // 'm' ada di kiri
    makanan = Value(Left(P));
  } else { //'m' ada di kanan
    makanan = Value(Right(P));
  }
  return makanan;
}
void Placing(int pelanggan,int waktuOut,Pelayan *P, Ruangan *R)
/*
  I.S. pelanggan,waktuOut,Pelayan, retoran terdefinisi,
      meja sebelah Pelayan adalah kosong
      pelanggan merupakan jumlah pelanggan yang akan duduk
      waktuOut merupakan waktu pelanggan pergi
      hanya dipanggil jika yakin pelanggan bisa diletakkan
  F.S. pelanggan di tempatkan di meja sesuai jumlahnya
      karakter jadi 'c', value jadi waktuOut
*/
{
  //KAMUS
  int nomorMeja;
  int x,y;
  int i,j;
  int jumlahBangku;
  //ALGORITMA
  P_GetXY(Pelayan_Posisi(*P),&x,&y);

  nomorMeja = GetTableNumber(*P,*R);
  P_GetXY(IndeksMeja(nomorMeja,*R),&i,&j);
  Status(AM_Elmt(Meja(*R),nomorMeja)) = 1;
  jumlahBangku = Bangku(AM_Elmt(Meja(*R),nomorMeja));

  //menaruh minimal 2 pelanggan
  MT_Elmt(Room(*R),i,j+1) = MT_CreateTile('c',waktuOut);
  MT_Elmt(Room(*R),i,j-1) = MT_CreateTile('c',waktuOut);
  if (pelanggan == 4) {
    /*jika pelanggannya 4, tambah 2 karakter 'c' lagi
      bukan saat ada 4 jumlahBangku*/
    MT_Elmt(Room(*R),i+1,j) = MT_CreateTile('c',waktuOut);
    MT_Elmt(Room(*R),i-1,j) = MT_CreateTile('c',waktuOut);
  }
  if (jumlahBangku==4) { //jika bangku ada 4, tambah 2 valur dan deskripsi lagi
    MT_Elmt(Room(*R),i+1,j) = MT_CreateTile('x',waktuOut);
    MT_Elmt(Room(*R),i-1,j) = MT_CreateTile('x',waktuOut);
  }
  PlacePelayan(P,x,y,Room(*R));
}

// *** LAIN LAIN ***
Tile GetTableTile(Pelayan P, Ruangan R)
/*
  mengembalikan nilai meja yang bersebelahan dengan Pelayan
*/
{
  //KAMUS
  int nomor;
  int i,j;
  //ALGORITMA
  nomor = GetTableNumber(P,R);
  P_GetXY(IndeksMeja(nomor,R),&i,&j);
  return MT_Elmt(Room(R),i,j);
}
MatTile GetMatTileSekarang(Restoran R)
/*
  fungsi mengembalikan suatu matriks yang sedang digunakan berdasarkan Ruangan(R)
*/
{
  //KAMUS
  GR_address P;
  //ALGORITMA
  P = GR_Search(Ruangan(R),RoomNow(R));
  return (Room(GR_Info(P)));
  //return (Room(AR_Elmt(GR_Info(Ruangan(R)),RoomNow(R))));
}
Ruangan GetRuanganSekarang(Restoran R)
/*
  fungsi mengembalikan suatu tipe Ruangan yang sedang digunakan berdasarkan Ruangan(R)
*/
{
  return(GR_Info(GR_Search(Ruangan(R),RoomNow(R))));
}
void PelangganKabur(int waktuNow,Pelayan *P,Restoran *R, int *jumlah)
/*
  I.S. waktuNow, P, R terdefinisi
  F.S. jumlah berisi jumlah pelanggan yang waktu keluarnya sudah sama
    dengan waktuNow sekaligus mengembalikan status meja menjadi kosong, karakter
    menjadi x deskripsi menjadi "-" dan Value menjadi ValUndeff
*/
{
  //KAMUS
  int x,y;
  int a,b;
  Ruangan tmp;
  GR_address p;
  GR_address p1;
  //ALGORITMA
  *jumlah = 0;
  p1 = GR_First(Ruangan(*R));
  while (p1!=Nil) {
    tmp = GR_Info(p1);
    for(int i = AM_GetFirstIdx(Meja(tmp));i<=AM_GetLastIdx(Meja(tmp));i++) {
        P_GetXY(IndeksMeja(i,tmp),&a,&b);
        if (Value(MT_Elmt(Room(tmp),a,b+1))==waktuNow) {
          //saatnya keluar
          *jumlah += 1;
          SetTableEmpty(i,&GR_Info(p1));
        }
    }
    p1= GR_Next(p1);
  }
  if (*jumlah > 0) { //hanya dilakukan jika ada pelanggan kabur
    P_GetXY(Pelayan_Posisi(*P),&x,&y);
    // p = GR_Search(Ruangan(*R),RoomNow(*R));
    tmp = GetRuanganSekarang(*R);
    // PlacePelayan(P,x,y,Room(GR_Info(p)));
    PlacePelayan(P,x,y,Room(tmp));
  }
}
Point IndeksMeja(int nomor,Ruangan R)
// mengembalikan koordinat dari nomor meja yang diinput. nomor [1..12]
{
  //ALGORITMA
  return Meja_Posisi(AM_Elmt(Meja(R),nomor));
}
void SetTableEmpty(int nomorMeja, Ruangan *R)
/*
  I.S. P dan R terdefinisi, dipanggil jika p dekat meja
  F.S. membuat meja dengan nomor meja nomorMeja menjadi kosong
*/
{
  //KAMUS
  int i,j;
  int jumlahBangku;
  //ALGORITMA
  P_GetXY(IndeksMeja(nomorMeja,*R),&i,&j);
  jumlahBangku = Bangku(AM_Elmt(Meja(*R),nomorMeja));

  //merubah minimal 2 pelanggan
  MT_Elmt(Room(*R),i,j+1) = MT_CreateTile('x',ValUndeff);
  MT_Elmt(Room(*R),i,j-1) = MT_CreateTile('x',ValUndeff);
  if (jumlahBangku==4) { //jika bangku ada 4, tambah 2 valur dan deskripsi lagi
    MT_Elmt(Room(*R),i+1,j) = MT_CreateTile('x',ValUndeff);
    MT_Elmt(Room(*R),i-1,j) = MT_CreateTile('x',ValUndeff);
  }
  Status(AM_Elmt(Meja(*R),nomorMeja)) = 0;
}
int GetTableNumber (Pelayan P, Ruangan R)
/*
  fungsi mengembalikan nomor jika P berada di dekat meja
  akan mengembalikan 0 jika tidak
*/
{
  //KAMUS
  ArrMeja arrmeja;
  Meja meja;
  int x,y;
  int i;
  int a,b;
  boolean found;
  int tmp;
  //ALGORITMA
  P_GetXY(Pelayan_Posisi(P),&x,&y);
  arrmeja = Meja(R);
  found = false;
  i = AM_GetFirstIdx(arrmeja);
  while (!found && i <=AM_GetLastIdx(arrmeja)) {
    meja = AM_Elmt(arrmeja,i);
    P_GetXY(Meja_Posisi(meja),&a,&b);
    if (a-1 == x && b ==y) {
      found = true;
    } else if (a+1 == x && b ==y) {
      found = true;
    } else if (a-2 == x && b ==y) {
      found = true;
    } else if (a+2 == x && b ==y) {
      found = true;
    } else if (a == x && b ==y-2) {
      found = true;
    } else if (a == x && b ==y+2) {
      found = true;
    } else if (a-1 == x && b ==y+1) {
      found = true;
    } else if (a-1 == x && b ==y-1) {
      found = true;
    } else if (a+1 == x && b ==y+1) {
      found = true;
    } else if (a+1 == x && b ==y-1) {
      found = true;
    } else i++;
  } //found == true || i > AM_GetLastIdx(arrmeja)
  if (!found) {
    tmp = 0;
  } else {
    tmp = Value(MT_Elmt(Room(R),a,b));
  }
  return tmp;
}
