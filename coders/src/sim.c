/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 11:15:19 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/06 12:08:54 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

int	stop_sim(t_sim *sim)
{
	pthread_mutex_lock(&sim->sim_mutex);
	sim->is_running = 0;
	pthread_mutex_unlock(&sim->sim_mutex);
	return (0);
}

int	sim_running(t_sim *sim)
{
	int	running;

	pthread_mutex_lock(&sim->sim_mutex);
	running = sim->is_running;
	pthread_mutex_unlock(&sim->sim_mutex);
	return (running == 1);
}

static int	init_sim(t_sim *sim, t_params *params)
{
	sim->params = params;
	sim->coders = NULL;
	sim->dongles = NULL;
	sim->monitor = 0;
	sim->is_running = 0;
	if (pthread_mutex_init(&sim->sim_mutex, NULL))
		return (0);
	if (!init_dongles(sim))
		return (0);
	if (!init_coders(sim))
		return (0);
	return (1);
}

// Runs the sim, returns 0 on success and 1 on error
int	run_sim(char **argv)
{
	t_params	params;
	t_sim		sim;

	if (!parse_params(&params, argv))
		return (1);
	if (!init_sim(&sim, &params))
		return (1);
	if (!start_threads(&sim))
		return (1);
	if (!join_threads(&sim))
		return (1);
	return (0);
}
