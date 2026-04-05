/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 11:49:57 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/05 14:21:36 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

void	print_action(t_sim *sim, char *msg, int coder_id)
{
	int	is_running;

	pthread_mutex_lock(&sim->sim_mutex);
	is_running = sim->is_running;
	pthread_mutex_unlock(&sim->sim_mutex);
	if (is_running)
	{
		pthread_mutex_lock(&sim->print_mutex);
		printf("%lli %i is %s\n", get_time_since_start(), coder_id, msg);
		pthread_mutex_unlock(&sim->print_mutex);
	}
}

void	print_dongle(t_sim *sim, int coder_id)
{
	pthread_mutex_lock(&sim->print_mutex);
	printf("%lli %i has taken a dongle\n", get_time_since_start(), coder_id);
	pthread_mutex_unlock(&sim->print_mutex);
}
