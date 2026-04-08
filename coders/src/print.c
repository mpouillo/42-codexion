/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 11:49:57 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/08 13:17:30 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

// Print the specified coder action.
void	print_action(t_sim *sim, char *msg, int coder_id)
{
	pthread_mutex_lock(&sim->sim_mutex);
	if (sim->is_running == 1)
		printf("%lli %i is %s\n", get_timestamp(), coder_id, msg);
	pthread_mutex_unlock(&sim->sim_mutex);
}

// Print a specific message.
int	print_msg(t_sim *sim, char *msg)
{
	pthread_mutex_lock(&sim->sim_mutex);
	printf("%s\n", msg);
	pthread_mutex_unlock(&sim->sim_mutex);
	return (0);
}

void	print_usage(void)
{
	printf("Usage: ./codexion <number_of_coders> <time_to_burnout> \
<time_to_compile> <time_to_debug> <time_to_refactor> \
<number_of_compiles_required> <dongle_cooldown> <scheduler>\n");
}
