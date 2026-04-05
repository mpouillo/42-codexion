/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 12:31:57 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/05 13:37:35 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *) arg;
	pthread_mutex_lock(&sim->print_mutex);
	printf("Monitor thread started\n");
	pthread_mutex_unlock(&sim->print_mutex);
	return (NULL);
}
