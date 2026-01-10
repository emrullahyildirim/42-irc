#ifndef COMMANDS_HPP
#define COMMANDS_HPP

class Client;
class Parser;

void Command_Cap(Server &server, Client &client, const Parser &parser);
void Command_Ping(Server &server, Client &client, const Parser &parser);
void Command_Pass(Server &server, Client &client, const Parser &parser);
void Command_Nick(Server &server, Client &client, const Parser &parser);
void Command_User(Server &server, Client &client, const Parser &parser);
void Command_Join(Server &server, Client &client, const Parser &parser);
void Command_Privmsg(Server &server, Client &client, const Parser &parser);
void Command_Kick(Server &server, Client &client, const Parser &parser);

#endif