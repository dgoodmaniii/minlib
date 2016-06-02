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
#include<unistd.h>
#include"errcodes.h"
#include"options.h"

extern struct options globopts[];

int load_gui(char **ptr, char **formlist, int *recnums, int numrecs,
struct options *globopts)
{
	int i, j; int c; int d;
	ITEM **lib_list;
	ITEM *cur_item;
	MENU *lib_menu;
	WINDOW *lib_menu_win;
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
	initialize_colors(globopts);
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
	set_menu_fore(lib_menu,COLOR_PAIR(3) | A_REVERSE);
	set_menu_back(lib_menu,COLOR_PAIR(3));
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
		case 'o':
			open_app(ptr,item_index(current_item(lib_menu)),globopts,recnums);
			frame_main_screen(numrecs, row, col);
			refresh();
			break;
		case ':':
			shell_out();
			break;
		case 10: /* enter */
			sel_rec = item_index(current_item(lib_menu));
			unpost_menu(lib_menu);
			display_details(ptr,recnums,sel_rec,row,col,globopts);
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

int menu_help_scr()
{
	WINDOW *menu_help_win;
	int row, col;

	getmaxyx(stdscr,row,col);
	werase(stdscr); refresh();
	frame_menu_help(row, col); refresh();
	menu_help_win = newwin(row-3,col,1,0);
	wrefresh(menu_help_win); refresh();
	keypad(menu_help_win,TRUE);
	wbkgd(menu_help_win,COLOR_PAIR(6));
	wattron(menu_help_win,COLOR_PAIR(4));
	mvwprintw(menu_help_win,4,2,"m\t");
	wattroff(menu_help_win,COLOR_PAIR(4));
	wattron(menu_help_win,COLOR_PAIR(5));
	mvwprintw(menu_help_win,4,2+8,"match");
	wattroff(menu_help_win,COLOR_PAIR(5));
	wattron(menu_help_win,COLOR_PAIR(4));
	mvwprintw(menu_help_win,6,2,"/\t");
	wattroff(menu_help_win,COLOR_PAIR(4));
	wattron(menu_help_win,COLOR_PAIR(5));
	mvwprintw(menu_help_win,6,2+8,"full search (POSIX regexps)");
	wattroff(menu_help_win,COLOR_PAIR(5));
	wattron(menu_help_win,COLOR_PAIR(4));
	mvwprintw(menu_help_win,8,2,":\t");
	wattroff(menu_help_win,COLOR_PAIR(4));
	wattron(menu_help_win,COLOR_PAIR(5));
	mvwprintw(menu_help_win,8,2+8,"access to system shell");
	wattroff(menu_help_win,COLOR_PAIR(5));
	wattron(menu_help_win,COLOR_PAIR(4));
	mvwprintw(menu_help_win,10,2,"o\t");
	wattroff(menu_help_win,COLOR_PAIR(4));
	wattron(menu_help_win,COLOR_PAIR(5));
	mvwprintw(menu_help_win,10,2+8,"open file");
	wattroff(menu_help_win,COLOR_PAIR(5));
	wrefresh(menu_help_win); refresh();
	wgetch(menu_help_win);
	werase(menu_help_win);
	wrefresh(menu_help_win);
	delwin(menu_help_win);
	return 0;
}

int shell_out()
{
	const char *name = "SHELL";
	char *shname;
	char *defshell = "/bin/sh";

	def_prog_mode();
	endwin();
	shname = getenv(name);
	if (shname == NULL) {
		if ((shname = malloc((strlen(defshell)+1) * sizeof(char))) == NULL) {
			fprintf(stderr,"minlib:  insufficient memory to "
			"store the name of the default shell, \"%s\"\n",defshell);
			exit(INSUFF_INTERNAL_MEMORY);
		}
		strcat(shname,defshell);
	}
	system(shname);
	reset_prog_mode();
	refresh();
	return 0;
}

int execute(int c, char **ptr, int sel_rec, struct options *globopts,
		int *recnums)
{
	char *t;
	int num;
	int i, j;
	char *path = NULL;
	char filetype[120] = "";

	if (c == 'p') {
		num = PDF_VIEWER;
		strcpy(filetype,".pdf");
	} else if (c == 'h') {
		num = HTML_VIEWER;
		strcpy(filetype,".html");
	} else if (c == 'e') {
		num = EPUB_VIEWER;
		strcpy(filetype,".epub");
	} else if (c == 't') {
		num = OGV_VIEWER;
		strcpy(filetype,".ogv");
	} else if (c == 'v') {
		num = OGG_VIEWER;
		strcpy(filetype,".ogg,.spx");
	} else if (c == 'd') {
		num = DVI_VIEWER;
		strcpy(filetype,".dvi");
	} else if (c == 'P') {
		num = PS_VIEWER;
		strcpy(filetype,".ps");
	} else if (c == 'o') {
		num = OFFICE_VIEWER;
		strcpy(filetype,".doc,.ppt,.xls,.docx,.pptx,.xlsx,.odt,.odf,.ods,.odp");
	} else {
		return 1;
	}
	for (i = 0; *(ptr+i) != NULL; ++i) {
		if ((atoi(*(ptr+i)) == (*(recnums+sel_rec)+1)) &&
			strstr(*(ptr+i),"%%")) {
			break;
		}
	}
	for (i=i+1; *(ptr+i) != NULL && !strstr(*(ptr+i),"%%"); ++i) {
		if (!strcmp(*(ptr+i),"PATH")) {
			if (include_substr(*(ptr+(i+1)),filetype) == 0) {
				path = *(ptr+(i+1));
			}
		}
	}
	if (path == NULL) {
		print_bottom_message(filetype);
		return 1;
	}
	if ((t = malloc((strlen((globopts+num)->optval)
	+ strlen(path) + 28) * sizeof(char))) == NULL) {
		fprintf(stderr,"minlib:  insufficient memory to "
		"store the command line for opening this file\n");
		exit(INSUFF_INTERNAL_MEMORY);
	}
	t[0] = '\0';
	sprintf(t,(globopts+num)->optval,path);
	strcat(t," > /dev/null 2>&1 &");
	system(t);
	free(t);
	print_bottom_open(path);
	return 0;
}

int open_app(char **ptr,int sel_rec,struct options *globopts,int *recnums)
{
	int c;
	int row, col;

	getmaxyx(stdscr,row,col);
	highlight_line(stdscr,row-2,col);
	set_comm_open(row, col);
	c = getch();
	if (c == 'q')
		return 0;
	else
		execute(c,ptr,sel_rec,globopts,recnums);
	return 0;
}

int display_details(char **ptr,int *recnums,int sel_rec,int row,
int col,struct options *globopts)
{
	WINDOW *sel_item_win;
	int i; int j;
	int k = 0; /* track whether it's a field title or not */
	int d;
	int wrapped = 0;
	int lines_rec;

	werase(stdscr);
	frame_detail_screen(row, col, *(recnums+sel_rec)+1); refresh();
	for (i = 0; *(ptr+i) != NULL; ++i) {
		if ((atoi(*(ptr+i)) == (*(recnums+sel_rec)+1)) &&
			strstr(*(ptr+i),"%%")) {
			break;
		}
	}
	sel_item_win = newpad((3*row),col);
	keypad(sel_item_win,TRUE);
	wbkgd(sel_item_win,COLOR_PAIR(6));
	for (i=i+1,j=2; *(ptr+i) != NULL && !strstr(*(ptr+i),"%%"); ++i,j) {
		if (k == 0) {
			++k;
			wattron(sel_item_win,COLOR_PAIR(4));
			mvwprintw(sel_item_win,j,2,"%s:",*(ptr+i));
			wattroff(sel_item_win,COLOR_PAIR(4));
		} else {
			wattron(sel_item_win,COLOR_PAIR(5));
			wrapped = wrap_print(sel_item_win,*(ptr+i),col,j);
			wattroff(sel_item_win,COLOR_PAIR(5));
			j += (wrapped + 1);
			k = 0;
		}
		lines_rec = j - row + 4;
		prefresh(sel_item_win,0,0,1,0,row-3,col-1);
	}
	i = 0;
	while ((d = wgetch(sel_item_win)) != 'q') {
		switch(d) {
		case KEY_DOWN: case 'j':
			if (i < lines_rec)
				++i;
			frame_detail_screen(row, col, *(recnums+sel_rec)+1); refresh();
			prefresh(sel_item_win,i,0,1,0,row-3,col-1);
			wrefresh(sel_item_win);
			refresh();
			break;
		case KEY_UP: case 'k':
			if (i > 1)
				--i;
			frame_detail_screen(row, col, *(recnums+sel_rec)+1); refresh();
			prefresh(sel_item_win,i,0,1,0,row-3,col-1);
			wrefresh(sel_item_win);
			refresh();
			break;
		case 'o':
			open_app(ptr,sel_rec,globopts,recnums);
			frame_detail_screen(row, col, *(recnums+sel_rec)+1); refresh();
			break;
		case ':':
			shell_out();
			break;
		}
	}
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

int set_comm_open(int row, int col)
{
	attron(A_REVERSE | A_BOLD);
	attron(COLOR_PAIR(2));
	clean_bottom_line(row-1,col); refresh();
	mvwprintw(stdscr,row-2,0,"OPEN:  ");
	mvwprintw(stdscr,row-2,6,"p:pdf  e:epub  h:html  t:theora  v:vorbis"
	"  P:ps  d:dvi o:office");
	attroff(COLOR_PAIR(2));
	attroff(A_REVERSE | A_BOLD);
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

int frame_menu_help(int row, int col)
{
	print_top_menu_help(stdscr,row,col);
	print_bot_menu_help(stdscr,row,col);
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
	refresh();
	print_bot_line(stdscr, row, col);
	refresh();
	return 0;
}

int print_top_menu_help(WINDOW *win, int row, int col)
{
	attron(A_BOLD);
	attron(COLOR_PAIR(1));
	highlight_line(win,0,col);
	attron(A_REVERSE);
	mvwprintw(win,0,0,"--minlib, v0.9");
	print_center(win,0,"Menu View Help");
	print_right(win,0,"Menu View--");
	wmove(win,row,col-1);
	attroff(COLOR_PAIR(1));
	attroff(A_REVERSE | A_BOLD);
	return 0;
}

int print_bot_menu_help(WINDOW *win, int row, int col)
{
	attron(COLOR_PAIR(2));
	highlight_line(win,row-2,col);
	attron(A_REVERSE | A_BOLD);
	mvwprintw(win,row-2,0,"q:back");
	attroff(A_REVERSE | A_BOLD);
	attroff(COLOR_PAIR(2));
	return 0;
}

int print_top_details(WINDOW *win, int row, int col, int recordnum)
{
	char *heading; char *buffer;
	int numdigs, i;

	attron(A_REVERSE | A_BOLD);
	attron(COLOR_PAIR(1));
	highlight_line(win,0,col);
	attroff(A_REVERSE | A_BOLD);
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
	attroff(COLOR_PAIR(1));
	return 0;
}

int print_bot_details(WINDOW *win, int row, int col)
{
	attron(COLOR_PAIR(2));
	attron(A_BOLD);
	highlight_line(win,row-2,col);
	attron(A_REVERSE);
	mvwprintw(win,row-2,0,"q:back  o:open  :=shell");
	attroff(A_REVERSE | A_BOLD);
	attroff(COLOR_PAIR(2));
	return 0;
}

int print_top_line(WINDOW *win, int row, int col, int numrecs)
{
	char *heading; char *buffer;
	int numdigs, i;

	attron(COLOR_PAIR(1));
	attron(A_REVERSE | A_BOLD);
	highlight_line(win,0,col);
	attroff(A_REVERSE | A_BOLD);
	numdigs = num_digs(numrecs);
	heading = malloc((numdigs + 2 + 9) * sizeof(char));
	sprintf(heading,"(%d records)",numrecs);
	attron(A_REVERSE | A_BOLD);
	mvwprintw(win,0,0,"--minlib, v0.9");
	print_center(win,0,heading);
	print_right(win,0,"Menu View--");
	wmove(win,row,col-1);
	attroff(A_REVERSE | A_BOLD);
	attroff(COLOR_PAIR(1));
	free(heading);
	return 0;
}

int print_bottom_open(char *s)
{
	int row, col;

	getmaxyx(stdscr,row,col);
	clean_bottom_line(row, col);
	mvprintw(row-1,0,"Opening %s...",s);
	return 0;
}

int print_bottom_message(char *s)
{
	int row, col;

	getmaxyx(stdscr,row,col);
	clean_bottom_line(row, col);
	mvprintw(row-1,0,"Error:  no %s-type file found",s);
	return 0;
}

int print_bot_line(WINDOW *win, int row, int col)
{
	attron(COLOR_PAIR(2));
	attron(A_BOLD);
	highlight_line(win,row-2,col);
	attron(A_REVERSE | A_BOLD);
	mvwprintw(win,row-2,0,"q:quit  m:match  /:search  o:open "
	":=shell");
	attroff(A_REVERSE | A_BOLD);
	attroff(COLOR_PAIR(2));
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

/* return number of wrapped lines, 0 if all on one */
int wrap_print(WINDOW *win,char *s, int cols, int row)
{
	int len; int left; int numchars;
	char t[cols];
	char *ptr;
	int numline = 0;

	len = strlen(s);
	left = cols - 24 - 2;
	if (len < left) {
		mvwprintw(win,row,24,"%s",s);
		return 0;
	}
	if (len >= left) {
		numchars = len;
		ptr = s;
		while (numchars > 0) {
			strncpy(t,ptr,left);
			t[left] = '\0';
			ptr += left;
			numchars -= left;
			mvwprintw(win,row++,24,"%s",t);
			numline++;
		}
		return (int) len / left;
	}
}

int initialize_colors(struct options *globopts)
{
	start_color();
	init_pair(1,get_col_int((globopts+TOP_BACK_COLOR)->optval),
		get_col_int((globopts+TOP_FORE_COLOR)->optval));
	init_pair(2,get_col_int((globopts+BOT_BACK_COLOR)->optval),
		get_col_int((globopts+BOT_FORE_COLOR)->optval));
	init_pair(3,get_col_int((globopts+MEN_FORE_COLOR)->optval),
		get_col_int((globopts+MEN_BACK_COLOR)->optval));
	init_pair(4,get_col_int((globopts+DET_FIELD_FORE_COLOR)->optval),
		get_col_int((globopts+DET_FIELD_BACK_COLOR)->optval));
	init_pair(5,get_col_int((globopts+DET_TXT_FORE_COLOR)->optval),
		get_col_int((globopts+DET_TXT_BACK_COLOR)->optval));
	init_pair(6,get_col_int((globopts+DET_BACK_COLOR)->optval),
		get_col_int((globopts+DET_BACK_COLOR)->optval));
	return 0;
}

int get_col_int(char *s)
{
	int colint;

	if (!strcmp(s,"COLOR_BLACK"))
		colint = 0;
	if (!strcmp(s,"COLOR_RED"))
		colint = 1;
	if (!strcmp(s,"COLOR_GREEN"))
		colint = 2;
	if (!strcmp(s,"COLOR_YELLOW"))
		colint = 3;
	if (!strcmp(s,"COLOR_BLUE"))
		colint = 4;
	if (!strcmp(s,"COLOR_MAGENTA"))
		colint = 5;
	if (!strcmp(s,"COLOR_CYAN"))
		colint = 6;
	if (!strcmp(s,"COLOR_WHITE"))
		colint = 7;
	return colint;
}
