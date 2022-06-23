#include "../includes/Client.hpp"

Client::Client(int connection) : fd(connection), is_ready(false), header_received(false) {} 

Client::~Client() {}