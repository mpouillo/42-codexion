/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 11:42:54 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/08 13:18:37 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

long long	get_timestamp(void)
{
	static long long	start_time = -1;
	struct timeval		tv;
	long long			current_time;

	if (gettimeofday(&tv, NULL))
		return (-1);
	current_time = (long long) tv.tv_sec * 1000 + tv.tv_usec / 1000;
	if (start_time == -1)
		start_time = current_time;
	return (current_time - start_time);
}

void	sleep_thread(t_sim *sim, long long sleep_time)
{
	long long	start;

	start = get_timestamp();
	while (sim_running(sim) == 1 && get_timestamp() - start <= sleep_time)
		usleep(100);
}
