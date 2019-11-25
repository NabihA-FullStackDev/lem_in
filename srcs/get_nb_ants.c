/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_nb_ants.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabih <naali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/24 15:01:32 by nabih             #+#    #+#             */
/*   Updated: 2019/11/25 20:01:16 by nabih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <get_info.h>

/*
** Recuperation du nombre
**      DE  FOURMIES
*/
int8_t					get_nb_ants(t_lemin *lem)
{
	lem->nb_ants = 0;
	if (get_next_line(0, &(lem->line)) == 1)
	{
		lem->nb_ants = ft_atoi(lem->line);
		ft_memdel((void**)&(lem->line));
	}
	if (lem->nb_ants <= 0)
		ft_putstr("Error ants number, can't be less than 1\n");
	return ((lem->nb_ants > 0) ? LM_SUCCESS : LM_ERROR);
}
