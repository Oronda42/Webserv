/* ************************************************************************** */
/*                       .-.                       .                          */
/*                      / -'                      /                           */
/*       .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (               */
/*        )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )         .    */
/*   By: '/   /   (`.'  /      `-'-.-/   /.- (.''--'`-`-'  `--':        /     */
/*                  -'            (   \  / .-._.).--..-._..  .-.  .-../ .-.   */
/*   Created: 17-05-2022  by       `-' \/ (   )/    (   )  )/   )(   / (  |   */
/*   Updated: 17-05-2022 15:49 by      /\  `-'/      `-'  '/   (  `-'-..`-'-' */
/*                                 `._;  `._;                   `-            */
/* ************************************************************************** */

#include "../include/Config.hpp"

bool Config::parse(const std::string &configFile)
{
	std::vector<Server> servers;
	std::ifstream ifs(configFile.c_str());

	if (!ifs.is_open())
	{
		std::cerr << "File '" << configFile << "' not found" << std::endl;
		return false;
	}

	

	return true;
}