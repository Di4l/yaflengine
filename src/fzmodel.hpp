//-----------------------------------------------------------------------------
#ifndef __FZMODEL_HPP__
#define __FZMODEL_HPP__
//-----------------------------------------------------------------------------
#include "fzrules.hpp"
//-----------------------------------------------------------------------------
/** Número de puntos en cada función para propósitos de pintado */
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
