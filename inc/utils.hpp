#pragma once

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <cstddef>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <poll.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define TCP_PORT 5000
#define UDP_PORT 6000
# ifndef SEND_IP
#  define SEND_IP "0.0.0.0"
# endif

#define BUF_SIZE 1024
#define TABLE_MAX_SIZE 6
#define TABLE_USERNAME_MAX 64
#define TABLE_MACHINE_ID_MAX 64

#define UDP_REGEX "%1;%63[^;];%63[^;];%63[^;];"
#define CONFIG_PATH ".usr_info.txt"


typedef int ExitCode;

#define SERVER_COMMAND_COUNT 5
#define HELP_MSG  "\
usage: chatty [--version] [--help] [--login] [<username> + <message>]\n\
              [--colour-list] [--colour-set (+ <colour>::<colour>)]\n\
              [--block + <username>]\n\n\
These are common chatty commands used in various situations:\n\n\
  --connect                      Start chatty process.\n\
  --help                         Print help page.\n\
  --version                      State current version.\n\
  --colour-list                  List all ANSI colours.\n\
  --colour-set <colour::colour>  Set profile colours.\n\
  <username> + <message>         Send message to username.\n\
\n\
Example: chat42 --colour-set BLUE::GREEN\n"

#define VERSION_MSG "chat42 pre-pre-alpha 0.0.1\n"

#define OFFLINE_MSG "%s<%s><%s>%s %sis now offline.%s\n"

#define ONLINE_MSG "%s<%s><%s>%s %sis now online.%s\n"

#define RESET       "\x1b[0m"
#define BLACK       "\x1b[0;30m"
#define RED         "\x1b[0;31m"
#define GREEN       "\x1b[0;32m"
#define YELLOW      "\x1b[0;33m"
#define BLUE        "\x1b[0;34m"
#define MAGENTA     "\x1b[0;35m"
#define CYAN        "\x1b[0;36m"
#define WHITE       "\x1b[0;37m"
#define BOLD_BLACK   "\x1b[1;30m"
#define BOLD_RED     "\x1b[1;31m"
#define BOLD_GREEN   "\x1b[1;32m"
#define BOLD_YELLOW  "\x1b[1;33m"
#define BOLD_BLUE    "\x1b[1;34m"
#define BOLD_MAGENTA "\x1b[1;35m"
#define BOLD_CYAN    "\x1b[1;36m"
#define BOLD_WHITE   "\x1b[1;37m"


const char *get_color_name(const char *code);
char  	*get_colour(const char *name);
char	*strjoin(const char *s1, const char *s2);
char	*build_coloured_name(const char *username, const char *colour_b);
void	get_usr_file_colours(); 
void	set_default_colours();
char	*get_user_name(void);
char *get_machine_id(void);
char	*build_user_string();