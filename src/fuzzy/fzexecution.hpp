/**
 * \file fzexecution.hpp
 *
 *  Class for executing models. This is the beast that makes the fuzzy
 *  calculations based on a TFuzzyModel.
 */

/*     Copyright (C) 2014  Raúl Hermoso Sánchez
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
 *    created: 15/08/2012
 *    author: Raúl Hermoso Sánchez
 *    e-mail: raul.hermoso@gmail.com
 *
 */

//-----------------------------------------------------------------------------
#ifndef __FZEXECUTION_HPP__
#define __FZEXECUTION_HPP__
//-----------------------------------------------------------------------------
#include "fzmodel.hpp"
#include <fstream>
//-----------------------------------------------------------------------------

namespace nsFuzzy
{
	enum TExeStatus { esUnset = 0, esSet };
	//-------------------------------------------------------------------------

	class TExecData;
	//-------------------------------------------------------------------------

	typedef std::map<fzhndl, TExecData*> fuzzexemap;
	typedef std::vector<SRuleAtom*>      fuzzrullist;
	//-------------------------------------------------------------------------

	class TExecData
	{
	private:
		static fuzzmap m_fmResults;

		TFuzzySet*   m_fsSet;
		fuzzexemap   m_emInputs;
		fuzzrullist  m_rlRules;
		fuzzmap      m_fmLimits;
		fuzzvar      m_fvResult;
		TExeStatus   m_esStatus;

		inline TFuzzySet& fuzzySet() { return *m_fsSet; }

		void initialize();

		void clearInputs();
		void clearRules();
		void clearResults();

		fuzzvar   ruleResult(SRuleAtom* raAtom);
		void      resetLimits();
		void      setLimits();
		fuzzlist& fuzzyfy(fuzzlist& flOutput);
		fuzzvar   defuzzyfy(fuzzlist& flData);

	protected:
	public:
		TExecData(TFuzzySet* fsSet);
		virtual ~TExecData();

		inline fuzzexemap&  inputs() { return m_emInputs; }
		inline fuzzrullist& rules()  { return m_rlRules;  }
		inline fuzzvar      result() { return m_fvResult; }
//		inline TExeStatus   status() { return m_esStatus; }

		inline void set()   { m_esStatus = esSet;   }
		inline void unset() { m_esStatus = esUnset; }

		std::string name();

		void add(fzhndl fhId, TExecData* edData);
		void del(fzhndl fhId);
		void clear();

		fuzzvar execute(fuzzvar x);
		fuzzvar calculate();
	};
	//-------------------------------------------------------------------------

	class TFuzzyExec : public TFuzzyBase
	{
	private:
		TFuzzyModel*  m_fmModel;
		fuzzexemap    m_mExecData;
		std::ofstream m_osLogFile;

		void createData();
		void destroyData();
		void setProcessChain();

	protected:
		TExecData* operator[](fzhndl fhId);
		TExecData* operator[](std::string& strName);

	public:
		TFuzzyExec();
		virtual ~TFuzzyExec();

		inline TFuzzyModel*   model()   { return m_fmModel;             }
		inline std::ofstream& log()     { return m_osLogFile;           }
		inline bool           logging() { return m_osLogFile.is_open(); }

		void model(TFuzzyModel* fmModel);

		void input(fzhndl fhSet, fuzzvar x);
		void input(std::string strSet, fuzzvar x);

		fuzzvar output(fzhndl fhSet);
		fuzzvar output(std::string strSet);

		bool logOpen(std::string strLogFile);
		bool logClose();

		void calculate();
	};
	//-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif /* __FZEXECUTION_HPP__ */
//-----------------------------------------------------------------------------
