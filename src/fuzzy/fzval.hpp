/**
 * \file fzval.hpp
 *
 *  A Value defines a set's adjective (warm, cold, hot, melting).
 *
 *     Copyright (C) 2014  Ra鷏 Hermoso S醤chez
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
 *    author: Ra鷏 Hermoso S醤chez
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
	 * Esta clase personaliza una funci贸n en l贸gica difusa para luego usarla
	 * en un set.
	 *
	 * \sa TFuzzySet
	 */
	class TFuzzyVal : public TFuzzyBase
	{
	private:
		/** Lista de todas funciones registradas */
		static TFuzzyFunctions m_ffFunctions;

		/** Punto donde la funci贸n es m谩xima */
		fuzzvar              m_fvMaxF;
		/** Lista de par谩metros */
		std::vector<fuzzvar> m_vParams;
        /** Funci贸n de grado de verdad */
        fzhndl               m_ffId;

        /** Ejecuta la funci贸n */
        fuzzvar exeFunction(fuzzvar x);
        /** Calcula el punto donde la funci贸n es m谩xima */
        fuzzvar calcMaxFunc();

	protected:
	public:
        /** Constructor principal de la clase */
		TFuzzyVal(std::string strName);
		/** Destructor de la clase */
		virtual ~TFuzzyVal();

		/** N煤mero de par谩metros */
		inline size_t        size() { return m_vParams.size() > 1 ? m_vParams.size() - 2 : 0; }
		/** Valor m铆nimo sobre el que aplicar la funci贸n */
		inline fuzzvar       min()  { return m_vParams.size() > 0 ? m_vParams[0] : 0.0; }
		/** Valor m谩ximo sobre el que aplicar la funci贸n */
		inline fuzzvar       max()  { return m_vParams.size() > 1 ? m_vParams[1] : 0.0; }
		/** Devuelve el valor x donde la funci贸n es m谩xima */
		inline fuzzvar       maxF() { return m_fvMaxF; }

		/** Asigna el n煤mero de par谩metros */
		void size(size_t szCount);
		/** Asigna el valor m铆nimo sobre el que aplicar la funci贸n */
		void min(fuzzvar fvMin);
		/** Asigna el valor m谩ximo sobre el que aplicar la funci贸n */
		void max(fuzzvar fvMax);
		/** Asigna el valor del par谩metro en la posici贸n indicada */
		void set(int iIndex, fuzzvar Value);

		/** Devuelve la lista de funciones registradas para uso de la clase */
		inline TFuzzyFunctions& functions() { return m_ffFunctions;         }
		/** Devuelve la funci贸n a la que est谩 asociada este valor */
		inline SFuzzyFunction* function()   { return m_ffFunctions[m_ffId]; }
		/** Asigna una funci贸n para el grado de verdad a este valor */
		fzhndl setFunction(std::string strName);
		/** Asigna una funci贸n para el grado de verdad a este valor */
		std::string setFunction(fzhndl ffId);

		/** Calcula el resultado de la funci贸n de grado de verdad */
		inline fuzzvar execute(fuzzvar fvX) { return exeFunction(fvX); }
		/** Devuelve el par谩metro en la posici贸n especificada */
		fuzzvar& operator[](int iIndex);
	};
	//-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif /* __FZVAL_HPP__ */
//-----------------------------------------------------------------------------
