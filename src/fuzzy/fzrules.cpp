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
#include "fzrules.hpp"
//-----------------------------------------------------------------------------
using namespace nsFuzzy;
//-----------------------------------------------------------------------------

/**
 *
 */
SRuleAtom::SRuleAtom(TFuzzyVal* fVal)
{
	Next  = NULL;
	Value = fVal;
	Modifiers.clear();
}
//-----------------------------------------------------------------------------

/**
 *
 */
SRuleAtom::~SRuleAtom()
{
	Modifiers.clear();
	Value = NULL;
	if(Next)
		delete Next;
	Next  = NULL;
}
//-----------------------------------------------------------------------------







/**
 *
 */
bool SFuzzyRule::operator==(SFuzzyRule& cmp)
{
	return this == &cmp ? true : String == cmp.String;
}
//-----------------------------------------------------------------------------

/**
 *
 */
bool SFuzzyRule::operator!=(SFuzzyRule& cmp)
{
	return this != &cmp ? true : String != cmp.String;
}
//-----------------------------------------------------------------------------






/**
 *
 */
SFuzzyRule TFuzzyRules::invalid(NULL);
//-----------------------------------------------------------------------------

/**
 *
 */
TFuzzyRules::TFuzzyRules(TFuzzySets& fsSets) : TFuzzyBase(FL_ID_RUL), m_fsSets(fsSets)
{
	m_strName = "rules";
}
//-----------------------------------------------------------------------------

/**
 *
 */
TFuzzyRules::~TFuzzyRules()
{
	clear();
}
//-----------------------------------------------------------------------------

/**
 *
 */
std::string TFuzzyRules::splitModsValue(std::string& strValue)
{
	std::string str_mods = "";

	size_t pos = strValue.rfind(".");
	if(pos != std::string::npos)
	{
		str_mods = strValue.substr(0, pos);
		str_mods = trim(toLower(str_mods));
		strValue = strValue.substr(pos + 1);
	}

	return str_mods;
}
//-----------------------------------------------------------------------------

/**
 *
 */
void TFuzzyRules::setModifiers(SRuleAtom* flLink, std::string& strMods)
{
	std::string str_aux;
	size_t      pos;

	while(!strMods.empty())
	{
		pos = strMods.find(".");
		if(pos == std::string::npos)
			pos = strMods.size();
		str_aux = strMods.substr(0, pos);
		str_aux = trim(toLower(str_aux));
		if(pos == strMods.size())
			strMods = "";
		else
			strMods = strMods.substr(pos + 1);

		if(str_aux == "very" || str_aux == "muy")
			flLink->Modifiers.push_back(vmVery);
		else if(str_aux == "slightly" || str_aux == "little" || str_aux == "few"
				|| str_aux == "ligeramente" || str_aux == "algo")
			flLink->Modifiers.push_back(vmSlightly);
		else if(str_aux == "not" || str_aux == "no")
			flLink->Modifiers.push_back(vmNot);
	}
}
//-----------------------------------------------------------------------------

/**
 *
 */
SFuzzyRule* TFuzzyRules::parseRule(std::string strRule)
{
	toLower(strRule);

	if(strRule.find("if ") == std::string::npos)
	{
		throw TFuzzyError("parseRule", "No se encuentra la clausula 'if'",
							__LINE__, __FILE__);
	}
	if(strRule.find(" then ") == std::string::npos)
	{
		throw TFuzzyError("parseRule", "No se encuentra la clausula 'then'",
							__LINE__, __FILE__);
	}

	std::string  str_rule = strRule;
	SFuzzyRule*  fr_rule  = NULL;
	size_t       pos      = strRule.find("if ");
    SRuleAtom  *f_link   = NULL, *f_first = NULL;
    std::string  str_cmd, str_val, str_set;
    std::string  str_mods;

    strRule = strRule.substr(pos + 3);
    trim(strRule);

    pos     = strRule.find(" and ") != std::string::npos
    		? strRule.find(" and ") : strRule.find(" then ");

    try
    {
    	while(!strRule.empty())
    	{
    		//-- Obtiene el 'comando' (set.[mod.]value) y prepara strRule para
    		//   la siguiente pasada a este while
    		str_cmd = strRule.substr(0, pos);
    		trim(str_cmd);
			if(pos == strRule.size())
				pos -= 5;
            strRule = strRule.substr(pos + 5);
            trim(strRule);

            //-- Al menos debe haber un punto (set.value)... si no hay ninguno,
            //   error en la regla
            pos = str_cmd.find(".");
            if(pos == std::string::npos)
            	throw TFuzzyError("parseRule", "No se ha encontrado el operador '.'",
            					__LINE__, __FILE__);

            //-- Obtiene el set
            str_set = str_cmd.substr(0, pos);
           	if(m_fsSets[str_set] == invalidObject())
   			{
   				std::string str_why = "No existe el set " + str_set;
           		throw TFuzzyError("parseRule", str_why.c_str(), __LINE__, __FILE__);
   			}

           	//-- Obtiene el valor y busca un modificador ([mod.]value)
           	str_val  = str_cmd.substr(pos + 1);
           	str_mods = splitModsValue(str_val);
   			if(m_fsSets[str_set][str_val] == invalidObject())
   			{
   				std::string str_why = "No existe el valor " + str_val + "en el set " + str_set;
   				throw TFuzzyError("parseRule", str_why.c_str(), __LINE__, __FILE__);
   			}

   			if(f_first)
   			{
   				f_link->Next = new SRuleAtom(&m_fsSets[str_set][str_val]);
   				f_link           = f_link->Next;
   			}
   			else
   			{
   				fr_rule = new SFuzzyRule(&m_fsSets[str_set][str_val]);
   				fr_rule->String    = str_rule;
   				f_first          = fr_rule->Links;
   				f_link           = f_first;
   			}
   			//-- Asigna los modificadores al valor
			setModifiers(f_link, str_mods);

   		    pos = strRule.find(" and ") != std::string::npos
   		    	? strRule.find(" and ") : strRule.find(" then ");

   		    if(pos == std::string::npos)
   		    	pos = strRule.size();
    	}
    }
    catch(TFuzzyError& e)
    {
    	if(fr_rule)
    		delete fr_rule;
    	fr_rule = NULL;

    	throw e;
    }
    catch(...)
    {
    	if(fr_rule)
    		delete fr_rule;
    	fr_rule = NULL;

    	throw TFuzzyError("parseRule", "Error sin especificar", __LINE__, __FILE__);
    }

    return fr_rule;
}
//-----------------------------------------------------------------------------

/**
 *
 */
void TFuzzyRules::add(std::string strRule)
{
	trim(strRule);

	SFuzzyRule* fr_rule = parseRule(strRule);
	if(fr_rule)
		m_vRules.push_back(fr_rule);
}
//-----------------------------------------------------------------------------

/**
 *
 */
void TFuzzyRules::del(size_t szIndex)
{
    if(szIndex >= 0 && szIndex < m_vRules.size())
    {
    	std::vector<SFuzzyRule*>::iterator itfl = m_vRules.begin();
    	size_t sz_ndx = 0;

    	for(; sz_ndx < szIndex; ++itfl, ++sz_ndx);

    	delete m_vRules[sz_ndx];
    	m_vRules.erase(itfl);
    }
}
//-----------------------------------------------------------------------------

/**
 *
 */
void TFuzzyRules::clear()
{
	while(!m_vRules.empty())
	{
		delete m_vRules.back();
		m_vRules.pop_back();
	}
}
//-----------------------------------------------------------------------------

/**
 *
 */
SFuzzyRule& TFuzzyRules::operator[](size_t szIndex)
{
	return szIndex >= 0 && szIndex < m_vRules.size()
			? *m_vRules[szIndex] : TFuzzyRules::invalid;
}
//-----------------------------------------------------------------------------
