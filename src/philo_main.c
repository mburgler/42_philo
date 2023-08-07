/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburgler <mburgler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:52:15 by mburgler          #+#    #+#             */
/*   Updated: 2023/08/07 18:25:13 by mburgler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

//PROTOTYPE: nb_philo; time_to_die; time_to_eat; time_to_sleep;
//[number_of_times_each_philosopher_must_eat]
//(200, 2147483647, 2147483647, 2147483647, 2147483647)

int	init_mutex(t_msc *msc)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	if (pthread_mutex_init(&msc->mutex->print, NULL) != 0)
		return (-1);
	if (pthread_mutex_init(&msc->mutex->death, NULL) != 0)
		return (pthread_mutex_destroy(&msc->mutex->print), -1);
	if (pthread_mutex_init(&msc->mutex->meal_count, NULL) != 0)
		return (pthread_mutex_destroy(&msc->mutex->print),
			pthread_mutex_destroy(&msc->mutex->death), -1);
	while (++i < msc->nb_philo)
	{
		if (pthread_mutex_init(&msc->mutex->forks[i], NULL) != 0)
		{
			while (++j < i)
				pthread_mutex_destroy(&msc->mutex->forks[j]);
			return (pthread_mutex_destroy(&msc->mutex->print),
				pthread_mutex_destroy(&msc->mutex->death),
				pthread_mutex_destroy(&msc->mutex->meal_count), -1);
		}
	}
	msc->mutex_initialised = true;
	return (0);
}

int	init_philo(int i, t_msc *msc)
{
	msc->philo[i] = ft_calloc(sizeof(t_philo), 1);
	if (msc->philo[i] == NULL)
		return (-1);
	msc->philo[i]->nb_philo = i + 1;
	msc->philo[i]->left_fork = i;
	if (i > 0)
		msc->philo[i - 1]->right_fork = &(msc->philo[i]->left_fork);
	msc->philo[i]->meal_count = 0;
	msc->philo[i]->time_last_meal = 0;
	msc->philo[i]->msc = msc;
	return (0);
}

int	init(t_msc *msc)
{
	int	i;

	i = 0;
	msc->philo = ft_calloc(sizeof(t_philo *), msc->nb_philo);
	if (msc->philo == NULL)
		return (ft_err("malloc failed", msc), -1);
	msc->mutex = ft_calloc(sizeof(t_mutex), 1);
	if (msc->mutex == NULL)
		return (ft_err("malloc failed", msc), -1);
	msc->mutex->forks = ft_calloc(sizeof(pthread_mutex_t), msc->nb_philo);
	if (msc->mutex->forks == NULL)
		return (ft_err("malloc failed", msc), -1);
	while (i < msc->nb_philo)
	{
		if (init_philo(i, msc) == -1)
			return (ft_err("malloc failed", msc), -1);
		i++;
	}
	msc->philo[msc->nb_philo - 1]->right_fork = &(msc->philo[0]->left_fork);
	if (init_mutex(msc) == -1)
		return (ft_err("mutex init failed", msc), -1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_msc	*msc;
	int		i;

	i = -1;
	msc = ft_calloc(sizeof(t_msc), 1);
	if (msc == NULL)
		return (ft_err("malloc failed", msc), -1);
	msc->mutex_initialised = false;
	if (argc < 5 || argc > 6)
		return (ft_err("wrong number of arguments", NULL), -1);
	if (parsing(argc, argv, msc) == -1)
		return (-1);
	if (init(msc) == -1)
		return (-1);
	if (msc->nb_philo == 1)
	{
		if (just_one_philo(msc, i) == -1)
			return (-1);
	}
	else 
	{
		if (simulation_startup(msc, i) == -1)
			return (-1);
	}
	return (free_ma_boi(msc), 0);
}
