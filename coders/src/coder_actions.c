/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 11:40:37 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/05 14:24:37 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

static void	compile(t_coder *coder)
{
	print_action(coder->sim, "compiling", coder->id);
	usleep(coder->sim->params->compile_t * 1000);
	coder->last_compile = get_time_since_start();
	coder->compile_count++;
}

static void	debug(t_coder *coder)
{
	print_action(coder->sim, "debugging", coder->id);
	usleep(coder->sim->params->debug_t * 1000);
}

static void	refactor(t_coder *coder)
{
	print_action(coder->sim, "refactoring", coder->id);
	usleep(coder->sim->params->refactor_t * 1000);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *) arg;
	while (coder->compile_count < coder->sim->params->req_compiles_nb)
	{
		while (coder->sim->is_running == 0)
			continue ; // wait until simulation is running
		compile(coder);
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}
