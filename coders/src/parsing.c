/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 13:05:10 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/05 12:56:06 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

// Returns 1 if string *nptr is a valid positive integer, else 0.
static int	int_validation(const char *nptr)
{
	size_t	i;
	long	res;

	res = 0;
	i = 0;
	while (nptr[i])
	{
		if (!(nptr[i] >= '0' && nptr[i] <= '9'))
			return (0);
		res = (res * 10) + nptr[i] - '0';
		if (res > INT_MAX || res < INT_MIN)
			return (0);
		i++;
	}
	return (1);
}

// Returns 1 if argv[1 - 8] are valid, else 0.
static int	validate_args(char **argv)
{
	int	i;

	i = 1;
	while (i < 8)
	{
		if (!int_validation(argv[i]))
			return (0);
		if (i == 1 && atoi(argv[i]) == 0)
			return (0);
		i++;
	}
	if ((strcmp(argv[8], "fifo") != 0) && (strcmp(argv[8], "edf") != 0))
		return (0);
	return (1);
}

// Checks params and returns parsed struct, or 1 on error.
t_params	*parse_params(t_params *params, char **argv)
{
	params->coders_nb = atoi(argv[1]);
	params->burnout_t = atoi(argv[2]);
	params->compile_t = atoi(argv[3]);
	params->debug_t = atoi(argv[4]);
	params->refactor_t = atoi(argv[5]);
	params->req_compiles_nb = atoi(argv[6]);
	params->dongle_cd = atoi(argv[7]);
	params->scheduler = argv[8];
	if (!validate_args(argv))
	{
		printf("Invalid arguments.\n");
		return (NULL);
	}
	return (params);
}
