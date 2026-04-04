/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 11:40:14 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/04 14:36:57 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

t_dongle	**create_dongle_array(t_parameters *parameters)
{
	int				i;
	t_dongle		**dongle_array;
	t_dongle		*dongle;
	pthread_mutex_t	*mutex;
	pthread_cond_t	*cond;

	dongle_array = (t_dongle **) malloc(sizeof(t_dongle) \
							* parameters->number_of_coders);

	i = 0;
	while (i < parameters->number_of_coders)
	{
		dongle = (t_dongle *) malloc(sizeof(t_dongle));
		mutex = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(mutex, NULL);
		dongle->mutex = mutex;
		cond = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
		pthread_cond_init(cond, NULL);
		dongle->cond = cond;
		dongle->cooldown_ms = parameters->dongles_cooldown;
		dongle->queue = q_create();
		dongle_array[i] = dongle;
		i++;
	}
	return (dongle_array);
}

t_coder_data	*create_coder_data(t_data *data, int id)
{
	t_coder_data	*coder_data;

	coder_data = (t_coder_data *) malloc(sizeof(t_coder_data));
	coder_data->data = data;
	coder_data->id = id;
	coder_data->last_compile = -1;
	coder_data->nbr_compiles = 0;
	coder_data->dongle_1 = data->dongle_array[id];
	coder_data->dongle_2 = data->dongle_array[(id + 1) % data->parameters->number_of_coders];
	return (coder_data);
}

t_coder_data	**create_coder_data_array(t_data *data)
{
	t_coder_data	**coder_data_array;
	int	i;

	coder_data_array = (t_coder_data **) malloc(sizeof(t_coder_data) \
							* data->parameters->number_of_coders);

	i = 0;
	while (i < data->parameters->number_of_coders)
	{
		coder_data_array[i] = create_coder_data(data, i);
		i++;
	}
	return (coder_data_array);
}

t_coder	*create_coder(t_coder_data *coder_data)
{
	t_coder		*coder;
	pthread_t	thread;

	coder = (t_coder *) malloc(sizeof(t_coder));
	coder->coder_data = coder_data;
	pthread_create(&thread, NULL, vibe_coding, (void *) coder_data);
	coder->thread = thread;
	coder->last_compile = 0;
	return (coder);
}

t_coder	**create_coder_array(t_parameters *parameters, t_coder_data **coder_data_array)
{
	t_coder	**coder_array;
	int		i;

	coder_array = (t_coder **) malloc(sizeof(t_coder) * parameters->number_of_coders);

	i = 0;
	while (i < parameters->number_of_coders)
	{
		coder_array[i] = create_coder(coder_data_array[i]);
		i++;
	}
	return (coder_array);
}

t_coder	**init_coders(t_data *data)
{
	t_coder_data	**coder_data_array;
	t_coder			**coder_array;

	coder_data_array = create_coder_data_array(data);
	coder_array = create_coder_array(data->parameters, coder_data_array);
	return (coder_array);
}

void	join_threads(t_data *data)
{
	int	i;
	int	status;

	i = 0;
	while (i < data->parameters->number_of_coders)
	{
		status = pthread_join(data->coders[i]->thread, NULL);
		i++;
	}
}

t_data	*create_data(char **argv)
{
	t_data			*data;
	pthread_mutex_t	*print_mutex;

	data = (t_data *) malloc(sizeof(t_data));
	data->parameters = get_parameters(argv);
	data->dongle_array = create_dongle_array(data->parameters);
	print_mutex = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
	data->print_mutex = print_mutex;
	data->coders = init_coders(data);
	return (data);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	if (argc != 9 || !validate_arguments(argv))
	{
		printf("Invalid arguments.\n");
		return (1);
	}

	get_time_since_start();
	data = create_data(argv);
	join_threads(data);

	return (0);
}
