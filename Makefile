NAME = chatty

CC = cc
CFLAGS = -Wall -Wextra -MMD -MP
LDFLAGS = -lpthread -lreadline -lncurses

RM = rm -rf

WHITE = \033[1;37m
GREEN = \033[1;32m
RED  =  \033[1;31m
ORANGE  = \033[1;38;5;208m
RESET = \033[0m


SRCS = \
	src/main.c \
	src/manager/init.c \
	src/manager/utils.c \
	src/manager/commands.c \
	src/manager/exit.c \
	src/table/hash_table.c \
	src/tcp/init.c \
	src/tcp/send.c \
	src/tcp/receive.c \
	src/udp/init.c \
	src/udp/send.c \
	src/udp/receive.c \

OBJDIR = objs
OBJS = $(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
DEPS = $(OBJS:.o=.d)



SEND_IP = $(shell ip -4 addr show enp4s0f0 | grep -oP '(?<=brd\s)\d+(\.\d+){3}')
LOCAL_IP = $(shell ip -4 -o addr show | awk '{print $$4}' | cut -d/ -f1 | grep '^192\.168\.' | head -n 1)



all: CFLAGS += -D SEND_IP=\"$(SEND_IP)\"
all: $(NAME)
	@echo "Using IP: $(SEND_IP)"
	@echo "$(WHITE)Execute with ./chatty --connect$(RESET)"

local: CFLAGS += -D SEND_IP=\"$(LOCAL_IP)\"
local: $(NAME)
	@echo "Using Local IP: $(LOCAL_IP)"

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@echo "$(GREEN)Files Compiled$(RESET)"
	@$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

clean:
	@echo "$(ORANGE)Executable Removed$(RESET)"
	@$(RM) $(OBJDIR)

fclean:
	@echo "$(ORANGE)Executable Removed$(RESET)"
	@echo "$(RED)Object Files Removed$(RESET)"
	@$(RM) $(OBJDIR) $(NAME)

re: fclean clean all

-include $(DEPS)
