#pragma once

#include "utils.hpp"
#include "Table.hpp"


class Udp {

	private:
    	static char*        userString;
    	static size_t       userStringLen;
    	static int          sockFd;
    	static sockaddr_in  receiveAddr;
    	static sockaddr_in  sendAddr;
    	static pthread_t    sendThread;
    	static pthread_t    receiveThread;
    	static bool         sendStarted;
    	static bool         receiveStarted;


	private:
		static int 		socketInit();
		static int 		receiveAddrInit();
		static int 		sendAddrInit();
		static void*	broadcast(void *arg);
		static void*	receive(void *arg);
		static void		handleUser(sockaddr_in* newClient, char status, const char* username, const char* machineId, const char* tcpPort);
	

	public:
		Udp() = default;
		static int 		init(const char* user_string);
		static int 		start();
		static void		stop();
		static void		cleanup();
		static void		sendMessage(const char* msg);
		static void		sendDisconnect();
		static ExitCode	getExitCode();

};