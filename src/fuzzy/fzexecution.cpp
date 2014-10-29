/**
 * \file fzexecution.cpp
 *
 *  Class for executing models. This is the beast that makes the fuzzy
 *  calculations based on a TFuzzyModel.
 */

/*
 *      Copyright (C) 2014  Raúl Hermoso Sánchez
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
#include "fzexecution.hpp"
#ifdef __BORLANDC__
#include <math.h>
#else
#include <cmath>
#endif
//-----------------------------------------------------------------------------
using namespace nsFuzzy;
//-----------------------------------------------------------------------------
#define FZ_EXE_DELTA_EXEC	0.01	//-- En porcentaje (al cuadrado)
#define FZ_INVALID_RESULT	9.8E56
//-----------------------------------------------------------------------------
fuzzmap TExecData::m_fmResults;
static TFuzzyObjects Objects = TFuzzyObjects();
//-----------------------------------------------------------------------------

TExecData::TExecData(TFuzzySet* fsSet)
{
	m_fsSet    = fsSet;
	m_fvResult = FZ_INVALID_RESULT;
	clear();

	initialize();
}
//-----------------------------------------------------------------------------

TExecData::~TExecData()
{
	clear();
}
//-----------------------------------------------------------------------------

void TExecData::initialize()
{
	m_fmLimits.clear();
	for(int i = 0; i < int(m_fsSet->size()); ++i)
	{
		m_fmLimits[fuzzySet()[i].handle()]  = 0.0;
		m_fmResults[fuzzySet()[i].handle()] = 0.0;
	}
}
//-----------------------------------------------------------------------------

void TExecData::clearInputs()
{
	while(!m_emInputs.empty())
	{
		//-- Do NOT delete the TExecData of our siblings, as we did not create
		//   them. We were given to us in our add() method
//		delete m_emInputs.begin()->second;
		m_emInputs.begin()->second = NULL;
		m_emInputs.erase(m_emInputs.begin());
	}
}
//-----------------------------------------------------------------------------

void TExecData::clearRules()
{
	//-- Do NOT delete the SRulAtom* as it was not created by us.
	m_rlRules.clear();
}
//-----------------------------------------------------------------------------

void TExecData::clearResults()
{
	fuzzmap::iterator iter;

	for(int i = 0; i < int(m_fsSet->size()); ++i)
	{
		iter = m_fmResults.find(fuzzySet()[i].handle());
		if(iter != m_fmResults.end())
			m_fmResults.erase(iter);
	}
}
//-----------------------------------------------------------------------------

void TExecData::resetLimits()
{
	fuzzmap::iterator iter = m_fmLimits.begin();

	while(iter != m_fmLimits.end())
	{
		iter->second = 0.0;
		++iter;
	}
}
//-----------------------------------------------------------------------------

fuzzvar TExecData::ruleResult(SRuleAtom* raAtom)
{
	fuzzvar fv_ret = FZ_INVALID_RESULT;

	if(raAtom)
	{
		fuzzmap::iterator fm_val = m_fmResults.find(raAtom->Value->handle());
		if(fm_val != m_fmResults.end())
		{
			fv_ret = fm_val->second;

			for(size_t i = 0; i < raAtom->Modifiers.size(); ++i)
			{
				switch(raAtom->Modifiers[i])
				{
				case vmVery:
					fv_ret = pow(double(fv_ret), double(2.0));
					break;

				case vmSlightly:
					fv_ret = sqrt(fv_ret);
					break;

				case vmNot:
					fv_ret = 1.0 - fv_ret;
					break;

				case vmNone:
				default:
					break;
				}
			}
		}
		else
		{
			std::string str_why = std::string("No result stored for the value '")
								+ raAtom->Value->parent()->name()
								+ std::string(".")
								+ raAtom->Value->name()
								+ std::string("'.");
			throw TFuzzyError("ruleResult", str_why.c_str(), __LINE__, __FILE__);
		}
	}
	else
	{
		throw TFuzzyError("ruleResult", "No rule to apply modifiers found.", __LINE__, __FILE__);
	}

	return fv_ret;
}
//-----------------------------------------------------------------------------

void TExecData::setLimits()
{
	fuzzmap::iterator     fm_val;
	fuzzmap::iterator     fm_lim;
	fuzzvar               fv_aux;
	fuzzvar               fv_res;
	SRuleAtom*            ra_rule = NULL;
	fuzzrullist::iterator iter    = m_rlRules.begin();

	resetLimits();
	while(iter != m_rlRules.end())
	{
		fv_aux  = 1.0;
		ra_rule = *iter;

		//-- Apply the 'ands' of the rule (keep the lowest value)
		while(ra_rule && ra_rule->Next)
		{
			fv_res  = ruleResult(ra_rule);
			fv_aux  = fv_res < fv_aux ? fv_res : fv_aux;
			ra_rule = ra_rule->Next;
		}

		//-- Apply the 'or' of the rules (keep the highest)
		fm_lim = m_fmLimits.find(ra_rule->Value->handle());
		fm_lim->second = fv_aux > fm_lim->second ? fv_aux : fm_lim->second;

		++iter;
	}
}
//-----------------------------------------------------------------------------

fuzzlist& TExecData::fuzzyfy(fuzzlist& flOutput)
{
	TFuzzyVal* fs_val = NULL;
	int        i_index;
	fuzzvar    x_tmp;
	fuzzvar    fv_tmp;
	fuzzvar    fv_val;

	fuzzmap::iterator fl_lim;

	flOutput.clear();
	for(int i = 0; i < FL_CRV_COUNT + 1; ++i)
	{
		i_index = 2 * i;
		x_tmp   = m_fsSet->min() + i * (m_fsSet->max() - m_fsSet->min()) / FL_CRV_COUNT;
		flOutput.push_back(x_tmp);
		flOutput.push_back(0.0);

		for(int j = 0; j < int(m_fsSet->size()); ++j)
		{
			fs_val = &(fuzzySet()[j]);
			fl_lim = m_fmLimits.find(fs_val->handle());
			fv_val = fs_val->execute(x_tmp);
			fv_tmp = 0.0;

			fv_tmp = fv_val < fl_lim->second ? fv_val : fl_lim->second;

			flOutput[i_index + 1] = flOutput[i_index + 1] > fv_tmp
							   ? flOutput[i_index + 1] : fv_tmp;
		}
	}

    return flOutput;
}
//-----------------------------------------------------------------------------

fuzzvar TExecData::defuzzyfy(fuzzlist& flData)
{
    if(0 == flData.size())
        throw TFuzzyError("defuzzyfy",
        		"No hay valores con los que calcular", __LINE__, __FILE__);

    fuzzvar left    = 0.0;
    fuzzvar right   = 0.0;
    fuzzvar dx;
    int     i_left  = 0;
    int     i_right = flData.size() - 2;
    bool    b_left  = true;

    while(i_left + 2 < i_right)
    {
        if(b_left)
        {
            dx      = flData[i_left + 2] - flData[i_left];
            left   += (dx * (flData[i_left + 3] + flData[i_left + 1]) / 2.0);
            i_left += 2;
        }
        else
        {
            dx       = flData[i_right] - flData[i_right - 2];
            right   += (dx * (flData[i_right + 1] + flData[i_right - 1]) / 2.0);
            i_right -= 2;
        }
        b_left = left < right ? true : false;
    }

    // For a first approximation...the error is +- dx / 2.0
    return (flData[i_left] + flData[i_right]) / 2.0;
}
//-----------------------------------------------------------------------------

std::string TExecData::name()
{
	return m_fsSet ? m_fsSet->name() : std::string("");
}
//-----------------------------------------------------------------------------

void TExecData::add(fzhndl fhId, TExecData* edData)
{
	if(m_emInputs.find(fhId) == m_emInputs.end())
	{
		m_emInputs[fhId] = edData ? edData : new TExecData((TFuzzySet*)&Objects[fhId]);
	}
}
//-----------------------------------------------------------------------------

void TExecData::del(fzhndl fhId)
{
	fuzzexemap::iterator found = m_emInputs.find(fhId);
	if(found != m_emInputs.end())
	{
		delete found->second;
		m_emInputs.erase(found);
	}
}
//-----------------------------------------------------------------------------

void TExecData::clear()
{
	clearResults();
	clearInputs();
	clearRules();
}
//-----------------------------------------------------------------------------

fuzzvar TExecData::execute(fuzzvar x)
{
	if(m_fvResult != x)
	{
		TFuzzyVal* fz_val = NULL;
		for(int i = 0; i < int(m_fsSet->size()); ++i)
		{
			fz_val = &(fuzzySet()[i]);
			m_fmResults[fz_val->handle()] = fz_val->execute(x);
		}

		m_fvResult = x;
	}
	//-- We flag ourselves as set, as we have just calculated the outcome
	m_esStatus = esSet;

	return x;
}
//-----------------------------------------------------------------------------

fuzzvar TExecData::calculate()
{
	//-- If we have no Inputs it means we are a pure input and need no
	//   further calculation. m_fvResult already has the value for this set.
	if(m_emInputs.empty())
		return m_fvResult;

	//-- If we already have been calculated, do not do it again.
	switch(m_esStatus)
	{
	case esSet:
		return m_fvResult;

	case esUnset:
	default:
		break;
	}

	//-- If we have inputs we depend upon, calculate them first
	fuzzexemap::iterator iter = m_emInputs.begin();

	while(iter != m_emInputs.end())
	{
		iter->second->calculate();
		++iter;
	}

	//-- Once all my inputs have been set to their values, we can proceed
	//   to calculate my outcome.
	setLimits();
	fuzzlist fl_outcurve;
	return execute(defuzzyfy(fuzzyfy(fl_outcurve)));
}
//-----------------------------------------------------------------------------








TFuzzyExec::TFuzzyExec() : TFuzzyBase(FL_ID_EXEC)
{
	m_fmModel  = NULL;
}
//-----------------------------------------------------------------------------

TFuzzyExec::~TFuzzyExec()
{
	logClose();
	destroyData();
	m_fmModel = NULL;
}
//-----------------------------------------------------------------------------

void TFuzzyExec::createData()
{
	destroyData();

	if(!m_fmModel)
		return;

	TFuzzySet* fs_set = NULL;
	for(int i = 0; i < int(m_fmModel->sets().size()); ++i)
	{
		fs_set = &(m_fmModel->sets()[i]);
		m_mExecData[fs_set->handle()] = new TExecData(fs_set);
	}
}
//-----------------------------------------------------------------------------

void TFuzzyExec::destroyData()
{
	while(!m_mExecData.empty())
	{
		delete m_mExecData.begin()->second;
		m_mExecData.erase(m_mExecData.begin());
	}
}
//-----------------------------------------------------------------------------

void TFuzzyExec::setProcessChain()
{
	if(!m_fmModel)
		return;

	createData();

	SRuleAtom* fl_link = NULL;
	SRuleAtom* fl_out  = NULL;
	fzhndl     fh_id, fh_out;

	//-- First step: Fill the input and output trees from
	//   the rules.
	for(size_t i = 0; i < m_fmModel->rules().size(); ++i)
	{
		fl_link = m_fmModel->rules()[i].Links;
		fl_out  = fl_link;

		while(fl_out->Next)
			fl_out = fl_out->Next;

		fh_out = fl_out->Value->parent()->handle();

		m_mExecData[fh_out]->rules().push_back(fl_link);

		while(fl_link->Next)
		{
			fh_id = fl_link->Value->parent()->handle();

			//-- Add the inputs as siblings of the output of this rule.
			m_mExecData[fh_out]->add(fh_id, m_mExecData[fh_id]);

			fl_link = fl_link->Next;
		}
	}
}
//-----------------------------------------------------------------------------

TExecData* TFuzzyExec::operator[](fzhndl fhId)
{
	fuzzexemap::iterator found = m_mExecData.find(fhId);
	return found == m_mExecData.end() ? NULL : found->second;
}
//-----------------------------------------------------------------------------

TExecData* TFuzzyExec::operator[](std::string& strName)
{
	fuzzexemap::iterator iter = m_mExecData.begin();

	while(iter != m_mExecData.end() && iter->second->name() != strName)
		++iter;

	return iter == m_mExecData.end() ? NULL : iter->second;
}
//-----------------------------------------------------------------------------

void TFuzzyExec::model(TFuzzyModel* fmModel)
{
	if(m_fmModel != fmModel)
	{
		m_fmModel = fmModel;
		parent()  = m_fmModel;
		setProcessChain();
	}
}
//-----------------------------------------------------------------------------

void TFuzzyExec::input(fzhndl fhSet, fuzzvar x)
{
	TExecData* ed_data = (*this)[fhSet];

	if(ed_data)
		ed_data->execute(x);
}
//-----------------------------------------------------------------------------

void TFuzzyExec::input(std::string strSet, fuzzvar x)
{
	TExecData* ed_data = (*this)[strSet];

	if(ed_data)
		ed_data->execute(x);
}
//-----------------------------------------------------------------------------

fuzzvar TFuzzyExec::output(fzhndl fhSet)
{
	TExecData* ed_data = (*this)[fhSet];

	return ed_data ? ed_data->result() : 0.0;
}
//-----------------------------------------------------------------------------

fuzzvar TFuzzyExec::output(std::string strSet)
{
	TExecData* ed_data = (*this)[strSet];

	return ed_data ? ed_data->result() : 0.0;
}
//-----------------------------------------------------------------------------

void TFuzzyExec::calculate()
{
	if(!m_fmModel)
		return;
	//-- All sets should have their input set by now

	//-- 1st: Starting from the output tree, travel down calculating its output
	fuzzexemap::iterator iter = m_mExecData.begin();

	//-- Assign all output sets the "unset" status so that they are all
	//   recalculated
	while(iter != m_mExecData.end())
	{
		if(!iter->second->inputs().empty())
			iter->second->unset();
		++iter;
	}

	iter = m_mExecData.begin();
	while(iter != m_mExecData.end())
	{
		iter->second->calculate();
		++iter;
	}
}
//-----------------------------------------------------------------------------

bool TFuzzyExec::logOpen(std::string strLogFile)
{
	if(!m_osLogFile.is_open())
		m_osLogFile.open(strLogFile.c_str(), std::ios::trunc | std::ios::out);

	return m_osLogFile.is_open();
}
//-----------------------------------------------------------------------------

bool TFuzzyExec::logClose()
{
	if(m_osLogFile.is_open())
		m_osLogFile.close();

	return !m_osLogFile.is_open();
}
//-----------------------------------------------------------------------------
