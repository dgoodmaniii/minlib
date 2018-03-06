/*
 * +AMDG
 */
/*
 * This document was begun on 5 Mar 1202, a feria in Lent,
 * and it is humbly dedicated to the Immaculate Heart of
 * Mary, for her prayers, and to the Sacred Heart of Jesus,
 * for His mercy.
 */

int num_digs(int num);
int count_lines_file(char *s);
int is_blank_line(char *s);
int chomp(char *s);
int front_chomp(char *s);
int new_strsep(char *s, char *t, char c);
int new_strcpy(char *s, char *t, int num);
int quicksort(char **formrecs, int *recnums, int len);
int get_size(char **ptr);
int get_record_num(char **ptr, int matchedrec);
int validate_line(char *s);
int include_substr(char *s, char *t);
int count_recs_file(char *s);
int swap_str_ptrs(int *int1,int *int2,char **arg1,
		char **arg2);
int new_strsep(char *s, char *t, char c);
int new_strcpy(char *s, char *t, int num);
int new_strcat(char *s, char *t, int num);
