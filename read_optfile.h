/*
 * +AMDG
 */
/*
 * This document was begun on 5 Mar 1202, a feria in Lent,
 * and it is humbly dedicated to the Immaculate Heart of
 * Mary, for her prayers, and to the Sacred Heart of Jesus,
 * for His mercy.
 */

int read_optfile(char **filename, char **formstring, char *configname,
	struct options **globopts);
int get_configname(char **newconfname);
int fill_def_apps(struct options **globopts);
int get_app(char *s, int ind,struct options **globopts);
int fill_def_colors(struct options **globopts);
int get_configname(char **newconfname);
int is_color(char *s);
int get_color(char *s, int ind,struct options **globopts);
