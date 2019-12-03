/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleblond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/29 17:25:51 by jleblond          #+#    #+#             */
/*   Updated: 2019/11/29 17:25:54 by jleblond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "solver.h"

void		iter_adja_of_current(t_node *current, t_list **visited, t_list **open, t_lemin *lem)
{
	t_node		*adjacen_node;
	t_path		*p;

	p = current->path_lst;
	while (p)
	{
		adjacen_node = get_node_in_hash(lem, p->name);
		if (p->flow > 0 && not_in_address_lst(*visited, adjacen_node))
		{
			ft_lstadd_bot(visited, address_list_new(&adjacen_node));
			ft_lstadd_bot(open, address_list_new(&adjacen_node));
			adjacen_node->parent_name = (current->name);
		}
		p = p->next;
	}
}

t_node		**get_top_elem(t_list *lst)
{
	t_node	**ret;

	ret = NULL;
	if (lst != NULL)
	{
		ret = (t_node**)(lst->content);
	}
	return (ret);
}

/*
** here, t_list store addree of pointer of node in lst->content
** "open" are nodes to be evaluate, elements are always added to the end,
** and taken from the top to be evaluated. Delete top elem after evaluation.
** "visited" are nodes already visited, if inside, we don't evaluate this node
*/

uint8_t		breadth_first_search(t_lemin *lem)
{
	t_list	*open;
	t_list	*visited;
	t_node	**current;

	visited = NULL;
	open = address_list_new(&(lem->start));
	while (open != NULL)
	{
		current = get_top_elem(open);
		// printf("debug open start\n");
		// debug_print_address_lst(&open);
		// printf("debug end\n");
		// printf("debug visited start\n");
		// debug_print_address_lst(&visited);
		// printf("debug end\n");
		if (not_in_address_lst(visited, *current))
			ft_lstadd_bot(&visited, address_list_new(current));
		if (*current == lem->end)
		{
			del_address_lst(visited);
			del_address_lst(open);
			return (LM_TRUE);
		}
		iter_adja_of_current(*current, &visited, &open, lem);
		del_top_elem(&open);
	}
	del_address_lst(visited);
	del_address_lst(open);
	return (LM_FALSE);
}

int8_t			flow_plus_modif(t_node *enter, t_node *exit, int8_t modif)
{
	t_path	*p;

	p = enter->path_lst;
	while(p)
	{
		if (ft_strcmp(p->name, exit->name) == 0)
		{
			// printf("flow of pathname[%s] changed from [%d] to [%d]\n", p->name, p->flow, modif);
			p->flow += modif;
			return (LM_SUCCESS);
		}
		p = p->next;
	}
	return (LM_ERROR);
}

char			*get_occupied_node(t_node *enter)
{
	t_path		*p;

	p = enter->path_lst;
	while(p)
	{
		if (p->flow == 2)
		{
			// printf("Enter node[%s] to exit node[%s]\n",enter->name, p->name);
			return (p->name);
		}
		p = p->next;
	}
	return (NULL);
}

/*
** if wanted_flow == 0, then get max_flow, else, stop when arrived wanted_flow
*/
uint32_t		fulkerson_algo(t_lemin *lem, uint32_t wanted_flow)
{
	uint32_t	max_flow;
	t_node		*parent;
	t_node		*child;

	max_flow = 0;
	while (breadth_first_search(lem))
	{
		max_flow++;
		child = lem->end;
		while (child != lem->start)
		{
			// printf("parent name%s\n", (child->parent_name));
			parent = get_node_in_hash(lem, child->parent_name);
			flow_plus_modif(parent, child, -1);
			flow_plus_modif(child, parent, 1);
			child = parent;
		}
		printf("BFS lauched, iter_flow = [%d]\n", max_flow);
		if (wanted_flow > 0)
		{
			if (max_flow == wanted_flow)
				return (wanted_flow);
		}
	}
	return (max_flow);
}
/*
** circuits est tableau de circuits trouvé, tab len = nb_p
*/
t_list		**retrace_circuits_from_graph(t_lemin *lem, uint32_t nb_p)
{
	t_list			**circuits;
	uint32_t		i;
	t_node			*enter;
	t_node			*exit;
	char			*exit_name;

	i = 0;
	circuits = (t_list**)malloc(sizeof(t_list*) * nb_p);
	if (!circuits)
		return (LM_FALSE);
	ft_bzero(circuits, nb_p);
	while (i < nb_p)
	{
		enter = lem->end;
		circuits[i] = address_list_new(&enter);
		while (enter != lem->start)
		{
			if ((exit_name = get_occupied_node(enter)) == NULL)
			{
				printf("exit_name == NULL\n");
				return (NULL);
			}
			exit = get_node_in_hash(lem, exit_name);
			ft_lstadd_top(&(circuits[i]), address_list_new(&exit));
			flow_plus_modif(enter, exit, -1);
			flow_plus_modif(exit, enter, 1);
			enter = exit;
		}
		i++;
	}
	return (circuits);
}


void		debug_print_circuits(t_list **circuits, int nb_paths)
{
	int		i;
	t_node	*node;
	t_list	*cp;

	i = 0;
	while (i < nb_paths)
	{
		printf("circuit[%d]:\n", i);
		cp = circuits[i];
		while (cp)
		{
			node = *(t_node**)(cp->content);
			printf("%s\n", node->name);
			cp = cp->next;
		}
		i++;
	}
}

uint32_t		nb_ants_inside(uint32_t new_enter, uint32_t new_exit)
{
	static uint32_t nb_inside = 0;

	nb_inside += new_enter;
	nb_inside -= new_exit;
	// printf("nb_inside:%d, new_enter:%d, new_exit:%d\n", nb, new_enter, new_exit);
	return(nb_inside);
}

char		*get_node_in_circuit(t_list *cir, uint32_t floor)
{
	t_node *node;

	while (cir && floor != 0)
	{
		cir = cir->next;
		floor--;
	}
	if (cir)
	{
		node = *((t_node**)(cir->content));
		return (node->name);
	}
	return (NULL);
}

uint32_t		print_anthill(t_lemin *lem, t_list **cir, uint32_t nb_inside, uint32_t nb_enter, uint32_t flow_var[2])
{
	uint32_t	i;
	static uint32_t	initial_floor = 1;
	uint32_t	floor;
	uint32_t	nb_exit;
	char		*name;
	static uint32_t total_exit = 0;
	uint32_t	index;

	i = 0;
	nb_exit = 0;
	if (nb_enter == 0)
		initial_floor += 1;
	index = nb_inside + total_exit;
	floor = initial_floor;
	while (nb_inside)
	{
		while (i < nb_paths)
		{
			name = get_node_in_circuit(cir[i], floor);
			printf("L%d-%s ", index, name);
			if (ft_strcmp(name, (lem->end)->name) == 0)
			{
				nb_exit++;
				total_exit++;
			}
			i++;
			nb_inside--;
			index--;
		}
		i = 0;
		floor++;
	}
	return (nb_exit);
}

void		send_ants(t_lemin *lem, t_list **cir, t_list **last_cir, int32_t flow_var[2])
{
	uint32_t 	new_enter;
	uint32_t 	new_exit;
	uint32_t	max_round;

	new_exit = 0;
	max_round = lem->nb_ants / flow_var[0];
	while (LM_TRUE)
	{
		if (max_round)
		{
			new_enter = flow_var[0];
			max_round--;
		}
		else if (flow_var[1])
			new_enter = flow_var[1];
		else
			new_enter = 0;
		new_exit = print_anthill(lem, cir, nb_inside, new_enter, flow_var[2]);
		printf("\n");
		if (nb_ants_inside(new_enter, new_exit) == 0)
			break ;
	}
}




void		solver(t_lemin *lem)
{
	int32_t			flow_var[2];
	t_list			**max_cir;
	t_list			**last_cir;

	last_cir = NULL;
	flow_var[0] = fulkerson_algo(lem, lem->nb_ants);
	flow_var[1] = 0;
	// printf("fulkerson_algo done, flow_var[0]=%d\n", flow_var[0]);
	// print_tab(lem->tab, HASHCODE);
	max_cir = retrace_circuits_from_graph(lem, flow_var[0]);
	// printf("after retrac flow_var[0]\n");
	// print_tab(lem->tab, HASHCODE);

	if (lem->nb_ants > flow_var[0])
	{
		flow_var[1] = lem->nb_ants % flow_var[0];
		flow_var[1] = fulkerson_algo(lem, flow_var[1]);
		// printf("flow_var[1]%d\n", flow_var[1]);
		// print_tab(lem->tab, HASHCODE);
		last_cir = retrace_circuits_from_graph(lem, flow_var[1]);
		// printf("after retrac flow_var[1]\n");
		// print_tab(lem->tab, HASHCODE);
	}
	send_ants(lem, max_cir, last_cir, flow_var);
}



