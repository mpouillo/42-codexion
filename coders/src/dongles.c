/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 10:37:52 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/08 12:19:55 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

// Destroys n dongle mutexes and returns 0.
int	destroy_dongles(t_sim *sim, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		while (sim->dongles[i].available == 0)
			usleep(10);
		sim->dongles[i].available = 0;
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		i++;
	}
	return (0);
}

// Fills dongle data with start values.
// Returns 1 on success or 0 on error.
static int	create_dongles(t_sim *sim)
{
	int		i;

	i = 0;
	while (i < sim->params->coders_nb)
	{
		sim->dongles[i].last_use = -1;
		sim->dongles[i].available = 1;
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL))
		{
			pthread_mutex_lock(&sim->sim_mutex);
			printf("Error creating dongles (%i).\n", i);
			pthread_mutex_unlock(&sim->sim_mutex);
			return (destroy_dongles(sim, i));
		}
		memset(sim->dongles[i].queue, 0, sizeof(int) * 2);
		i++;
	}
	return (1);
}

// Initializes dongle data.
// Returns 1 on success or 0 on error.
int	init_dongles(t_sim *sim)
{
	sim->dongles = malloc(sizeof(t_dongle) * sim->params->coders_nb);
	if (!sim->dongles)
		return (0);
	if (!create_dongles(sim))
	{
		free(sim->dongles);
		return (0);
	}
	return (1);
}
