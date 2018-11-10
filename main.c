/* Kelompok  : UAS
   Nama file : main.c
   Tanggal   : 3 November 2018
   Deskripsi : Program utama Engi's Kitchen Expansion */

#include "std.h"
#include "ADT.h"

//tampilan main menu
ArrKata MainMenu(){
  //KAMUS
  ArrKata hasil;
  //ALGORITMA
  AK_CreateEmpty(&hasil);
  AK_AddAsLastEl(&hasil,K_MakeKata("Engi's Kitchen"));
  AK_AddAsLastEl(&hasil,K_MakeKata(""));
  AK_AddAsLastEl(&hasil,K_MakeKata("MAIN MENU"));
  AK_AddAsLastEl(&hasil,K_MakeKata("--NEW"));
  AK_AddAsLastEl(&hasil,K_MakeKata("--START"));
  AK_AddAsLastEl(&hasil,K_MakeKata("--LOAD"));
  AK_AddAsLastEl(&hasil,K_MakeKata("--EXIT"));
  return hasil;
}

//tampilan credit
ArrKata Credit(){
  //KAMUS
  ArrKata isicredit;
  //ALGORITMA
  AK_CreateEmpty(&isicredit);
  AK_AddAsLastEl(&isicredit,K_MakeKata("GAME OVER"));
  AK_AddAsLastEl(&isicredit,K_MakeKata(""));
  AK_AddAsLastEl(&isicredit,K_MakeKata(""));
  AK_AddAsLastEl(&isicredit,K_MakeKata("THIS GAME IS CREATED BY :"));
  AK_AddAsLastEl(&isicredit,K_MakeKata(""));
  AK_AddAsLastEl(&isicredit,K_MakeKata("13517012 - JOHANES"));
  AK_AddAsLastEl(&isicredit,K_MakeKata("13517039 - STEVE ANDREAS"));
  AK_AddAsLastEl(&isicredit,K_MakeKata("13517063 - JOSHUA CHRISTO RANDINY"));
  AK_AddAsLastEl(&isicredit,K_MakeKata("13517066 - WILLY SANTOSO"));
  AK_AddAsLastEl(&isicredit,K_MakeKata("13517114 - SEKAR LARASATI MUSLIMAH"));
  return isicredit;
}

//fungsi bernilai true jika input adalah salah satu kata dari main menu
boolean InputBenar(Kata input,Kata new,Kata start,Kata load,Kata keluar){
  return (K_IsKataSama(input,new)||K_IsKataSama(input,start)||K_IsKataSama(input,load)||K_IsKataSama(input,keluar));
}

void RandomPelanggan(PrioQueueCustomer *pqc,int waktuNow)
/*
  I.S. pqc dan waktuNow terdefinisi
  F.S. me-random kedatangan pelanggan
*/
{
  //KAMUS
  int chance;
  int jumlah;
  customer pelanggan;
  //ALGORITMA
  chance = rand()%50;
  if (chance==0 && PQC_Tail(*pqc)<PQC_MaxEl){ //chance customer : 1/10 kemungkinan
    chance = rand()%4;
    PQC_Prio(pelanggan) = (chance==0)? 1:0; //chance prio : 1/4 kemungkinan
    do {
      jumlah = rand()%3+2;
    } while (jumlah ==3);
    PQC_Jumlah(pelanggan) = jumlah;
    PQC_Waktu(pelanggan) = waktuNow + 30;
    PQC_Add(pqc, pelanggan);
  }
}

void PelangganPergi(PrioQueueCustomer *pqc,int waktuNow,int *jumlah)
/*
  I.S. pqc dan waktuNow terdefinisi
  F.S. jumlah = jumlah pelanggan yang waktu == waktuNow
*/
{
  //KAMUS
  PrioQueueCustomer Q2;
  customer pelanggan;
  //ALGORITMA
  PQC_CreateEmpty(&Q2);
  *jumlah = 0;
  while (!PQC_IsEmpty(*pqc)) {
    PQC_Del(pqc,&pelanggan);
    if (PQC_Waktu(pelanggan)==waktuNow) {
      *jumlah += 1;
    } else {
      PQC_Add(&Q2,pelanggan);
    }
  }
  *pqc = Q2;
}

void CekToolTip(Restoran R, Pelayan P,boolean pindahRuang, Kata*IsiToolTip, TreeFood tree)
/*
  I.S. grafRuangan dan posisi pelayang terdefinisi
  F.S. IsiToolTip sebagai kata bantuan untuk barang-barang disiktar pelayang
       boolean pindah ruang bernilai true jika pelayan baru pindah ruangan
*/
{
  //Kamus lokal
  int idtemp; //variabel idmakanan sementara
  char roomtemp[2]; //variabel nomor ruangan sementara
  Kata room, idroom;

  //Algoritma
  if(pindahRuang){
    roomtemp[0] = (char) RoomNow(R) + '0';
    roomtemp[1] = '\0';
    room = K_MakeKata("Room ");
    idroom = K_MakeKata(roomtemp);
    K_KonkatKata(&room,idroom);
    *IsiToolTip = room;
  }
  else if(CanTake(P)){
    idtemp = Taking(P);
    *IsiToolTip = F_NamaMakanan(TF_Akar(TF_Search(tree,idtemp)));
  }
}

int JadiApa(StackFood sf, TreeFood tf)
/* Mengembalikan id makanan jika valid, 0 jika tidak */
{
  /* KAMUS LOKAL */
  Food tempFood;
  int tempLeft;
  int tempRight;

  /* ALGORITMA */
  if(SF_IsEmpty(sf)){
    if(TF_IsEmpty(tf)){
      return 0;
    }else{
      return F_IDMakanan(TF_Akar(tf));
    }
  }else if(TF_IsEmpty(tf)){
    return 0;
  }else{
    SF_Pop(&sf,&tempFood);
    if(F_IDMakanan(tempFood)==F_IDMakanan(TF_Akar(tf))){
      tempLeft  = JadiApa(sf,TF_Left(tf));
      tempRight = JadiApa(sf,TF_Right(tf));
      if(tempLeft==0 && tempRight==0){
        return 0;
      }else{
        if(tempLeft==0){
          return tempRight;
        }else{
          return tempLeft;
        }
      }
    }else{
      return 0;
    }
  }
}

int main() {
  //KAMUS PROGRAM UTAMA
  int money,life,waktu; //uang dan nyawa yang dimiliki pemain dan tik waktu
  int nomorMeja; //nomor meja di dekat pelayan
  int jumlahKabur; //jumlah pelanggan yang kabur (sudah duduk)
  int jumlahPergi; //jumlah pelangan yang pergi (belum duduk)
  int idMakanan; //id dari makanan
  int waktuOut; //waktu saat pelanggan akan kabur
  int kodeArah; //1==up, 2==right, 3==down, 4==left
  int status; //menyatakan berhasil load file atau tidak
  int harga; //harga makanan sesuai kedalaman food tree
  time_t t; //variabel random
  Restoran R; //tipe restoran dengan graf
  Ruangan *room; //tipe ruangan pada restoran
  Pelayan P; //tipe pelayan restoran
  ArrInt arrayNomorMeja; //array berisi nomor-nomor meja pelanggan yang kabur
  ArrOrder arrayOrder; //array berisi orderan yang belum di give
  AO_IdxType indeksOrder; //indeks dari array of order
  PrioQueueCustomer Q1,Q2; //tipe barisan pelanggan
  customer pelanggan; //tipe pelanggan
  Order order; //tipe order
  Order orderKabur; //tipe order yang isinya orderan dari pelanggan kabur
  Food makanan; //tipe food
  TreeFood tree; //tipe tree food
  TF_address masakan; //tipe address food yang berisi alamat masakan
  StackFood hand; //tipe tumpukan makanan di hand
  StackFood tray; //tipe tumpukan makanan di tray
  Kata input; //input dari user
  Kata username,usernameSaved; // Kata untuk nama user
  Kata new,start,load,keluar; //tipe kata pembanding
  Kata IsiToolTip; //kata bantuan yang berisi informasi disekitar pelayan
  Kata namaMakanan; //kata untuk nama makanan orderan
  boolean lose,aksiValid; //tipe validasi
  boolean loaded; //menyatakan ada file yang di load
  boolean saved; //menyatakan sudah save di saat bermain
  boolean pindahRuang; //menyatakan pelayan baru saja pindah ruangan
  GameScreen gs; //tipe untuk GameScreen ncruses

  //ALGORITMA PROGRAM UTAMA

  //inisialisasi awal
  srand((unsigned) time(&t)); //inisiasi random
  new = K_MakeKata("NEW");
  start = K_MakeKata("START");
  load = K_MakeKata("LOAD");
  keluar = K_MakeKata("EXIT");
  saved = true;
  username = K_MakeKata("");
  PQC_CreateEmpty(&Q1);
  LoadTree(&status,&tree);

  do { //looping game
    InitScreen(&gs);
    WriteText(&gs,MainMenu()); //main menu
    loaded = false; // masuk main menu == belum load
    do { //meminta input hingga benar
      input = GetInput(&gs,K_MakeKata("CHOICE: "));
    }
    while (!InputBenar(input,new,start,load,keluar));

    if (!K_IsKataSama(input,keluar)) { //inputnya bukan exit
      if (K_IsKataSama(input,new)) { //NEW
        username=GetInput(&gs,K_MakeKata("NEW USERNAME : "));
        //baca file konfigurasi normal
      }
      else if (K_IsKataSama(input,start)) { //START
        //menampilkan username yang tersedia
        if(username.Length==0)
          username = GetInput(&gs,K_MakeKata("INPUT USERNAME: "));
        //dibaca diulang hingga daper username yang benar
        //baca save file dengan nama
      }
      else if (K_IsKataSama(input,load)) { //LOAD
        //prosedure load
        loaded = true;
        do {
          username = GetInput(&gs,K_MakeKata("SAVED USERNAME: "));
          LoadFile(&status,&username,&money,&life,&waktu,&R,&P,&Q1,&hand,&tray,&arrayOrder);
          if (status ==0 && !K_IsKataSama(input,keluar)) {
            do {
              input = GetInput(&gs,K_MakeKata("USERNAME SALAH!"));
            } while(!K_IsKataSama(input,K_MakeKata("")));
          }
        } while (status==0 && !K_IsKataSama(input,keluar));
      }
      if (!K_IsKataSama(input,keluar)) { //inputnya langsung exit
        if (!loaded) { //tidak ada file yang di load
          //load konfigurasi normal
          usernameSaved = K_MakeKata("basic");
          LoadFile(&status,&usernameSaved,&money,&life,&waktu,&R,&P,&Q1,&hand,&tray,&arrayOrder);
          SF_CreateEmpty(&hand,10);
          SF_CreateEmpty(&tray,5);
          AO_CreateEmpty(&arrayOrder);
        }

        //inisialisasi game
        lose = false;
        pindahRuang = false;
        RefreshMap(&gs,GetMatTileSekarang(R),Pelayan_Posisi(P));
        IsiToolTip = K_MakeKata("");

        do{ //looping command di dalam game
          CekToolTip(R,P,pindahRuang,&IsiToolTip,tree);
          aksiValid = false;
          //refresh tampilan di layar
          RefreshTooltipPanel(&gs,IsiToolTip);
          RefreshTopPanel(&gs,K_KataToChar(username),money,life,waktu);
          //meminta input perintah
          input = GetInput(&gs,K_MakeKata("COMMAND : "));
          if(input.TabKata[1]=='G' && input.Length==2){ //inputnya move
            if(input.TabKata[2]=='U'){ //GU
              kodeArah=1;
            }
            else if(input.TabKata[2]=='R'){ //GR
              kodeArah=2;
            }
            else if(input.TabKata[2]=='D'){ //GD
              kodeArah=3;
            }
            else if(input.TabKata[2]=='L'){ //GL
              kodeArah=4;
            }
            Move(&P,&R,kodeArah,&aksiValid,&pindahRuang);
            if (aksiValid) {
              RefreshMap(&gs,GetMatTileSekarang(R),Pelayan_Posisi(P));
            }
          }
          else if(K_IsKataSama(input,K_MakeKata("PUT"))){ //nunggu stack dan tree
            if(CanPut(P)){
              if(!SF_IsFull(tray)){
                idMakanan = JadiApa(SF_ReversStack(hand),tree);
                if(idMakanan<0){
                  SF_CreateEmpty(&hand,10);
                  SF_Push(&tray,TF_Akar(TF_Search(tree,idMakanan)));
                  aksiValid = true;
                  RefreshHandPanel(&gs,hand);
                }
              }
            }
          } //akhir command put
          else if(K_IsKataSama(input,K_MakeKata("TAKE"))){ //nunggu stack tree
            if (CanTake(P)) {
              if(!SF_IsFull(hand)){
                idMakanan = Taking(P);
                makanan = TF_Akar(TF_Search(tree,idMakanan));
                SF_Push(&hand,makanan);
                aksiValid = true;
                RefreshHandPanel(&gs,hand);
              }
            }
          } //akhir command take
          else if(K_IsKataSama(input,K_MakeKata("CH"))){ //nunggu stack
            if(!SF_IsEmpty(hand)){
              SF_CreateEmpty(&hand,10);
              aksiValid = true;
              RefreshHandPanel(&gs,hand);
            }
          } //akhir command ch
          else if(K_IsKataSama(input,K_MakeKata("CT"))){ //nunggu stack
            if(!SF_IsEmpty(tray)){
              SF_CreateEmpty(&tray,5);
              aksiValid = true;
              RefreshFoodPanel(&gs,tray);
            }
          } //akhir command ct
          else if(K_IsKataSama(input,K_MakeKata("RECIPE"))){ //nunggu tree makanan
            ShowTree(&gs,tree);
          } //akhir command recipe
          else if(K_IsKataSama(input,K_MakeKata("SAVE"))){
            //procedure save
            saved = true;
            SaveFile(username,money,life,waktu,R,P,Q1,hand,tray,arrayOrder);
            do {
              input = GetInput(&gs,K_MakeKata("GAME SAVED."));
            } while (!K_IsKataSama(input, K_MakeKata("")));
          }//akhir command save
          else if(K_IsKataSama(input,K_MakeKata("EXIT"))){
            if (!saved) { //dipanggil jika belum di save
              do {
                input=GetInput(&gs,K_MakeKata("ARE YOU SURE EXIT WITHOUT SAVE CURRENT FILE ? : "));
                if(K_IsKataSama(input,K_MakeKata("YES"))){ //jadi exit
                  input = K_MakeKata("EXIT");
                }
                else if(K_IsKataSama(input,K_MakeKata("NO"))) { //batal exit
                  //inisialisasi agar tidak exit game
                  input = K_MakeKata("batalexit");
                }
              } while(!K_IsKataSama(input,K_MakeKata("YES")) && !K_IsKataSama(input,K_MakeKata("NO")));
            }
          } //akhir comman exit
          else { //aksi2 yang butuh dekat Meja
            room = GetRuanganSekarang(R);
            nomorMeja = GetTableNumber(P,*room);

            if (nomorMeja!=0) { //artinya deket meja
              if(K_IsKataSama(input,K_MakeKata("PLACE"))){
                PQC_CreateEmpty(&Q2);
                while(!CanPlace(PQC_Jumlah(PQC_InfoHead(Q1)),P,*room) && !PQC_IsEmpty(Q1)){
                  PQC_Del(&Q1,&pelanggan);
                  PQC_Add(&Q2,pelanggan);
                }//bisa place atau sudah dicek semua

                if (CanPlace(PQC_Jumlah(PQC_InfoHead(Q1)),P,*room)) { //ada yang bisa di place
                  aksiValid = true;
                  PQC_Del(&Q1,&pelanggan);
                  if(PQC_Prio(pelanggan)==1){
                    waktuOut = waktu + (rand()%40+81); //[81..120]
                  }
                  else{
                    waktuOut = waktu + (rand()%60+121); //[121..180]
                  }

                  Placing(PQC_Jumlah(pelanggan),waktuOut,&P,room);

                  while(!PQC_IsEmpty(Q1)) {
                    PQC_Del(&Q1,&pelanggan);
                    PQC_Add(&Q2,pelanggan);
                  } //Q1 pasti kosong
                  RefreshMap(&gs,GetMatTileSekarang(R),Pelayan_Posisi(P));
                  RefreshWaitingPanel(&gs, Q1);
                } //akhir placing
                Q1 = Q2;
              } //akhir command place
              else if(K_IsKataSama(input,K_MakeKata("GIVE"))){  //nunggu stack
                  indeksOrder = AO_Search(arrayOrder,nomorMeja);
                  if(indeksOrder!=IdxUndeff){
                    makanan = SF_InfoTop(tray);
                    if(O_IDMakanan(AO_Elmt(arrayOrder,indeksOrder))==F_IDMakanan(makanan)){
                      SF_Pop(&tray,&makanan);
                      money += F_Harga(makanan);
                      AO_DelEli(&arrayOrder,indeksOrder,&orderKabur);
                      SetTableEmpty(nomorMeja,room);
                      aksiValid = true;
                      RefreshMap(&gs,GetMatTileSekarang(R),Pelayan_Posisi(P));
                      RefreshFoodPanel(&gs,tray);
                      RefreshOrderPanel(&gs,arrayOrder);
                    }
                  }
              } //akhir command give
              else if(K_IsKataSama(input,K_MakeKata("ORDER"))){ //nunggu stack dan converter idmakanan
                if(CanOrder(P,*room)){
                  aksiValid = true;
                  Ordering(P,room,&idMakanan,&nomorMeja);
                  masakan = TF_Search(tree,idMakanan);
                  namaMakanan = F_NamaMakanan(TF_Akar(masakan));
                  order = O_CreateOrder(idMakanan,namaMakanan,nomorMeja); //tree pada namaMakanan
                  AO_AddAsLastEl(&arrayOrder,order);
                  RefreshOrderPanel(&gs,arrayOrder);
                } //akhir can order
              } //akhir command order
            } //akhir cek nomor meja
          } //akhir proses validasi command

          if(aksiValid){//proses yang terjadi jika inputnya valid
            RandomPelanggan(&Q1,waktu);
            waktu++; //tik bertambah
            PelangganKabur(waktu,&P,&R,&jumlahKabur,&arrayNomorMeja);
            while(!AI_IsEmpty(arrayNomorMeja)){
              AI_DelLastEl(&arrayNomorMeja,&nomorMeja);
              indeksOrder = AO_Search(arrayOrder,nomorMeja);
              if(indeksOrder!=IdxUndeff){
                AO_DelEli(&arrayOrder,indeksOrder,&orderKabur);
              }
            }
            PelangganPergi(&Q1,waktu,&jumlahPergi);
            RefreshWaitingPanel(&gs, Q1);
            RefreshOrderPanel(&gs,arrayOrder);
            life -=jumlahKabur+jumlahPergi;
            saved = false; //saved false karena ada aksi yang berhasil
            if(life<=0){ //jika nyawa==0 maka kalah
              lose = true;
            }
          }//akhir proses aksiValid
        }
        while(!K_IsKataSama(input,keluar) && !lose);

        if(lose){ //pemain kalah tampilkan credit
          InitScreen(&gs);
          WriteText(&gs,Credit());
          do {
            input = GetInput(&gs,K_MakeKata("YOU LOSE!"));
          } while(!K_IsKataSama(input,K_MakeKata("")));
        }
      } //inputnya langsung keluar
      //inisialisasi input agar kembali ke main menu
      input = K_MakeKata("mainmenu");
    } //input == exit
  } while (!K_IsKataSama(input,keluar));
  //input == exit
  QuitScreen(&gs); //quit game
  return 0;
}
