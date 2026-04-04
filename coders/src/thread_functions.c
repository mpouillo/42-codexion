/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 11:40:37 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/04 12:39:48 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

static void	compile(t_coder_data *coder_data)
{
	pthread_mutex_t	*dongle1;
	pthread_mutex_t	*dongle2;

	dongle1 = coder_data->dongle_1->mutex;
	dongle2 = coder_data->dongle_2->mutex;

	pthread_mutex_lock(dongle1);
	pthread_mutex_lock(dongle2);

	print_status(coder_data->data->print_mutex, COMPILING, coder_data->id, get_delta_time(coder_data->data->start_time_us) / 1000);
	usleep(coder_data->data->parameters->time_to_compile * 1000);
	coder_data->last_compile = get_current_time();

	pthread_mutex_unlock(dongle1);
	pthread_mutex_unlock(dongle2);

}

static void	debug(t_coder_data *coder_data)
{
	print_status(coder_data->data->print_mutex, DEBUGGING, coder_data->id, get_delta_time(coder_data->data->start_time_us) / 1000);
	usleep(coder_data->data->parameters->time_to_debug * 1000);
}

static void	refactor(t_coder_data *coder_data)
{
	print_status(coder_data->data->print_mutex, REFACTORING, coder_data->id, get_delta_time(coder_data->data->start_time_us) / 1000);
	usleep(coder_data->data->parameters->time_to_refactor * 1000);
}

void	*vibe_coding(void *arg)
{
	t_coder_data	*coder_data;
	int				i;

	coder_data = (t_coder_data *) arg;
	i = 0;
	while (i < coder_data->data->parameters->number_of_compiles_required)
	{
		get_mutexes(coder_data->dongle_1, coder_data->dongle_2);
		compile(coder_data);
		return_mutexes(coder_data->dongle_1, coder_data->dongle_2);
		debug(coder_data);
		refactor(coder_data);
		coder_data->nbr_compiles++;
		i++;
	}
	printf("%i finished work!\n", coder_data->id);
	return (NULL);
}






void	*do_work(void *arg)
{
	t_coder	*coder;
	t_dongle	dongle_1;
	t_dongle	dongle_2;
	int		i;

	coder = (t_coder *) arg;
	i = 0;
	while(coder->nbr_compiles < coder->data->params->number_of_compiles_required)
	{
		while (!(mutex_1 && mutex2))
		{
			// return NULL if cant get mutex
			mutex_1 = get_dongle_from_queue(dongle_1, thread_id)
			mutex_2 = get_dongle_from_queue(dongle_2, thread_id)
		}
		pthread)
		compile()
		give_mutex_back(mutex, get_current_time())
		debug()
		refactor()
		coder->nbr_compiles++;
	}
}

get_mutex_from_queue(mutex_ptr, thread_id)
{
	if (get_current_time() - last_mutex_use < params->dongle_cooldown)
		return (NULL)

	mutex = get_mutex_if_next_up_else_null(thread_id)
	if (mutex)
	{
		print
		return ()
	}
	else
		return (NULL);
}
