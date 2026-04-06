/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 14:11:10 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/06 15:35:48 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

// Add coder_id to dongle queue.
void	queue_push(t_dongle *dongle, int coder_id)
{
	int	*q;

	q = dongle->queue;
	if (!q[0])
		q[0] = coder_id;
	else if (!q[1] && q[0] != coder_id)
		q[1] = coder_id;
}

// Pops coder_id from queue and returns 1 on success or 0 on failure.
int	queue_pop(t_dongle *dongle, int coder_id)
{
	int	*q;

	q = dongle->queue;
	if (q[0] == coder_id)
	{
		q[0] = q[1];
		q[1] = 0;
		return (1);
	}
	else if (q[1] == coder_id)
	{
		q[1] = 0;
		return (1);
	}
	else
		return (0);
}

// Returns 1 if coder_id is next in line to acquire a dongle.
int	is_next(t_sim *sim, t_dongle *dongle, int coder_id)
{
	int	*q;

	q = dongle->queue;
	if (!dongle->queue[1])
		return (q[0] == coder_id);
	if (sim->params->scheduler == EDF)
	{
		if (sim->coders[q[0] - 1].last_compile > \
sim->coders[q[1] - 1].last_compile)
			return (q[1] == coder_id);
	}
	return (q[0] == coder_id);
}
