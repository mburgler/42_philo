/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburgler <mburgler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:52:15 by mburgler          #+#    #+#             */
/*   Updated: 2023/06/05 14:29:55 by mburgler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

t_msc	*init(int nb_args, char **strs)
{
	t_msc	*msc;

	msc = ft_calloc(sizeof(t_msc));
	if (msc == NULL)
		ft_error("malloc failed", msc);
	msc->nb_philo = ft_atoi(strs[1]);
	msc->time_to_die = ft_atoi(strs[2]);
	msc->time_to_eat = ft_atoi(strs[3]);
	msc->time_to_sleep = ft_atoi(strs[4]);
	if (nb_args == 6)
		msc->nb_must_eat = ft_atoi(strs[5]);
	else
		msc->nb_must_eat = -1;
	return (msc);
}
//ABOVE IS NEW

int main(int argc, char **argv)
{
	t_msc	*msc;
	
	if(argc < 5 || argc > 6)
		ft_error("wrong number of arguments", msc);
	msc = init(argc, argv);
	return (0);
}