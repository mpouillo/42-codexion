/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 13:05:10 by mpouillo          #+#    #+#             */
/*   Updated: 2026/03/26 13:14:55 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

void	fill_variables(t_variables *variables, char **argv)
{
	variables->number_of_coders = atoi(argv[1]);
	variables->time_to_burnout = atoi(argv[2]);
	variables->time_to_compile = atoi(argv[3]);
	variables->time_to_debug = atoi(argv[4]);
	variables->time_to_refactor = atoi(argv[5]);
	variables->number_of_compiles_required = atoi(argv[6]);
	variables->dongles_cooldown = atoi(argv[7]);
	variables->scheduler = argv[8];
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
