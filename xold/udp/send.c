#include "../../inc/chatty.h"

void    *udp_broadcast(void* arg) {

    t_udp              *udp = (t_udp *)arg;

    while (1) {

        sendto(
            udp->sockfd, 
            udp->user_string, 
            udp->user_string_len, 
            0, 
            (struct sockaddr*)udp->send_addr, 
            sizeof(*udp->send_addr)
        );
        sleep(3);
    }
    return (NULL);      
}


void    send_udp_message(t_udp *udp, char *msg)
{
    pthread_mutex_lock(&colour_mutex);

    char *together = strjoin(udp->user_string, msg);
    size_t len = strlen(together);
    sendto(udp->sockfd, together, len, 0, (struct sockaddr*)udp->send_addr, sizeof(*udp->send_addr));
    free(together);
    pthread_mutex_unlock(&colour_mutex);
    
}

void    udp_send_disconnect_msg(t_udp *udp) {

    memcpy(udp->user_string, "0;", 2);
	size_t user_len = strlen(udp->user_string);

	for (int i = 0; i < TABLE_MAX_SIZE; i++) {

		t_client *cur = udp->users_table[i];
		while (cur) {
			
            sendto(udp->sockfd, udp->user_string, user_len, 0, 
                (struct sockaddr*)&cur->CLIENT_ADDR, sizeof(cur->CLIENT_ADDR));
			cur = cur->next;
		}
	}
}