/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 12:30:27 by mpouillo          #+#    #+#             */
/*   Updated: 2026/03/28 16:36:04 by mpouillo         ###   ########.fr       */
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

void	compile(t_variables *variables)
{
	usleep(variables->time_to_compile * 1000);
	printf("A coder finished compiling.\n");
}

void	debug(t_variables *variables)
{
	usleep(variables->time_to_debug * 1000);
	printf("A coder finished debugging.\n");
}

void	refactor(t_variables *variables)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	printf("Current time: %li\n", tv.tv_usec);
	usleep(variables->time_to_refactor * 1000);
	printf("A coder finished refactoring.\n");
}

void	*vibe_coding(void *arg)
{
	t_variables	*variables;

	variables = (t_variables*) arg;
	compile(variables);
	debug(variables);
	refactor(variables);
	return (NULL);
}

t_coder	**create_coders(t_variables *variables)
{
	t_coder	**coders;
	t_coder	*coder;
	int			i;
	int			status;

	coders = (t_coder **) malloc(sizeof(t_coder *) * variables->number_of_coders);
	if (!coders)
		return (NULL);
	i = 0;
	while (i < variables->number_of_coders)
	{
		coder = (t_coder *) malloc(sizeof(t_coder));
		if (!coder)
		{
			free(coders);
			return (NULL);
		}
		status = pthread_create(&coder->thread, NULL, vibe_coding, variables);
		if (status)
		{
			free(coders);
			return (NULL);
		}
		coder->id = i + 1;
		coder->last_compile = 0; // TEMP
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

	variables = (t_variables *) malloc(sizeof(t_variables));
	if (!variables)
		return (NULL);
	fill_variables(variables, argv);

	dongles = (t_dongle *) malloc(sizeof(t_dongle));
	if (!dongles)
		return (NULL);

	coders = create_coders(variables);
	if (!coders)
		return (NULL);

	data = (t_data *) malloc(sizeof(t_data));
	data->coders = coders;
	data->dongles = dongles;
	data->variables = variables;
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
	free(data->coders);
	free(data->dongles);
	free(data->variables);
	free(data);
	return (0);
}
