/**
 * \file fzval.cpp
 *
 *  A Value defines a set's adjective (warm, cold, hot, melting).
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
 */
//-----------------------------------------------------------------------------
#include "fzval.hpp"
#include <cmath>
//-----------------------------------------------------------------------------
using namespace nsFuzzy;
//-----------------------------------------------------------------------------
TFuzzyFunctions TFuzzyVal::m_ffFunctions;
//-----------------------------------------------------------------------------

/**
 * \param [in] strName Nombre identificativo para la instancia
 *
 * Constructor de la clase. Esta forma del constructor necesita como par�metro
 * el nombre con el que se le quiere identificar dentro de un set.
 *
 * \return Nada
 * \sa ~TFuzzyVal()
 */
TFuzzyVal::TFuzzyVal(std::string strName) : TFuzzyBase(FL_ID_VAL)
{
	if(strName.empty())
        throw TFuzzyError("contructor", "Nombre no especificado. El Objeto "
        				  "no puede construirse", __LINE__, __FILE__);

	m_fvMaxF  = TFuzzyBase::invalidNumber();
    m_ffId    = INVALID_FUNC;
    m_strName = strName;

	toLower(trim(m_strName));
}
//-----------------------------------------------------------------------------

/**
 * Destructor de la clase
 *
 * \return Nada
 * \sa TFuzzyVal()
 */
TFuzzyVal::~TFuzzyVal()
{
}
//-----------------------------------------------------------------------------

/**
 *  \param [in] x Valor de entrada a la funci�n
 *
 *  Esta funci�n recibe el valor x, lo aplica la funci�n asociada y retorna el
 *  resultado.
 *
 *  \return Resultado de aplicar el valor x a la funci�n definida para este
 *  valor.
 *  \sa FFuzzyFunction, execute()
 */
fuzzvar TFuzzyVal::exeFunction(fuzzvar x)
{
	if(INVALID_FUNC == m_ffId)
		return 0.0;

    if(!m_vParams.size())
    {
        size(0);
        m_vParams[0] = 0.0;
        m_vParams[1] = 10.0;
    }
    return m_ffFunctions[m_ffId]->Function(m_vParams, x);
}
//-----------------------------------------------------------------------------

/**
 * Calcula el punto donde la funci�n es m�xima
 */
fuzzvar TFuzzyVal::calcMaxFunc()
{
	fuzzvar x   = min();
	fuzzvar y   = 0.0;
	fuzzvar dy  = 2.0;
	fuzzvar Dx  = (max() - min()) / 2;
	fuzzvar de2 = 1.0E-6;

	while(dy > de2)
	{
		x += Dx;

		dy  = y;
		y   = exeFunction(x);

		if(y < dy)
			Dx /= -2.0;

		dy = pow(y - dy, 2.0);

		if(x > max())
		{
			x  = max();
			dy = 0.0;
		}
		else if(x < min())
		{
			x  = min();
			dy = 0.0;
		}
	}

	return x;
}
//-----------------------------------------------------------------------------

/**
 * \param [in] szCount N�mero de par�metros a asignar
 *
 * Asigna el n�mero de par�metros que necesita la funci�n para poder calcular
 * un resultado.
 *
 * La mayor�a de las funciones predeterminadas en la librer�a Fuzzy son
 * expresiones matem�ticas bien definidas y no necesitan ning�n par�metro
 * extra a parte del valor X sobre el que se quiere calcular el resultado. Para
 * funciones de interpolaci�n, por ejemplo, es necesario suministrar como
 * par�metros los puntos sobre los que debe extrapolar la funci�n.
 *
 * \return Nada
 * \sa operator[](), size()
 */
void TFuzzyVal::size(size_t szCount)
{
	size_t sz_cnt = szCount + 2;

	while(m_vParams.size() > sz_cnt)
		m_vParams.pop_back();
	while(m_vParams.size() < sz_cnt)
		m_vParams.push_back(0.0);
}
//-----------------------------------------------------------------------------

/**
 * \param [in] fvMin Valor m�nimo de la funci�n
 *
 * Este m�todo asigna el l�mite inferior sobre el que la funci�n est� definida.
 * Todo aquel valor \a x menor que fvMin devolver� como resultado a una llamada a
 * execute() el mismo valor que si <i>x = fvMin</i>.
 *
 * El prop�sito de esto es extender fuera de los l�mites definido el resultado
 * de caulquier funci�n.
 *
 * \return Nada
 * \sa max(), execute()
 */
void TFuzzyVal::min(fuzzvar fvMin)
{
	if(m_vParams.size() > 0)
		m_vParams[0] = fvMin;
	else
		m_vParams.push_back(fvMin);
}
//-----------------------------------------------------------------------------

/**
 * \param [in] fvMax Valor m�ximo de la funci�n
 *
 * Este m�todo asigna el l�mite superior sobre el que la funci�n est� definida.
 * Todo aquel valor \a x mayor que fvMax devolver� como resultado a una llamada a
 * execute() el mismo valor que si <i>x = fvMax</i>.
 *
 * El prop�sito de esto es extender fuera de los l�mites definido el resultado
 * de caulquier funci�n.
 *
 * \return Nada
 * \sa min(), execute()
 */
void TFuzzyVal::max(fuzzvar fvMax)
{
	switch(m_vParams.size())
	{
	case 0:
		m_vParams.push_back(0.0);

	case 1:
		m_vParams.push_back(fvMax);
		break;

	case 2:
	default:
		m_vParams[1] = fvMax;
		break;
	}
}
//-----------------------------------------------------------------------------

/**
 * \param [in] strName Nombre de la funci�n a asignar.
 *
 * Cada valor (TFuzzyVal) de un conjunto (TFuzzySet) est� asociado a una
 * funci�n que define su grado de verdad dentro del conjunto. Éste m�todo
 * asigna la funci�n a partir del nombre de la misma.
 *
 * La librer�a Fuzzy incorpora una serie de funciones consideradas est�ndar
 * que pueden emplearse de manera nativa. La definici�n de estas funciones y
 * sus nombres se encuentran en TStdFuzzyFunctions.
 *
 * \return Descriptor a la funci�n con nombre strName, FL_INVALID_HANDLE en
 * caso de no existir la funci�n.
 * \sa TStdFuzzyFunctions, TFuzzyFunctions
 */
fzhndl TFuzzyVal::setFunction(std::string strName)
{
	SFuzzyFunction* ff_func = m_ffFunctions[strName];
    m_ffId = ff_func ? ff_func->handle() : FL_INVALID_HANDLE;

    m_fvMaxF = calcMaxFunc();

    return m_ffId;
}
//-----------------------------------------------------------------------------

/**
 * \param [in] ffId Descriptor de la funci�n a asignar.
 *
 * Cada valor (TFuzzyVal) de un conjunto (TFuzzySet) est� asociado a una
 * funci�n que define su grado de verdad dentro del conjunto. Éste m�todo
 * asigna la funci�n a partir del descriptor de la misma.
 *
 * La librer�a Fuzzy incorpora una serie de funciones consideradas est�ndar
 * que pueden emplearse de manera nativa. La definici�n de estas funciones y
 * sus nombres y descriptores se encuentran en TStdFuzzyFunctions.
 *
 * \return Nombre de la funci�n con descriptor ffId, FL_INVALID_HANDLE en caso
 * de no existir el descriptor.
 * \sa TStdFuzzyFunctions, TFuzzyFunctions
 */
std::string TFuzzyVal::setFunction(fzhndl ffId)
{
	SFuzzyFunction* ff_func = m_ffFunctions[ffId];

	m_ffId = ff_func ? ffId : INVALID_FUNC;

    return ff_func ? ff_func->name() : std::string("");
}
//-----------------------------------------------------------------------------

/**
 * \param [in] iIndex Posici�n del par�metro.
 *
 * Para aquellas funciones con alg�n par�metro de configuraci�n, este operador
 * devuelve el valor del par�metro en la posici�n iIndex, siendo el primero el
 * par�metro 0. Para determinar cu�ntos par�metros tiene una funcion se puede
 * usar el m�todo size().
 *
 * \return Valor del par�metro en la posici�n iIndex, TFuzzyBase::invalid()
 * en caso de no existir dicha posici�n.
 * \sa size()
 */
fuzzvar& TFuzzyVal::operator[](int iIndex)
{
	return (iIndex >= 0 && size_t(iIndex) < size())
			? m_vParams[iIndex + 2] : invalidNumber();
}
//-----------------------------------------------------------------------------
