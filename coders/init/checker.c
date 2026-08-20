/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 19:58:09 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/20 23:27:55 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include "../includes/macros.h"
#include "../includes/structers.h"

static int	ft_is_numeric_string(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (1);
		i++;
	}
	return (0);
}

static int	check_rules(char **av)
{
	int	i;

	i = 0;
	while (++i < 8)
	{
		if (ft_is_numeric_string(av[i]))
		{
			printf("%sError:%s Rules can't have letters\n", RED, RESET);
			return (1);
		}
	}
	return (0);
}

int	check_args(int ac, char **av)
{
	if (ac != 9)
	{
		printf("%s Error: Simulation needs to have 8 rules:\n %s", RED, RESET);
		printf("%s -> Usage: %s", YELLOW, RESET);
		printf("<number_of_coders> <time_to_burnout> ");
		printf("<time_to_compile> <time_to_debug> <time_to_refactor> ");
		printf("<number_of_compiles_required> <dongle_cooldown> <scheduler>\n");
		return (1);
	}
	return (check_rules(av));
}
