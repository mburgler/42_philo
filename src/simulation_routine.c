/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_routine.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburgler <mburgler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 20:41:56 by mburgler          #+#    #+#             */
/*   Updated: 2023/08/07 17:20:36 by mburgler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	*matrix(void *arg)
{
	t_philo	*one_philo;

	if (!arg)
		return (NULL);
	one_philo = (t_philo *)arg;
	pthread_mutex_lock(&one_philo->msc->mutex->death);
	while (one_philo->msc->stop_simulation == false)
	{
		pthread_mutex_unlock(&one_philo->msc->mutex->death);
		philo_eats(one_philo, one_philo->msc);
		if (philo_sleeps(one_philo, one_philo->msc) == -1)
			return (NULL);
		ft_mutex_print(one_philo->msc, one_philo, "is thinking");
		pthread_mutex_lock(&one_philo->msc->mutex->death);
		if (one_philo->msc->stop_simulation == true)
			return (pthread_mutex_unlock(&one_philo->msc->mutex->death), NULL);
	}
	return (NULL);
}

void	philo_eats(t_philo *one_philo, t_msc *msc)
{
	if ((one_philo->nb_philo) % 2)
	{
		usleep(1000);
		pthread_mutex_lock(&msc->mutex->forks[one_philo->left_fork]);
		ft_mutex_print(msc, one_philo, "has taken his left fork");
		pthread_mutex_lock(&msc->mutex->forks[*one_philo->right_fork]);
		ft_mutex_print(msc, one_philo, "has taken his right fork");
	}
	else
	{
		pthread_mutex_lock(&msc->mutex->forks[*one_philo->right_fork]);
		ft_mutex_print(msc, one_philo, "has taken his right fork");
		pthread_mutex_lock(&msc->mutex->forks[one_philo->left_fork]);
		ft_mutex_print(msc, one_philo, "has taken his left fork");
	}
	ft_mutex_print(msc, one_philo, "is eating");
	pthread_mutex_lock(&msc->mutex->meal_count);
	one_philo->meal_count++;
	pthread_mutex_unlock(&msc->mutex->meal_count);
	pthread_mutex_lock(&msc->mutex->death);
	one_philo->time_last_meal = sys_time();
	pthread_mutex_unlock(&msc->mutex->death);
	usleep(msc->time_to_eat * 1000);
	pthread_mutex_unlock(&msc->mutex->forks[one_philo->left_fork]);
	pthread_mutex_unlock(&msc->mutex->forks[*one_philo->right_fork]);
}

int	philo_sleeps(t_philo *one_philo, t_msc *msc)
{
	pthread_mutex_lock(&msc->mutex->death);
	if (msc->stop_simulation == true)
	{
		pthread_mutex_unlock(&msc->mutex->death);
		return (-1);
	}
	pthread_mutex_unlock(&msc->mutex->death);
	ft_mutex_print(msc, one_philo, "is sleeping");
	usleep(msc->time_to_sleep * 1000);
	return (0);
}
