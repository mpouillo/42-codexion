/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 13:05:10 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/04 13:30:30 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

t_params	*get_parameters(char **argv)
{
	t_params	*params;

	params = (t_params *) malloc(sizeof(t_params));
	params->number_of_coders = atoi(argv[1]);
	params->time_to_burnout = atoi(argv[2]);
	params->time_to_compile = atoi(argv[3]);
	params->time_to_debug = atoi(argv[4]);
	params->time_to_refactor = atoi(argv[5]);
	params->number_of_compiles_required = atoi(argv[6]);
	params->dongles_cooldown = atoi(argv[7]);
	params->scheduler = argv[8];
	return (params);
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
