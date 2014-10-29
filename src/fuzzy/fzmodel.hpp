/**
 * \file fzmodel.hpp
 *
 *  A model binds a group of Sets and Rules. Rules define how sets relate to
 *  each other.
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
#ifndef __FZMODEL_HPP__
#define __FZMODEL_HPP__
//-----------------------------------------------------------------------------
#include "fzrules.hpp"
//-----------------------------------------------------------------------------
/** NÃºmero de puntos en cada funciÃ³n para propÃ³sitos de pintado */
#define FL_CRV_COUNT		1000
//-----------------------------------------------------------------------------

namespace nsFuzzy
{
	//-------------------------------------------------------------------------

	class TFuzzyModel : public TFuzzyBase
	{
	private:
		TFuzzySets  m_fsSets;
		TFuzzyRules m_frRules;
		std::string m_strDescription;

	protected:
        std::string& configurationHint(std::string& strHint);

	public:
		TFuzzyModel();
		virtual ~TFuzzyModel();

		inline std::string& description() { return m_strDescription; }
		inline TFuzzyRules& rules()       { return m_frRules;        }
		inline TFuzzySets&  sets()        { return m_fsSets;         }

		void clear();

		void loadFromFile(std::string strFileName);
		void saveToFile(std::string strFileName, bool bAddComments = true);
	};
	//-------------------------------------------------------------------------

	class TFuzzyModels : public TFuzzyBase
	{
	private:
		std::map<fzhndl, TFuzzyModel*> m_mModels;

	protected:
	public:
		TFuzzyModels();
		virtual ~TFuzzyModels();

		inline size_t size() { return m_mModels.size(); }

		fzhndl add();
		void   del(fzhndl fzId);
		void   clear();

		TFuzzyModel& operator[](fzhndl fzId);
		TFuzzyModel& operator[](int iIndex);
		TFuzzyModel& operator[](std::string strName);
	};
}
//-----------------------------------------------------------------------------
#endif /* __FZMODEL_HPP__ */
//-----------------------------------------------------------------------------
