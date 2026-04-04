/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 12:30:36 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/02 14:34:52 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

// Returns a new t_queue item
t_queue	*q_create(void)
{
	t_queue *q;
	pthread_mutex_t	mutex;

	q = (t_queue *) malloc(sizeof(t_queue));
	q->mutex = &mutex;
	q->current_size = 0;
	return (q);
}

// Tries to enqueue a coder_id
// Returns TRUE on success or FALSE if queue is full
int	q_enqueue(t_queue *q, int coder_id)
{
	if (!q || q->current_size == 2)
		return (FALSE);
	pthread_mutex_lock(q->mutex);
	q->array[q->current_size] = coder_id;
	q->current_size++;
	pthread_mutex_unlock(q->mutex);
	return (TRUE);
}

// Returns a coder_id depending on the scheduler, or FALSE (-1) if queue is empty
int	q_dequeue(t_queue *q, t_data *data, t_scheduler scheduler)
{
	int coder_id;

	if (!q || q->current_size == 0)
		return (FALSE);
	pthread_mutex_lock(q->mutex);
	if (q->current_size == 1)
	{
		q->current_size = 0;
		return (q->array[0]);
	}
	if (scheduler == fifo)
	{
		coder_id = q->array[0];
		q->array[0] = q->array[1];
		q->current_size--;
		return (coder_id);
	}
	else
	{
		if (data->coders[q->array[0]]->last_compile <= data->coders[q->array[1]]->last_compile)
		{
			coder_id = q->array[0];
			q->array[0] = q->array[1];
			q->current_size--;
			return (coder_id);
		}
		else
		{
			coder_id = q->array[1];
			q->current_size--;
			return (coder_id);
		}
	}
	pthread_mutex_unlock(q->mutex);
}

void	q_destroy(t_queue *q)
{
	free(q);
}
