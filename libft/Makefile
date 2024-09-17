# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/19 17:27:14 by adapassa          #+#    #+#              #
#    Updated: 2024/02/08 17:39:17 by adapassa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = libft.a
LIB = ar rcs $(NAME)
# Directory per i file oggetto creati
OBJ_DIR = obj/
SRC_DIR = ./

SRCS = ft_bzero ft_isalnum \
		ft_isalpha ft_isascii \
		ft_isdigit ft_isprint ft_memset \
		ft_strlen ft_memcpy ft_memmove \
		ft_strlcpy ft_strlcat ft_toupper \
		ft_tolower ft_strchr ft_strrchr \
		ft_strncmp ft_memchr ft_strnstr \
		ft_atoi ft_calloc ft_strdup \
		ft_substr ft_strjoin ft_strtrim \
		ft_split ft_itoa ft_strmapi \
		ft_striteri ft_putchar_fd ft_putstr_fd \
		ft_putendl_fd ft_putnbr_fd ft_memcmp \
		ft_printf ft_utils get_next_line \
		get_next_line_utils \
		ft_lstnew ft_lstadd_back ft_lstadd_front \
		ft_lstsize ft_lstdelone ft_lstlast \
		ft_lstclear ft_lstiter ft_lstmap \

SRC = $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRCS)))
OBJ = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRCS)))

# Special target for creating obj directory
OBJF = .cache_exists

DEFAULT = \033[0;39m
GREEN = \033[0;92m
RED = \033[0;91m
CIANO = \033[0;96m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
GRAY = \033[0;90m
WHITE = \033[0;97m

all: $(NAME)

$(NAME): $(OBJ)
		@$(LIB) $(NAME) $(OBJ)
		@echo "$(GREEN)$(NAME) compilata con successo❗️ 📁$(DEFAULT)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJF)
		@echo "$(YELLOW)Compiling: $< $(DEFAULT)"
		@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJF):
		@mkdir -p $(OBJ_DIR)

clean:
		@rm -f $(OBJ)
		@rm -rf $(OBJ_DIR)
		@echo "$(CIANO)directory OBJ rimossa...$(DEFAULT)"
		@echo "$(CIANO)file oggetto di $(NAME) rimossi con successo❗️ 🪦$(DEFAULT)"

fclean: clean
		@rm -f $(NAME)
		@echo "$(RED)$(NAME) rimossa con successo❗️ ❌$(DEFAULT)"

re: fclean all

.PHONY: all clean fclean bonus
