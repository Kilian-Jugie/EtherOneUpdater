#ifndef DEFS_H
#define DEFS_H

#define FOLDER_TEMP "local"
#define FOLDER_IMGS "imgs"
#define FOLDER_UP_DATA "Updater"

#define FILE_VERSION "Version.ver"
#define FILE_UP_VERSION FOLDER_UP_DATA"/UPVersion.ver"
#define FILE_DATA FOLDER_UP_DATA"/data.eoid"

#define FTP_USER "752016042616225"
#define FTP_PASS
#define FTP_PORT 21
#define FTP_HOST "etherone.web.boxtoplay.com"
#define FTP_UPBASEFILE "Updater\\"

#define EXTERN_DL_PROGRAM_64 "wget64.exe"
#define EXTERN_DL_PROGRAM_32 "wget"

#define EXTERN_DL_PROGRAM EXTERN_DL_PROGRAM_32

#define DB_FOLDER_BASE "http://etherone.web.boxtoplay.com/Updater/"
#define DB_FOLDER_VERSIONS "Versions/"
#define DB_FOLDER_DATA "Data/"
#define DB_FILE_UPDATER_VERSIONS DB_FOLDER_BASE DB_FOLDER_VERSIONS"UPVersions.ver"
#define DB_FILE_VERSION DB_FOLDER_BASE DB_FOLDER_VERSIONS"Versions.ver"

#define DL_UPDATER_VERSIONS FOLDER_UP_DATA"/DUPVersions.ver"
#define DL_VERSIONS FOLDER_TEMP"/DVersions.ver"
/*! \def IGNORE_AND_PROMP_UPDATE_ERRORS
 *  \brief Affiche les messages d'erreur lors de l'installation du modpack mais ceux-ci ne font pas arreter l'installation
 */
#define IGNORE_AND_PROMP_UPDATE_ERRORS
/*! \def DONT_PROMP_UPDATE_ERRORS
 *  \brief N'affiche pas les messages d'erreur mais ceux-ci arretent l'installation
 */
//#define DONT_PROMP_UPDATE_ERRORS
/*! \def IGNORE_DONT_PROMP_UPDATE_ERRORS
 *  \brief N'affiche pas les messages d'erreur et continue l'installation malgrès tout
 */
//#define IGNORE_DONT_PROMP_UPDATE_ERRORS

#ifdef IGNORE_DONT_PROMP_UPDATE_ERRORS
#define IGNORE_AND_PROMP_UPDATE_ERRORS
#define DONT_PROMP_UPDATE_ERRORS
#endif


#endif // DEFS_H
