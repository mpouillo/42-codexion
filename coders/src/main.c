/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 11:40:14 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/07 14:22:11 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

int	main(int argc, char **argv)
{
	int	exit_status;

	if (argc != 9)
	{
		printf("Usage: ./codexion <number_of_coders> <time_to_burnout> \
<time_to_compile> <time_to_debug> <time_to_refactor> \
<number_of_compiles_required> <dongle_cooldown> <scheduler>\n");
		return (1);
	}
	get_timestamp();
	exit_status = run_sim(argv);
	return (exit_status);
}
