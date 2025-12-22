NAME = ircserv

CC = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

SRCDIR = src
OBJDIR = objects
BINDIR = bin

SRCS = $(SRCDIR)/main.cpp 
OBJS = $(OBJDIR)/main.o

all: $(BINDIR)/$(NAME)

$(BINDIR)/$(NAME): $(OBJS)
	mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(OBJS) -o $(BINDIR)/$(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(BINDIR)

re: fclean all

.PHONY: all clean fclean re