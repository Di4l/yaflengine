/**
 * \file fzrules.hpp
 *
 *  Rules define how sets relate to each other.
 *
 *     Copyright (C) 2014  Raúl Hermoso Sánchez
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License along
 *    with this program; if not, write to the Free Software Foundation, Inc.,
 *    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *
 *    created: 02/03/2007
 *    author: Raúl Hermoso Sánchez
 *    e-mail: raul.hermoso@gmail.com
 *
 */
//-----------------------------------------------------------------------------
#ifndef __FZRULES_HPP__
#define __FZRULES_HPP__
//-----------------------------------------------------------------------------
#include "fzset.hpp"
//-----------------------------------------------------------------------------

namespace nsFuzzy
{
	//-------------------------------------------------------------------------

	enum TValModifier { vmNone = 0, vmVery, vmSlightly, vmNot };
	//-------------------------------------------------------------------------

	struct SRuleAtom
	{
		SRuleAtom*                Next;
		TFuzzyVal*                Value;
		std::vector<TValModifier> Modifiers;

		SRuleAtom(TFuzzyVal* fVal);
		virtual ~SRuleAtom();
	};
	//-------------------------------------------------------------------------

	/**
	 *
	 */
	struct SFuzzyRule
	{
		std::string String;
		SRuleAtom*  Links;

		SFuzzyRule(TFuzzyVal* fVal) { Links = new SRuleAtom(fVal); String = "";          }
		virtual ~SFuzzyRule()       { if(Links) delete Links; Links = NULL; String = ""; }

		bool operator==(SFuzzyRule& cmp);
		bool operator!=(SFuzzyRule& cmp);
	};
	//-------------------------------------------------------------------------

	/**
	 *
	 */
	class TFuzzyRules : public TFuzzyBase
	{
	private:
		std::vector<SFuzzyRule*> m_vRules;

		TFuzzySets& m_fsSets;

		std::string splitModsValue(std::string& strValue);
		void        setModifiers(SRuleAtom* flLink, std::string& strMods);
		SFuzzyRule* parseRule(std::string strRule);

	protected:
	public:
		/** Constructor principal */
		TFuzzyRules(TFuzzySets& fsSets);
		/** Destructor principal */
		virtual ~TFuzzyRules();

		/** NÃºmero de reglas existentes */
		inline size_t size() { return m_vRules.size(); }

		/** AÃ±ade una nueva regla al conjunto */
		void add(std::string strRule);
		/** Elimina una regla del conjunto */
		void del(size_t szIndex);
		/** Limpia la lista de reglas*/
		void clear();

		/** Devuelve la regla en la posiciÃ³n indicada */
		SFuzzyRule& operator[](size_t szIndex);

		static SFuzzyRule invalid;
	};
	//-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif /* __FZRULES_HPP__ */
//-----------------------------------------------------------------------------
