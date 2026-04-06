/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 11:49:57 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/06 14:03:42 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

void	print_action(t_sim *sim, char *msg, int coder_id)
{
	pthread_mutex_lock(&sim->sim_mutex);
	if (sim->is_running)
		printf("%lli %i is %s\n", get_timestamp(), coder_id, msg);
	pthread_mutex_unlock(&sim->sim_mutex);
}
