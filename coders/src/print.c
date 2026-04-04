/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 11:49:57 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/02 12:02:44 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

void	print_status(pthread_mutex_t *print_mutex, int status, int coder_id, int time)
{
	pthread_mutex_lock(print_mutex);
	if (status == COMPILING)
		printf("%i %i is compiling\n", time, coder_id);
	else if (status == DEBUGGING)
		printf("%i %i is debugging\n", time, coder_id);
	else if (status == REFACTORING)
		printf("%i %i is refactoring\n", time, coder_id);
	pthread_mutex_unlock(print_mutex);
}

void	print_dongle(pthread_mutex_t *print_mutex, int coder_id, int time)
{
	pthread_mutex_lock(print_mutex);
	printf("%i %i has taken a dongle\n", time, coder_id);
	pthread_mutex_unlock(print_mutex);
}
