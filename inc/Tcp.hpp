#pragma once

#include "utils.hpp"
#include "Table.hpp"


class Tcp {


	private:
    	static char*        userString;
    	static size_t       userStringLen;
    	static int          sockFd;
    	static sockaddr_in  receiveAddr;
    	static sockaddr_in  sendAddr;
    	static pthread_t    sendThread;
    	static pthread_t    receiveThread;
    	static bool         receiveStarted;


	private:
		static int 		socketInit();
		static int 		receiveAddrInit(int tcpPort);
		static void*	receive(void *arg);	

	public:
		Tcp() = default;
		static int 		init(const char* user_string, int tcpPort);
		static int 		start();
		static void		stop();
		static void		cleanup();
		static void		sendMessage(Users *user, const char* msg);
};