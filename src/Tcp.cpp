#include "Tcp.hpp"

char*        Tcp::userString = NULL;
size_t       Tcp::userStringLen = 0;
int          Tcp::sockFd = -1;
sockaddr_in  Tcp::receiveAddr;
sockaddr_in  Tcp::sendAddr;
pthread_t    Tcp::sendThread;
pthread_t    Tcp::receiveThread;
bool         Tcp::receiveStarted = false;



int		Tcp::init(const char *user_string, int tcpPort) {

	userString = strdup(user_string);
	if (!userString)
		return (1);
	userStringLen = strlen(userString);
	if (Tcp::socketInit())
		return (1);
	if (Tcp::receiveAddrInit(tcpPort))
		return (1);
	return (0);
}


int		Tcp::socketInit() {

	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockFd < 0) {
		perror("Tcp::socketInit() socket creation failed");
		return (1);
	}

	int opt = 1;
   	setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	return (0);
}


int		Tcp::receiveAddrInit(int tcpPort) {

	memset(&receiveAddr, 0, sizeof(receiveAddr));
	receiveAddr.sin_family        = AF_INET;
    receiveAddr.sin_addr.s_addr   = INADDR_ANY;
    receiveAddr.sin_port          = htons(tcpPort);

	int b = bind(sockFd, (struct sockaddr*)&receiveAddr, sizeof(receiveAddr));
	if (b < 0) {
		perror("Tcp::receiveAddrInit() bind failed");
		return (1);
	}
	int l = listen(sockFd, 5);
	if (l < 0) {

        perror("Tcp::receiveAddrInit() listen failed");
        return (1);
    }
    return (0);
}

int 	Tcp::start() {

	int r = pthread_create(&receiveThread, NULL, receive, NULL);
	if (r != 0) {
		perror("Tcp::start() receive thread");
		return (1);
	}
	receiveStarted = true;
	return (0);
}


void	Tcp::stop() {

    if (receiveStarted) {
		pthread_join(receiveThread, NULL); 
		receiveStarted = false;
	}
}


void	Tcp::cleanup() {

	if (sockFd >= 0) {
		close(sockFd);
		sockFd = -1;
	}
	free(userString);
	userString = NULL;
	userStringLen = 0;
}


void*	Tcp::receive(void *arg) {

	char	buf[BUF_SIZE * 2];
	int		r;
	ssize_t n;


	while (ChaTTY::isRunning()) {

		pollfd				p = {sockFd, POLLIN, 0};
		int					r = poll(&p, 1, 200);
		struct sockaddr_in	newClient;
		socklen_t 			len;
		ssize_t				n;
		int 				newSockFd;
		char 				status, username[64], machineId[64], tcpPort[64], ;


		if (r == 0) continue;
		else if (r < 0) {
			if (errno == EINTR)
				continue ;
			break ;
		}
		

		len = sizeof(newClient);
		newSockFd = accept(sockFd, (struct sockaddr*)&newClient, &len);
		if (newSockFd < 0)
			continue ;
		
		pollfd cp = {newSockFd, POLLIN, 0};
		if (poll(&cp, 1, 1000) <= 0) {
			close(newSockFd);
			continue;
		}

		n = read(newSockFd, buf, BUF_SIZE * 2 - 1);
		if (n <= 0) {
			close(sockFd);
			continue;
		}
		buf[n] = '\0';


		fflush(stdout);
		write(STDOUT_FILENO, "\r\33[K", 4);
        printf("%s\n", buf);
		fflush(stdout);


        send(newSockFd, "OK\n", 3, MSG_NOSIGNAL);
        close(newSockFd);


	}
	return (NULL);
}


void		Tcp::sendMessage(Users *user, const char* msg) {

	int	userSockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (userSockFd < 0) {
		printf("chaTTY error: Udp failed to send messsage\n");
        return ;
    }

	int c = connect(userSockFd, (struct sockaddr*)&user->CLIENT_ADDR, sizeof(user->CLIENT_ADDR));
	if (c < 0) {

        printf("User is offline\n");
		close(userSockFd);
		return ;
	}

    char	*userMsg = strjoin(userString, msg);
	write(userSockFd, userMsg, strlen(userMsg));
	close(userSockFd);
    free(userMsg);
	
}
