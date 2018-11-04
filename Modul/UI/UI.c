#include "UI.h"
#include <ncurses.h>
#include "../../ADT/point/point.h"
#include "../../boolean.h"
#include "../../ADT/space/matTile/matTile.h"
#include "../../ADT/customer/prioqueuecustomer.h"
#include "../../ADT/food/stacktfood.h"
#include <locale.h>

void DrawBorders(WINDOW *screen)
/* I.S. : screen sudah diinisialisasi */
/* F.S. : Digambar border pada screen yang dimau, tipe border standar */
{
  BorderType standardBorder;

  UL(standardBorder) = ACS_ULCORNER;
  UR(standardBorder) = ACS_URCORNER;
  LL(standardBorder) = ACS_LLCORNER;
  LR(standardBorder) = ACS_LRCORNER;

  DrawBordersCC(screen, standardBorder);
}

void DrawBordersCC(WINDOW *screen, BorderType border)
/* I.S. : Screen sudah diinisialisasi, border terdefinisi */
/* F.S. : Digambar border pada screen yanh dimau, tipe border sesuai input*/
{
  /* KAMUS LOKAL */
  int x, y, i;

  /* ALGORITMA */
  getmaxyx(screen, y, x);

  // Pojok
  mvwaddch(screen, 0, 0, UL(border));
  mvwaddch(screen, y - 1, 0, LL(border));
  mvwaddch(screen, 0, x - 1, UR(border));
  mvwaddch(screen, y - 1, x - 1, LR(border));

  // sisi
  for (i = 1; i < (y - 1); i++) {
    mvwaddch(screen, i, 0, ACS_VLINE);
    mvwaddch(screen, i, x - 1, ACS_VLINE);
  }

  // atas bawah
  for (i = 1; i < (x - 1); i++) {
    mvwaddch(screen, 0, i, ACS_HLINE);
    mvwaddch(screen, y - 1, i, ACS_HLINE);
  }
}

void InitScreen(GameScreen *gs)
/* I.S. : Bebas */
/* F.S. : Terinisialisai layar kosong dengan window diset ke gs */
{
  /* KAMUS LOKAL */
  int parentX,parentY;
  int sideWidth, sideHeight;
  int topBarHeight;
  int mainWidth, mainHeight;
  int top2Width;

  /* ALGORITMA */
  setlocale(LC_ALL, "");

  initscr();
  noecho();
  getmaxyx(stdscr, parentY, parentX);

  topBarHeight = 3;

  sideWidth = parentX*UIC_Side;
  sideHeight = (parentY-(2*topBarHeight))/2;
  mainWidth = parentX - (2*sideWidth);
  mainHeight = parentY - 2*topBarHeight;

  top2Width = mainWidth*6/10;

  Top_1_Panel(*gs) = newwin(topBarHeight,sideWidth,0,0);
  Top_2_Panel(*gs) = newwin(topBarHeight,top2Width,0,sideWidth);
  Top_3_Panel(*gs) = newwin(topBarHeight, mainWidth-top2Width, 0,sideWidth+top2Width);
  Top_4_Panel(*gs) = newwin(topBarHeight, sideWidth,0,sideWidth+mainWidth);

  Waiting_Panel(*gs) = newwin(sideHeight,sideWidth,topBarHeight,0);
  Food_Panel(*gs) = newwin(sideHeight, sideWidth, topBarHeight, sideWidth+mainWidth);
  Order_Panel(*gs) = newwin(sideHeight,sideWidth,topBarHeight+sideHeight,0);
  Hand_Panel(*gs) = newwin(sideHeight,sideWidth, topBarHeight+sideHeight, sideWidth+mainWidth);

  Command_Panel(*gs) = newwin(topBarHeight, parentX, topBarHeight+mainHeight, 0);

  Main_Panel(*gs) = newwin(2*sideHeight, mainWidth,topBarHeight, sideWidth);

  Main_Panel_Width(*gs) = mainWidth;
  Main_Panel_Height(*gs) = mainHeight;
  Side_Panel_Width(*gs) = sideWidth;
  Side_Panel_Height(*gs) = sideHeight;

  RefreshBorder(gs);
}

void QuitScreen(GameScreen *gs)
{
  endwin();
}

void RefreshBorder(GameScreen *gs)
/* I.S. : Bebas sudah initScreen */
/* F.S. : Dicetak semua border */
{
  clear();

  refresh();

  wclear(Top_1_Panel(*gs));
  wclear(Top_2_Panel(*gs));
  wclear(Top_3_Panel(*gs));
  wclear(Top_4_Panel(*gs));
  wclear(Waiting_Panel(*gs));
  wclear(Food_Panel(*gs));
  wclear(Order_Panel(*gs));
  wclear(Hand_Panel(*gs));
  wclear(Command_Panel(*gs));
  wclear(Main_Panel(*gs));

  DrawBorders(Top_1_Panel(*gs));
  DrawBorders(Top_2_Panel(*gs));
  DrawBorders(Top_3_Panel(*gs));
  DrawBorders(Top_4_Panel(*gs));

  DrawBorders(Waiting_Panel(*gs));
  DrawBorders(Food_Panel(*gs));
  DrawBorders(Order_Panel(*gs));
  DrawBorders(Hand_Panel(*gs));
  DrawBorders(Command_Panel(*gs));
  DrawBorders(Main_Panel(*gs));

  wrefresh(Main_Panel(*gs));
  wrefresh(Top_1_Panel(*gs));
  wrefresh(Top_2_Panel(*gs));
  wrefresh(Top_3_Panel(*gs));
  wrefresh(Top_4_Panel(*gs));
  wrefresh(Command_Panel(*gs));
  wrefresh(Waiting_Panel(*gs));
  wrefresh(Hand_Panel(*gs));
  wrefresh(Order_Panel(*gs));
  wrefresh(Food_Panel(*gs));
}

void refreshLayout(GameScreen *gs)
/* I.S. : gs Terinisialisai screen diresize*/
/* F.S. : layout menyesuaikan*/
{
  /* KAMUS LOKAL */
  int parentX,parentY;
  int sideWidth, sideHeight;
  int topBarHeight;
  int mainWidth, mainHeight;
  int top2Width;

  /* ALGORITMA */
  getmaxyx(stdscr, parentY, parentX);

  topBarHeight = 3;

  sideWidth = parentX*UIC_Side;
  sideHeight = (parentY-(2*topBarHeight))/2;
  mainWidth = parentX - (2*sideWidth);
  mainHeight = parentY - 2*topBarHeight;

  top2Width = mainWidth*6/10;

  wresize(Top_1_Panel(*gs), topBarHeight, sideWidth);
  wresize(Top_2_Panel(*gs), topBarHeight, top2Width);
  wresize(Top_3_Panel(*gs), topBarHeight, mainWidth-top2Width);
  wresize(Top_4_Panel(*gs), topBarHeight, sideWidth);

  wresize(Waiting_Panel(*gs), sideHeight, sideWidth);
  wresize(Food_Panel(*gs), sideHeight, sideWidth);
  wresize(Order_Panel(*gs), sideHeight, sideWidth);
  wresize(Hand_Panel(*gs), sideHeight, sideWidth);

  wresize(Command_Panel(*gs), topBarHeight, parentX);

  wresize(Main_Panel(*gs), 2*sideHeight, mainWidth);

  mvwin(Top_2_Panel(*gs), 0, sideWidth);
  mvwin(Top_3_Panel(*gs), 0, sideWidth+top2Width);
  mvwin(Top_4_Panel(*gs), 0, sideWidth+mainWidth);

  mvwin(Waiting_Panel(*gs), topBarHeight, 0);
  mvwin(Food_Panel(*gs), topBarHeight,sideWidth+mainWidth);
  mvwin(Order_Panel(*gs),topBarHeight+sideHeight,0);
  mvwin(Hand_Panel(*gs),topBarHeight+sideHeight,sideWidth+mainWidth);

  mvwin(Command_Panel(*gs),topBarHeight+mainHeight,0);

  mvwin(Main_Panel(*gs),topBarHeight,sideWidth);

  Main_Panel_Width(*gs) = mainWidth;
  Main_Panel_Height(*gs) = mainHeight;
  Side_Panel_Width(*gs) = sideWidth;
  Side_Panel_Height(*gs) = sideHeight;

  RefreshBorder(gs);

}

void WriteText(GameScreen *gs,ArrKata ak)
/* I.S. : Bebas sudah initScreen, aka terdefinisi */
/* F.S. : Ditulis ke layar main isi ak*/
{
  /* KAMUS LOKAL */
  AK_IdxType i;
  int j;
  char* output;

  /* ALGORITMA */
  wclear(Main_Panel(*gs));

  for(i = AK_GetFirstIdx(ak);i<=AK_GetLastIdx(ak);i++){
    output = K_KataToChar(AK_Elmt(ak, i));
    mvwprintw(Main_Panel(*gs), 2+i, 2, output);
    free(output);
  }

  wrefresh(Main_Panel(*gs));
}

void RefreshWaiter(GameScreen *gs, Point waiter)
/* I.S. : Bebas sudah initScreen */
/* F.S. : Digambar sebuah pelayan pada posisi tertentu di layar */
{
  /* KAMUS LOKAL */

  /* ALGORITMA */
  mvwaddch(Main_Panel(*gs),P_Kolom(waiter),P_Baris(waiter),'P');
  wrefresh(Main_Panel(*gs));

}

void RefreshMap(GameScreen *gs, MatTile peta, Point waiter)
/* I.S. : Bebas sudah initScreen */
/* F.S. : Digambar peta sesuai MatTile peta */
{
  /* KAMUS LOKAL */
  int i;
  int j;
  int maxX;
  int maxY;
  int marginX;
  int marginY;
  int posy;
  int posx;

  /* ALGORITMA */
  wclear(Main_Panel(*gs));
  DrawBorders(Main_Panel(*gs));

  /* Draw tile */
  maxX = MT_NKolEff(peta);
  maxY = MT_NBrsEff(peta);

  /* Margin calculation */
  /* (total width - (table width-1(for border compensation)))/2 */
  marginX = ((Main_Panel_Width(*gs)-((maxX-1)*5))/2);
  /* (total height - (table height-1(border compensation)))/2 */
  marginY = ((Main_Panel_Height(*gs) - (maxY-1)*3)/2);


  for(i=1;i<=maxY;i++){
    for(j=1;j<=maxX;j++){
      posy=i*2 + marginY;
      posx=j*4 + marginX;

      /* Membentuk
          | P |
          +---+
      */
      /* Lokasi posx dan posy berada di kanan P (kotak isi cell paling kanan) */

      /* Isi */
      mvwaddch(Main_Panel(*gs),posy,posx-1,Karakter(MT_Elmt(peta, i, j)));

      /* Garis bawah */
      if(i!=maxY){
        mvwaddch(Main_Panel(*gs), posy+1, posx, ACS_HLINE);
        mvwaddch(Main_Panel(*gs), posy+1, posx-1, ACS_HLINE);
        mvwaddch(Main_Panel(*gs), posy+1, posx-2, ACS_HLINE);
      }

      /* Garis samping */
      if(j!=maxX){
        mvwaddch(Main_Panel(*gs),posy,posx+1,ACS_VLINE);
      }

      /* Pojok (simbol +) */
      if(i!=maxY && j!=maxX){
        mvwaddch(Main_Panel(*gs),posy+1,posx+1,ACS_PLUS);
      }



    }
  }

  /* Waiter */
  posx=P_Kolom(waiter)*4 + marginX;
  posy=P_Baris(waiter)*2 + marginY;
  // mvwprintw(Main_Panel(*gs), posy, posx-1, "\xF0\x9F\x92\x81");
  mvwaddch(Main_Panel(*gs), posy, posx-1, 'P');

  wrefresh(Main_Panel(*gs));

}


void RefreshWaitingPanel(GameScreen *gs, PrioQueueCustomer waitQueue)
/* I.S. : Bebas sudah initScreen */
/* F.S. : Digambar panel daftar pelanggan menunggu */
{
  /* KAMUS LOKAL */
  char ch;
  int idx;

  /* ALGORITMA */
  wclear(Waiting_Panel(*gs));
  DrawBorders(Waiting_Panel(*gs));

  mvwprintw(Waiting_Panel(*gs),1, 2, "Waiting Cust");
  idx = 1;

  if(!PQC_IsEmpty(waitQueue)){
    while(idx<=PQC_NBElmt(waitQueue)){
      ch = PQC_Jumlah(PQC_Elmt(waitQueue, idx))+'0';
      mvwaddch(Waiting_Panel(*gs), idx+1, 2, ch);
      idx++;
    }
  }

  wrefresh(Waiting_Panel(*gs));
}

void RefreshFoodPanel(GameScreen *gs, StackFood foodStack)
/* I.S. : Bebas sudah initScreen */
/* F.S. : Digambar panel stack makanan */
{
  /* KAMUS LOKAL */
  SF_infotype temp;
  int y;

  /* ALGORITMA */
  wclear(Food_Panel(*gs));
  DrawBorders(Food_Panel(*gs));

  mvwprintw(Food_Panel(*gs),1, 2, "Food Stack");

  y = 2;
  while(!SF_IsEmpty(foodStack)){
    SF_Pop(&foodStack, &temp);
    mvwprintw(Food_Panel(*gs), y, 2, SF_NamaMakanan(temp));
    y++;
  }

  wrefresh(Food_Panel(*gs));
}

void RefreshHandPanel(GameScreen *gs, StackFood handStack)
/* I.S. : Bebas sudah initScreen */
/* F.S. : Digambar panel stack di tangan */
{
  /* KAMUS LOKAL */
  SF_infotype temp;
  int y;

  /* ALGORITMA */
  wclear(Hand_Panel(*gs));
  DrawBorders(Hand_Panel(*gs));

  mvwprintw(Hand_Panel(*gs),1, 2, "Food Stack");

  y = 2;
  while(!SF_IsEmpty(handStack)){
    SF_Pop(&handStack, &temp);
    mvwprintw(Hand_Panel(*gs), y, 2, SF_NamaMakanan(temp));
    y++;
  }

  wrefresh(Hand_Panel(*gs));
}

void RefreshTopPanel(GameScreen *gs, char* name, int money, int life, int time)
/* I.S. : Bebas sudah initScreen */
/* F.S. : Digambar panel atas */
{
  /* KAMUS LOKAL */

  /* ALGORITMA */
  wclear(Top_1_Panel(*gs));
  wclear(Top_2_Panel(*gs));
  wclear(Top_3_Panel(*gs));
  wclear(Top_4_Panel(*gs));

  DrawBorders(Top_1_Panel(*gs));
  DrawBorders(Top_2_Panel(*gs));
  DrawBorders(Top_3_Panel(*gs));
  DrawBorders(Top_4_Panel(*gs));

  mvwprintw(Top_1_Panel(*gs),1,2,"%s",name);
  mvwprintw(Top_2_Panel(*gs),1,2,"Money: %d",money);
  mvwprintw(Top_3_Panel(*gs),1,2,"Life: %d",life);
  mvwprintw(Top_4_Panel(*gs),1,2,"Time: %d",time);

  wrefresh(Top_1_Panel(*gs));
  wrefresh(Top_2_Panel(*gs));
  wrefresh(Top_3_Panel(*gs));
  wrefresh(Top_4_Panel(*gs));

}

void RefreshCommandPanel(GameScreen *gs,Kata prompt)
/* I.S. : Bebas sudah initScreen */
/* F.S. : DigambarKata panel command */
{
  /* KAMUS LOKAL */
  int i;

  /* ALGORITMA */
  wclear(Command_Panel(*gs));
  DrawBorders(Command_Panel(*gs));
  for(i = 1;i<=prompt.Length;i++){
    mvwaddch(Command_Panel(*gs),1,2+i,prompt.TabKata[i]);
  }
  wrefresh(Command_Panel(*gs));
}

Kata GetInput(GameScreen *gs,Kata prompt)
/* I.S. : Bebas sudah initScreen */
/* F.S. : Mengembalikan input user (command) dalam huruf besar, jika window diresize, kembalikan RESIZE*/
{
  /* KAMUS LOKAL */
  Kata output;
  int tempInput;
  boolean finishReading;
  int kataLength;
  int i;

  /* ALGORITMA */
  finishReading = false;

  keypad(Command_Panel(*gs),true);

  output.Length = 0;

  while(!finishReading){
    RefreshCommandPanel(gs,prompt);
    i = 1;
    while(i<=output.Length){
      mvwaddch(Command_Panel(*gs),1,prompt.Length+2+i,output.TabKata[i]);
      i++;
    }

    tempInput = wgetch(Command_Panel(*gs));

    if((tempInput==13)||(tempInput==10)||(tempInput==KEY_ENTER)){
      finishReading =true;
    }else if(tempInput == KEY_BACKSPACE){
      if(output.Length>0){
        output.Length--;
      }
    }else if(tempInput == KEY_RESIZE){
      output = K_MakeKata("RESIZE");
      finishReading = true;
    }else if(tempInput == KEY_UP){
      output = K_MakeKata("GU");
      finishReading = true;
    }else if(tempInput == KEY_DOWN){
      output = K_MakeKata("GD");
      finishReading = true;
    }else if(tempInput == KEY_LEFT){
      output = K_MakeKata("GL");
      finishReading = true;
    }else if(tempInput == KEY_RIGHT){
      output = K_MakeKata("GR");
      finishReading = true;
    }else if(((tempInput>='A')&&(tempInput<='Z'))||((tempInput>='0')&&(tempInput<='9'))){
      output.TabKata[output.Length+1] = tempInput;
      output.Length++;
    }else if((tempInput>='a')&&(tempInput<='z')){
      output.TabKata[output.Length+1] = tempInput-32;
      output.Length++;
    }

  }

  RefreshCommandPanel(gs,prompt);

  return output;
}
