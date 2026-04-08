/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 12:31:57 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/08 12:09:18 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

// Returns 1 if all coders are done compiling, else 0.
static int	done_compiling(t_sim *sim)
{
	int			i;

	pthread_mutex_lock(&sim->sim_mutex);
	i = 0;
	while (i < sim->params->coders_nb)
	{
		if (sim->coders[i].compile_count < sim->params->req_compiles_nb)
			break ;
		i++;
	}
	if (i != sim->params->coders_nb)
	{
		pthread_mutex_unlock(&sim->sim_mutex);
		return (0);
	}
	sim->is_running = 0;
	pthread_mutex_unlock(&sim->sim_mutex);
	return (1);
}

// Returns 1 if a corder burned out, else 0.
static int	burned_out(t_sim *sim)
{
	long long	time;
	int			i;

	pthread_mutex_lock(&sim->sim_mutex);
	time = get_timestamp();
	i = 0;
	while (i < sim->params->coders_nb)
	{
		if (time - sim->coders[i].last_compile > sim->params->burnout_t)
		{
			printf("%lli %i burned out\n", time, i + 1);
			sim->is_running = 0;
			pthread_mutex_unlock(&sim->sim_mutex);
			return (1);
		}
		i++;
	}
	pthread_mutex_unlock(&sim->sim_mutex);
	return (0);
}

// Entry point of monitor thread.
void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *) arg;
	while (!sim_running(sim))
		usleep(100);
	if (sim->is_running == 2)
		return (NULL);
	while (1)
	{
		if (burned_out(sim))
			break ;
		if (done_compiling(sim))
			break ;
		usleep(10);
	}
	return (NULL);
}
