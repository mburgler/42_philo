/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_management.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburgler <mburgler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 17:08:58 by mburgler          #+#    #+#             */
/*   Updated: 2023/08/01 18:14:28 by mburgler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	ft_err(char *str, t_msc *msc)
{
	write(2, "Error: ", 7);
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	free_ma_boi(msc);
}

void	free_null(void **ptr)
{
	if (ptr != NULL && *ptr != NULL)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	free_ma_boi(t_msc *msc)
{
	int	i;

	i = -1;
	if (msc != NULL)
	{
		if (msc->mutex != NULL)
		{
			if (msc->mutex_initialised == true)
				destroy_mutexes(msc);
			if (msc->mutex->forks != NULL)
				free_null((void **)&msc->mutex->forks);
			free_null((void **)&msc->mutex);
		}
		if (msc->philo != NULL)
		{
			while (++i < msc->nb_philo)
				free_null((void **)&msc->philo[i]);
			free_null((void **)&msc->philo);
		}
		free_null((void **)&msc);
	}
}

void	destroy_mutexes(t_msc *msc)
{
	int	i;

	i = -1;
	while (++i < msc->nb_philo)
		pthread_mutex_destroy(&msc->mutex->forks[i]);
	pthread_mutex_destroy(&msc->mutex->print);
	pthread_mutex_destroy(&msc->mutex->death);
	pthread_mutex_destroy(&msc->mutex->meal_count);
}
