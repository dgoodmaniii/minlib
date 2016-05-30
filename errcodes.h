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

/* defines constants for the error codes */

#define ALLGOOD 0
#define BAD_OPTION 1
#define NEED_ARGUMENT_ARG 2
/* insufficient memory errors; all exceedingly unlikely */
/* insufficient memory to load all the lines from the data file */
#define INSUFF_MEMORY_LOAD 3
/* insufficient memory to load formatted lines built from
 * information in data file */
#define INSUFF_MEMORY_FORMAT 4
/* insufficient memory to track formatted lines built from
 * information in data file once it's sorted */
#define INSUFF_MEMORY_TRACK 5
/* insufficient memory to store format string */
#define INSUFF_MEMORY_FORMSTRING 6
/* insufficient memory for various internal deals */
#define INSUFF_INTERNAL_MEMORY 7
/* error with the input file (data file) */
#define BAD_INPUT_FILE 8
/* insufficient memory for filename */
#define INSUFF_MEMORY_FILENAME 9
/* bad config file */
#define BAD_CONFIG_FILE 10
/* no data file specified */
#define NO_DATA_FILE 11
/* can't open temp file */
#define NO_TEMP_FILE 12
/* something indescribable is wrong with added file */
#define BAD_ADDED_FILE 13
/* can't open the added file */
#define ERROR_OPEN_ADDFILE 14

/* define length constants */
#define MAX_ERR_LENGTH 67
#define MAX_TITLE_LEN 1728
#define MAX_FIELD_LEN 16
#define MAX_REGEXP_LEN 432
#define NUM_COLORS 11
#define NUM_APPS 8

/* define option identifiers */
#define TOP_FORE_COLOR 0
#define TOP_BACK_COLOR 1
#define BOT_FORE_COLOR 2
#define BOT_BACK_COLOR 3
#define MEN_FORE_COLOR 4
#define MEN_BACK_COLOR 5
#define DET_FIELD_FORE_COLOR 6
#define DET_FIELD_BACK_COLOR 7
#define DET_TXT_FORE_COLOR 8
#define DET_TXT_BACK_COLOR 9
#define DET_BACK_COLOR 10
#define PDF_VIEWER 11
#define HTML_VIEWER 12
#define EPUB_VIEWER 13
#define OGV_VIEWER 14
#define OGG_VIEWER 15
#define DVI_VIEWER 16
#define PS_VIEWER 17
#define OFFICE_VIEWER 18
