/* ************************************************************************** */
/*                       .-.                       .                          */
/*                      / -'                      /                           */
/*       .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (               */
/*        )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )         .    */
/*   By: '/   /   (`.'  /      `-'-.-/   /.- (.''--'`-`-'  `--':        /     */
/*                  -'            (   \  / .-._.).--..-._..  .-.  .-../ .-.   */
/*   Created: 16-05-2022  by       `-' \/ (   )/    (   )  )/   )(   / (  |   */
/*   Updated: 16-05-2022 11:30 by      /\  `-'/      `-'  '/   (  `-'-..`-'-' */
/*                                 `._;  `._;                   `-            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct Response
{
	int type;
	std::string content_type;
	int content_lenght;
	std::string (*get_content)(std::string, Response&);

	std::string getDocumentContent(std::string file_path);
};


#endif