/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 11:40:37 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/08 13:38:33 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

// Print action and sleep for compile_t ms.
// Also updates last_compile and compile_count variables.
static void	compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->sim_mutex);
	coder->last_compile = get_timestamp();
	pthread_mutex_unlock(&coder->sim->sim_mutex);
	print_action(coder->sim, "compiling", coder->id);
	sleep_thread(coder->sim, coder->sim->params->compile_t);
	pthread_mutex_lock(&coder->sim->sim_mutex);
	if (coder->sim->is_running == 1)
		coder->compile_count++;
	pthread_mutex_unlock(&coder->sim->sim_mutex);
}

// Print action and sleep for debug_t ms.
static void	debug(t_coder *coder)
{
	print_action(coder->sim, "debugging", coder->id);
	sleep_thread(coder->sim, coder->sim->params->debug_t);
}

// Print action and sleep for refactor_t ms.
static void	refactor(t_coder *coder)
{
	print_action(coder->sim, "refactoring", coder->id);
	sleep_thread(coder->sim, coder->sim->params->refactor_t);
}

// Attempt to get dongle and start compiling.
// Returns 1 on success or 0 on error.
static int	try_compiling(t_coder *coder)
{
	if (!acquire_dongles(coder))
		return (0);
	compile(coder);
	release_dongles(coder);
	return (1);
}

// Main coder thread logic.
// A coder will attempt to get dongles, compile, refactor, and debug.
// It will return when it has compiled enough times.
void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *) arg;
	while (!sim_running(coder->sim))
		usleep(100);
	if (sim_running(coder->sim) == 1 && coder->id % 2 == 0)
		usleep((coder->sim->params->compile_t / 2 + 1) * 1000);
	while (sim_running(coder->sim) == 1)
	{
		if (!try_compiling(coder))
			continue ;
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}
