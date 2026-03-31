/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 12:30:27 by mpouillo          #+#    #+#             */
/*   Updated: 2026/03/31 13:06:20 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

void	print_variables(t_variables *variables)
{
	printf("===========================\n");
	printf("Number of coders:  %i\n", variables->number_of_coders);
	printf("Time to burnout:   %i\n", variables->time_to_burnout);
	printf("Time to compile:   %i\n", variables->time_to_compile);
	printf("Time to debug:     %i\n", variables->time_to_debug);
	printf("Time to refactor:  %i\n", variables->time_to_refactor);
	printf("Compiles required: %i\n", variables->number_of_compiles_required);
	printf("Dongles cooldown:  %i\n", variables->dongles_cooldown);
	printf("Scheduler:         %s\n", variables->scheduler);
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

void	compile(t_data *data)
{
	usleep(data->variables->time_to_compile * 1000);
	printf("%lli - A coder finished compiling.\n", get_current_time() );
}

void	debug(t_data *data)
{
	usleep(data->variables->time_to_debug * 1000);
	printf("%lli - A coder finished debugging.\n", get_current_time());
}

void	refactor(t_data *data)
{
	usleep(data->variables->time_to_refactor * 1000);
	printf("%lli - A coder finished refactoring.\n", get_current_time());
}

void	*vibe_coding(void *arg)
{
	t_data	*data;

	data = (t_data*) arg;
	compile(data);
	debug(data);
	refactor(data);
	return (NULL);
}

t_coder	**create_coders(t_data* data)
{
	t_coder	**coders;
	t_coder	*coder;
	int			i;
	int			status;

	coders = (t_coder **) malloc(sizeof(t_coder *) * data->variables->number_of_coders);
	if (!coders)
		return (NULL);
	i = 0;
	while (i < data->variables->number_of_coders)
	{
		coder = (t_coder *) malloc(sizeof(t_coder));
		if (!coder)
		{
			free(coders);
			return (NULL);
		}
		status = pthread_create(&coder->thread, NULL, vibe_coding, data);
		if (status)
		{
			free(coders);
			return (NULL);
		}
		coder->id = i + 1;
		coder->last_compile = get_current_time();
		coders[i] = coder;
		i++;
	}
	return (coders);
}

void	join_threads(t_coder **coders, t_variables *variables)
{
	int	i;
	int	status;

	i = 0;
	while (i < variables->number_of_coders)
	{
		status = pthread_join(coders[i]->thread, NULL);
		if (status)
			printf("An error occurred joining coder %i\n", i);
		i++;
	}
}

t_data	*create_data(char **argv)
{
	t_variables	*variables;
	t_dongle	*dongles;
	t_coder		**coders;
	t_data		*data;

	data = (t_data *) malloc(sizeof(t_data));
	data->start_time_us = get_current_time();
	printf("Current time: %llims\n", data->start_time_us);

	variables = (t_variables *) malloc(sizeof(t_variables));
	fill_variables(variables, argv);
	data->variables = variables;

	dongles = (t_dongle *) malloc(sizeof(t_dongle));
	data->dongles = dongles;

	coders = create_coders(data);
	data->coders = coders;

	return (data);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	if (argc != 9 || !validate_arguments(argv))
	{
		printf("Invalid arguments. Can you read?\n");
		return (1);
	}
	data = create_data(argv);
	if (!data)
	{
		printf("Error initializing data. What the heck happened there?\n");
		return (1);
	}
	join_threads(data->coders, data->variables);
	printf("Time elapsed: %llims\n", get_delta_time(data->start_time_us) / 1000);
	free(data->coders);
	free(data->dongles);
	free(data->variables);
	free(data);
	return (0);
}
