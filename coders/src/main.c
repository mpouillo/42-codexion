/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 12:30:27 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/01 15:04:40 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

void	print_parameters(t_parameters *parameters)
{
	printf("===========================\n");
	printf("Number of coders:  %i\n", parameters->number_of_coders);
	printf("Time to burnout:   %i\n", parameters->time_to_burnout);
	printf("Time to compile:   %i\n", parameters->time_to_compile);
	printf("Time to debug:     %i\n", parameters->time_to_debug);
	printf("Time to refactor:  %i\n", parameters->time_to_refactor);
	printf("Compiles required: %i\n", parameters->number_of_compiles_required);
	printf("Dongles cooldown:  %i\n", parameters->dongles_cooldown);
	printf("Scheduler:         %i\n", parameters->scheduler);
	printf("===========================\n");
}

long long	get_current_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000LL + tv.tv_usec);
}

long long	get_delta_time(long long start_time)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000000LL + tv.tv_usec) - start_time);
}

void	compile(t_coder_data *coder_data)
{
	printf("%5i %i is compiling.\n", (int) (get_delta_time(coder_data->start_time_us) / 1000), coder_data->id);
	usleep(coder_data->parameters->time_to_compile * 1000);
}

void	debug(t_coder_data *coder_data)
{
	printf("%5i %i is debugging.\n", (int) (get_delta_time(coder_data->start_time_us) / 1000), coder_data->id);
	usleep(coder_data->parameters->time_to_debug * 1000);
}

void	refactor(t_coder_data *coder_data)
{
	printf("%5i %i is refactoring.\n", (int) (get_delta_time(coder_data->start_time_us) / 1000), coder_data->id);
	usleep(coder_data->parameters->time_to_refactor * 1000);
}

void	*vibe_coding(void *arg)
{
	t_coder_data	*coder_data;

	coder_data = (t_coder_data *) arg;
	compile(coder_data);
	debug(coder_data);
	refactor(coder_data);
	return (NULL);
}

t_coder_data	*create_coder_data(t_data *data, int id)
{
	t_coder_data	*coder_data;
	t_dongle		*dongle;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;

	dongle = (t_dongle *) malloc(sizeof(t_dongle));
	pthread_mutex_init(&mutex, NULL);
	dongle->mutex = &mutex;
	pthread_cond_init(&cond, NULL);
	dongle->cond = &cond;
	coder_data = (t_coder_data *) malloc(sizeof(t_coder_data));
	coder_data->parameters = data->parameters;
	coder_data->dongle = dongle;
	coder_data->id = id;
	coder_data->last_compile = -1;
	coder_data->nbr_compiles = 0;
	coder_data->start_time_us = data->start_time_us;
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
		if (status)
			printf("An error occurred joining coder %i (%i)\n", i, status);
		i++;
	}
}

t_data	*create_data(char **argv)
{
	t_data			*data;

	data = (t_data *) malloc(sizeof(t_data));
	data->start_time_us = get_current_time();
	data->parameters = get_parameters(argv);
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

	data = create_data(argv);
	join_threads(data);

	return (0);
}
