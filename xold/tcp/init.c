#include "../../inc/chatty.h"


int     tcp_struct_init() {

    t_tcp       *tcp = manager->TCP;

    
    if ((tcp->user_string = strdup(manager->USER_STRING)) == NULL)
        return (1);
    manager->EXIT_LVL_tcp++;


    tcp->user_string_len = strlen(tcp->user_string);
    tcp->users_table = manager->USERS_TABLE;

    if (tcp_socket_init(tcp)) {
        return (1);
    }

    if (tcp_receive_addr_init(tcp)) {
        return (1);
    }

    return (0);
}


int     tcp_socket_init(t_tcp *tcp) {

    if ((tcp->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {

        perror("tcp socket()");
        return (1);
    }
    manager->EXIT_LVL_tcp++;

    tcp->opt = 1;
    setsockopt(tcp->sockfd, SOL_SOCKET, SO_REUSEADDR, &tcp->opt, sizeof(tcp->opt));
    return (0);
}


int     tcp_receive_addr_init(t_tcp *tcp) {


    if ( (tcp->receive_addr = calloc(1, sizeof(struct sockaddr))) == NULL) {
        return (1);
    }
    manager->EXIT_LVL_tcp++;

    tcp->receive_addr->sin_family = AF_INET;
    tcp->receive_addr->sin_addr.s_addr = INADDR_ANY;
    tcp->receive_addr->sin_port = htons(manager->PORT_TCP);

    if (bind(tcp->sockfd, (struct sockaddr*)tcp->receive_addr, sizeof(struct sockaddr_in)) < 0) {
        
        perror("tcp bind");
        return (1);
    }
    if (listen(tcp->sockfd, 5) < 0) {

        perror("tcp listen");
        return (1);
    }
    return (0);
}