#include "../../inc/chatty.h"

void    *udp_receive(void* arg) {

    t_udp              *udp = (t_udp *)arg;
    char               clients_buffer[BUF_SIZE * 2];
    struct sockaddr_in new_cliaddr;
    socklen_t          addr_len = sizeof(new_cliaddr);
    ssize_t            n;

    while (1) {

        
        if ((n = recvfrom(udp->sockfd, clients_buffer, BUF_SIZE * 2 - 1, 0, (struct sockaddr*)&new_cliaddr, &addr_len)) <= 0)
            continue;

        char sender_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &new_cliaddr.sin_addr, sender_ip, sizeof(sender_ip));

        clients_buffer[n] = '\0';  
        int status;
        char username[64], machine_id[64], tcp_port[64]; 

        int ret = sscanf(clients_buffer, UDP_REGEX, &status, username, machine_id, tcp_port);
        if (ret == 4) {
            pthread_mutex_lock(&hash_table_mutex);
            udp_handle_user(status, udp, &new_cliaddr, username, machine_id, tcp_port);
            pthread_mutex_unlock(&hash_table_mutex);
        }
        // else if (ret == 7)
        //     printf("%s%s%s::%s%s%s %s(all): %s%s\n", get_colour(colour_a), machine_id, RESET, get_colour(colour_b), username, RESET, BOLD_GREEN, msg, RESET);
        
    }
    return (NULL);      
}

void    udp_handle_user(int status, t_udp *udp, struct sockaddr_in *new_cliaddr,  char *username, char *machine_id, char *tcp_port) {

    if (status == 0) {
        t_client *found = ht_delete(udp->users_table, username, machine_id);
        if (found) {

            printf("%s<%s><%s>%s %sis now offline.%s\n", BOLD_WHITE ,found->USERNAME, found->MACHINE_ID, RESET, BOLD_RED, RESET);
            free(found);
        }
        return ;
    }
    else if (status == 1) {

        if (ht_search(udp->users_table, username) == NULL) {

            t_client *new = ht_add(new_cliaddr, tcp_port, username, machine_id);
            if (!new)
                return ;

            if (ht_insert(udp->users_table, new)) {
                free(new);
                return ;
            }
            printf("%s<%s><%s>%s %sis now online.%s\n", BOLD_WHITE ,new->USERNAME, new->MACHINE_ID, RESET, BOLD_GREEN, RESET);
        }
    }
    return ;
}