/**
 * \file fzset.cpp
 *
 *  A Set groups related "concepts" (cold, warm, hot) called Values, and
 *  defines a "characteristic" (temperature) in natural language ALWAYS within
 *  a given context. It is not the same to define what warm is in the context
 *  of stellar temperatures than in low temperature physics context.
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
 *
 */
//-----------------------------------------------------------------------------
#include "fzset.hpp"
//-----------------------------------------------------------------------------
using namespace nsFuzzy;
//-----------------------------------------------------------------------------

/**
 * \param [in] strName Nombre del conjunto
 *
 * Inicializa la clase, asignando un nombre al conjunto y especificando su tipo
 * (FL_ID_SET). El nombre, se pasa como parámetro (strName) al constructor y es
 * un parámetro obligatorio. El nombre se convierte a minúsculas.
 *
 * Pueden existir nombres duplicados, pero no se recomienda porque algunas
 * funciones (operator[](std::string)) no funcionarán como se espera.
 *
 * \return Nada.
 * \sa ~TFuzzySet()
 */
TFuzzySet::TFuzzySet(std::string strName) : TFuzzyBase(FL_ID_SET)
{
	m_strName  = strName;
    toLower(trim(m_strName));
}
//-----------------------------------------------------------------------------

/**
 *  Libera todos los recursos creados por la clase. Vacía la lista de valores
 *  (TFuzzyVal) y la memoria ocupada.
 *
 *  \return Nada
 *  \sa TFuzzySet()
 */
TFuzzySet::~TFuzzySet()
{
	clear();
}
//-----------------------------------------------------------------------------

/**
 * \param [in] strName Nombre del valor
 *
 * Devuelve el descriptor del valor (TFuzzyVal) con nombre strName. Si el valor
 * no existe como parte de este conjunto la función devuelve FL_INVALID_HANDLE.
 *
 * Es importante tener en cuenta que, si la función devuelve FL_INVALID_HANDLE
 * no significa que NO exista dicho valor en el sistema, sólo que no fomra
 * parte del conjunto consultado.
 *
 * \return Descriptor del valor con nombre strName. FL_INVALID_HANDLE en caso
 * de no existir.
 * \sa fzhndl, operator[]()
 */
fzhndl TFuzzySet::id(std::string strName)
{
	std::map<fzhndl, TFuzzyVal*>::iterator iter = m_fvValues.begin();

	while(iter != m_fvValues.end() && iter->second->name() != strName)
		++iter;

	return iter == m_fvValues.end() ? FL_INVALID_HANDLE : iter->first;
}
//-----------------------------------------------------------------------------

/**
 * Devuelve el valor inferior de la zona de influencia del conjunto. Este valor
 * es el valor mínimo del mínimo (TFuzzyVal::min()) de todos los valores
 * (TFuzzyVal) del conjunto.
 *
 * Este min() no siginifica que por debajo de este número el conjunto no esté
 * definido. Lo que indica es que, por debajo de este valor, el conjunto
 * devuelve siempre la misma respuesta, que depende de los valores que lo
 * componen.
 *
 * \return El límite inferior en el que el conjunto está definido.
 * \sa max(), TFuzzyVal::min()
 */
fuzzvar TFuzzySet::min()
{
    fuzzvar                                rtn  = 1E+64;
    std::map<fzhndl, TFuzzyVal*>::iterator iter = m_fvValues.begin();

    for(iter = m_fvValues.begin(); iter != m_fvValues.end(); ++iter)
    	rtn = iter->second->min() < rtn ? iter->second->min() : rtn;

    return rtn;
}
//-----------------------------------------------------------------------------

/**
 * Devuelve el valor superior de la zona de influencia del conjunto. Este valor
 * es el valor máximo del máximo (TFuzzyVal::max()) de todos los valores
 * (TFuzzyVal) del conjunto.
 *
 * Este max() no siginifica que por debajo de este número el conjunto no esté
 * definido. Lo que indica es que, por encima de este valor, el conjunto
 * devuelve siempre la misma respuesta, que depende de los valores que lo
 * componen.
 *
 * \return El límite superior en el que el conjunto está definido.
 * \sa min(), TFuzzyVal::max()
 */
fuzzvar TFuzzySet::max()
{
    fuzzvar                                rtn  = -1E+64;
    std::map<fzhndl, TFuzzyVal*>::iterator iter = m_fvValues.begin();

    for(iter = m_fvValues.begin(); iter != m_fvValues.end(); ++iter)
    	rtn = iter->second->max() > rtn ? iter->second->max() : rtn;

    return rtn;
}
//-----------------------------------------------------------------------------

/**
 * \param [in] strName Nombre a asignar al valor
 *
 * Crea un nuevo valor (TFuzzyVal) con nombre strName, y lo añade a la lista
 * de valores del conjunto. El nombre no tiene por qué ser único en el sistema,
 * pero se recomienda que no se dupliquen nombres porque hay funciones
 * (operator[](std::string)) que no devolverán los datos esperados.
 *
 * \return descriptor (fzhndl) del valor con nombre strName. FL_INVALID_HANDLE
 * en caso de no encontrarse.
 * \sa del(), operator[]()
 */
fzhndl TFuzzySet::add(std::string strName)
{
	fzhndl fv_id = INVALID_FUNC;

	if((*this)[strName] != invalidObject())
	{
		fv_id = id(strName);
	}
	else
	{
		TFuzzyVal* fv_aux = new TFuzzyVal(strName);
		fv_aux->parent()  = this;
		fv_id = fv_aux->handle();
		m_fvValues[fv_id] = fv_aux;
	}

	return fv_id;
}
//-----------------------------------------------------------------------------

/**
 * \param [in] fzValue Valor a eliminar
 *
 * Busca en la lista de valores (TFuzzyVal) del conjunto aquel cuyo descriptor
 * sea fzValue y lo elimina de la lista, liberando los recursos empleados por
 * éste. En caso de no encontrarse dicho valor la función no hace nada.
 *
 * \return Nada.
 * \sa clear(), add(), id()
 */
void TFuzzySet::del(fzhndl fzValue)
{
	std::map<fzhndl, TFuzzyVal*>::iterator found = m_fvValues.find(fzValue);

	if(found != m_fvValues.end())
	{
		delete found->second;
		m_fvValues.erase(found);
	}
}
//-----------------------------------------------------------------------------

/**
 * Vacía la lista de valores (TFuzzyVal) del conjunto, liberando también la
 * memoria ocupada por ellos.
 *
 * \return Nada.
 * \sa del(), add()
 */
void TFuzzySet::clear()
{
	while(m_fvValues.size())
	{
		if(m_fvValues.begin()->second)
			delete m_fvValues.begin()->second;
		m_fvValues.begin()->second = NULL;
		m_fvValues.erase(m_fvValues.begin());
	}
}
//-----------------------------------------------------------------------------

/**
 * \param [in] x Número a calcular.
 *
 * Este método se emplea en uno de los pasos intermedios a la hora de calcular
 * el resultado final de un modelo dada una entrada \a x.
 *
 * \return x
 * \sa TFuzzyVal::execute()
 */
fuzzvar TFuzzySet::execute(fuzzvar x)
{
    if(m_fvValues.size() > 0)
    {
		std::map<fzhndl, TFuzzyVal*>::iterator iter = m_fvValues.begin();
        for(; iter != m_fvValues.end(); ++iter)
        	iter->second->execute(x);
    }
    return x;
}
//-----------------------------------------------------------------------------

/**
 * \param [in] fzId Descriptor del valor (TFuzzyVal).
 *
 * \return Valor del conjunto con descriptor fzId. En caso de no encontrarse
 * dicho valor, retorna TFuzzyBase::invalid.
 * \sa operator[](int), operator[](std::string), size()
 */
TFuzzyVal& TFuzzySet::operator[](fzhndl fzId)
{
	std::map<fzhndl, TFuzzyVal*>::iterator found = m_fvValues.find(fzId);
	return found == m_fvValues.end() ? *((TFuzzyVal*)&invalidObject()) : *found->second;
}
//-----------------------------------------------------------------------------

/**
 * \param [in] iIndex Posición en la lista del valor.
 *
 * \return Valor del conjunto en la posición i Index. En caso de no encontrarse
 * dicho valor (out of bounds), retorna TFuzzyBase::invalid.
 * \sa operator[](fzhndl), operator[](std::string), size()
 */
TFuzzyVal& TFuzzySet::operator[](int iIndex)
{
	std::map<fzhndl, TFuzzyVal*>::iterator iter = m_fvValues.begin();
	int i_ndx = 0;

	while(iter != m_fvValues.end() && i_ndx++ < iIndex)
		++iter;

	return iter == m_fvValues.end() ? *((TFuzzyVal*)&invalidObject()) : *iter->second;
}
//-----------------------------------------------------------------------------

/**
 * \param [in] strName Nombre del valor.
 *
 * \return Valor del conjunto con nombre strName. En caso de no encontrarse
 * dicho valor, retorna TFuzzyBase::invalid.
 * \sa operator[](fzhndl), operator[](std::string), size()
 */
TFuzzyVal& TFuzzySet::operator[](std::string strName)
{
	std::map<fzhndl, TFuzzyVal*>::iterator iter = m_fvValues.begin();
	strName = toLower(trim(strName));

	while(iter != m_fvValues.end() && iter->second->name() != strName)
		++iter;

	return iter == m_fvValues.end() ? *((TFuzzyVal*)&invalidObject()) : *iter->second;
}
//-----------------------------------------------------------------------------









//-----------------------------------------------------------------------------

/**
 *
 */
TFuzzySets::TFuzzySets() : TFuzzyBase(FL_ID_SETS)
{
}
//-----------------------------------------------------------------------------

/**
 *
 */
TFuzzySets::~TFuzzySets()
{
	clear();
}
//-----------------------------------------------------------------------------

/**
 *
 */
fzhndl TFuzzySets::add(std::string strName)
{
	TFuzzySet* fs_new = NULL;
	std::map<fzhndl, TFuzzySet*>::iterator iter = m_mSets.begin();

	while(iter != m_mSets.end())
	{
		if(iter->second->name() == strName)
			break;
		++iter;
	}

	if(iter == m_mSets.end())
	{
		fs_new = new TFuzzySet(strName);
		fs_new->parent() = this;
		m_mSets[fs_new->handle()] = fs_new;
	}

	return iter == m_mSets.end() ?
			(fs_new ? fs_new->handle() : FL_INVALID_HANDLE) :
			iter->first;
}
//-----------------------------------------------------------------------------

/**
 *
 */
void TFuzzySets::del(fzhndl fhId)
{
	std::map<fzhndl, TFuzzySet*>::iterator found = m_mSets.find(fhId);
	if(found != m_mSets.end())
		m_mSets.erase(found);
}
//-----------------------------------------------------------------------------

/**
 *
 */
void TFuzzySets::clear()
{
	while(m_mSets.size() > 0)
	{
		if(m_mSets.begin()->second)
			delete m_mSets.begin()->second;
		m_mSets.begin()->second = NULL;
		m_mSets.erase(m_mSets.begin());
	}
}
//-----------------------------------------------------------------------------

/**
 *
 */
TFuzzySet& TFuzzySets::operator[](fzhndl fhId)
{
	std::map<fzhndl, TFuzzySet*>::iterator found = m_mSets.find(fhId);
	return found == m_mSets.end() ? *((TFuzzySet*)&invalidObject()) : *found->second;
}
//-----------------------------------------------------------------------------

/**
 *
 */
TFuzzySet& TFuzzySets::operator[](int iIndex)
{
	std::map<fzhndl, TFuzzySet*>::iterator iter = m_mSets.begin();
	int i_ndx = 0;

	while(iter != m_mSets.end() && i_ndx++ < iIndex)
		++iter;

	return iter == m_mSets.end() ? *((TFuzzySet*)&invalidObject()) : *iter->second;
}
//-----------------------------------------------------------------------------

/**
 *
 */
TFuzzySet& TFuzzySets::operator[](std::string strName)
{
	std::map<fzhndl, TFuzzySet*>::iterator iter = m_mSets.begin();

	while(iter != m_mSets.end() && iter->second->name() != strName)
		++iter;

	return iter == m_mSets.end() ? *((TFuzzySet*)&invalidObject()) : *iter->second;
}
//-----------------------------------------------------------------------------
