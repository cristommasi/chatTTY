#include "../../inc/chatty.h"

void	exit_manager() {
	
	pthread_mutex_unlock(&hash_table_mutex);
	pthread_mutex_lock(&hash_table_mutex);

	if (manager->EXIT_LVL_MAIN >= 4) {
		rl_clear_history();
    	rl_free_line_state();
    	rl_cleanup_after_signal();
	}

	if (manager->EXIT_LVL_MAIN >= 3) {
		pthread_cancel(manager->TCP->receive_thread);
    	pthread_join(manager->TCP->receive_thread, NULL);
	}

	if (manager->EXIT_LVL_MAIN >= 2) {
		pthread_cancel(manager->UDP->receive_thread);
    	pthread_join(manager->UDP->receive_thread, NULL);
	}

	if (manager->EXIT_LVL_MAIN >= 1) {
		udp_send_disconnect_msg(manager->UDP);
		pthread_cancel(manager->UDP->send_thread);
    	pthread_join(manager->UDP->send_thread, NULL);
	}

	free_tcp(manager->EXIT_LVL_tcp);
	free_udp(manager->EXIT_LVL_udp);
	free_manager(manager->EXIT_LVL_man);

	pthread_mutex_unlock(&hash_table_mutex);

	printf("\nServer shutdown cleanly.\n");
}

void	free_tcp(int level) {

	if (level >= 3) free(manager->TCP->receive_addr);
	if (level >= 2) close(manager->TCP->sockfd);
	if (level >= 1) free(manager->TCP->user_string);
}


void	free_udp(int level) {

	if (level >= 4) free(manager->UDP->send_addr);
	if (level >= 3) free(manager->UDP->receive_addr);
	if (level >= 2) close(manager->UDP->sockfd);
	if (level >= 1) free(manager->UDP->user_string);
}

void	free_manager(int level) {

	if (level >= 7) free(manager->USER_STRING);
	if (level >= 6) free(manager->MACHINE_ID);
	if (level >= 5) free(manager->USERNAME);
	if (level >= 4) free(manager->TCP);
	if (level >= 3) free(manager->UDP);
	if (level >= 2) free(manager->USERS_TABLE);
	if (level >= 1) free(manager);
}

