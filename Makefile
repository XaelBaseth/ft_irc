# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acharlot <acharlot@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/08 10:14:02 by acharlot          #+#    #+#              #
#    Updated: 2024/01/26 16:15:19 by acharlot         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Variable
MAKEFLAGS	+= --silent

NAME		=	ircserv
NAME_CAPS	= 	IRCSERV
INC			=	inc/
HEADER		=	-I inc
SRC_DIR 	=	src/
OBJ_DIR 	=	obj/
DEBUG_DIR   =   debug/
CC			=	c++
CFLAGS		=	-Wall -Wextra -Werror -std=c++98
DFLAGS		= 	-g3 -fsanitize=address
RM			=	rm -rf
ECHO		=	echo

#Colors
DEF_COLOR	=	\033[0;39m
ORANGE		=	\033[0;33m
GRAY		=	\033[0;90m
RED			=	\033[0;91m
GREEN		=	\033[1;92m
YELLOW		=	\033[1;93m
BLUE		=	\033[0;94m
MAGENTA		=	\033[0;95m
CYAN		=	\033[0;96m
WHITE		=	\033[0;97m

#Files
MAIN_DIR	=	main/
MAIN_FILES	=	ircserv

SRC_MAI_FILE=	$(addprefix $(MAIN_DIR), $(MAIN_FILES))

MAINSRC		=	$(addprefix $(SRC_DIR), $(addsuffix .cpp, $(SRC_MAI_FILE)))
MAINOBJ		=	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_MAI_FILE)))
DEBUGOBJ	=	$(addprefix $(DEBUG_DIR), $(addsuffix .o, $(SRC_MAI_FILE)))

OBJF		=	.cache_exists

OBJ 		=	$(MAINOBJ)

#Rules
all:			$(NAME)

$(NAME):		$(OBJ)
					@$(CC) $(CFLAGS) $(OBJ) $(HEADER) -o $(NAME)
					@$(ECHO) "$(YELLOW)[$(NAME_CAPS)]:\t$(ORANGE)[==========]\t$(GREEN) => Success!$(DEF_COLOR)\n"

$(OBJ_DIR)%.o:	$(SRC_DIR)%.cpp $(OBJF)
					@$(CC) $(CFLAGS) -c $< -o $@

$(OBJF):		
					@mkdir -p $(OBJ_DIR)
					@mkdir -p $(OBJ_DIR)$(MAIN_DIR)
					@touch $(OBJF)

debug: $(DEBUG_DIR) $(DEBUGOBJ)
					@$(CC) $(CFLAGS) $(DFLAGS) $(DEBUGOBJ) $(HEADER) -o $(DEBUG_DIR)$(NAME)
					@$(ECHO) "$(RED)[$(NAME_CAPS)]:\tdebug files$(DEF_COLOR)\t$(GREEN) => Success!$(DEF_COLOR)\n"

$(DEBUG_DIR):
					@mkdir -p $(DEBUG_DIR)
					@mkdir -p $(DEBUG_DIR)$(MAIN_DIR)

$(DEBUG_DIR)%.o: $(SRC_DIR)%.cpp $(OBJF)
					@$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

help: ## Print help on Makefile.
					@grep '^[^.#]\+:\s\+.*#' Makefile | \
					sed "s/\(.\+\):\s*\(.*\) #\s*\(.*\)/`printf "$(GRAY)"`\1`printf "$(DEF_COLOR)"`	\3 /" | \
					expand -t8

clean: ## Clean generated files and cache.
					@$(RM) $(OBJ_DIR)
					@$(RM) $(OBJF)
					@$(RM) $(DEBUG_DIR)
					@$(ECHO) "$(BLUE)[$(NAME_CAPS)]:\tobj. files$(DEF_COLOR)\t$(GREEN) => Cleaned!$(DEF_COLOR)\n"

fclean: ## Clean all generated file, including binaries.		
					@make clean
					@$(RM) $(NAME)
					@$(ECHO) "$(CYAN)[$(NAME_CAPS)]:\texe. files$(DEF_COLOR)\t$(GREEN) => Cleaned!$(DEF_COLOR)\n"
					
re: ## Clean and rebuild binary file.
					@make fclean all
					@$(ECHO) "\n$(GREEN)###\tCleaned and rebuilt everything for [$(NAME_CAPS)]!\t###$(DEF_COLOR)\n"

.PHONY:			all clean fclean re help debug
