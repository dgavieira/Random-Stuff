/******************************************************************************\
 *                                H C R E F . C                               *
 *                               ===============                              *
 *                                                                            *
 *  This file contains the C HTML CROSS-REFERENCE PROGRAM (hcref).            *
 *  This program produces a cross-reference listing of one or more input      *
 *  files.                                                                    *
 *                                                                            *
 *  Usage:    hcref { <option> | <filename> }                                 *
 *                                                                            *
 *  options: -K       place C keywords in listing                             *
 *           -N       place numbers in listing                                *
 *           -O<hdir> use <hdir> as directory to store hcref listing files in *
 *           -V       print version and quit                                  *
 *           -Wnnn    sets output width to 'nnn' columns                      *
 *           -X<name> reads file <name> for more filenames                    *
 *           -#       place preprocessor #words in listing                    *
 *                                                                            *
 *  for further information about the options, just type "hcref"              *
 *                                                                            *
\******************************************************************************/

#include "portable.h"
#ifdef __BORLANDC__
#   include <dir.h>
#else
#   include <sys/types.h>
#   include <sys/stat.h>
#endif
#include <time.h>

#include "hcref.h"
#include "scanner.h"
#include "globals.h"
#include "list.h"
#include "error.h"
#include "utils.h"

/******
 * Local definitions and constants.
 ******/

#define DEF_OUTPUTDIR           "hcref.out"

#define DEF_INDEXFILE           "index.html"
#define DEF_MENUFILE            "menu.html"
#define DEF_VIEWFILE            "view.html"

#define DEF_NUMBERTOKENS        FALSE
#define DEF_HASHTOKENS          FALSE
#define DEF_CTOKENS             FALSE

#define DEF_PAGEWIDTH           145

#define MIN_PAGEWIDTH           90


/******
 * Function prototypes.
 ******/

static bool keyword(char *token);


/******
 * Local variables.
 ******/

static char *outputdir;
static int   pagewidth;

static bool  hashtokens;
static bool  ctokens;

static int   end_hash;

static char *indexfile;
static char *menufile;
static char *viewfile;

static char *ProgName = NULL;

static int   start_bin;
static int   end_bin;

static FILE *vfile;



/*******************************************************************************
       Name : start_view()
Description : 
 Parameters : 
    Returns :
   Comments : 
*******************************************************************************/
static void
start_view(void)
{
    char buff[FILENAME_LEN];

/******
 * Create the view.html
 ******/

    sprintf(buff, "%s/%s", outputdir, viewfile);

    vfile = fopen(buff, "w");
    if (vfile == NULL)
        error("Can't open file '%s' for writing: %s",
              buff, strerror(errno));

    fprintf(vfile,
            "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n"
            "<!-- Generated by hcref %d.%d at %s -->\n"
            "<HTML>\n"
            "  <HEAD>\n"
            "    <META http-equiv=\"Content-Type\" "
                "content=\"text/html; charset=iso-8859-1\">\n"
            "    <title>hcref %d.%d at %s</title>\n"
            "  </HEAD>\n"
            "  <BODY ONLOAD=\"window.defaultStatus='';\" "
            "LEFTMARGIN=\"%d\" TOPMARGIN=\"%d\">\n"
            "    <pre>\n"
            "    This listing produced by hcref %d.%d at %s\n"
            "    \n"
            "    Files in this cross-reference are:\n"
            "    \n",
            VERSION, RELEASE, timebuff,
            VERSION, RELEASE, timebuff,
            DEF_LEFTMARGIN, DEF_TOPMARGIN,
            VERSION, RELEASE, timebuff);
}

/*******************************************************************************
       Name : append_view()
Description : 
 Parameters : 
    Returns :
   Comments : 
*******************************************************************************/
static void
append_view(char *fname)
{
    fprintf(vfile, "\t%s\n", fname);
    fflush(vfile);
    fprintf(stderr, "\t%s\n", fname);
}

/*******************************************************************************
       Name : close_view()
Description : 
 Parameters : 
    Returns :
   Comments : 
*******************************************************************************/
static void
close_view(void)
{
    fprintf(vfile,
            "    </pre>\n"
            "  </BODY>\n"
            "</HTML>\n");

    fclose(vfile);
}


/*******************************************************************************
       Name : do_file()
Description : Process one input file.
 Parameters : fname - string with filename
    Returns :
   Comments : Just find each token.  If it's NOT a keyword, generate the HTML
            : link to the listing entry for it.
*******************************************************************************/
void
do_file(char *fname)
{
    char *token;
    int  lnum;
    bool defn;

    append_view(fname);

    scan_open(fname, fname, outputdir);

    while ((token = scan_token(&lnum, &defn)) != NULL)
    {
        if (!keyword(token))
        {
            list_add(token, fname, lnum, defn);
            scan_emit(TRUE);
        }
        else
        {
            scan_emit(FALSE);
        }
    }

    scan_close();
}


/*******************************************************************************
       Name : hcref_init()
Description : Initialize things that need initializing.
 Parameters :
    Returns :
   Comments :
*******************************************************************************/
static void
hcref_init(void)
{
    int numkeys = 0;

/******
 * Initialize the key_table[].
 ******/

    key_table[numkeys++] = "#define";
    key_table[numkeys++] = "#elif";
    key_table[numkeys++] = "#else";
    key_table[numkeys++] = "#endif";
    key_table[numkeys++] = "#error";
    key_table[numkeys++] = "#if";
    key_table[numkeys++] = "#ifdef";
    key_table[numkeys++] = "#ifndef";
    key_table[numkeys++] = "#include";
    key_table[numkeys++] = "#line";
    key_table[numkeys++] = "#pragma";
    end_hash = numkeys;
    key_table[numkeys++] = "auto";
    key_table[numkeys++] = "break";
    key_table[numkeys++] = "case";
    key_table[numkeys++] = "char";
    key_table[numkeys++] = "const";
    key_table[numkeys++] = "continue";
    key_table[numkeys++] = "default";
    key_table[numkeys++] = "do";
    key_table[numkeys++] = "double";
    key_table[numkeys++] = "else";
    key_table[numkeys++] = "enum";
    key_table[numkeys++] = "extern";
    key_table[numkeys++] = "float";
    key_table[numkeys++] = "for";
    key_table[numkeys++] = "goto";
    key_table[numkeys++] = "if";
    key_table[numkeys++] = "int";
    key_table[numkeys++] = "long";
    key_table[numkeys++] = "register";
    key_table[numkeys++] = "return";
    key_table[numkeys++] = "short";
    key_table[numkeys++] = "signed";
    key_table[numkeys++] = "sizeof";
    key_table[numkeys++] = "static";
    key_table[numkeys++] = "struct";
    key_table[numkeys++] = "switch";
    key_table[numkeys++] = "typedef";
    key_table[numkeys++] = "union";
    key_table[numkeys++] = "unsigned";
    key_table[numkeys++] = "void";
    key_table[numkeys++] = "volatile";
    key_table[numkeys++] = "while";
    if (numkeys > MAX_KEYS)
        error("KEY_TABLE too small! Try \"#define MAX_KEYS %d\"\n", numkeys);

    start_bin = 0;
    end_bin = numkeys;

/******
 * Init the local globals
 ******/

    outputdir = DEF_OUTPUTDIR;
    pagewidth = DEF_PAGEWIDTH;

    numbertokens = DEF_NUMBERTOKENS;
    hashtokens = DEF_HASHTOKENS;
    ctokens = DEF_CTOKENS;

    indexfile = DEF_INDEXFILE;
    menufile = DEF_MENUFILE;
    viewfile = DEF_VIEWFILE;
}


/*******************************************************************************
       Name : keyword()
Description : Decides if the symbol at "token" is a keyword or not.
 Parameters :
    Returns : Return TRUE if a keyword, else FALSE.
   Comments : We don't treat as special if it's a keyword.
*******************************************************************************/
static bool
keyword(char *token)
{
    int fore;
    int mid;
    int aft;
    int status;

    fore = end_bin;                      /* init the pointers */
    aft  = start_bin;

    while (aft < fore)
    {
        mid = (fore + aft) / 2;

        status = strcmp(key_table[mid], token);
        if (status == 0)
        {
            return TRUE;
        }

        if (status < 0)
            aft  = mid+1;
        else
            fore = mid;
    }

    return FALSE;
}


/*******************************************************************************
       Name : make_files()
Description : Create the support HTML files.
 Parameters :
    Returns :
   Comments :
*******************************************************************************/
static void
make_files(void)
{
    char       buff[FILENAME_LEN];
    FILE      *out;

/******
 * Create the output directory
 ******/

#ifdef __BORLANDC__
    if (mkdir(outputdir) == -1)
    {
        if (errno != EACCES)
            error("Can't create output directory '%s': %s (%d)",
                  outputdir, strerror(errno), errno);
    }
#else
    if (mkdir(outputdir, S_IRWXU + S_IRWXG + S_IRWXO) == -1)
    {
        if (errno != 17)
            error("X Can't create output directory '%s': %s (%d)",
                  outputdir, strerror(errno), errno);
    }
#endif

/******
 * Create the index file.  This is a simple frameset & should never change.
 ******/

    sprintf(buff, "%s/%s", outputdir, indexfile);

    out = fopen(buff, "w");
    if (out == NULL)
        error("Can't open file '%s' for writing: %s",
              buff, strerror(errno));

    fprintf(out,
          "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n");
    fprintf(out, "<!-- Generated by hcref %d.%d at %s -->\n",
                 VERSION, RELEASE, timebuff);
    fprintf(out, "<HTML>\n");
    fprintf(out, "  <HEAD>\n");
    fprintf(out, "    <META http-equiv=\"Content-Type\" "
                      "content=\"text/html; charset=iso-8859-1\">\n");
    fprintf(out, "    <title>hcref %d.%d at %s</title>\n",
                 VERSION, RELEASE, timebuff);
    fprintf(out, "  </HEAD>\n");
    fprintf(out, "  <FRAMESET BORDER=\"1\" ROWS=\"38,*\">\n");
    fprintf(out, "    <FRAME MARGINWIDTH=\"%d\" MARGINHEIGHT=\"%d\" "
                 "name=\"menu\" scrolling=\"no\" src=\"%s\">\n",
                 DEF_LEFTMARGIN, DEF_TOPMARGIN, menufile);
    fprintf(out, "    <FRAME MARGINWIDTH=\"0\" MARGINHEIGHT=\"0\" "
                 "name=\"view\" scrolling=\"auto\" src=\"%s\">\n",
                 viewfile);
    fprintf(out, "  </FRAMESET>\n");
    fprintf(out, "</HTML>\n");

    fclose(out);

/*****
 * Start making the 'view' file.
 *****/

    start_view();
}


/*******************************************************************************
       Name : make_menufile()
Description : Create the menu HTML file.
 Parameters :
    Returns :
   Comments :
*******************************************************************************/
static void
make_menufile(void)
{
    char  buff[FILENAME_LEN];
    FILE *out;

/******
 * Create the menu file
 ******/

    sprintf(buff, "%s/%s", outputdir, menufile);

    out = fopen(buff, "w");
    if (out == NULL)
        error("Can't open file '%s' for writing: %s",
              buff, strerror(errno));

    fprintf(out,
            "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n"
            "<!-- Generated by hcref %d.%d at %s -->\n"
            "<HTML>\n"
            "  <HEAD>\n"
            "    <title>hcref %d.%d at %s</title>\n"
            "    <META http-equiv=\"Content-Type\" "
                "content=\"text/html; charset=iso-8859-1\">\n"
            "  </HEAD>\n"
            "  <BODY ONLOAD=\"window.defaultStatus='';\" BGCOLOR=\"#ffe080\">\n"
            "   <font face=\"Lucida Sans\" size=\"2\">\n"
            "    <CENTER>\n"
            "      <TABLE BORDER=0 CELLSPACING=\"0\" CELLPADDING=\"5\">\n"
            "        <TR>\n",
            VERSION, RELEASE, timebuff,
            VERSION, RELEASE, timebuff);

    list_menuhtml(out, outputdir);

    fprintf(out,
            "        </TR>\n"
            "      </TABLE>\n"
            "    </CENTER>\n"
            "   </font>\n"
            "  </BODY>\n"
            "</HTML>\n");

    fclose(out);
}


/*******************************************************************************
       Name : usage()
Description : Prints some usage help.
 Parameters :
    Returns : Doesn't, calls exit().
   Comments :
*******************************************************************************/
static void
usage(void)
{
    fprintf(stderr, "usage: %s { <option> | <filename> }\n", ProgName);
    fprintf(stderr, "where -K       place C keywords in listing "
                    "(default %s)\n", (ctokens) ? "YES" : "NO");
    fprintf(stderr, "      -N       place numbers in listing "
                    "(default %s)\n", (numbertokens==TRUE) ? "YES" : "NO");
    fprintf(stderr, "      -O<dir>  set directory to store hcref "
                    "listing in (default %s)\n", outputdir);
    fprintf(stderr, "      -V       print the version and quit\n");
    fprintf(stderr, "      -Wnnn    sets output width to 'nnn' columns "
                    "(default %d)\n", pagewidth);
    fprintf(stderr, "      -X<name> reads file <name> for more filenames\n");
    fprintf(stderr, "      -#       place preprocessor #words in listing "
                    "(default %s)\n", (hashtokens) ? "YES" : "NO");

    exit(RET_ERROR);
}


/*******************************************************************************
       Name : main()
Description : The entry point of the program.
 Parameters :
    Returns :
   Comments : See usage() for details of parameters.
*******************************************************************************/
int
main(int argc, char *argv[])
{
    int        argcount;
    time_t     t;
    struct tm *tmptr;

/******
 * Set the start time buffer up.
 ******/

    time(&t);
    tmptr = localtime(&t);
    strftime(timebuff, sizeof(timebuff), "%H:%M:%S on %d %B, %Y", tmptr);

/******
 * Initialize globals
 ******/

    globals_init();

    ProgName = strrchr(argv[0], '\\');
    if (ProgName == NULL)
	ProgName = "hcref";
    else
	++ProgName;

    hcref_init();

/******
 * Now analyze params.  First, look for all options except '-x<file>'.
 * Then rescan args and pick out -x<file> and any filenames.
 * Handle each file as it is found.
 ******/

    if (argc < 2)
        usage();

    for (argcount = 1; argcount < argc; ++argcount)
    {
        if (argv[argcount][0] == '-')
        {
            switch (tolower(argv[argcount][1]))
            {
                case '#':
                    start_bin = end_hash;
                    break;

                case 'k':
                    end_bin = end_hash;
                    break;

                case 'n':
                    numbertokens = TRUE;
                    break;

                case 'o':
                    outputdir = &argv[argcount][2];
                    break;

                case 'v':
                    error("hcref %d.%d - %s\n", VERSION, RELEASE, DATE);
                    exit(RET_OK);
                    
                case 'w':
                    pagewidth = atoi(&argv[argcount][2]);
                    if (pagewidth < MIN_PAGEWIDTH)
                    {
                        error("Sorry, output width must be >= %d\n",
                              MIN_PAGEWIDTH);
                        exit(RET_ERROR);
                    }
                    break;
                    
                case 'x':
                    /* ignore X option this pass */
                    break;

                default:
                    usage();
                    break;
            }
        }
    }

/******
 * Create the index and other files.
 * Prepare the list routines.
 ******/
 
    make_files();
    list_open(outputdir, pagewidth);

/******
 * Now rescan args and pick out -x<file> and any filenames.
 * Handle each file as it is found.
 ******/

    for (argcount = 1; argcount < argc; ++argcount)
    {
        if (argv[argcount][0] == '-')
        {
            char filename[FILENAME_LEN];
            FILE *x_file;

            switch (tolower(argv[argcount][1]))
            {
                case 'x':
                    if ((x_file = fopen(&argv[argcount][2], "r")) == NULL)
                    {
                        error("Cannot open -x file '%s' for reading: %s\n",
                              &argv[argcount][2], strerror(errno));
                        break;
                    }

                    while (fgets(filename, FILENAME_LEN, x_file))
                    {
                        char *ch_ptr;

                        for (ch_ptr = filename;
                             !iscntrl(*ch_ptr) && !isspace(*ch_ptr);
                             ++ch_ptr)
                            ;
                        *ch_ptr = '\0';           /* strip trailing chars */
                        do_file(filename);
                    }
                    fclose(x_file);
                    break;

                default:
                    /* ignore all other options in this pass */
                    break;
            }
        }
        else
        {
            do_file(argv[argcount]);
        }
    }

/******
 * Close the list routines.
 ******/

    list_close();

/******
 * Now output the HTML cross-reference files.
 * token files: token_a.html, token_abc.html, ...
 * token list:  tlist_a.html, tlist_b.html, ...
 ******/

    make_menufile();		/* create the top-bar menu */

    list_cref();		/* make the individual token files */

    list_tokenlist();		/* list of alpha tokens files */

/******
 * Close the view file
 ******/

    close_view();

/******
 * Close down
 ******/

    return 0;                   /* to keep compiler happy! */
}
