/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 10:37:52 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/05 12:59:47 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

int	destroy_dongles(t_sim *sim, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		sim->dongles[i].available = 0;
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		i++;
	}
	return (0);
}

static int	create_dongles(t_sim *sim)
{
	int		i;

	i = 0;
	while (i < sim->params->coders_nb)
	{
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL))
		{
			pthread_mutex_lock(&sim->print_mutex);
			printf("Error creating dongles (%i).\n", i);
			pthread_mutex_unlock(&sim->print_mutex);
			return (destroy_dongles(sim, i));
		}
		sim->dongles[i].last_use = 0;
		sim->dongles[i].available = 1;
		i++;
	}
	return (1);
}

int	init_dongles(t_sim *sim)
{
	sim->dongles = malloc(sizeof(t_dongle) * sim->params->coders_nb);
	if (!sim->dongles)
		return (0);
	if (!create_dongles(sim))
		return (0);
	return (1);
}
