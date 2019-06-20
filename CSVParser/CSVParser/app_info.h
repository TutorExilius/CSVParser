// App Informations here

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 4

#define APP_NAME "CSV Parser"
#define AUTHOR "Tutor Exilius"
#define AUTHOR_EMAIL "tutorexilius@gmail.com"
#define COPYRIGHT "Tutor Exilius - Copyright 2018"

#define STRING_VERSION "" STR(VERSION_MAJOR) "." STR(VERSION_MINOR) "." STR(VERSION_PATCH)
#define PRODUCT_VERSION VERSION_MAJOR,VERSION_MINOR,VERSION_PATCH,0
#define STRING_COPYRIGHT AUTHOR " (" AUTHOR_EMAIL ") - " COPYRIGHT
