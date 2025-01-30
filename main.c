// ldd: -lncurses
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

void INIT_SCREEN (void) {
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
}

void DONE_SCREEN (void) {
  endwin();
  exit(0);
}

#define PLAYERS 2
char CHIP[PLAYERS] = "XO";

#define SIZE 16

char board[SIZE][SIZE];

void init_board(void)
{
  int row, col;
  for (row = 0; row < SIZE; row++) {
    for (col = 0; col < SIZE; col++) {
      board[row][col] = ' ';
    }
  }
}

void draw_board(void)
{
  move(0, 0);
  char line[] = " A B C D E F G H I J K L M N O P Q R S T U V W X Y Z";
  
  for (int i = 0; i < SIZE; i++) {
  	for (int j = 0; j < SIZE; j++) {
  		printw("|%c",board[i][j]);
    	refresh();
  	}
    printw("|\n");
    refresh();
  }
  
  for (int i = 0; i < SIZE; i++) {
    printw("+-");
    refresh();
  }
  
  printw("+\n");
  refresh();
  
  for (int i = 0; i <= SIZE*2; i++) {
    printw("%c", line[i]);
    refresh();
  }
  printw("\n");
}

int get_col (void) {
  int key;
  int index = 0;
  
  while(1) {
  	key = getch();
    
  	if (key == KEY_BACKSPACE) {
    	return -1;
  	}
  
  	else if (key >= 'A' && key <= 'A' + SIZE-1) {
      return key - 'A'; 
  	}
  
  	else if (key >= 'a' && key <= 'a' + SIZE-1) {
      return key - 'a'; 
  	}
  }
}

int add_coin (int col, int player) {
  for (int row = SIZE; row >= 0; row--) {
    if (board[row][col] == ' ')
    {
      board[row][col] = CHIP[player];
      return (player + 1) % PLAYERS;
    }
  }
  return player;
}

int game_over(void) {
  int full = 1;
  
  for (int row = 0; row < SIZE; row++) {
    for (int col = 0; col < SIZE; col++) {
      if (board[row][col] == ' ') {
        full = 0;
        break;
      }
    }
    if (!full) {
      break;
    }
  }
  
  if (full) {
    return 1;
  }
  
  for (int row = 0; row < SIZE; row++) {
    for (int col = 0; col < SIZE; col++) {
      
      if (board[row][col] == ' ') {
        continue;
      }
      
      if (col < SIZE - 4 &&
          board[row][col] == board[row][col + 1] &&
          board[row][col] == board[row][col + 2] &&
          board[row][col] == board[row][col + 3]) {
        return board[row][col];
      }
      
      if (col < SIZE - 4 &&
          board[row][col] == board[row + 1][col] &&
          board[row][col] == board[row + 2][col] &&
          board[row][col] == board[row + 3][col]) {
        return board[row][col];
      }
      
      if (col < SIZE - 4 &&
          board[row][col] == board[row + 1][col + 1] &&
          board[row][col] == board[row + 2][col + 2] &&
          board[row][col] == board[row + 3][col + 3]) {
        return board[row][col];
      }
      if (col < SIZE - 4 &&
          board[row][col] == board[row + 1][col - 1] &&
          board[row][col] == board[row + 2][col - 2] &&
          board[row][col] == board[row + 3][col - 3])
      {
        return board[row][col];
      }
    }
  }
  return 0;
}

void play(void) {
  int current_player = 0;
  int status;
  
  while (1) {
    status = game_over();
    if (status != 0) {
      break;
    }
    clear();
    draw_board();
    printw("C'est au tour du joueur %c\n", CHIP[current_player]);
    refresh();
    int col = get_col();
    int result = add_coin(col, current_player);
    
    if (result == current_player) {
      printw("Colonne pleine, réessayez !\n");
      refresh();
    }
    current_player = result;
    clear();
  	draw_board();
  }
 
  
  if (status == 1) {
    printw("Match Nul ! La grille est pleine\n");
    refresh();
  }
  else {
    printw("Le joueur %i a gagné\n", current_player);
    refresh();
  }   
}

int main (void) {
  INIT_SCREEN();
  init_board();
  draw_board();
  get_col();
  play();
  getch();
  DONE_SCREEN();
}
