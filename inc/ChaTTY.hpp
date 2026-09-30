#pragma once

#include "utils.hpp"
#include "Table.hpp"

struct MyInfo {

	int              PORT_TCP;
	char            *USERNAME;
	char            *MACHINE_ID;
	char            *USER_STRING;
	char            *colour_a;
	char            *colour_b;
};


struct ExitLvl {

    int             self;
    int             main;
    int             udp;
    int             tcp;
};

class ChaTTY {

	private:
		static t_udp           *UDP;
    	static t_tcp           *TCP;
    	static MyInfo			myInfo;
		static ExitLvl			exitLvl


	public:

		ChaTTY(char *tcp_port, char *udp_port);
		~ChaTTY();
		ChaTTY(const ChaTTY&) = delete;
		ChaTTY&	operator=(const ChaTTY&) = delete;
		
};