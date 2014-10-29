/**
 * \file fzval.hpp
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
 *
 */
//-----------------------------------------------------------------------------
#ifndef __FZVAL_HPP__
#define __FZVAL_HPP__
//-----------------------------------------------------------------------------
#include <vector>
#include "fzfunction.hpp"
//-----------------------------------------------------------------------------

namespace nsFuzzy
{
	//-------------------------------------------------------------------------

	/**
	 * Esta clase personaliza una funci�n en l�gica difusa para luego usarla
	 * en un set.
	 *
	 * \sa TFuzzySet
	 */
	class TFuzzyVal : public TFuzzyBase
	{
	private:
		/** Lista de todas funciones registradas */
		static TFuzzyFunctions m_ffFunctions;

		/** Punto donde la funci�n es m�xima */
		fuzzvar              m_fvMaxF;
		/** Lista de par�metros */
		std::vector<fuzzvar> m_vParams;
        /** Funci�n de grado de verdad */
        fzhndl               m_ffId;

        /** Ejecuta la funci�n */
        fuzzvar exeFunction(fuzzvar x);
        /** Calcula el punto donde la funci�n es m�xima */
        fuzzvar calcMaxFunc();

	protected:
	public:
        /** Constructor principal de la clase */
		TFuzzyVal(std::string strName);
		/** Destructor de la clase */
		virtual ~TFuzzyVal();

		/** N�mero de par�metros */
		inline size_t        size() { return m_vParams.size() > 1 ? m_vParams.size() - 2 : 0; }
		/** Valor m�nimo sobre el que aplicar la funci�n */
		inline fuzzvar       min()  { return m_vParams.size() > 0 ? m_vParams[0] : 0.0; }
		/** Valor m�ximo sobre el que aplicar la funci�n */
		inline fuzzvar       max()  { return m_vParams.size() > 1 ? m_vParams[1] : 0.0; }
		/** Devuelve el valor x donde la funci�n es m�xima */
		inline fuzzvar       maxF() { return m_fvMaxF; }

		/** Asigna el n�mero de par�metros */
		void size(size_t szCount);
		/** Asigna el valor m�nimo sobre el que aplicar la funci�n */
		void min(fuzzvar fvMin);
		/** Asigna el valor m�ximo sobre el que aplicar la funci�n */
		void max(fuzzvar fvMax);
		/** Asigna el valor del par�metro en la posici�n indicada */
		void set(int iIndex, fuzzvar Value);

		/** Devuelve la lista de funciones registradas para uso de la clase */
		inline TFuzzyFunctions& functions() { return m_ffFunctions;         }
		/** Devuelve la funci�n a la que est� asociada este valor */
		inline SFuzzyFunction* function()   { return m_ffFunctions[m_ffId]; }
		/** Asigna una funci�n para el grado de verdad a este valor */
		fzhndl setFunction(std::string strName);
		/** Asigna una funci�n para el grado de verdad a este valor */
		std::string setFunction(fzhndl ffId);

		/** Calcula el resultado de la funci�n de grado de verdad */
		inline fuzzvar execute(fuzzvar fvX) { return exeFunction(fvX); }
		/** Devuelve el par�metro en la posici�n especificada */
		fuzzvar& operator[](int iIndex);
	};
	//-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif /* __FZVAL_HPP__ */
//-----------------------------------------------------------------------------
