#include "../inc/Table.hpp"

Users				*Table::users[TABLE_MAX_SIZE];
pthread_mutex_t		Table::table_lock = PTHREAD_MUTEX_INITIALIZER;	

Users**  Table::getUsersTable() {return users;}


size_t		Table::hash(const char *username) {

	size_t hash_value = 5381;
    for (size_t i = 0; username[i]; i++) {

        hash_value *= 33;
        hash_value = (hash_value * 33 + username[i]) % TABLE_MAX_SIZE;
    }
    return (hash_value);
}

Users*	Table::create(struct sockaddr_in *new_cliaddr, const char *username, const char *machine_id, const char *tcp_port) {

	Users *new_user;
	
	new_user = (Users*)calloc(1, sizeof(Users));
	if (!new_user) {
		return (NULL);
	}

	new_user->CLIENT_ADDR.sin_family = AF_INET;
    new_user->CLIENT_ADDR.sin_addr.s_addr = new_cliaddr->sin_addr.s_addr;
    new_user->CLIENT_ADDR.sin_port = htons(atoi(tcp_port));
    strncpy(new_user->USERNAME, username, TABLE_USERNAME_MAX - 1);
    strncpy(new_user->MACHINE_ID, machine_id, TABLE_MACHINE_ID_MAX - 1);
	new_user->next = NULL;

	return (new_user);
}

int		Table::add(struct sockaddr_in *new_cliaddr, const char *username, const char *machine_id, const char *tcp_port) {

    
	if (!new_cliaddr || !username || !machine_id || !tcp_port)
		return (1);
	
	pthread_mutex_lock(&table_lock);

	Users *new_user = Table::create(new_cliaddr, username, machine_id, tcp_port);
    if (!new_user) {

		return (pthread_mutex_unlock(&table_lock), 1);
	}

	size_t hash_id = hash(username);

	if (!users[hash_id]) {

		users[hash_id] = new_user;
	}
	else {

		Users *c = users[hash_id];
		while (c->next) {
			c = c->next;
		}
		c->next = new_user;
	}
    return (pthread_mutex_unlock(&table_lock), 0);
}


int		Table::remove(const char *username, const char *machine_id) {

	if (!username || !machine_id) return (1);
	pthread_mutex_lock(&table_lock);

	size_t	hash_id = hash(username);
	Users *cur 	= users[hash_id];
    Users *prev 	= NULL;

    while (cur) {

        if (!strcmp(cur->USERNAME, username) && !strcmp(cur->MACHINE_ID, machine_id)) {
            if (prev == NULL)
                prev->next = cur->next;
            else
               	users[hash_id] = cur->next;
            free(cur);
			pthread_mutex_unlock(&table_lock);
            return (0);
        }
        prev = cur;
        cur = cur->next;
    }

	pthread_mutex_unlock(&table_lock);	
    return (1);
}


int		Table::find(const char *username, Users *out) {

	if (!username) return (1);
	pthread_mutex_lock(&table_lock);


	for (Users *cur = users[hash(username)]; cur; cur = cur->next) {

		if (!strcmp(cur->USERNAME, username)) {
			*out = *cur;
			out->next = NULL;
			pthread_mutex_unlock(&table_lock);
			return (0);
		}
			
	}

	pthread_mutex_unlock(&table_lock);
	return (1);
}




void	Table::print() {
	pthread_mutex_lock(&table_lock);

    for (int i = 0; i < TABLE_MAX_SIZE; i++) {

        Users *cur = users[i];
        if (cur == NULL)
            printf("[%d] = NULL\n", i);

        while (cur) {

            if (cur == NULL)
                printf("[%d] = NULL\n", i);
            else
                printf("[%d] = 1;%s;%s\n", i, users[i]->MACHINE_ID, users[i]->USERNAME);
            cur = cur->next;
        }
    }

	pthread_mutex_unlock(&table_lock);
}


void	Table::clear() {
	pthread_mutex_lock(&table_lock);

	for (int i = 0; i < TABLE_MAX_SIZE; i++) {

		Users *cur = users[i];
		Users *next;

		while (cur) {
			next = cur->next;
			free(cur);
			cur = next;
		}
		users[i] = NULL;
	}

	pthread_mutex_unlock(&table_lock);
}

