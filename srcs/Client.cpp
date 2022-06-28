#include "Client.hpp"

Client::Client() : fd(-1), is_ready(false), header_received(false)
{ }

Client::Client(int connection) : fd(connection), is_ready(false), header_received(false)
{ } 

Client::~Client()
{ }