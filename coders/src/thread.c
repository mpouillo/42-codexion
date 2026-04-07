/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 11:16:56 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/07 14:19:44 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

// Joins all monitor and coder threads.
// Returns 1 on success or 0 on error.
int	join_threads(t_sim *sim)
{
	int	i;
	int	status;

	i = 0;
	while (i < sim->params->coders_nb)
	{
		status = pthread_join(sim->coders[i].thread, NULL);
		if (status)
		{
			pthread_mutex_lock(&sim->sim_mutex);
			printf("Error joining thread %i (%i)\n", i + 1, status);
			sim->is_running = 0;
			return (pthread_mutex_unlock(&sim->sim_mutex));
		}
		i++;
	}
	status = pthread_join(sim->monitor, NULL);
	if (status)
	{
		pthread_mutex_lock(&sim->sim_mutex);
		printf("Error joining monitor thread (%i)\n", status);
		sim->is_running = 0;
		return (pthread_mutex_unlock(&sim->sim_mutex));
	}
	return (1);
}

// Creates monitor and coder threads.
// Returns 1 on success or 0 on error.
int	start_threads(t_sim *sim)
{
	int	i;
	int	status;

	status = pthread_create(&sim->monitor, NULL, monitor_routine, (void *) sim);
	if (status)
		return (0);
	i = 0;
	while (i < sim->params->coders_nb)
	{
		status = pthread_create(&sim->coders[i].thread, NULL, \
coder_routine, (void *) &sim->coders[i]);
		if (status)
			break ;
		i++;
	}
	pthread_mutex_lock(&sim->sim_mutex);
	if (i != sim->params->coders_nb)
	{
		sim->is_running = 0;
		return (pthread_mutex_unlock(&sim->sim_mutex));
	}
	sim->is_running = 1;
	pthread_mutex_unlock(&sim->sim_mutex);
	return (1);
}
