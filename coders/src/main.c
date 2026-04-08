/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 11:40:14 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/08 12:36:40 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

int	main(int argc, char **argv)
{
	int	exit_status;

	if (argc != 9)
	{
		print_usage();
		return (1);
	}
	get_timestamp();
	exit_status = run_sim(argv);
	return (exit_status);
}
