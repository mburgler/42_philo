/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburgler <mburgler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:52:15 by mburgler          #+#    #+#             */
/*   Updated: 2023/06/08 16:20:55 by mburgler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	*init(int nb_args, char **strs, t_msc *msc)
{
	int i;
	int	j;

	i = 0;
	j = 0;
	while(strs[i])
	{
		while(strs[i][j])
		{
			if((strs[i][j] >= '0' && strs[i][j] <= '9') \
				 || ((j == 0 && strs[i][j] == '+')))
				j++;
			else
				return (ft_error("forbidden character", msc), -1);
		}
		i++;
	}
	msc = ft_calloc(sizeof(t_msc), 1);
	if (msc == NULL)
		return (ft_error("malloc failed", msc), -1);
	msc->nb_philo = ft_atoi(strs[1]);
	if(msc->nb_philo == 0)
		return(ft_error("no philo", msc), -1);
	msc->time_to_die = ft_atoi(strs[2]);
	msc->time_to_eat = ft_atoi(strs[3]);
	msc->time_to_sleep = ft_atoi(strs[4]);
	if (nb_args == 6)
		msc->nb_must_eat = ft_atoi(strs[5]);
	else
		msc->nb_must_eat = -1;
	return (0);
}

int main(int argc, char **argv)
{
	t_msc	*msc;
	
	if(argc < 5 || argc > 6)
		return (ft_error("wrong number of arguments", msc), -1);
	if (init(argc, argv, msc) == -1)
		return (-1);
	return (0);
}