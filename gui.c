/*
 * +AMDG
 */
/*
 * This document was begun on 10 May 1200, the feast of Ss.
 * Nereus, Achilleus, Domitilla V, and Pancras, MM, and it
 * is humbly dedicated to them and to the Immaculate Heart
 * of Mary for their prayers and to the Sacred Heart of
 * Jesus for His mercy.
 */

/* Loads the ncurses GUI interface for minlib. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ncurses.h>
#include<menu.h>

int load_gui(char **ptr, char **formlist, int *recnums, int numrecs)
{
	int i, j; int c; int d;
	ITEM **lib_list;
	ITEM *cur_item;
	MENU *lib_menu;
	WINDOW *lib_menu_win;
	WINDOW *sel_item_win;
	int row, col;
	char buf[12];
	int sel_rec;

	initscr(); cbreak(); noecho(); keypad(stdscr,TRUE); curs_set(0);
	getmaxyx(stdscr,row,col);
	frame_main_screen(numrecs, row, col);
	lib_list = (ITEM **)calloc(numrecs+1, sizeof(ITEM *));
	for (i = 0; i < numrecs; ++i)
		lib_list[i] = new_item(*(formlist+i),"");
	lib_list[numrecs] = (ITEM *)NULL;
	lib_menu = new_menu((ITEM **)lib_list);
	lib_menu_win = newwin(row-3,col,1,0);
	keypad(lib_menu_win,TRUE);
	set_menu_win(lib_menu,lib_menu_win);
	set_menu_sub(lib_menu,derwin(lib_menu_win,0,0,0,0));
	set_menu_format(lib_menu,row-3,1);
	set_menu_mark(lib_menu,"");
	refresh();
	post_menu(lib_menu);
	wrefresh(lib_menu_win);
	while ((c = wgetch(lib_menu_win)) != 'q') {
		switch (c) {
		case KEY_DOWN:
			menu_driver(lib_menu, REQ_DOWN_ITEM);
			break;
		case KEY_UP:
			menu_driver(lib_menu, REQ_UP_ITEM);
			break;
		case KEY_PPAGE:
			menu_driver(lib_menu, REQ_SCR_UPAGE);
			break;
		case KEY_NPAGE:
			menu_driver(lib_menu, REQ_SCR_DPAGE);
			break;
		case KEY_HOME:
			menu_driver(lib_menu, REQ_FIRST_ITEM);
			break;
		case KEY_END:
			menu_driver(lib_menu, REQ_LAST_ITEM);
			break;
		case 'n':
			set_bot_line_search(row,col,buf);
			menu_driver(lib_menu, REQ_NEXT_MATCH);
			break;
		case 'N':
			set_bot_line_search(row,col,buf);
			menu_driver(lib_menu, REQ_PREV_MATCH);
			break;
		case '/':
			set_pattern_buffer(buf,row,col);
			set_menu_pattern(lib_menu,buf);
			break;
		case 10: /* enter */
			sel_rec = item_index(current_item(lib_menu));
			unpost_menu(lib_menu);
			display_details(ptr,recnums,sel_rec,row,col);
			frame_main_screen(numrecs, row, col);
			post_menu(lib_menu);
			wrefresh(lib_menu_win);
			refresh();
			break;
		}
		wrefresh(lib_menu_win);
	}
	unpost_menu(lib_menu);
	free_menu(lib_menu);
	for (i = 0; i <= numrecs; ++i)
		free_item(lib_list[i]);
	free(lib_list);
	endwin();
	return 0;
}

int display_details(char **ptr,int *recnums,int sel_rec,int row,int col)
{
	WINDOW *sel_item_win;
	int i; int j;
	int d;

	frame_detail_screen(row, col, *(recnums+sel_rec)+1); refresh();
	for (i = 0; atoi(*(ptr+i)) != *(recnums+sel_rec) + 1; ++i);
	sel_item_win = newwin(row-3,col,1,0);
	keypad(sel_item_win,TRUE);
	box(sel_item_win,0,0);
	for (i = i+1, j=2; *(ptr+i) != NULL && atoi(*(ptr+i)) == 0; ++i, ++j) {
		mvwprintw(sel_item_win,j,2,"%s",*(ptr+i));
		wrefresh(sel_item_win);
	}
	while ((d = wgetch(sel_item_win)) != 'q');
	werase(sel_item_win);
	wrefresh(sel_item_win);
	delwin(sel_item_win);
	return 0;
}

int clean_bottom_line(int row, int col)
{
	int i;
	for (i = 0; i < col; ++i)
		mvprintw(row-1,i," ");
	refresh();
	return 0;
}

int set_bot_line_search(int row, int col, char *s)
{
	clean_bottom_line(row,col);
	mvwprintw(stdscr,row-1,0,"/%s",s);
	refresh();
	return 0;
}

int set_pattern_buffer(char *s, int row, int col)
{
	s[0] = '\0';
	echo();
	char searchstr[] = "Search for:  ";
	mvprintw(row-1,0,"%s",searchstr);
	mvscanw(row-1,strlen(searchstr),"%11s",s);
	clean_bottom_line(row,col);
	noecho();
	set_bot_line_search(row,col,s);
	refresh();
	return 0;
}

int frame_detail_screen(int row, int col, int recordnum)
{
	print_top_details(stdscr,row,col,recordnum);
	print_bot_details(stdscr,row,col);
	return 0;
}

int frame_main_screen(int numrecs, int row, int col)
{
	print_top_line(stdscr, row, col, numrecs);
	print_bot_line(stdscr, row, col);
	return 0;
}

int print_top_details(WINDOW *win, int row, int col, int recordnum)
{
	char *heading; char *buffer;
	int numdigs, i;

	highlight_line(win,0,col);
	numdigs = recordnum % 10;
	heading = malloc((numdigs + 2 + 9) * sizeof(char));
	sprintf(heading,"(Record %d)",recordnum);
	attron(A_REVERSE | A_BOLD);
	mvwprintw(win,0,0,"--minlib, v0.9");
	print_center(win,0,heading);
	print_right(win,0,"Detailed View--");
	wmove(win,row,col-1);
	attroff(A_REVERSE | A_BOLD);
	free(heading);
	return 0;
}

int print_bot_details(WINDOW *win, int row, int col)
{
	highlight_line(win,row-2,col);
	attron(A_REVERSE | A_BOLD);
	mvwprintw(win,row-2,0,"q:back  o:open");
	attroff(A_REVERSE | A_BOLD);
	return 0;
}

int print_top_line(WINDOW *win, int row, int col, int numrecs)
{
	char *heading; char *buffer;
	int numdigs, i;

	highlight_line(win,0,col);
	numdigs = numrecs % 10;
	heading = malloc((numdigs + 2 + 9) * sizeof(char));
	sprintf(heading,"(%d records)",numrecs);
	attron(A_REVERSE | A_BOLD);
	mvwprintw(win,0,0,"--minlib, v0.9");
	print_center(win,0,heading);
	print_right(win,0,"Menu View--");
	wmove(win,row,col-1);
	attroff(A_REVERSE | A_BOLD);
	free(heading);
	return 0;
}

int print_bot_line(WINDOW *win, int row, int col)
{
	highlight_line(win,row-2,col);
	attron(A_REVERSE | A_BOLD);
	mvwprintw(win,row-2,0,"q:quit  /:search  o:open");
	attroff(A_REVERSE | A_BOLD);
	return 0;
}

int print_center(WINDOW *win, int row, char *s)
{
	int len, y, col;

	len = strlen(s);
	getmaxyx(win,y,col);
	mvwprintw(win,row,col/2-len/2,"%s",s);
	return 0;
}

int print_right(WINDOW *win, int row, char *s)
{
	int len, y, col;

	len = strlen(s);
	getmaxyx(win,y,col);
	mvwprintw(win,row,col-len,"%s",s);
	return 0;
}

int highlight_line(WINDOW *win, int row, int col)
{
	int i;
	attron(A_REVERSE);
	for (i = 0; i < col; ++i)
		mvwprintw(win,row,i," ");
	attroff(A_REVERSE);
	return 0;
}
