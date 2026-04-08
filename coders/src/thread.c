/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 11:16:56 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/08 12:15:19 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

// Joins all monitor and coder threads.
// Returns 1 on success or 0 on error.
int	join_threads(t_sim *sim)
{
	int	i;
	int	status;
	int	error;

	error = 0;
	i = 0;
	while (i < sim->params->coders_nb)
	{
		status = pthread_join(sim->coders[i].thread, NULL);
		if (status)
		{
			print_msg(sim, "Error joining coder thread");
			error = 1;
		}
		i++;
	}
	status = pthread_join(sim->monitor, NULL);
	if (status)
	{
		print_msg(sim, "Error joining monitor thread");
		error = 1;
	}
	return (error == 0);
}

// Sets sim status to 2 and waits for threads to exit.
static int	destroy_created_threads(t_sim *sim, int n)
{
	int	i;

	pthread_mutex_lock(&sim->sim_mutex);
	sim->is_running = 2;
	pthread_mutex_unlock(&sim->sim_mutex);
	i = 0;
	while (i < n)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor, NULL);
	return (0);
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
	if (i != sim->params->coders_nb)
		return (destroy_created_threads(sim, i));
	pthread_mutex_lock(&sim->sim_mutex);
	sim->is_running = 1;
	pthread_mutex_unlock(&sim->sim_mutex);
	return (1);
}
