NAME = ircserv

CXX = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g

SRCDIR = src
OBJDIR = objects
BINDIR = bin

SRCS =	$(SRCDIR)/main.cpp \
		$(SRCDIR)/Server.cpp \
		$(SRCDIR)/Client.cpp \
		$(SRCDIR)/Command.cpp \
		$(SRCDIR)/CommandManager.cpp \
		$(SRCDIR)/ClientManager.cpp \
		$(SRCDIR)/Channel.cpp \
		$(SRCDIR)/Parser.cpp \
		$(SRCDIR)/ChannelManager.cpp \
		$(SRCDIR)/commands/Privmsg.cpp \
		$(SRCDIR)/commands/Pass.cpp \
		$(SRCDIR)/commands/Nick.cpp \
		$(SRCDIR)/commands/User.cpp \
		$(SRCDIR)/commands/Cap.cpp \
		$(SRCDIR)/commands/Ping.cpp \
		$(SRCDIR)/commands/Join.cpp \
		$(SRCDIR)/commands/Part.cpp \
		$(SRCDIR)/commands/Kick.cpp \
		$(SRCDIR)/commands/Invite.cpp \
		$(SRCDIR)/commands/Topic.cpp \
		$(SRCDIR)/commands/Mode.cpp \
		$(SRCDIR)/commands/Who.cpp

OBJS = $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

all: $(BINDIR)/$(NAME)

$(BINDIR)/$(NAME): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CPPFLAGS) $(OBJS) -o $(BINDIR)/$(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(BINDIR)

re: fclean all

.PHONY: all clean fclean re