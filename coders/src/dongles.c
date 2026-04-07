/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 10:37:52 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/07 16:41:12 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

int	is_on_cd(t_dongle *dongle, int cooldown)
{
	long long	time;

	time = get_timestamp();
	if (dongle->last_use == -1)
		return (0);
	return (time - dongle->last_use < cooldown);
}

// Try to acquire both dongles, returning 1 on success or 0 on failure.
int	acquire_dongles(t_coder *coder)
{
	int	cd;

	cd = coder->sim->params->dongle_cd;
	pthread_mutex_lock(&coder->sim->sim_mutex);
	if (!coder->sim->is_running || \
is_on_cd(coder->dongle_1, cd) || is_on_cd(coder->dongle_2, cd))
		return (pthread_mutex_unlock(&coder->sim->sim_mutex));
	queue_push(coder->dongle_1, coder->id);
	queue_push(coder->dongle_2, coder->id);
	if (coder->dongle_1->available && coder->dongle_2->available && \
is_next(coder->sim, coder->dongle_1, coder->id) && coder->dongle_1 !=\
coder->dongle_2 && is_next(coder->sim, coder->dongle_2, coder->id))
	{
		queue_pop(coder->dongle_1, coder->id);
		queue_pop(coder->dongle_2, coder->id);
		pthread_mutex_lock(&coder->dongle_1->mutex);
		coder->dongle_1->available = 0;
		printf("%lli %i has taken a dongle\n", get_timestamp(), coder->id);
		pthread_mutex_lock(&coder->dongle_2->mutex);
		coder->dongle_2->available = 0;
		printf("%lli %i has taken a dongle\n", get_timestamp(), coder->id);
		pthread_mutex_unlock(&coder->sim->sim_mutex);
		return (1);
	}
	return (pthread_mutex_unlock(&coder->sim->sim_mutex));
}

// Release both dongles, setting them as available.
void	release_dongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->sim_mutex);
	pthread_mutex_unlock(&coder->dongle_1->mutex);
	coder->dongle_1->last_use = get_timestamp();
	coder->dongle_1->available = 1;
	pthread_mutex_unlock(&coder->dongle_2->mutex);
	coder->dongle_2->last_use = get_timestamp();
	coder->dongle_2->available = 1;
	pthread_mutex_unlock(&coder->sim->sim_mutex);
}

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
