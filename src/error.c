/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburgler <mburgler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 17:08:58 by mburgler          #+#    #+#             */
/*   Updated: 2023/06/02 17:38:13 by mburgler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void    ft_error(char *str, t_msc *msc)
{
    printf("Error: %s\n", str); //write to stderr
    (void)msc; //change this
    exit(1); //kill exit since not allowed
}