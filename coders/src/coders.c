/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 09:56:03 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/05 10:47:38 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

static int	create_coders(t_sim *sim)
{
	int		i;

	i = 0;
	while (i < sim->params->coders_nb)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].dongle_1 = &sim->dongles[i];
		sim->coders[i].dongle_2 = &sim->dongles[(i + 1) % \
sim->params->coders_nb];
		sim->coders[i].last_compile = 0;
		sim->coders[i].compile_count = 0;
		sim->coders[i].sim = sim;
		i++;
	}
	return (0);
}

int	init_coders(t_sim *sim)
{
	sim->coders = malloc(sizeof(t_coder) * sim->params->coders_nb);
	if (!sim->coders)
		return (1);
	if (!create_coders(sim))
		return (1);
	return (0);
}
