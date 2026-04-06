/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 11:40:37 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/06 15:58:49 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

static void	compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->sim_mutex);
	coder->last_compile = get_timestamp();
	pthread_mutex_unlock(&coder->sim->sim_mutex);
	print_action(coder->sim, "compiling", coder->id);
	sleep_thread(coder->sim, coder->sim->params->compile_t);
	pthread_mutex_lock(&coder->sim->sim_mutex);
	coder->last_compile = get_timestamp();
	pthread_mutex_unlock(&coder->sim->sim_mutex);
	coder->compile_count++;
}

static void	debug(t_coder *coder)
{
	print_action(coder->sim, "debugging", coder->id);
	sleep_thread(coder->sim, coder->sim->params->debug_t);
}

static void	refactor(t_coder *coder)
{
	print_action(coder->sim, "refactoring", coder->id);
	sleep_thread(coder->sim, coder->sim->params->refactor_t);
}

static int	try_compiling(t_coder *coder)
{
	if (!acquire_dongles(coder))
		return (0);
	compile(coder);
	release_dongles(coder);
	return (1);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *) arg;
	while (!sim_running(coder->sim))
		usleep(100);
	while (sim_running(coder->sim))
	{
		if (coder->compile_count == coder->sim->params->req_compiles_nb)
			break ;
		if (!try_compiling(coder))
			continue ;
		if (!sim_running(coder->sim))
			break ;
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}
