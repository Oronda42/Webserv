/* ************************************************************************** */
/*                       .-.                       .                          */
/*                      / -'                      /                           */
/*       .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (               */
/*        )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )         .    */
/*   By: '/   /   (`.'  /      `-'-.-/   /.- (.''--'`-`-'  `--':        /     */
/*                  -'            (   \  / .-._.).--..-._..  .-.  .-../ .-.   */
/*   Created: 16-05-2022  by       `-' \/ (   )/    (   )  )/   )(   / (  |   */
/*   Updated: 16-05-2022 11:04 by      /\  `-'/      `-'  '/   (  `-'-..`-'-' */
/*                                 `._;  `._;                   `-            */
/* ************************************************************************** */

#ifndef REQUEST_CPP
# define REQUEST_CPP

#include <iostream>

struct Request
{
	int type;
	std::string rawRequest;
	std::string filePath;
	Request(int type, std::string rawRequest) : type(type), rawRequest(rawRequest), filePath(get_file_path(rawRequest)){}

	std::string get_file_path(std::string &request)
	{
		std::string first_line = get_first_line(request);
		std::vector<std::string> splited_fl = split(first_line, ' ');

		if (!splited_fl[1].compare("/"))
			return (ROOT);
		std::string file_path = splited_fl[1];
		return file_path;
	}
};



#endif