/**
 * \file fzfunction.hpp
 *
 *  Defines a function later to be used as the curve for a value in a set.
 *  This creates a repository of functions where values can share the
 *  mathematical evaluation function.
 */

/*
 *
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
#ifndef __FZFUNCTION_HPP__
#define __FZFUNCTION_HPP__
//-----------------------------------------------------------------------------
#include <string>
#include <map>
#include "fzbase.hpp"
//-----------------------------------------------------------------------------
#define INVALID_FUNC	0
//-----------------------------------------------------------------------------

namespace nsFuzzy
{
	//-------------------------------------------------------------------------

	/** Define una funci�n en el contexto de l�gica difusa.
	 *
	 * Las funciones en l�gica difusa corresponden a funciones matem�ticas.
	 * Definen el grado de verdad de un 'valor'. Los valores engloban ideas
	 * concretas pero vagamente definidas (fr�o, templado, alto, bajo...).
	 * Estos valores, a su vez, se agrupan en conjuntos seg�n conceptos
	 * (temperatura, altura...). La figura a continuaci�n muestra el conjunto
	 * 'temperatura' con tres 'valores' definidos mediante 'funciones'.
	 *
	 * \image html degree_of_truth.bmp "Grados de verdad, valores y conjuntos"
	 *
	 * Las funciones matem�ticas que definen un grado de verdad deben cumplir
	 * los siguientes requisitos:
	 *
	 * \li Estar definidas dentro del rango de valores de inter�s (la ventana).
	 * Esto significa que la funci�n debe retornar un s�lo valor �nico para
	 * cada valor de la variable independiente dentro de la ventana.
	 * \li Debe ser contin�a (su deriva no debe ser infinita) dentro de la
	 * ventana.
	 * \li La funci�n debe devolver un valor comprendido entre 0 y 1 (ambos
	 * inclusive), para todos los puntos dentro de la ventana. 0 significa un
	 * grado de verdad 'nulo' y un 1 un grado de 'completa' verdad.
	 *
	 * \sa TStdFuzzyFunctions, TFuzzyFunctions
	 */
	struct SFuzzyFunction : public TFuzzyBase
	{
        /** N�mero de par�metros que necesita (sin contar min y max) */
        int            ParamCount;
        /** Puntero a la funci�n */
        FFuzzyFunction Function;

        /** Constructor de la estructura */
        SFuzzyFunction();
        /** Destructor de la estructura */
        virtual ~SFuzzyFunction();

        /** Compara dos funciones y devuelve true si son iguales */
        virtual bool operator==(SFuzzyFunction& cmp);
        /** Compara dos funciones y devuelve false si son iguales */
        virtual bool operator!=(SFuzzyFunction& cmp);
	};
	//-------------------------------------------------------------------------

	/** Contenedor est�ndar de funciones para un entorno de l�gica difusa
	 *
	 *  Las funciones que contiene se usan para asignar valores a los conjuntos
	 *  de un modelo. La lista interna de funciones se est�tica, lo que
	 *  significa que la misma lista se comparte entre diferentes objetos de
	 *  este tipo (y sus heredados).
	 *
	 *  \remark TStdFuzzyFunctions se considera una clase base y no se
	 *  recomienda instanciar ning�n objeto de este tipo directamente. En su
	 *  lugar, instanciar un objeto de tipo TFuzzyFunctions.
	 *
	 *  \sa SFuzzyFunction, TFuzzyFunctions
	 */
	class TStdFuzzyFunctions : public TFuzzyBase
	{
    private:
    	/** Mapa con la lista de funciones registradas */
    	static std::map<fzhndl, SFuzzyFunction*> m_mFunctions;
    	/** Numero de instancias de esta clase */
    	static unsigned int                      m_uiInstances;

    	/** Limpia la lista de funciones registradas */
    	void clear();
    	/** Registra las funciones est�ndar disponibles */
    	void registerFunctions();
    	/** Devuelve el valor absoluto de un n�mero
    	 *
    	 * \param [in] x Valor del que obtener su absoluto.
    	 *
    	 * Es la versi�n de coma flotante de abs definido en math.h.
    	 *
    	 * \return el valor positivo del par�metro x
    	 */
    	static inline fuzzvar abs(fuzzvar x) { return x < 0 ? -x : x; }

    	//-- Funciones est�ndar
    	/** Gauss Bell Function */
        static fuzzvar gaussBell(fuzzlist& params, fuzzvar x);
    	/** An S-Curve function */
        static fuzzvar sCurve(fuzzlist& params, fuzzvar x);
    	/** An mirrored S-curved about the y-axis */
        static fuzzvar inverseSCurve(fuzzlist& params, fuzzvar x);
    	/** A triangle function */
        static fuzzvar triangle(fuzzlist& params, fuzzvar x);
    	/** An inverse triangle. A 'V'? */
        static fuzzvar inverseTriangle(fuzzlist& params, fuzzvar x);
        /** An interpolated curve */
        static fuzzvar interpolate(fuzzlist& params, fuzzvar x);

    protected:
        /** Hace accesible la lista de funciones a las clases derivadas
         *
         * \return mapa de las funciones registradas por la librer�a.
         * \sa m_mFunctions
         */
        std::map<fzhndl, SFuzzyFunction*>& functions() { return m_mFunctions; }

        /** Devuelve el n�mero de instancias existentes de esta clase */
        virtual inline unsigned int instances() { return m_uiInstances; }

    public:
        /** Constructor de la clase */
    	TStdFuzzyFunctions();
    	/** Destructor de la clase */
    	virtual ~TStdFuzzyFunctions();

    	/** N�mero de funciones registradas */
    	inline size_t size() { return m_mFunctions.size(); }

    	/** Registra una nueva funci�n en el sistema */
        fzhndl add(SFuzzyFunction* fFunc);
		/** Devuelve la funci�n con el id especificado */
		SFuzzyFunction* operator[](fzhndl ffId);
		/** Devuelve la funci�n en la posici�n especificada */
		SFuzzyFunction* operator[](int iIndex);
		/** Devuelve la funci�n con el nombre especificado */
		SFuzzyFunction* operator[](std::string strName);
    };
	//-------------------------------------------------------------------------

    class TFuzzyFunctions : public TStdFuzzyFunctions
    {
    private:
    protected:
    public:
    	TFuzzyFunctions();
    	virtual ~TFuzzyFunctions();
    };
	//-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif /* __FZFUNCTION_HPP__ */
//-----------------------------------------------------------------------------
