/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 12:40:30 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/09 13:36:41 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_H
# define INCLUDES_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef enum e_scheduler
{
	EDF,
	FIFO
}	t_scheduler;

typedef struct s_params
{
	int			coders_nb;
	int			burnout_t;
	int			compile_t;
	int			debug_t;
	int			refactor_t;
	int			req_compiles_nb;
	int			dongle_cd;
	t_scheduler	scheduler;
}	t_params;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	int				queue[2];
	long long		last_use;
	int				available;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongle		*dongle_1;
	t_dongle		*dongle_2;
	long long		last_compile;
	int				compile_count;
	struct s_sim	*sim;
}	t_coder;

typedef struct s_sim
{
	t_coder			*coders;
	t_dongle		*dongles;
	t_params		*params;
	pthread_t		monitor;
	pthread_mutex_t	sim_mutex;
	int				is_running;

}	t_sim;

//		coder_actions.c
void		*coder_routine(void *arg);

//		coders.c
int			init_coders(t_sim *sim);

//		dongle_actions.c
int			acquire_dongles(t_coder *coder);
void		release_dongles(t_coder *coder);

//		dongles.c
int			init_dongles(t_sim *sim);
int			destroy_dongles(t_sim *sim, int n);

//		monitor.c
void		*monitor_routine(void *arg);

//		parsing.c
t_params	*parse_params(t_params *params, char **argv);

//		print.c
void		print_action(t_sim *sim, char *msg, int coder_id);
int			print_msg(t_sim *sim, char *msg);
void		print_usage(void);

//		queue.c
int			is_next(t_sim *sim, t_dongle *dongle, int coder_id);
int			queue_pop(t_dongle *dongle, int coder_id);
void		queue_push(t_dongle *dongle, int coder_id);

//		sim.c
int			run_sim(char **argv);
int			sim_running(t_sim *sim);
void		clean_up_sim(t_sim *sim);
void		stop_sim(t_sim *sim);

//		timing.c
long long	get_timestamp(void);
void		sleep_thread(t_sim *sim, long long sleep_time);

//		thread.c
int			start_threads(t_sim *sim);
int			join_threads(t_sim *sim);

#endif
