#include "../inc/chatty.h"
#include <sys/select.h>

pthread_mutex_t			hash_table_mutex   		= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t			colour_mutex 	   		= PTHREAD_MUTEX_INITIALIZER;
t_manager 			   *manager;

int		main(int argc, char **argv) {

    if (argc < 2) {
		return (printf(HELP_MSG), 1);
	}

	if ( (manager = calloc(1, sizeof(t_manager))) == NULL) {
		return (1);
	}
	
	if (init_manager(argv[1])) {
		return (exit_manager(), 1);
	}

    if (udp_struct_init()) {
		return (exit_manager(), 1);
	}

    if (tcp_struct_init()) {
		return (exit_manager(), 1);
	}
	printf("Using Port: %d\n", manager->PORT_TCP);
	printf("Username: %s\n", manager->USERNAME);
	printf("Machine: %s\n", manager->MACHINE_ID);
	printf("User String: %s\n\n", manager->USER_STRING);
	if (pthread_create(&manager->UDP->send_thread, NULL, udp_broadcast, (void*)manager->UDP) == -1) {
        return (perror("udp send thread"), exit_manager(), 1);
    }
	manager->EXIT_LVL_MAIN++;

    if (pthread_create(&manager->UDP->receive_thread, NULL, udp_receive, (void*)manager->UDP) == -1) {
        return (perror("udp send thread"), exit_manager(), 1);
    }
	manager->EXIT_LVL_MAIN++;

	if (pthread_create(&manager->TCP->send_thread, NULL, tcp_receive, (void*)manager->TCP) == -1) {
        return (perror("tcp thread"), exit_manager(), 1);
    }
	manager->EXIT_LVL_MAIN++;


	while (1) { 

		char *input = readline("");
		if (!input)
			break;
		if (*input)
			add_history(input);
		handle_commands(input, manager->TCP);
		free(input);
	}
	manager->EXIT_LVL_MAIN++;


    return (exit_manager(), 0);
}

