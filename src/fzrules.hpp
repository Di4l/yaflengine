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

		/** Número de reglas existentes */
		inline size_t size() { return m_vRules.size(); }

		/** Añade una nueva regla al conjunto */
		void add(std::string strRule);
		/** Elimina una regla del conjunto */
		void del(size_t szIndex);
		/** Limpia la lista de reglas*/
		void clear();

		/** Devuelve la regla en la posición indicada */
		SFuzzyRule& operator[](size_t szIndex);

		static SFuzzyRule invalid;
	};
	//-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif /* __FZRULES_HPP__ */
//-----------------------------------------------------------------------------
