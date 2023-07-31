/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburgler <mburgler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 19:22:35 by mburgler          #+#    #+#             */
/*   Updated: 2023/07/31 21:54:42 by mburgler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	simulation_startup(t_msc* msc)
{
	pthread_t *philo_thread;
	//pthread_t	philo_thread[4242];
	int	i;

	i = -1;
	philo_thread = ft_calloc(sizeof(pthread_t), msc->nb_philo);
	while(++i < msc->nb_philo)
	{
		msc->philo[i]->time_birth = sys_time();
		pthread_mutex_lock(&msc->mutex->death);
		msc->philo[i]->time_last_meal = msc->philo[i]->time_birth;
		pthread_mutex_unlock(&msc->mutex->death);
		//printf("DEBUG NO_PHILO#, no %d\n", msc->philo[i]->nb_philo);
		if (pthread_create(&philo_thread[i], NULL, \
			simulation_running, msc->philo[i]) != 0)
		{
			if(ft_pthread_join(i, philo_thread, msc) == -1)
				return (ft_error("pthread_create and pthread_join failed", msc), -1);
			else
				return (ft_error("pthread_create failed", msc), -1);
		}
	}
	simulation_shutdown(msc);
	//printf("*** BEFORE THREAD_JOIN\n");
	if(ft_pthread_join(i, philo_thread, msc) == -1)
	 	return (ft_error("pthread_join failed", msc), -1);
	printf("*** AFTER THREAD_JOIN\n");
	free_and_nullify((void **)&philo_thread); //correct?
	return (0);
}

void	simulation_shutdown(t_msc *msc)
{
	int i;
	bool all_ate;

	while(msc->stop_simulation == false)
	{
		all_ate = true;
		i = -1;
		while(++i < msc->nb_philo && msc->stop_simulation == false) // < or <= since 0-indexed
		{
			pthread_mutex_lock(&msc->mutex->death);
			if(sys_time() - msc->philo[i]->time_last_meal > msc->time_to_die)
			{
				msc->philo[i]->dead = true;
				pthread_mutex_unlock(&msc->mutex->death);
				msc->stop_simulation = true;
				ft_mutex_print_death(msc, msc->philo[i]);
				return ;
			}
			else
				pthread_mutex_unlock(&msc->mutex->death);
			pthread_mutex_lock(&msc->mutex->meal_count);
			if(msc->philo[i]->meal_count < msc->nb_must_eat || msc->nb_must_eat == -1)
				all_ate = false;
			pthread_mutex_unlock(&msc->mutex->meal_count);
			//usleep(1000);
			usleep(9000); //LEAVE THIS?
		}
		if(all_ate == true)
			msc->stop_simulation = true;
	}
}

void	*simulation_running(void *arg)
{
	t_philo *one_philo;

	if (!arg)
		return (NULL);
	one_philo = (t_philo *)arg;
	// if (one_philo->nb_philo % 2)
	//  	usleep(1000);
	while(one_philo->msc->stop_simulation == false)
	{
		/*if(one_philo->meal_count == 0) //NEW SECTION
			usleep(1000);*/
		philo_eats(one_philo, one_philo->msc);
		if(philo_sleeps(one_philo, one_philo->msc) == -1)
			return (NULL);
		ft_mutex_print(one_philo->msc, one_philo, "is thinking");
		if(one_philo->msc->stop_simulation == true)
			return (NULL);
	}
	return(NULL);
}

void	philo_eats(t_philo *one_philo, t_msc *msc)
{
	usleep(5000);
	if((one_philo->nb_philo) % 2)
	{
		pthread_mutex_lock(&msc->mutex->forks[one_philo->left_fork]);
		ft_mutex_print(msc, one_philo, "has taken his left fork");
		pthread_mutex_lock(&msc->mutex->forks[*one_philo->right_fork]);
		ft_mutex_print(msc, one_philo, "has taken his right fork");
	}
	else
	{
		//usleep(1000);
		//msc->time_to_eat * 1000);
		pthread_mutex_lock(&msc->mutex->forks[*one_philo->right_fork]);
		ft_mutex_print(msc, one_philo, "has taken his right fork");
		pthread_mutex_lock(&msc->mutex->forks[one_philo->left_fork]);
		ft_mutex_print(msc, one_philo, "has taken his left fork");
	}
	// pthread_mutex_lock(&msc->mutex->death);
	// one_philo->time_last_meal = sys_time();
	// pthread_mutex_unlock(&msc->mutex->death);
	ft_mutex_print(msc, one_philo, "is eating");
	pthread_mutex_lock(&msc->mutex->meal_count);
	one_philo->meal_count++;
	pthread_mutex_unlock(&msc->mutex->meal_count);
	//put from above down here - START
	pthread_mutex_lock(&msc->mutex->death);
	one_philo->time_last_meal = sys_time();
	pthread_mutex_unlock(&msc->mutex->death);
	//put from above down here - END
	usleep(msc->time_to_eat * 1000);
	pthread_mutex_unlock(&msc->mutex->forks[one_philo->left_fork]);
	pthread_mutex_unlock(&msc->mutex->forks[*one_philo->right_fork]);
}

int	philo_sleeps(t_philo *one_philo, t_msc *msc)
{

	if(msc->stop_simulation == true)
		return (-1);
	// pthread_mutex_lock(&msc->mutex->death);
	// if(time_now - one_philo->time_last_meal > msc->time_to_die)
	// {
	// 	one_philo->dead = true;
	// 	pthread_mutex_unlock(&msc->mutex->death);
	// 	msc->stop_simulation = true;
	// 	ft_mutex_print_death(msc, one_philo);		
	// 	return (-1);
	// }
	// else
	// 	pthread_mutex_unlock(&msc->mutex->death);
	ft_mutex_print(msc, one_philo, "is sleeping");
	usleep(msc->time_to_sleep * 1000);
	return(0);
}

/*WHAT SHOULD BE MONITORED?
- did all eat nb_must_eat - uneven, 3 sessions
- did one die
*/
