#include <ncurses/ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define EXIT 0
#define GAMEOVER 1
#define GAME 2
#define LIMIT 50

int maxY, maxX;

typedef struct obstacle {
  char sprite;
  int y, x;
} Obstacle;

typedef struct players {
  char sprite;
  int y, x;
} Players;

typedef struct window {
  int height, width, y, x;
} Window;

void prints(const char *str, unsigned int plus){
  mvprintw((maxY/2)+plus, (maxX-strlen(str))/2, str);
}

int gameOver(WINDOW *win, Window w) {
  
  prints("game over", 0);
  prints("press enter to continue or q to quit", 1);
  int c;
  while((c=getch())!='\n'){
    if (c == 'q'){
      return EXIT;
    }
    usleep(100 * 1000);
  }
  clear();
  return GAME;
}

int gameRunning(WINDOW *win, Window w){
  Players pl;
  pl.sprite = '#';
  pl.y = w.height-2;
  pl.x = 1;

  Obstacle obstacles;
  obstacles.y = 1;
  obstacles.x = (rand() % (w.width - 2)) + 1;
  obstacles.sprite='#';

  int tick = 200;
  int c;
  while((c=getch())!='q'){
    clear();
    wclear(win);
    switch(c){
    case KEY_LEFT:
      if(pl.x>1){
      pl.x--;
      }
      break;
    case KEY_RIGHT:
      if(pl.x<w.width-2){
      pl.x++;
      }
      break;
    }
    if (obstacles.y >= w.height-1){
      if (tick > LIMIT){
	tick -= 5;
      }
      if(obstacles.x == pl.x){
	tick=200;
	return GAMEOVER;
      }
      obstacles.y = 1;
      obstacles.x = (rand() % (w.width - 2)) + 1;
    }

    box(win, 0, 0);
    printw("speed: %d", tick);
    mvwprintw(win, obstacles.y, obstacles.x, "%c", obstacles.sprite);
    mvwprintw(win, pl.y, pl.x, "%c", pl.sprite);
    wrefresh(win);
    obstacles.y++;
    usleep(tick * 1000);
    
  };
  return EXIT;
}

int main(){
  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  curs_set(0);

  getmaxyx(stdscr, maxY, maxX);

  Window w;
  w.height = 7;
  w.width = 7;
  w.y = (maxY - w.height) / 2;
  w.x = (maxX - w.width) / 2;
  refresh();
  
  WINDOW *win = newwin(w.height, w.width, w.y, w.x);
  box(win, 0, 0);
  wrefresh(win);
  
  int ret = gameRunning(win, w);
  while (true){
    switch(ret){
    case EXIT:
      delwin(win);
      endwin();
      return 0;
    case GAMEOVER:
      ret = gameOver(win, w);
      break;
    case GAME:
      ret = gameRunning(win, w);
      break;
    }
  }
}
