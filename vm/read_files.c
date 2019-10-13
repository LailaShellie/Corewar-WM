/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshellie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/13 15:53:01 by lshellie          #+#    #+#             */
/*   Updated: 2019/10/13 15:53:02 by lshellie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../corewar.h"

int			check_file_format(char *str)
{
	int 	len;

	len = ft_strlen(str);
	if (len < 4)
		return (0);
	if (len == 4 && ft_strcmp(str, ".cor"))
		return (0);
	if (len > 4 && ft_strcmp(&str[len - 4], ".cor"))
		return (0);
	return (1);
}

int			choose_num(t_player *player, int fl_n)
{
	int 	i;

	if (fl_n < 0)
	{
		i = -1;
		while (++i < MAX_PLAYERS)
		{
			if (player[i].size < 0)
				return (i);
		}
		return (-1);
	}
	else if (fl_n > 0 && fl_n <= MAX_PLAYERS)
		return (fl_n - 1);
	return (-1);
}

int			parse_all(int fd, t_player *player, int fl_n)
{
	int 	i;

	if ((i = choose_num(player, fl_n)) < 0)
		return (error(WRONG_NUM));
	if (!(get_header(&player[i], fd)))
		return (0);
	if (!(get_name_or_comment(&player[i], fd, GET_NAME)))
		return (0);
	if (!(get_null(&player[i], fd)))
		return (0);
	if (!(get_size(&player[i], fd)))
		return (0);
	if (!(get_name_or_comment(&player[i], fd, GET_COMMENT)))
		return (0);
	if (!(get_null(&player[i], fd)))
		return (0);
	if (!(get_code(&player[i], fd)))
		return (0);
	return (1);
}

int			read_player(t_player *player, int fl_n, char *str)
{
	int 	fd;

	if ((fd = open(str, O_RDONLY)) < 0)
		return (error(CANT_OPEN_FILE));
	if (!(parse_all(fd, player, fl_n)))
	{
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

int			read_files(t_player *player, int ac, char **av)
{
	int 	i;
	int 	fl_n;

	fl_n = -1;
	i = 0;
	while (++i < ac)
	{
		if (fl_n < 0 && !ft_strcmp(av[i], "-n") && i + 1 < ac)
		{
			fl_n = ft_atoi(av[++i]);
			if (fl_n > MAX_PLAYERS)
				return (error(WRONG_NUM));
			continue ;
		}
		if (check_file_format(av[i]) && fl_n <= MAX_PLAYERS)
		{
			if (!(read_player(player, fl_n, av[i])))
				return (0);
			fl_n = -1;
		}
		else
			return (error(BAD_FILE_FORMAT));
	}
	return ((fl_n <= 0 || fl_n > MAX_PLAYERS) ? 1 : error(BAD_FLAG));
}
