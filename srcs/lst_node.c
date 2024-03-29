/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabih <naali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 20:50:08 by nabih             #+#    #+#             */
/*   Updated: 2019/12/18 14:24:50 by naali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lem_in.h>

/*
** Detruit le noeud
** en fonction du nom
*/

void					del_node(t_node **start, char *name)
{
	t_node			*tmp1;
	t_node			*tmp2;

	if (start != NULL && *start != NULL && name != NULL)
	{
		tmp1 = *start;
		if (ft_strcmp(tmp1->name, name) == 0)
		{
			*start = tmp1->next;
			free_node(&tmp1);
		}
		else
		{
			while (tmp1->next != NULL && ft_strcmp(tmp1->next->name, name) != 0)
				tmp1 = tmp1->next;
			if (tmp1->next != NULL && ft_strcmp(tmp1->next->name, name) == 0)
			{
				tmp2 = tmp1->next;
				tmp1->next = tmp1->next->next;
				free_node(&tmp2);
			}
		}
	}
}

/*
** Vide la liste et met
** a NULL le pointeur
*/

void					clear_node(t_node **start)
{
	t_node			*tmp;

	if (start != NULL && *start != NULL)
	{
		tmp = *start;
		while (tmp != NULL)
		{
			*start = tmp->next;
			free_node(&tmp);
			tmp = *start;
		}
	}
}

/*
** Retire le noeud avec le nom passe en parametre de la liste
** elle ne le detruit pas mais renvoi un pointeur sur lui.
*/

static t_node			*remov_first_node(t_node **start)
{
	t_node			*ret;

	ret = *start;
	*start = ret->next;
	ret->next = NULL;
	return (ret);
}

t_node					*remove_from_node(t_node **start, char *name)
{
	t_node			*tmp;
	t_node			*ret;

	ret = NULL;
	if (start != NULL && *start != NULL && name != NULL)
	{
		tmp = *start;
		if (ft_strcmp(tmp->name, name) == 0)
			ret = remov_first_node(start);
		else
		{
			while (tmp->next != NULL && ft_strcmp(tmp->next->name, name) != 0)
				tmp = tmp->next;
			if (tmp->next != NULL)
			{
				ret = tmp->next;
				tmp->next = ret->next;
				ret->next = NULL;
			}
		}
	}
	return (ret);
}

/*
** Transfert un noeud d'une liste
** a une autre liste
*/

void					send_node_away(t_node **src, t_node **dst, char *name)
{
	t_node			*tmp;

	if (src != NULL && *src != NULL && dst != NULL && name != NULL)
	{
		tmp = remove_from_node(src, name);
		pushback_node(dst, tmp);
	}
}
