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

	/** Define una función en el contexto de lógica difusa.
	 *
	 * Las funciones en lógica difusa corresponden a funciones matemáticas.
	 * Definen el grado de verdad de un 'valor'. Los valores engloban ideas
	 * concretas pero vagamente definidas (frío, templado, alto, bajo...).
	 * Estos valores, a su vez, se agrupan en conjuntos según conceptos
	 * (temperatura, altura...). La figura a continuación muestra el conjunto
	 * 'temperatura' con tres 'valores' definidos mediante 'funciones'.
	 *
	 * \image html degree_of_truth.bmp "Grados de verdad, valores y conjuntos"
	 *
	 * Las funciones matemáticas que definen un grado de verdad deben cumplir
	 * los siguientes requisitos:
	 *
	 * \li Estar definidas dentro del rango de valores de interés (la ventana).
	 * Esto significa que la función debe retornar un sólo valor único para
	 * cada valor de la variable independiente dentro de la ventana.
	 * \li Debe ser continúa (su deriva no debe ser infinita) dentro de la
	 * ventana.
	 * \li La función debe devolver un valor comprendido entre 0 y 1 (ambos
	 * inclusive), para todos los puntos dentro de la ventana. 0 significa un
	 * grado de verdad 'nulo' y un 1 un grado de 'completa' verdad.
	 *
	 * \sa TStdFuzzyFunctions, TFuzzyFunctions
	 */
	struct SFuzzyFunction : public TFuzzyBase
	{
        /** Número de parámetros que necesita (sin contar min y max) */
        int            ParamCount;
        /** Puntero a la función */
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

	/** Contenedor estándar de funciones para un entorno de lógica difusa
	 *
	 *  Las funciones que contiene se usan para asignar valores a los conjuntos
	 *  de un modelo. La lista interna de funciones se estática, lo que
	 *  significa que la misma lista se comparte entre diferentes objetos de
	 *  este tipo (y sus heredados).
	 *
	 *  \remark TStdFuzzyFunctions se considera una clase base y no se
	 *  recomienda instanciar ningún objeto de este tipo directamente. En su
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
    	/** Registra las funciones estándar disponibles */
    	void registerFunctions();
    	/** Devuelve el valor absoluto de un número
    	 *
    	 * \param [in] x Valor del que obtener su absoluto.
    	 *
    	 * Es la versión de coma flotante de abs definido en math.h.
    	 *
    	 * \return el valor positivo del parámetro x
    	 */
    	static inline fuzzvar abs(fuzzvar x) { return x < 0 ? -x : x; }

    	//-- Funciones estándar
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
         * \return mapa de las funciones registradas por la librería.
         * \sa m_mFunctions
         */
        std::map<fzhndl, SFuzzyFunction*>& functions() { return m_mFunctions; }

        /** Devuelve el número de instancias existentes de esta clase */
        virtual inline unsigned int instances() { return m_uiInstances; }

    public:
        /** Constructor de la clase */
    	TStdFuzzyFunctions();
    	/** Destructor de la clase */
    	virtual ~TStdFuzzyFunctions();

    	/** Número de funciones registradas */
    	inline size_t size() { return m_mFunctions.size(); }

    	/** Registra una nueva función en el sistema */
        fzhndl add(SFuzzyFunction* fFunc);
		/** Devuelve la función con el id especificado */
		SFuzzyFunction* operator[](fzhndl ffId);
		/** Devuelve la función en la posición especificada */
		SFuzzyFunction* operator[](int iIndex);
		/** Devuelve la función con el nombre especificado */
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
