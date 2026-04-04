/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 11:42:54 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/04 13:33:44 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

long long	get_time_since_start(void)
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
