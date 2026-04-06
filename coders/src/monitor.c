/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 12:31:57 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/06 16:05:57 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

int	done_compiling(t_sim *sim)
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
	if (i == sim->params->coders_nb)
	{
		sim->is_running = 0;
		pthread_mutex_unlock(&sim->sim_mutex);
		return (1);
	}
	pthread_mutex_unlock(&sim->sim_mutex);
	return (0);
}

int	burned_out(t_sim *sim)
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

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *) arg;
	while (!sim_running(sim))
		usleep(100);
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
