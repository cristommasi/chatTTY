#include "../../inc/chatty.h"


int		init_manager(char *tcp_port) {


	manager->EXIT_LVL_man = 1;
	manager->EXIT_LVL_tcp = 0;
	manager->EXIT_LVL_udp = 0;
	manager->EXIT_LVL_MAIN = 0;
	

	if ((manager->USERS_TABLE = calloc(TABLE_MAX_SIZE, sizeof(t_client))) == NULL) {
		return (1);
	}
	manager->EXIT_LVL_man++;

	if ((manager->UDP = calloc(1, sizeof(t_udp))) == NULL) {
		return (1);
	}
	manager->EXIT_LVL_man++;

	if ((manager->TCP = calloc(1, sizeof(t_tcp))) == NULL) {
		return (1);
	}
	manager->EXIT_LVL_man++;

	manager->PORT_TCP = atoi(tcp_port);

	if ((manager->USERNAME = get_user_name()) == NULL) {
		return (1);
	}
	manager->EXIT_LVL_man++;

	if ((manager->MACHINE_ID = get_machine_id()) == NULL) {
		return (1);
	}
	manager->EXIT_LVL_man++;

	if ((manager->USER_STRING = build_user_string()) == NULL) {
		return (1);
	}
	manager->EXIT_LVL_man++;

	return (0);
}