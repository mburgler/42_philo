/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburgler <mburgler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 18:03:27 by mburgler          #+#    #+#             */
/*   Updated: 2023/08/04 17:19:33 by mburgler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	parsing(int nb_args, char **strs, t_msc *msc)
{
	int	i;
	int	j;

	i = 0;
	while (++i < nb_args)
	{
		j = 0;
		while (strs[i][j] != '\0')
		{
			if ((strs[i][j] >= '0' && strs[i][j] <= '9') || ((j == 0
						&& strs[i][j] == '+')))
				j++;
			else
				return (ft_err("forbidden char or negative value", msc), -1);
		}
		if (check_max_min_int(strs[i], msc) == -1)
			return (-1);
	}
	parsed_to_variables(nb_args, msc, strs);
	if (msc->nb_philo == 0 || msc->time_to_die == 0 || msc->time_to_eat == 0
		|| msc->time_to_sleep == 0 || (nb_args == 6 && msc->nb_must_eat == 0))
		return (ft_err("forbidden zero", msc), -1);
	return (0);
}

int	check_max_min_int(char *str, t_msc *msc)
{
	int		num;
	char	*temp;
	int		i;

	i = 0;
	if (str[0] == '+')
		i++;
	num = ft_atoi(str + i);
	temp = ft_itoa(num);
	if (temp == 0)
		return (ft_err("malloc failed", msc), -1);
	if (ft_strncmp(str + i, temp, ft_strlen(str + i)) != 0)
	{
		free(temp);
		return (ft_err("max/ min int violated", msc), -1);
	}
	else
	{
		free(temp);
		return (0);
	}
}

void	parsed_to_variables(int nb_args, t_msc *msc, char **strs)
{
	msc->nb_philo = ft_atoi(strs[1]);
	msc->time_to_die = ft_atoi(strs[2]);
	msc->time_to_eat = ft_atoi(strs[3]);
	msc->time_to_sleep = ft_atoi(strs[4]);
	if (nb_args == 6)
		msc->nb_must_eat = ft_atoi(strs[5]);
	else
		msc->nb_must_eat = -1;
	msc->stop_simulation = false;
}
