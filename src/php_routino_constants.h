
// Wrapper for routino conats. All constants is long
#define REGISTER_ROUTINO_CONSTANT(__c) REGISTER_LONG_CONSTANT(#__c, __c, CONST_CS | CONST_PERSISTENT)

REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_NONE);

REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_NO_DATABASE); //           1 /*+ A function was called without the database variable set. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_NO_PROFILE);  //          2 /*+ A function was called without the profile variable set. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_NO_TRANSLATION);  //      3 /*+ A function was called without the translation variable set. +*/

REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_NO_DATABASE_FILES);//    11 /*+ The specified database to load did not exist. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_BAD_DATABASE_FILES);//   12 /*+ The specified database could not be loaded. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_NO_PROFILES_XML);//      13 /*+ The specified profiles XML file did not exist. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_BAD_PROFILES_XML);//     14 /*+ The specified profiles XML file could not be loaded. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_NO_TRANSLATIONS_XML);//  15 /*+ The specified translations XML file did not exist. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_BAD_TRANSLATIONS_XML);// 16 /*+ The specified translations XML file could not be loaded. +*/

REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_NO_SUCH_PROFILE); //     21 /*+ The requested profile name does not exist in the loaded XML file. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_NO_SUCH_TRANSLATION);//  22 /*+ The requested translation language does not exist in the loaded XML file. +*/

REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_NO_NEARBY_HIGHWAY);//    31 /*+ There is no highway near the coordinates to place a waypoint. +*/

REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_PROFILE_DATABASE_ERR);// 41 /*+ The profile and database do not work together. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_NOTVALID_PROFILE);//     42 /*+ The profile being used has not been validated. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_BAD_USER_PROFILE);//     43 /*+ The user specified profile contained invalid data. +*/

REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_BAD_OPTIONS);//          51 /*+ The routing options specified are not consistent with each other. +*/

REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_WRONG_API_VERSION);//    61 /*+ There is a mismatch between the library and caller API version. +*/

REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_PROGRESS_ABORTED);//     71 /*+ The progress function returned false. +*/

REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_NO_ROUTE_1);//         1001 /*+ A route could not be found to waypoint 1. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_NO_ROUTE_2);//         1002 /*+ A route could not be found to waypoint 2. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ERROR_NO_ROUTE_3);//         1003 /*+ A route could not be found to waypoint 3. +*/
/*  Higher values of the error number refer to later waypoints. */


 /* Routino routing option constants */

REGISTER_ROUTINO_CONSTANT(ROUTINO_ROUTE_SHORTEST); //              0 /*+ Calculate the shortest route. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ROUTE_QUICKEST); //              1 /*+ Calculate the quickest route. +*/

REGISTER_ROUTINO_CONSTANT(ROUTINO_ROUTE_FILE_HTML);//             2 /*+ Output an HTML route file. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ROUTE_FILE_GPX_TRACK);//        4 /*+ Output a GPX track file. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ROUTE_FILE_GPX_ROUTE);//        8 /*+ Output a GPX route file. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ROUTE_FILE_TEXT);//            16 /*+ Output a text file with important junctions. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ROUTE_FILE_TEXT_ALL);//        32 /*+ Output a text file with all nodes and segments. +*/

REGISTER_ROUTINO_CONSTANT(ROUTINO_ROUTE_FILE_STDOUT);//          64 /*+ Output a single file type to stdout. +*/

REGISTER_ROUTINO_CONSTANT(ROUTINO_ROUTE_LIST_HTML);//           128 /*+ Output a linked list of points containing the HTML file information but as plain text. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ROUTE_LIST_HTML_ALL);//       256 /*+ Output a linked list of points containing the HTML file information as plain text and with all points. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ROUTE_LIST_TEXT);//           512 /*+ Output a linked list of points containing the text file information. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ROUTE_LIST_TEXT_ALL);//      1024 /*+ Output a linked list of points containing the text all file information. +*/

REGISTER_ROUTINO_CONSTANT(ROUTINO_ROUTE_LOOP);//               2048 /*+ Route between the points in a loop returning to the first point. +*/
REGISTER_ROUTINO_CONSTANT(ROUTINO_ROUTE_REVERSE);//            4096 /*+ Route between the points in reverse order. +*/
