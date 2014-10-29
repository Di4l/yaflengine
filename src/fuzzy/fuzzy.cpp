/**
 * \file fuzzy.cpp
 *
 *  This file is only used when compiling with the C++Builder IDE.
 */

/*
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
 */
//-----------------------------------------------------------------------------
#ifdef __BCPLUSPLUS__
//-----------------------------------------------------------------------------
#include "fzmodel.hpp"
#include <map>
//-----------------------------------------------------------------------------
using namespace nsFuzzy;
//-----------------------------------------------------------------------------

/** Autor de la librería */
#define FL_AUTHOR	"Raul Hermoso"
/** Versión de la librería */
#define FL_VERSION	"v1.0.0"
//-----------------------------------------------------------------------------

static unsigned int                 uiInstances = 0;
static FZ_INIT*                     fzInit      = NULL;
static fzhndl                       fhDefModel  = FL_INVALID_HANDLE;
static TFuzzyModels                 fzModels;
static TFuzzyObjects                foObjects;
static std::map<fzhndl, FZ_OBJECT*> fzObjects;
static std::vector<FZ_LIST*>        fzLists;
//-----------------------------------------------------------------------------
#ifndef DLL_FUNC_PROTO
#define DLL_FUNC_PROTO	__declspec(dllexport)
#endif
//-----------------------------------------------------------------------------

FZ_OBJECT* add_object(TFuzzyBase* fbObj)
{
	if(!fbObj)
		return NULL;

	FZ_OBJECT* fz_obj = NULL;

	std::map<fzhndl, FZ_OBJECT*>::iterator found = fzObjects.find(fbObj->handle());
	if(found == fzObjects.end())
	{
		fz_obj = new FZ_OBJECT;

		fz_obj->id   = fbObj->handle();
		fz_obj->name = fbObj->name();
		fz_obj->type = fbObj->type();

		fzObjects[fz_obj->id] = fz_obj;
	}

	return fz_obj ? fz_obj : found->second;
}
//-----------------------------------------------------------------------------

void del_object(fzhndl fzId)
{
	if(FL_INVALID_HANDLE == fzId)
		return;

	std::map<fzhndl, FZ_OBJECT*>::iterator found = fzObjects.find(fzId);
	if(found != fzObjects.end())
	{
		delete found->second;
		found->second = NULL;
		fzObjects.erase(found);
	}
}
//-----------------------------------------------------------------------------

FZ_OBJECT* get_object(fzhndl fzId)
{
	std::map<fzhndl, FZ_OBJECT*>::iterator found = fzObjects.find(fzId);

	return found == fzObjects.end() ? NULL : found->second;
}
//-----------------------------------------------------------------------------

void delete_list(FZ_LIST*& fzLst)
{
	if(fzLst)
	{
		delete_list(fzLst->next);
		fzLst->next = NULL;
		delete fzLst;
		fzLst = NULL;
	}
}
//-----------------------------------------------------------------------------

TFuzzyBase* get_fuzzy_object(fzhndl fzId)
{
	std::map<fzhndl, TFuzzyBase*>::iterator found = foObjects.objects().find(fzId);

	return found == foObjects.objects().end() ? NULL : found->second;
}
//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
//-----------------------------------------------------------------------------

FZ_INIT* DLL_FUNC_PROTO fuzzy_init()
{
	if(!fzInit)
	{
		fzInit = new FZ_INIT;

		fzInit->library = "libfuzzy";
		fzInit->author  = FL_AUTHOR;
		fzInit->version = FL_VERSION;
	}
	++uiInstances;

	return fzInit;
}
//-----------------------------------------------------------------------------

void DLL_FUNC_PROTO fuzzy_close()
{
	if(0 == --uiInstances && fzInit)
	{
		delete fzInit;

		fzModels.clear();
	}
	fzInit = NULL;
}
//-----------------------------------------------------------------------------

FZ_LIST* DLL_FUNC_PROTO fuzzy_list_create()
{
	FZ_LIST* fz_lst = new FZ_LIST;

	fz_lst->next   = NULL;
	fz_lst->object = NULL;

	fzLists.push_back(fz_lst);

	return fz_lst;
}
//-----------------------------------------------------------------------------

void DLL_FUNC_PROTO fuzzy_list_free(FZ_LIST* fzLst)
{
	if(!fzLst)
		return;

	std::vector<FZ_LIST*>::iterator iter = fzLists.begin();

	while(iter != fzLists.end() && *iter != fzLst)
		++iter;

	if(iter != fzLists.end())
	{
		delete_list(*iter);
		fzLists.erase(iter);
	}
}
//-----------------------------------------------------------------------------

fzhndl DLL_FUNC_PROTO fuzzy_use_model(fzhndl fzId)
{
	if(fzId != fhDefModel)
		fhDefModel = (fzModels[fzId] == TFuzzyBase::invalid)
				? FL_INVALID_HANDLE : fzId;

	return fhDefModel;
}
//-----------------------------------------------------------------------------

fzhndl DLL_FUNC_PROTO fuzzy_model_create()
{
	fzhndl fh_id = fzModels.add();

	if(fh_id != FL_INVALID_HANDLE)
		add_object(&fzModels[fh_id]);
	if(FL_INVALID_HANDLE == fhDefModel)
		fhDefModel = fh_id;

	return fh_id;
}
//-----------------------------------------------------------------------------

void DLL_FUNC_PROTO fuzzy_model_free(fzhndl fzId)
{
	del_object(fzId);
	fzModels.del(fzId);
	if(fhDefModel == fzId)
		fhDefModel = FL_INVALID_HANDLE;
}
//-----------------------------------------------------------------------------

void DLL_FUNC_PROTO fuzzy_model_load(std::string& strFileName, fzhndl fzId)
{
	fzId = fzId == FL_INVALID_HANDLE ? fhDefModel : fzId;
	if(fzModels[fzId] == TFuzzyBase::invalid)
		return;

	fzModels[fzId].loadFromFile(strFileName);
}
//-----------------------------------------------------------------------------

void DLL_FUNC_PROTO fuzzy_model_save(std::string& strFileName, fzhndl fzId)
{
	fzId = fzId == FL_INVALID_HANDLE ? fhDefModel : fzId;
	if(fzModels[fzId] == TFuzzyBase::invalid)
		return;

	fzModels[fzId].saveToFile(strFileName);
}
//-----------------------------------------------------------------------------

void DLL_FUNC_PROTO fuzzy_model_input(fzhndl fzSetId, fuzzvar fvValue)
{
	TFuzzySet* fs_set = DCAST(TFuzzySet*, get_fuzzy_object(fzSetId));
	if(NULL == fs_set)
		return;

	TFuzzyModel* fm_model = DCAST(TFuzzyModel*, fs_set->parent());
	if(NULL == fm_model)
		return;

	fm_model->input(fzSetId, fvValue);
}
//-----------------------------------------------------------------------------

void DLL_FUNC_PROTO fuzzy_model_calculate(fzhndl fzId)
{
	fzId = fzId == FL_INVALID_HANDLE ? fhDefModel : fzId;
	if(fzModels[fzId] != TFuzzyBase::invalid)
	{
		fzModels[fzId].calculate();
	}
}
//-----------------------------------------------------------------------------

fuzzvar DLL_FUNC_PROTO fuzzy_model_output(fzhndl fzSetId)
{
	TFuzzySet* fs_set = DCAST(TFuzzySet*, get_fuzzy_object(fzSetId));
	if(NULL == fs_set)
		return 0.0;

	return fs_set->result();
}
//-----------------------------------------------------------------------------

size_t DLL_FUNC_PROTO fuzzy_model_get_sets(fzhndl fzId, FZ_LIST*& fzList)
{
	if(!fzList)
		fzList = fuzzy_list_create();
	FZ_LIST* fzl_cur = fzList;

	for(int i = 0; i < int(fzModels[fzId].sets().size()); ++i)
	{
		if(i)
		{
			fzl_cur->next = new FZ_LIST;
			fzl_cur = fzl_cur->next;
			fzl_cur->next = NULL;
		}
		fzl_cur->object = add_object(&fzModels[fzId].sets()[i]);
	}

	return fzModels[fzId].sets().size();
}
//-----------------------------------------------------------------------------

size_t DLL_FUNC_PROTO fuzzy_set_get_values(fzhndl fzSetId, FZ_LIST*& fzList)
{
	TFuzzySet* fs_set = DCAST(TFuzzySet*, get_fuzzy_object(fzSetId));
	if(NULL == fs_set)
		return 0;

	if(!fzList)
		fzList = fuzzy_list_create();
	FZ_LIST* fzl_cur = fzList;

	for(int i = 0; i < int(fs_set->size()); ++i)
	{
		if(i)
		{
			fzl_cur->next = new FZ_LIST;
			fzl_cur = fzl_cur->next;
			fzl_cur->next = NULL;
		}
		fzl_cur->object = add_object(&((*fs_set)[i]));
	}

	return fs_set->size();
}
//-----------------------------------------------------------------------------

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */
//-----------------------------------------------------------------------------
#endif /* __BCPLUSPLUS__ */
//-----------------------------------------------------------------------------
