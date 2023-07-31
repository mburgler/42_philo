/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburgler <mburgler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:52:15 by mburgler          #+#    #+#             */
/*   Updated: 2023/07/31 02:03:12 by mburgler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

//PROTOTYPE: nb_philo; time_to_die; time_to_eat; time_to_sleep; 
//[number_of_times_each_philosopher_must_eat]
//INT OVERFLOW IN atoi with parsing  (200, 2147483647, 2147483647, 2147483647, 2147483647)

int	parsing(int nb_args, char **strs, t_msc *msc)
{
	int i;
	int	j;

	i = 0;
	while(++i < nb_args)
	{
		j = 0;
		while(strs[i][j] != '\0')
		{
			//printf("_%s_\n", &strs[i][j]);
			if((strs[i][j] >= '0' && strs[i][j] <= '9') \
				 || ((j == 0 && strs[i][j] == '+')))
					j++;
			else
				return (ft_error("forbidden char or negative value", msc), -1);
		}
	}
	parsed_to_variables(nb_args, msc, strs);
	if (msc->nb_philo == -1 || msc->time_to_die == 0 || msc->time_to_eat == 0 \
		|| msc->time_to_sleep == 0 || (nb_args == 6 && msc->nb_must_eat == 0))
		return(ft_error("forbidden zero", msc), -1);
	return (0);
}

void	parsed_to_variables(int nb_args, t_msc *msc, char **strs)
{
	msc->nb_philo = ft_atoi(strs[1]) - 1; //to synch overall and zero-index
	msc->time_to_die = ft_atoi(strs[2]);
	msc->time_to_eat = ft_atoi(strs[3]);
	msc->time_to_sleep = ft_atoi(strs[4]);
	if (nb_args == 6)
		msc->nb_must_eat = ft_atoi(strs[5]);
	else
		msc->nb_must_eat = -1;
	msc->stop_simulation = false;
}

int	init_mutex(t_msc *msc)
{
	int i;
	int j;

	i = -1;
	j = -1;
	if (pthread_mutex_init(&msc->mutex->print, NULL) != 0)
		return(-1);
	if (pthread_mutex_init(&msc->mutex->death, NULL) != 0)
		return(pthread_mutex_destroy(&msc->mutex->print), -1);
	if(pthread_mutex_init(&msc->mutex->meal_count, NULL) != 0)
		return(pthread_mutex_destroy(&msc->mutex->print), \
				pthread_mutex_destroy(&msc->mutex->death), -1);
	while(++i <= msc->nb_philo) //Correct nmb?
	{
		if (pthread_mutex_init(&msc->mutex->forks[i], NULL) != 0)
		{
			while(++j < i)
				pthread_mutex_destroy(&msc->mutex->forks[j]);
			return (pthread_mutex_destroy(&msc->mutex->print), \
				pthread_mutex_destroy(&msc->mutex->death), \
					pthread_mutex_destroy(&msc->mutex->meal_count), -1);
		}
	}
	msc->mutex_initialised = true;
	return (0);
}

int	init_philo(int i, t_msc *msc)
{
	msc->philo[i] = ft_calloc(sizeof(t_philo), 1);
	if(msc->philo[i] == NULL)
		return (-1);
	msc->philo[i]->nb_philo = i + 1; //array pos 0 ist philo 1
	msc->philo[i]->left_fork = i; //forkpos is 0
	if(i > 0)
		msc->philo[i - 1]->right_fork = &msc->philo[i]->left_fork;
	msc->philo[i]->meal_count = 0;
	msc->philo[i]->time_last_meal = 0;
	msc->philo[i]->dead = false;
	msc->philo[i]->msc = msc;
	printf("DEBUG 3 #FUNCTION:INIT_PHILO# i:%d\n", i);
	return (0);
}

int	init(t_msc *msc)
{
	int i;

	i = 0;
	msc->philo = ft_calloc(sizeof(t_philo *), msc->nb_philo); //ref #55 - correctl malloced the right amount; since it is an array?
	msc->mutex = ft_calloc(sizeof(t_mutex), 1);
	if (msc->philo == NULL || msc->mutex == NULL)
		return (ft_error("malloc failed", msc), -1);
	msc->mutex->forks = ft_calloc(sizeof(pthread_mutex_t), msc->nb_philo);
	if (msc->mutex->forks == NULL)
		return(ft_error("malloc failed", msc), -1);
	//printf("DEBUG 2 #FUNCTION:INIT#\n");
	while (i <= msc->nb_philo) //Correct nmb?
	{
		if (init_philo(i, msc) == -1)
			return(ft_error("malloc failed", msc), -1);
		i++;
		printf("DEBUG 4 #FUNCTION:INIT_PHILO# i:%d\n", i);
	}
	//printf("DEBUG 3 #FUNCTION:INIT#\n");
	msc->philo[msc->nb_philo]->right_fork = &msc->philo[0]->left_fork;
	if(init_mutex(msc) == -1)
		return(ft_error("mutex init failed", msc), -1);
	//printf("DEBUG 4 #FUNCTION:INIT#\n");
	return (0);
}

int main(int argc, char **argv)
{
	t_msc	*msc;
	
	msc = ft_calloc(sizeof(t_msc), 1);
	msc->mutex_initialised = false;
	if (msc == NULL)
		return (ft_error("malloc failed", msc), -1);
	if (argc < 5 || argc > 6)
		return (ft_error("wrong number of arguments", NULL), -1);
	if (parsing(argc, argv, msc) == -1)
		return (-1);
	//CHECKPOINT - WORKS TILL HERE
	if (init(msc) == -1)
		return (-1);
	if(simulation_startup(msc) == -1)
		return (-1);
	//printf("DEBUG _SUCCESS_ #end of FUNCTION MAIN#\n");
	ft_error("SUCCESS", msc);
	return (0);
}

/*
pthread_mutex_lock(&msc->mutex->forks[msc->philo->left_fork]
pthread_mutex_lock(&msc->mutex->forks[*(msc->philo->right_fork)]
//ADRESSE VON MUTEX
*/