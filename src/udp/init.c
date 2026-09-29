#include "../../inc/chatty.h"

int     udp_struct_init() {

    t_udp *udp          = manager->UDP;

    
    manager->EXIT_LVL_udp = 0;
    if ((udp->user_string = strdup(manager->USER_STRING)) == NULL) {  // 1;johndoe;8898;
        return (1);
    }
    manager->EXIT_LVL_udp = 1;
    udp->user_string_len = strlen(udp->user_string);
    udp->users_table     = manager->USERS_TABLE;

    if (udp_socket_init(udp)) {
        return (1);
    }
    if (udp_receive_addr_init(udp)) {
        return (1);
    }
    if (udp_send_addr_init(udp)) {
        return (1);
    }

    return (0);
}

int     udp_socket_init(t_udp *udp) {

    if ( (udp->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        return (perror("udp socket"), 1);
    }

    udp->opt = 1;
    setsockopt(udp->sockfd, SOL_SOCKET, SO_REUSEADDR, &udp->opt, sizeof(udp->opt));
    setsockopt(udp->sockfd, SOL_SOCKET, SO_BROADCAST, &udp->opt, sizeof(udp->opt));

    manager->EXIT_LVL_udp = 2;
    return (0);
}

int    udp_receive_addr_init(t_udp *udp) {

    if ( (udp->receive_addr = calloc(1, sizeof(struct sockaddr_in)) ) == NULL)
        return (1);
    manager->EXIT_LVL_udp = 3;

    udp->receive_addr->sin_family        = AF_INET;
    udp->receive_addr->sin_addr.s_addr   = INADDR_ANY;
    udp->receive_addr->sin_port          = htons(UDP_PORT);

    if (bind(udp->sockfd, (struct sockaddr*)udp->receive_addr, sizeof(struct sockaddr_in)) < 0) {
        return (perror("udp bind"), 1);
    }
    return (0);
}

int     udp_send_addr_init(t_udp *udp) {

    if ( (udp->send_addr = calloc(1, sizeof(struct sockaddr_in)) ) == NULL)
        return (1);
    manager->EXIT_LVL_udp = 4;


	udp->send_addr->sin_family       = AF_INET;
    udp->send_addr->sin_port         = htons(UDP_PORT);
    udp->send_addr->sin_addr.s_addr  = inet_addr("192.168.1.255");
    return (0);
}


