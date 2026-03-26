/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 12:30:27 by mpouillo          #+#    #+#             */
/*   Updated: 2026/03/26 14:05:47 by mpouillo         ###   ########.fr       */
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
	printf("Compiles required: %i\n", variables->number_of_compiles_required);
	printf("Dongles cooldown:  %i\n", variables->dongles_cooldown);
	printf("Scheduler:         %s\n", variables->scheduler);
	printf("===========================\n");
}

void	*do_smth(void *arg)
{
	(void) arg;
	printf("Created coder\n");
	return (NULL);
}

pthread_t	*create_threads(t_variables *variables)
{
	pthread_t	*coders;
	int			i;
	int			status;

	coders = (pthread_t *) malloc(sizeof(pthread_t)
			* variables->number_of_coders);
	if (!coders)
		return (NULL);
	i = 0;
	while (i < variables->number_of_coders)
	{
		status = pthread_create(&coders[i], NULL, do_smth, variables);
		if (status)
		{
			free(coders);
			return (NULL);
		}
		i++;
	}
	return (coders);
}

void	join_threads(pthread_t *threads, t_variables *variables)
{
	int	i;
	int	status;

	i = 0;
	while (i < variables->number_of_coders)
	{
		status = pthread_join(threads[i], NULL);
		if (status)
			printf("An error occurred joining coder %i\n", i);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_variables	*variables;
	pthread_t	*coders;

	if (argc != 9 || !validate_arguments(argv))
	{
		printf("Invalid arguments.\n");
		return (1);
	}
	variables = (t_variables *) malloc(sizeof(t_variables));
	fill_variables(variables, argv);
	print_variables(variables);
	coders = create_threads(variables);
	if (!coders)
		printf("Error while creating coders\n");
	join_threads(coders, variables);
	free(coders);
	free(variables);
	return (0);
}
