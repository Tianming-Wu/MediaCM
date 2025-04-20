#pragma once

#ifdef __clang__
#define COMPLIER_VERSION __clang_version__
#else if(defined gcc)
#define COMPLIER_VERSION GCC_VERSION
#endif

#define MEDIACM_ABOUT_TITLE "About MediaCM"
#define MEDIACM_ABOUT_TEXT "MediaCM" COMPLIER_VERSION "\nAuthor Tianming\n\nThanks icons8.com for their wonderful icons."

#define MEDIACM_ABOUT QMessageBox::about(this, MEDIACM_ABOUT_TITLE, MEDIACM_ABOUT_TEXT);
