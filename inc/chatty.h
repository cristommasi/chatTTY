#ifndef CHAT42_H
# define CHAT42_H

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

//-------------------------------------------------------------------------------------------------------NETWORKING VARS
#define TCP_PORT 5000
#define UDP_PORT 6000
# ifndef SEND_IP
#  define SEND_IP "0.0.0.0"
# endif
#define BUF_SIZE 1024
#define TABLE_MAX_SIZE 6
#define UDP_REGEX "%d;%63[^;];%63[^;];%63[^;];"
#define CONFIG_PATH ".usr_info.txt"
//-------------------------------------------------------------------------------------------------------NETWORKING VARS



//-------------------------------------------------------------------------------------------------------HASH TABLE
typedef struct s_client {

    struct sockaddr_in  CLIENT_ADDR;
    char                USERNAME[64];
    char                MACHINE_ID[64];
    struct s_client     *next;
}   t_client;

extern t_client **users_table;
extern pthread_mutex_t  hash_table_mutex;
extern pthread_mutex_t  colour_mutex;
extern pthread_mutex_t	msg_mutex;


void            ht_clear(t_client **users_table);
t_client        *ht_add(struct sockaddr_in *new_cliaddr, char *tcp_port, char *username, char *machine_id);
t_client        *ht_delete(t_client **users_table,  char *username, char *machine_id);
t_client        *ht_search(t_client **users_table, char *username);
int             ht_insert(t_client **users_table, t_client *new_user);
ssize_t         ht_hash(char *username);
void            print_client(t_client **users_table);
void            ht_init(t_client **users_table);
//-------------------------------------------------------------------------------------------------------HASH TABLE




//-------------------------------------------------------------------------------------------------------TCP THREAD
typedef struct s_tcp {

  char                *user_string;
  size_t              user_string_len;
  int                 sockfd;
  struct sockaddr_in  *receive_addr;
  struct sockaddr_in  *send_addr;
  int                 opt;
  t_client            **users_table;
  pthread_t           send_thread;
  pthread_t           receive_thread;

} t_tcp;

int     tcp_struct_init();
int     tcp_socket_init(t_tcp *tcp);
int     tcp_receive_addr_init(t_tcp *tcp);
void*   tcp_receive(void* arg);
void    send_tcp_message(t_client *client, const char *msg, t_tcp *tcp);
//-------------------------------------------------------------------------------------------------------TCP THREAD




//-------------------------------------------------------------------------------------------------------UDP THREAD
typedef struct s_udp {

  char                *user_string;
  size_t              user_string_len;
  int                 sockfd;
  int                 opt;
  struct sockaddr_in  *receive_addr;
  struct sockaddr_in  *send_addr;
  t_client            **users_table;
  pthread_t           send_thread;
  pthread_t           receive_thread;

} t_udp;


int     udp_send_addr_init(t_udp *udp);
int     udp_receive_addr_init(t_udp *udp);
int     udp_socket_init(t_udp *udp);
int     udp_struct_init(); 
void    udp_handle_user(int status, t_udp *udp, struct sockaddr_in *new_cliaddr,  char *username, char *machine_id, char *tcp_port);
void    *udp_receive(void* arg);
void    *udp_broadcast(void* arg);
void    send_udp_message(t_udp *udp, char *msg);
void    udp_send_disconnect_msg(t_udp *udp);
//-------------------------------------------------------------------------------------------------------UDP THREAD





//-------------------------------------------------------------------------------------------------------MANAGER STRUCT 
typedef struct s_manager
{
    t_udp           *UDP;
    t_tcp           *TCP;
    int              PORT_TCP;
    t_client        **USERS_TABLE;
    char            *USERNAME;
    char            *MACHINE_ID;
    char            *USER_STRING;
    char            *colour_a;
    char            *colour_b;
    int             EXIT_LVL_man;
    int             EXIT_LVL_udp;
    int             EXIT_LVL_tcp;
    int             EXIT_LVL_MAIN;
}             t_manager;

extern t_manager *manager;

int	            init_manager(char *tcp_port);
void            exit_manager();
void            free_tcp(int level);
void            free_udp(int level);
void            free_manager(int level);
void            get_usr_file_colours();
void            set_default_colours();
//-------------------------------------------------------------------------------------------------------MANAGER STRUCT 



//-------------------------------------------------------------------------------------------------------UTILS
char      	*build_user_string();
char        *get_machine_id(void);
char      	*get_user_name(void);
void      	set_default_colours();
void      	get_usr_file_colours();
char      	*build_coloured_name(const char *username, const char *colour_b);
char      	*strjoin(const char *s1, const char *s2);
char        *get_colour(const char *name);
const char  *get_color_name(const char *code);
//-------------------------------------------------------------------------------------------------------UTILS




//-------------------------------------------------------------------------------------------------------COMMANDS
typedef void (*command_func_t)(const char *arg1, const char *arg2);

struct command_entry {

	const char *name;
	command_func_t func;

};


void              handle_commands(const char *input, t_tcp *tcp);
command_func_t    is_server_command(char *cmd);
void              help_command(const char *arg1, const char *arg2);
void              disconnect_command(const char *arg1, const char *arg2);
void              version_command(const char *arg1, const char *arg2);
void              colour_set_command(const char *arg1, const char *arg2);
void              colour_list_command(const char *arg1, const char *arg2);

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
//-------------------------------------------------------------------------------------------------------COMMANDS


#endif
