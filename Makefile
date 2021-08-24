# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eparisot <eparisot@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/08/05 23:09:54 by eparisot          #+#    #+#              #
#    Updated: 2020/08/05 23:09:56 by eparisot         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ft_ssl

SRCS	=	srcs/main.c \
			srcs/ssl_io.c \
			srcs/tools.c \
			srcs/padding.c \
			srcs/ft_ssl.c \
			srcs/md5.c \
			srcs/sha224.c \
			srcs/sha256.c \
			srcs/base64.c \
			srcs/des_ecb.c \
			srcs/des_cbc.c \
			srcs/des_tools.c \
			srcs/pbkdf.c


INC		=	includes/ft_ssl.h \
			includes/des.h \

OBJS	=	$(SRCS:.c=.o)

LIBS	=	libft/libft.a \

CFLAGS	=	-Wall -Wextra -Werror -g3

RM		=	rm -f

all		:	$(LIBS) $(NAME)

$(NAME)	:	$(OBJS) $(INC)
	gcc $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(LIBS)	: .FORCE
	@$(MAKE) -C libft

.FORCE	:

clean	:
	$(RM) $(OBJS) && $(MAKE) clean -C libft

fclean	:	clean
	$(RM) $(NAME) libft/libft.a

re		:	fclean all

.PHONY : all re clean fclean
