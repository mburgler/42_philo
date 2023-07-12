/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburgler <mburgler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:52:15 by mburgler          #+#    #+#             */
/*   Updated: 2023/07/12 21:19:00 by mburgler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

//PROTOTYPE: nb_philo; time_to_die; time_to_eat; time_to_sleep; 
//[number_of_times_each_philosopher_must_eat]

int	parsing(int nb_args, char **strs, t_msc *msc)
{
	int i;
	int	j;

	i = 0;
	j = 0;
	while(++i < nb_args)
	{
		while(strs[i][j] != '\0')
		{
			//printf("_%s_\n", &strs[i][j]);
			if((strs[i][j] >= '0' && strs[i][j] <= '9') \
				 || ((j == 0 && strs[i][j] == '+')))
					j++;
			else
				return (ft_error("forbidden char or negative value", msc), -1);
		}
		j = 0;
		//printf("###\n");
	}
	//printf("done\n"); //working till here
	msc->nb_philo = ft_atoi(strs[1]) - 1; //to synch overall and zero index
	msc->time_to_die = ft_atoi(strs[2]);
	msc->time_to_eat = ft_atoi(strs[3]);
	msc->time_to_sleep = ft_atoi(strs[4]);
	if (nb_args == 6)
		msc->nb_must_eat = ft_atoi(strs[5]);
	if (msc->nb_philo == 0 || msc->time_to_die == 0 || msc->time_to_eat == 0 \
		|| msc->time_to_sleep == 0 || (nb_args == 6 && msc->nb_must_eat == 0))
		return(ft_error("forbidden zero", msc), -1);
	//printf("DEBUG #%d\n", msc->time_to_sleep);
	return (0);
}

int	init_philo(int i, t_msc *msc)
{
	msc->philo[i] = ft_calloc(sizeof(t_philo), 1);
	if(msc->philo[i] == NULL)
		return (-1);
	msc->philo[i]->nb_philo = i + 1;
	msc->philo[i]->left_fork = i;
	if(i > 1)
		msc->philo[i - 1]->right_fork = &msc->philo[i]->left_fork;
	msc->philo[i]->meal_count = 0;
	msc->philo[i]->last_meal = 0;
	msc->philo[i]->dead = false;
	return (0);
}

int	init(t_msc *msc)
{
	int i;

	i = 0;
	//printf("DEBUG #FUNCTION:INIT#; var no_philo %d\n", msc->nb_philo);
	msc->philo = ft_calloc(sizeof(t_philo *) * msc->nb_philo, 1); //ref #55 - correctl malloced the right amount; since it is an array?
	if (msc->philo == NULL)
		return (ft_error("malloc failed", msc), -1);
	//printf("DEBUG 2 #FUNCTION:INIT#\n");
	while (i <= msc->nb_philo) //Correct nmb?
	{
		init_philo(i, msc);
		if (init_philo(i, msc) == -1)
			return (ft_error("malloc failed", msc), -1);
		i++;
	}
	msc->philo[msc->nb_philo - 1]->right_fork = &msc->philo[0]->left_fork;
	return (0);
}

int main(int argc, char **argv)
{
	t_msc	*msc;
	
	msc = ft_calloc(sizeof(t_msc), 1);
	if (msc == NULL)
		return (ft_error("malloc failed", msc), -1);
	if (argc < 5 || argc > 6)
		return (ft_error("wrong number of arguments", NULL), -1);
	if (parsing(argc, argv, msc) == -1)
		return (-1);
	if (init(msc) == -1)
		return (-1);
	return (0);
}
