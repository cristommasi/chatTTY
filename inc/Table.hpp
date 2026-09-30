#pragma once

#include "utils.hpp"

typedef struct Users {

    struct sockaddr_in  CLIENT_ADDR;
    char                USERNAME[TABLE_USERNAME_MAX];
    char                MACHINE_ID[TABLE_MACHINE_ID_MAX];
    Users     			*next;
};


class Table {

	private:
    	static Users          *users[TABLE_MAX_SIZE];
    	static pthread_mutex_t	table_lock;
		static size_t			hash(const char *username);

	public:	
		Table() = delete;
		static Users*	create(struct sockaddr_in *new_cliaddr, const char *username, const char *machine_id, const char *tcp_port);
		static int		add(struct sockaddr_in *new_cliaddr, const char *username, const char *machine_id, const char *tcp_port);
		static int		remove(const char *username, const char *machine_id);
		static int		find(const char *username, Users *out);
		static void		print();
		static void		clear();
		static Users**  getUsersTable();

};