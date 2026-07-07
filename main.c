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

static int	ft_isalpha(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}

static void	ft_show_rules(t_node *rules)
{
	printf("Nbr of coders: %i\n", rules->number_of_coders);
	printf("Time Bunout: %i\n", rules->time_to_burnout);
	printf("Time to Compile: %i\n", rules->time_to_compile);
	printf("Time To Debug: %i\n", rules->time_to_debug);
	printf("Time to refactor: %i\n", rules->time_to_refactor);
	printf("Num of compiles: %i\n", rules->number_of_compiles_required);
	printf("Dongle cooldown: %i\n", rules->dongle_cooldown);
	printf("Scheduler: %i\n", rules->scheduler);
}

static int	check_letters(char **av)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (++i < 8)
		while (av[i][j])
			if (ft_isalpha(av[i][j++]))
				return (1);
	return (0);
}

int	main(int ac, char **av)
{
	t_node	*rules;

	if (ac != 9 || check_letters(av))
		return (printf("Nbr of args are wrong"), 1);
	rules = malloc(sizeof(t_node));
	if (!rules)
		return (printf("Something wrong whith malloc"), 1);
	ft_parser(av, rules);
	ft_show_rules(rules);
	return (free(rules), 0);
}
