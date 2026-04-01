/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 13:05:10 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/01 14:52:55 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

t_parameters	*get_parameters(char **argv)
{
	t_parameters	*parameters;

	parameters = (t_parameters *) malloc(sizeof(t_parameters));
	parameters->number_of_coders = atoi(argv[1]);
	parameters->time_to_burnout = atoi(argv[2]);
	parameters->time_to_compile = atoi(argv[3]);
	parameters->time_to_debug = atoi(argv[4]);
	parameters->time_to_refactor = atoi(argv[5]);
	parameters->number_of_compiles_required = atoi(argv[6]);
	parameters->dongles_cooldown = atoi(argv[7]);
	if (strcmp(argv[8], "fifo") == 0)
		parameters->scheduler = fifo;
	else
		parameters->scheduler = edf;
	return (parameters);
}

static int	int_validation(const char *nptr)
{
	size_t	i;
	long	res;

	res = 0;
	i = 0;
	while (nptr[i])
	{
		if (!(nptr[i] >= '0' && nptr[i] <= '9'))
			return (FALSE);
		res = (res * 10) + nptr[i] - '0';
		if (res > INT_MAX || res < INT_MIN)
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	validate_arguments(char **argv)
{
	int	i;

	i = 1;
	while (i < 8)
	{
		if (!int_validation(argv[i]))
			return (FALSE);
		i++;
	}
	if ((strcmp(argv[8], "fifo") != 0) && (strcmp(argv[8], "edf") != 0))
		return (FALSE);
	return (TRUE);
}
