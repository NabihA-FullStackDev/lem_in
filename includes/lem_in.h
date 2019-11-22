/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabih <naali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/26 17:03:23 by nabih             #+#    #+#             */
/*   Updated: 2019/11/22 23:23:01 by nabih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** INFORMATION IMPORTANTE !!!
** pour les types de retour
**
** "uint8_t" dans le cas d'un retour TRUE-FALSE
**
** "int8_t" dans le cas d'un retour SUCCESS-ERROR
*/

#ifndef		LEM_IN_H
# define	LEM_IN_H

# include <includes.h>

# define LM_TRUE 1
# define LM_FALSE 0
# define LM_SUCCESS 1
# define LM_ERROR -1

# define HASHCODE 1000

typedef struct	s_path	t_path;
typedef struct	s_node	t_node;

struct					s_path//	Liste des chemins d'un seul noeud!!!
{
	uint32_t		id;//			Static incrementer a la creation - modif de creation...
	char			*linked[2];//	Stockage des noms de noeuds relier.
	uint8_t			taken;//		Booleen Vrai ou Faux (Utiliser ou pas)
	int				weight;//		Poids == predecesseur + 1; =======> sera peut etre changer en unsigned
	t_path			*next;//		pointe sur le chemin suivant du meme noeud
};

struct					s_node//	Liste des noeuds a visiter ou deja visite
{
	uint32_t		id;//			Static incrementer a la creation - modif de creation
	char			*name;//		Nom du noeud
	uint8_t			taken;//		Booleen Vrai ou Faux (Utiliser ou pas)
	uint8_t			start;//		Booleen Vrai ou Faux
	uint8_t			end;//			Booleen Vrai ou Faux
	t_path			*path_lst;//	Pointe sur 1er chemin de la liste des chemins connecter au noeud
	t_node			*next;//		Pointe sur le noeud suivant
};

typedef struct			s_lemin
{
	t_node				*tab[HASHCODE];
}						t_lemin;

/*
** Fonctions de Gestion
**   DES LISTES PATH
*/
t_path					*new_path(/* surement les noms des noeuds entre lesquels il est */);
void					pushback_path(t_path **start, t_path *node);
void					pushfront_path(t_path **start, t_path *node);
void					pushafter_path(t_path **start, uint32_t id,\
										t_path *node);
uint32_t				count_path(const t_path *start);
void					del_path(t_path **start, uint32_t id);
void					clear_path(t_path **start);
void					swap_id_path(t_path *p1, t_path *p2);
t_path					*get_last_path(t_path **start);
t_path					*get_path_by_id(t_path **start, uint32_t id);
t_path					*remove_from_path(t_path **start, uint32_t id,
											uint8_t flag);
void					send_path_away(t_path **src, t_path **dst, uint32_t id);

/*
** Fonctions de Gestion
**   DES LISTES NODE
*/
t_node					*new_node(char *name, uint8_t start, uint8_t end);
void					pushback_node(t_node **start, t_node *node);
void					pushfront_node(t_node **start, t_node *node);
void					pushafter_node(t_node **start, char *name,\
										t_node *node);
uint32_t				count_node(const t_node *start);
t_node					*get_last_node(t_node **start);
t_node					*get_node_by_name(t_node **start, char *name);
void					del_node(t_node **start, char *name);
void					clear_node(t_node **start);
t_node					*remove_from_node(t_node **start, char *name);
void					send_node_away(t_node **src, t_node **dst, char *name);

/*
** Fonstions de Gestion
**   DES  ENCRYPTIONS
*/
int32_t					hash_name(char *name);// Stocker dans l'id des noeuds

/*
** Fonstions de Gestion
**  DU TABLEAU DE HASH
*/
void					add_node_in_tab(t_lemin *lem, t_node *node);

/*
** Fonctions de Gestion
**      DES TESTS
*/
void					print_tab(t_node *(tab[HASHCODE]), uint32_t max);

#endif
