/* ************************************************************************** */
/*                       .-.                       .                          */
/*                      / -'                      /                           */
/*       .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (               */
/*        )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )         .    */
/*   By: '/   /   (`.'  /      `-'-.-/   /.- (.''--'`-`-'  `--':        /     */
/*                  -'            (   \  / .-._.).--..-._..  .-.  .-../ .-.   */
/*   Created: 17-05-2022  by       `-' \/ (   )/    (   )  )/   )(   / (  |   */
/*   Updated: 17-05-2022 15:26 by      /\  `-'/      `-'  '/   (  `-'-..`-'-' */
/*                                 `._;  `._;                   `-            */
/* ************************************************************************** */

#ifndef ERRORS_HPP
# define ERRORS_HPP

class FileNotFoundException : public std::exception
{
	public:
		char const *what() const throw()
		{
			return "File not found";
		}
};

class InvalidFileException : public std::exception
{
	public:
		char const *what() const throw()
		{
			return "Invalid file";
		}
};

#endif