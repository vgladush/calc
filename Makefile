
NAME	=	calc
CXX		=	gcc
FLAGS	=	-Wall -Wextra
DIR		=	./
SRCS	=	calc.c
OBJC	=	$(SRCS:.c=.o)
OBJS	=	$(addprefix $(DIR), $(OBJC))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) -o $(NAME) $(OBJC)
	@echo "$(NAME) ready"

$(DIR)%.o: $(DIR)%.c
	@$(CXX) $(FLAGS) -o $@ -c $<

clean:
	@rm -f $(OBJS)
	@echo "objects $(NAME) deleted"

fclean: clean
	@rm -f $(NAME)
	@echo "$(NAME) deleted"

re: fclean all
