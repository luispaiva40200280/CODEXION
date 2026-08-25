/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpaiva <lpaiva@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 18:07:12 by lpaiva            #+#    #+#             */
/*   Updated: 2026/08/24 20:58:01 by lpaiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_H
# define MACROS_H

/*
ANSCII escape codes for colors:
to use on printing each action of
the coders and other prints on the terminal
*/
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[36m"
# define CYAN "\033[36m"
# define RESET "\033[0m"

/*
Macros to difine the diferrent tipes of
scheduler to be easear to read the code
*/
# define EDF 2
# define FIFO 1

# define WAITING 0
# define TAKEN_FIRST 1
# define TAKEN_SECOND 2

#endif