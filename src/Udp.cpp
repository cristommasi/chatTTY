#include "Udp.hpp"

char*        Udp::userString = NULL;
size_t       Udp::userStringLen = 0;
int          Udp::sockFd = -1;
sockaddr_in  Udp::receiveAddr;
sockaddr_in  Udp::sendAddr;
pthread_t    Udp::sendThread;
pthread_t    Udp::receiveThread;
bool         Udp::sendStarted = false;
bool         Udp::receiveStarted = false;



int		Udp::init(const char *user_string) {

	userString = strdup(user_string);
	if (!userString)
		return (1);
	userStringLen = strlen(userString);
	if (Udp::socketInit())
		return (1);
	if (Udp::receiveAddrInit())
		return (1);
	if (Udp::sendAddrInit())
		return (1);
	return (0);
}


int		Udp::socketInit() {

	sockFd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockFd < 0) {
		perror("Udp::socketInit() socket creation failed");
		return (1);
	}

	int opt = 1;
   	setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(sockFd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
	return (0);
}


int		Udp::receiveAddrInit() {

	memset(&receiveAddr, 0, sizeof(receiveAddr));
	receiveAddr.sin_family        = AF_INET;
    receiveAddr.sin_addr.s_addr   = INADDR_ANY;
    receiveAddr.sin_port          = htons(UDP_PORT);

	int b = bind(sockFd, (struct sockaddr*)&receiveAddr, sizeof(receiveAddr));
	if (b < 0) {
		perror("Udp::receiveAddrInit() bind failed");
		return (1);
	}
    return (0);
}


int		Udp::sendAddrInit() {

	memset(&sendAddr, 0, sizeof(sendAddr));
	sendAddr.sin_family       = AF_INET;
    sendAddr.sin_port         = htons(UDP_PORT);
    sendAddr.sin_addr.s_addr  = inet_addr("192.168.1.255");
	return (0);
}


int 	Udp::start() {

	int s = pthread_create(&sendThread, NULL, broadcast, NULL);
	if (s != 0) {
		perror("Udp::start() send thread");
		return (1);
	}
	sendStarted = true;

	int r = pthread_create(&receiveThread, NULL, receive, NULL);
	if (r != 0) {
		perror("Udp::start() receive thread");
		return (1);
	}
	receiveStarted = true;
	return (0);
}


void	Udp::stop() {

    bool any = sendStarted || receiveStarted;

    if (receiveStarted) {
		pthread_join(receiveThread, NULL); 
		receiveStarted = false;
	}
    if (sendStarted) {
		pthread_join(sendThread, NULL);
		sendStarted = false;
	}
    if (any) {
        sendDisconnect();     
	}
}


void	Udp::cleanup() {

	if (sockFd >= 0) {
		close(sockFd);
		sockFd = -1;
	}
	free(userString);
	userString = NULL;
	userStringLen = 0;
}


void*	Udp::broadcast(void *arg) {

	while (ChaTTY::isRunning()) {

		sendto(sockFd, userString, userStringLen, 0, (struct sockaddr*)&sendAddr, sizeof(sendAddr));

		for (int i = 0; i < 30; && ChaTTY::isRunning(); i++) {
			usleep(100000);
		}
	}
	return (NULL);
}


void*	Udp::receive(void *arg) {

	char	buf[BUF_SIZE * 2];
	int		r;
	ssize_t n;


	while (ChaTTY::isRunning()) {

		pollfd				p = {sockFd, POLLIN, 0};
		int					r = poll(&p, 1, 200);
		struct sockaddr_in	newClient;
		socklen_t 			len;
		ssize_t 			n;
		char 				status, username[64], machineId[64], tcpPort[64];


		if (r == 0) continue;
		else if (r < 0) {
			if (errno == EINTR)
				continue ;
			break ;
		}
		

		len = sizeof(newClient);
		n = recvfrom(sockFd, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&newClient, &len);
		if (n <= 0)
			continue ;
		buf[n] = '\0';

		

		if (sscanf(buf, UDP_REGEX, status, username, machineId, tcpPort) == 4)
			handleUser(&newClient, status, username, machineId, tcpPort);
		

	}
	return (NULL);
}


void		Udp::handleUser(sockaddr_in* newClient, char status, const char* username, const char* machineId, const char* tcpPort) {

	if (status == '0') {

		if (Table::remove(username, machineId) == 0)
			printf(OFFLINE_MSG, BOLD_WHITE ,username, machineId, RESET, BOLD_RED, RESET);
	}
	else if (status == '1') {

		Users	tmp;

		if (Table::find(username, &tmp) != 0 && Table::add(newClient, username, machineId, tcpPort) == 0)
			printf(ONLINE_MSG, BOLD_WHITE ,username, machineId, RESET, BOLD_GREEN, RESET);

	}
}


void		Udp::sendMessage(const char* msg) {

	char *together = strjoin(userString, msg);
	if (!together) {
		printf("chaTTY error: Udp failed to send messsage\n");
		return ;
	}
	size_t len = strlen(together);
	sendto(sockFd, together, len, 0, (struct sockaddr*)&sendAddr, sizeof(sendAddr));
	free(together);
}


void		Udp::sendDisconnect() {

	if (!userString) return ;

	memcpy(userString, "0", 1);
	size_t len = strlen(userString);
	Users** users = Table::getUsersTable();

	for (int i = 0; i < TABLE_MAX_SIZE; i++) {

		Users *cur = users[i];
		while (cur) {
			
            sendto(sockFd, userString, len, 0, (struct sockaddr*)&cur->CLIENT_ADDR, sizeof(cur->CLIENT_ADDR));
			cur = cur->next;
		}
	}
}

