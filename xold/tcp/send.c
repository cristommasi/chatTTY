#include "../../inc/chatty.h"

void    send_tcp_message(t_client *client, const char *msg, t_tcp *tcp)
{
	int         client_sockfd;
    char        *user_msg;

	if ((client_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return ;
    }

	if (connect(client_sockfd, (struct sockaddr*)&client->CLIENT_ADDR, sizeof(client->CLIENT_ADDR)) < 0) {

        perror("User is offline.\n");
		close(client_sockfd);
		return ;
	}
    user_msg = strjoin(build_coloured_name(manager->USERNAME, BOLD_WHITE), msg);
	write(client_sockfd, user_msg, strlen(user_msg));
	close(client_sockfd);
    free(user_msg);
}
