/**
 * \file fzfunction.cpp
 *
 *  Defines a function later to be used as the curve for a value in a set.
 *  This creates a repository of functions where values can share the
 *  mathematical evaluation function.
 */

/*
 *     Copyright (C) 2014  Ra�l Hermoso S�nchez
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
 *    author: Ra�l Hermoso S�nchez
 *    e-mail: raul.hermoso@gmail.com
 *
 */
//-----------------------------------------------------------------------------
#include "fzfunction.hpp"
#ifdef __BORLANDC__
#include <math.h>
#else
#include <cmath>
#endif
//-----------------------------------------------------------------------------
using namespace nsFuzzy;
//-----------------------------------------------------------------------------

/**
 * Inicializa la informaci�n de una funci�n.
 *
 * \return Nada
 * \sa ~SFuzzyFunction()
 */
SFuzzyFunction::SFuzzyFunction() : TFuzzyBase(FL_ID_FUN)
{
	ParamCount = 0;
	Function   = NULL;
}
//-----------------------------------------------------------------------------

/**
 *  Se liberan los recursos empleados por la estructura. En este caso se
 *  inicializan todas las variables a sus valores por defecto.
 *
 * \return Nada
 * \sa SFuzzyFunction()
 */
SFuzzyFunction::~SFuzzyFunction()
{
	ParamCount = 0;
	Function   = NULL;
}
//-----------------------------------------------------------------------------

/**
 * Las funciones de grado de verdad dentro del sistema pueden accederse a
 * trav�s de su descriptor o de su nombre. Esto �ltimo requiere que el nombre
 * de cada funci�n dentro del sistema sea �nico.
 *
 * Con esta filosof�a, pueden darse los siguientes dos casos a tener en cuenta:
 *
 * \li Que dos o m�s funciones con nombre diferente apunten a la misma funci�n.
 * Esto est� permitido. Puede verse este caso como que los diferentes nombres
 * son alias de una misma funci�n.
 * \li Que dos o m�s funciones con el mismo nombre apunten a diferentes
 * funciones. Esto NO est� permitido. En el sistema queda registrada la primera
 * funci�n que se haya registrado. Las dem�s ser�n ignoradas.
 *
 * \return true si el Nombre de las funciones coincide. false en cualquier otro
 * caso.
 * \sa TStdFuzzyFunctions, TStdFuzzyFunctions::add()
 */
bool SFuzzyFunction::operator ==(SFuzzyFunction& cmp)
{
	return name() == cmp.name();
}
//-----------------------------------------------------------------------------

/*
 * Las funciones de grado de verdad dentro del sistema pueden accederse a
 * trav�s de su descriptor o de su nombre. Esto �ltimo requiere que el nombre
 * de cada funci�n dentro del sistema sea �nico.
 *
 * Con esta filosof�a, pueden darse los siguientes dos casos a tener en cuenta:
 *
 * \li Que dos o m�s funciones con nombre diferente apunten a la misma funci�n.
 * Esto est� permitido. Puede verse este caso como que los diferentes nombres
 * son alias de una misma funci�n.
 * \li Que dos o m�s funciones con el mismo nombre apunten a diferentes
 * funciones. Esto NO est� permitido. En el sistema queda registrada la primera
 * funci�n que se haya registrado. Las dem�s ser�n ignoradas.
 *
 * \return false si el Nombre de las funciones coincide. true en cualquier otro
 * caso.
 * \sa TStdFuzzyFunctions, TStdFuzzyFunctions::add()
 */
bool SFuzzyFunction::operator !=(SFuzzyFunction& cmp)
{
	return name() != cmp.name();
}
//-----------------------------------------------------------------------------







/**
 * Este mapa almacena todas las funciones registradas en la librer�a. Estas son
 * las funciones que definen los grados de verdad de los diferentes 'valores'.
 *
 * La librer�a implementa esta lista como est�tica, lo que significa que ser�
 * compartida por todas las instancias de TStdFuzzyFunctions y heredadas. Esto
 * ahorra espacio en memoria RAM, entre otras cosas.
 *
 * \sa m_ffLastId
 */
std::map<fzhndl, SFuzzyFunction*> TStdFuzzyFunctions::m_mFunctions;
//-----------------------------------------------------------------------------

/**
 *
 */
unsigned int TStdFuzzyFunctions::m_uiInstances = 0;
//-----------------------------------------------------------------------------

/**
 * Se inicializa la clase registrando todas las funciones est�ndar. Para evitar
 * registrarlas m�s de una vez se comprueba el n�mero de funciones ya
 * registradas. En caso de ser mayor o igual al n�mero de funciones est�ndar no
 * se registra nada, pues significa que alguna otra instancia lo ha hecho con
 * antelaci�n. De esta manera solo la primera instancia registra todas las
 * funciones est�ndar.
 *
 * \sa registerFunctions(), ~TStdFuzzyFunctions()
 */
TStdFuzzyFunctions::TStdFuzzyFunctions() : TFuzzyBase(FL_ID_FUNS)
{
	++m_uiInstances;
	registerFunctions();
}
//-----------------------------------------------------------------------------

/**
 * Libera los recursos usados por la instancia.
 *
 * \sa TStdFuzzyFunctions()
 */
TStdFuzzyFunctions::~TStdFuzzyFunctions()
{
	if(0 == --m_uiInstances)
		clear();
}
//-----------------------------------------------------------------------------

/**
 *
 */
void TStdFuzzyFunctions::clear()
{
	while(m_mFunctions.size() > 0)
	{
		if(m_mFunctions.begin()->second)
			delete m_mFunctions.begin()->second;
		m_mFunctions.begin()->second = NULL;
		m_mFunctions.erase(m_mFunctions.begin());
	}
}
//-----------------------------------------------------------------------------

/**
 * Registra todas las funciones est�ndar definidas por la librer�a. Para evitar
 * registrar m�s de una vez estas funciones se comprueba que la lista de
 * funciones registradas est� vac�a.
 */
void TStdFuzzyFunctions::registerFunctions()
{
	if(0 == m_mFunctions.size())
	{
		SFuzzyFunction* func = new SFuzzyFunction;

	    // Funci�n gaussiana
		func->name("Gaussian Bell");
		func->ParamCount = 0;
		func->Function   = gaussBell;
		add(func);

	    // Funci�n S
	    func = new SFuzzyFunction;
	    func->name("S-Curve");
	    func->ParamCount = 0;
	    func->Function   = sCurve;
	    add(func);

	    // Funci�n S invertida
	    func = new SFuzzyFunction;
	    func->name("Inverted S-Curve");
	    func->ParamCount = 0;
	    func->Function   = inverseSCurve;
	    add(func);

	    // Funci�n triangular
	    func = new SFuzzyFunction;
	    func->name("Triangle");
	    func->ParamCount = 1;
	    func->Function   = triangle;
	    add(func);

	    // Funci�n triangular
	    func = new SFuzzyFunction;
	    func->name("Inverted Triangle");
	    func->ParamCount = 1;
	    func->Function   = inverseTriangle;
	    add(func);

	    // Funci�n de interpolaci�n de puntos
	    func = new SFuzzyFunction;
	    func->name("Interpolate");
	    func->ParamCount = 0;
	    func->Function   = interpolate;
	    add(func);
	}
}
//-----------------------------------------------------------------------------

fuzzvar TStdFuzzyFunctions::gaussBell(fuzzlist& params, fuzzvar x)
{
    if(params.size() < 2)
        throw TFuzzyError("gausBell", "N�mero de par�metros incorrecto",
        					__LINE__, __FILE__);

    fuzzvar width  = (abs(params[0]) + abs(params[1])) / 2.0;
    fuzzvar center = (params[0] + params[1]) / 2.0;
    fuzzvar attenu = -log(0.001) / width / width;
    fuzzvar X = (x - center) * (x - center);

    return fuzzvar(exp(-attenu * X));
}
//-----------------------------------------------------------------------------

fuzzvar TStdFuzzyFunctions::sCurve(fuzzlist& params, fuzzvar x)
{
    if(params.size() < 2)
        throw TFuzzyError("sCurve", "N�mero de par�metros incorrecto",
        					__LINE__, __FILE__);

    if(x < params[0])
        return 0.0;
    if(x > params[1])
        return 1.0;

    fuzzvar rtn, A, center;
    A = 2.0 / (params[0] - params[1]) / (params[0] - params[1]);
    center = (params[0] + params[1]) / 2.0;

    rtn = x > center
        ? 1.0 - A * (x - params[1]) * (x - params[1])
        : A * (x - params[0]) * (x - params[0]);

    return rtn;
}
//-----------------------------------------------------------------------------

fuzzvar TStdFuzzyFunctions::inverseSCurve(fuzzlist& params, fuzzvar x)
{
    return 1.0 - sCurve(params, x);
}
//-----------------------------------------------------------------------------

fuzzvar TStdFuzzyFunctions::triangle(fuzzlist& params, fuzzvar x)
{
	size_t sz_cnt = params.size();

    if(sz_cnt < 2)
        throw TFuzzyError("triangle", "N�mero de par�metros incorrecto",
        					__LINE__, __FILE__);

    fuzzvar mid = sz_cnt >= 3 ? params[2] : (params[0] + params[1]) / 2.0;
    fuzzvar b   = 1.0 / (mid - (x < mid ? params[0] : params[1]));
    fuzzvar a   = 1.0 - b * mid;

    return fuzzvar((x > params[0] && x < params[1]) ? a + b * x : 0.0);
}
//-----------------------------------------------------------------------------

fuzzvar TStdFuzzyFunctions::inverseTriangle(fuzzlist& params, fuzzvar x)
{
    return fuzzvar(1.0 - triangle(params, x));
}
//-----------------------------------------------------------------------------

fuzzvar TStdFuzzyFunctions::interpolate(fuzzlist& params, fuzzvar x)
{
    // params viene en pares... x,y, x,y, x,y...
	size_t sz_cnt = params.size();

    if(sz_cnt < 2)
        throw TFuzzyError("interpolate", "N�mero de par�metros incorrecto",
        					__LINE__, __FILE__);
    if(sz_cnt % 2)
        throw TFuzzyError("interpolate", "N�mero de par�metros debe ser par",
        					__LINE__, __FILE__);

    fuzzvar rtn;
    size_t i_index = 0;
    while(i_index < sz_cnt && params[2 * i_index] < x)
        ++i_index;

    if(i_index >= sz_cnt)
        rtn = params[2 * i_index - 1];
    else
    {
        fuzzvar Dy = params[2 * i_index + 1] - params[2 * i_index - 1];
        fuzzvar Dx = params[2 * i_index] - params[2 * (i_index - 1)];
        fuzzvar dx = x - params[2 * (i_index - 1)];
        rtn = dx * Dy / Dx + params[2 * i_index - 1];
    }
    return rtn;
}
//-----------------------------------------------------------------------------

/**
 * \param [in] fFunc Funci�n a registrar
 *
 *
 */
fzhndl TStdFuzzyFunctions::add(SFuzzyFunction* fFunc)
{
	if(!fFunc)
		return INVALID_FUNC;
	if(fFunc->name().empty())
        throw TFuzzyError("add", "Nombre de funci�n vac�o", __LINE__, __FILE__);
	if(fFunc->Function == NULL)
        throw TFuzzyError("add", "Puntero a funci�n NULL", __LINE__, __FILE__);

	std::map<fzhndl, SFuzzyFunction*>::iterator iter = m_mFunctions.begin();

	while(iter != m_mFunctions.end() && fFunc != iter->second && *fFunc != *(iter->second))
		++iter;

	if(iter == m_mFunctions.end())
	{
		m_mFunctions[fFunc->handle()] = fFunc;
		return fFunc->handle();
	}
	else if(iter->second == fFunc || iter->second->name() == fFunc->name())
	{
		return iter->first;
	}

	return INVALID_FUNC;
}
//-----------------------------------------------------------------------------

/**
 * \param [in] ffId Descriptor de la funci�n
 *
 * Las funciones de grado de verdad se almacenan en la librer�a m_mFunctions.
 * A cada funci�n se le asigna un descriptor �nico que sirve para identifcarla
 * y poder acceder a ella.
 *
 * \return Puntero a la funci�n cuyo descriptor es ffId. NULL en caso de no
 * encontrarse dicha funci�n
 * \sa add(), operator[], m_mFunctions
 */
SFuzzyFunction* TStdFuzzyFunctions::operator[](fzhndl ffId)
{
	std::map<fzhndl, SFuzzyFunction*>::iterator found = m_mFunctions.find(ffId);

	return found == m_mFunctions.end() ? NULL : found->second;
}
//-----------------------------------------------------------------------------

/**
 * \param [in] iIndex Posici�n de la funci�n a devolver
 *
 * Las funciones de grado de verdad se almacenan en la librer�a m_mFunctions.
 * A cada funci�n se le asigna un descriptor �nico que sirve para identifcarla
 * y poder acceder a ella.
 *
 * \return Puntero a la funci�n en la posici�n iIndex. NULL en caso de no
 * encontrarse dicha funci�n
 * \sa add(), operator[], m_mFunctions
 */
SFuzzyFunction* TStdFuzzyFunctions::operator[](int iIndex)
{
	std::map<fzhndl, SFuzzyFunction*>::iterator iter = m_mFunctions.begin();
	int i_ndx = 0;

	while(iter != m_mFunctions.end() && i_ndx++ < iIndex)
		++iter;

	return iter == m_mFunctions.end() ? NULL : iter->second;
}
//-----------------------------------------------------------------------------

/**
 * \param [in] strName Nombre de la funci�n
 *
 * Las funciones de grado de verdad se almacenan en la librer�a m_mFunctions.
 * Cada funci�n tiene un nombre que la identifica y puede accederse a ella
 * mediante �ste.
 *
 * \return Puntero a la funci�n cuyo nombre es strName. NULL en caso de no
 * encontrarse dicha funci�n
 * \sa add(), operator[], m_mFunctions
 */
SFuzzyFunction* TStdFuzzyFunctions::operator[](std::string strName)
{
	toLower(trim(strName));
	std::map<fzhndl, SFuzzyFunction*>::iterator iter = m_mFunctions.begin();

	while(iter != m_mFunctions.end() && iter->second->name() != strName)
		++iter;

	return iter == m_mFunctions.end() ? NULL : iter->second;
}
//-----------------------------------------------------------------------------









/**
 *
 */
TFuzzyFunctions::TFuzzyFunctions() : TStdFuzzyFunctions()
{
}
//-----------------------------------------------------------------------------

/**
 *
 */
TFuzzyFunctions::~TFuzzyFunctions()
{
}
//-----------------------------------------------------------------------------
