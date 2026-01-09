NAME = ircserv

CXX = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

SRCDIR = src
OBJDIR = objects
BINDIR = bin

SRCS = $(SRCDIR)/main.cpp $(SRCDIR)/Server.cpp $(SRCDIR)/Client.cpp $(SRCDIR)/Command.cpp $(SRCDIR)/CommandManager.cpp $(SRCDIR)/ClientManager.cpp $(SRCDIR)/Parser.cpp $(SRCDIR)/Commands.cpp
OBJS = $(OBJDIR)/main.o $(OBJDIR)/Server.o $(OBJDIR)/Client.o $(OBJDIR)/Command.o $(OBJDIR)/CommandManager.o $(OBJDIR)/ClientManager.o $(OBJDIR)/Parser.o $(OBJDIR)/Commands.o

all: $(BINDIR)/$(NAME)

$(BINDIR)/$(NAME): $(OBJS)
	mkdir -p $(BINDIR)
	$(CXX) $(CPPFLAGS) $(OBJS) -o $(BINDIR)/$(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(BINDIR)

re: fclean all

.PHONY: all clean fclean re