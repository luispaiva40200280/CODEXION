/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 22:26:09 by lpaiva            #+#    #+#             */
/*   Updated: 2026/07/06 22:29:18 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	ft_is_numeric_string(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
		{
			return (1);
		}
		i++;
	}
	return (0);
}

static void	ft_show_rules(t_data *rules)
{
	printf("Nbr of coders: %i\n", rules->number_of_coders);
	printf("Time Bunout: %lli\n", rules->time_to_burnout);
	printf("Time to Compile: %lli\n", rules->time_to_compile);
	printf("Time To Debug: %lli\n", rules->time_to_debug);
	printf("Time to refactor: %lli\n", rules->time_to_refactor);
	printf("Num of compiles: %i\n", rules->number_of_compiles_required);
	printf("Dongle cooldown: %i\n", rules->dongle_cooldown);
	printf("Scheduler: %i\n", rules->scheduler);
}

static int	check_letters(char **av)
{
	int	i;

	i = 0;
	while (++i < 8)
	{
		if (ft_is_numeric_string(av[i]))
			return (1);
	}
	return (0);
}

void	start_sim(t_data *rules)
{
	int	i;

	rules->start_time = get_time();
	i = -1;
	while (++i < rules->number_of_coders)
		pthread_create(&rules->coders[i].thread, NULL, coder_routine, &rules->coders[i]);
	run_simulation(rules);
	i = -1;
	while (++i < rules->number_of_coders)
		pthread_join(rules->coders[i].thread, NULL);
}

int	main(int ac, char **av)
{
	t_data	*rules;

	if (ac != 9)
		return (printf("Nbr of args are wrong"), 1);
	if (check_letters(av))
		return (1);
	rules = malloc(sizeof(t_data));
	if (!rules)
		return (printf("Something wrong whith malloc"), 1);
	ft_parser(av, rules);
	ft_show_rules(rules);
	if (init_data_lists(rules) != 0)
		return (printf("Failed to init data lists\n"), 1);
    	if (init_coders(rules) != 0)
        	return (printf("Failed to init coders\n"), 1);
	if (init_dongles(rules) != 0)
       		return (printf("Failed to init dongles\n"), 1);
	start_sim(rules);
	free(rules->queue);
	free(rules->coders);
	free(rules->dongles);
	return (free(rules), 0);
}
