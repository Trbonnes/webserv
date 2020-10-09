/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Compare.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 12:25:32 by trbonnes          #+#    #+#             */
/*   Updated: 2020/10/09 12:25:36 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPARE_HPP
# define COMPARE_HPP

template <class T>
struct   Compare
{
    bool operator() (const T& x, const T& y) const
    {
        if (x.length() > y.length())
            return true;
        return false;
    }
};

#endif

