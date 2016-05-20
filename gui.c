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
#include"errcodes.h"

int load_gui(char **ptr, char **formlist, int *recnums, int numrecs)
{
	int i, j; int c; int d;
	ITEM **lib_list;
	ITEM *cur_item;
	MENU *lib_menu;
	WINDOW *lib_menu_win;
	WINDOW *sel_item_win;
	int row, col;
	char buf[12] = "";
	char pattern[MAX_REGEXP_LEN+1];
	int sel_rec;
	char regexperror[MAX_ERR_LENGTH+1];
	int matchnum;
	int currmatch = 0;
	int *matched;

	if ((matched = malloc(1 * sizeof(int))) == NULL) {
		fprintf(stderr,"minlib:  insufficient memory to store "
		"the array of matched strings in a full search");
		exit(INSUFF_INTERNAL_MEMORY);
	}
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
		case KEY_DOWN: case 'j':
			menu_driver(lib_menu, REQ_DOWN_ITEM);
			break;
		case KEY_UP: case 'k':
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
		case 'p':
			if (strcmp(buf,"")) {
				set_bot_line_match(lib_menu,row,col,buf);
				menu_driver(lib_menu, REQ_NEXT_MATCH);
			}
			break;
		case 'P':
			if (strcmp(buf,"")) {
				set_bot_line_match(lib_menu,row,col,buf);
				menu_driver(lib_menu, REQ_PREV_MATCH);
			}
			break;
		case 'm':
			set_pattern_buffer(lib_menu,buf,row,col);
			set_menu_pattern(lib_menu,buf);
			break;
		case 'n':
			if (matchnum > 0) {
				proc_fsearch(lib_menu,matchnum,lib_list,matched,recnums,
					numrecs,'n');
			}
			break;
		case 'N':
			if (matchnum > 0) {
				proc_fsearch(lib_menu,matchnum,lib_list,matched,recnums,
					numrecs,'N');
			}
			break;
		case '/':
			if ((matched = realloc(matched,(1 * sizeof(int)))) == NULL) {
				fprintf(stderr,"minlib:  insufficient memory to store "
				"the array of matched strings in a full search");
				exit(INSUFF_INTERNAL_MEMORY);
			}
			set_fullsearch_buffer(lib_menu,pattern,row,col);
			matchnum = full_search(ptr,&matched,pattern,regexperror);
			if (matchnum > 0) {
				proc_fsearch(lib_menu,matchnum,lib_list,matched,recnums,
					numrecs,'f');
				set_search_line(row, col, pattern, matchnum);
			} else {
				set_search_line(row, col, regexperror, matchnum);
			}
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
	free(matched);
	endwin();
	return 0;
}

int display_details(char **ptr,int *recnums,int sel_rec,int row,int col)
{
	WINDOW *sel_item_win;
	int i; int j; int k = 0;
	int d;

	frame_detail_screen(row, col, *(recnums+sel_rec)+1); refresh();
	for (i = 0; atoi(*(ptr+i)) != *(recnums+sel_rec) + 1; ++i);
	sel_item_win = newwin(row-3,col,1,0);
	keypad(sel_item_win,TRUE);
	box(sel_item_win,0,0);
	for (i=i+1,j=2; *(ptr+i) != NULL && !strstr(*(ptr+i),"%%"); ++i,j) {
		if (k == 0) {
			++k;
			mvwprintw(sel_item_win,j,2,"%s:",*(ptr+i));
		} else {
			mvwprintw(sel_item_win,j++,12,"%s",*(ptr+i));
			k = 0;
		}
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
	for (i = 0; i < col; ++i) {
		mvprintw(row-1,i," ");
		refresh();
	}
	return 0;
}

int set_search_line(int row, int col, char *s, int matchnum)
{
	clean_bottom_line(row,col); refresh();
	if (matchnum == 0) {
		mvwprintw(stdscr,row-1,0,"%s",s);
		attron(A_BOLD);
		mvwprintw(stdscr,row-1,strlen(s)+4,"(not found)");
		attroff(A_BOLD);
	} else if (matchnum > 0) {
		mvwprintw(stdscr,row-1,0,"/%s",s);
		attron(A_BOLD);
		mvwprintw(stdscr,row-1,strlen(s)+4,"(%d) matches",matchnum);
		attroff(A_BOLD);
	} else {
		attron(A_BOLD);
		mvwprintw(stdscr,row-1,0,"error:  %s",s);
		attroff(A_BOLD);
	}
	refresh();
	return 0;
}

int set_bot_line_match(MENU *lib_menu, int row, int col, char *s)
{
	clean_bottom_line(row,col); refresh();
	mvwprintw(stdscr,row-1,0,"m/%s",s);
	if (set_menu_pattern(lib_menu,s) == E_NO_MATCH) {
		attron(A_BOLD);
		mvwprintw(stdscr,row-1,strlen(s)+4,"(not found)");
		attroff(A_BOLD);
	}
	refresh();
	return 0;
}

int set_fullsearch_buffer(MENU *lib_menu,char*s,int row, int col)
{
	s[0] = '\0';
	echo();
	clean_bottom_line(row,col); refresh();
	char searchstr[] = "Search for:  ";
	mvprintw(row-1,0,"%s",searchstr);
	mvscanw(row-1,strlen(searchstr),"%s",s);
	clean_bottom_line(row,col); refresh();
	noecho();
	refresh();
	return 0;
}

int set_pattern_buffer(MENU *lib_menu,char *s, int row, int col)
{
	s[0] = '\0';
	echo();
	clean_bottom_line(row,col); refresh();
	char searchstr[] = "Search for:  ";
	mvprintw(row-1,0,"%s",searchstr);
	mvscanw(row-1,strlen(searchstr),"%11s",s);
	clean_bottom_line(row,col); refresh();
	noecho();
	set_bot_line_match(lib_menu,row,col,s);
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
	numdigs = num_digs(recordnum);
	heading = malloc((numdigs + 2 + 10) * sizeof(char));
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
	numdigs = num_digs(numrecs);
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
	mvwprintw(win,row-2,0,"q:quit  m:match  /:search  o:open");
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
