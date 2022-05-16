/* ************************************************************************** */
/*                       .-.                       .                          */
/*                      / -'                      /                           */
/*       .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (               */
/*        )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )         .    */
/*   By: '/   /   (`.'  /      `-'-.-/   /.- (.''--'`-`-'  `--':        /     */
/*                  -'            (   \  / .-._.).--..-._..  .-.  .-../ .-.   */
/*   Created: 16-05-2022  by       `-' \/ (   )/    (   )  )/   )(   / (  |   */
/*   Updated: 16-05-2022 12:03 by      /\  `-'/      `-'  '/   (  `-'-..`-'-' */
/*                                 `._;  `._;                   `-            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

std::string Response::getDocumentContent(std::string file_path)
{
	file_path.erase(0,1);
	std::ifstream ifs(file_path.c_str());

	std::cout << "document path : "<<  file_path << std::endl << std::endl;
	if (!ifs.is_open() ) 
	{
		std::cerr << "could not open input file" << std::endl;
		return std::string();
	}

	std::vector<std::string> lines;
	std::string line;
	while(std::getline(ifs, line)) 
	{
		this->content_lenght += line.size();
		lines.push_back(line);
	}
	
	std::stringstream ss;
	for (int i = 0; i < lines.size(); i++ ) 
		ss << lines[i]; 
	std::string result = ss.str();
	return result;
}