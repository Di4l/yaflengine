//-----------------------------------------------------------------------------
#ifndef __FZSET_HPP__
#define __FZSET_HPP__
//-----------------------------------------------------------------------------
#include "fzval.hpp"
//-----------------------------------------------------------------------------

namespace nsFuzzy
{
	//-------------------------------------------------------------------------

	/**
	 * Define un conjunto (set) en lógica difusa. Un conjunto define, mediante
	 * \a valores (TFuzzyVal), los diferentes estados de una propiedad \a
	 * medible. Un ejemplo de conjunto podría ser la temperatura de una
	 * caldera, y algunos de sus \a valores serían: Caliente, Frío, Templado...
	 *
	 * \sa TFuzzyVal, TFuzzySets, TFuzzyModel
	 */
	class TFuzzySet : public TFuzzyBase
	{
	private:
		/** Lista de valores del conjunto
		 *
		 *  Los valores (TFuzzyVal) que forman parte del conjunto (TFuzzySet)
		 *  se almacenan en un mapa, cuya llave es el descriptor (fzhndl) del
		 *  mismo. Esto permite búsquedas rápidas en la lista.
		 *
		 *  \sa id(), operator[]()
		 */
		std::map<fzhndl, TFuzzyVal*> m_fvValues;

	protected:
		/** Devuelve el descriptor de un valor (TFuzzyVal) dado su nombre */
		fzhndl id(std::string strName);

	public:
		/** Constructor principal de un conjunto */
		TFuzzySet(std::string strName);
		/** Destructor de un conjunto */
		virtual ~TFuzzySet();

		/** Número de valores del conjunto
		 *
		 *  Devuelve el número de valores (TFuzzyVal) del conjunto.
		 *
		 *  \return Número de valores que contiene el grupo
		 *  \sa m_fvValues, operator[]()
		 */
		inline size_t   size()   { return m_fvValues.size(); }

		/** Devuelve el límite inferior de influencia del conjunto */
		fuzzvar min();
		/** Devuelve el límite superior de influencia del conjunto */
		fuzzvar max();

		/** Añade un nuevo valor al conjunto */
		fzhndl add(std::string strName);
		/** ELimina en valor del conjunto */
		void   del(fzhndl fzValue);
		/** Limpia el conjunto, eliminando todos los valores definidos */
		void   clear();

		/** Calcula el resultado de todos los valores del conjunto para
		 * el valor x */
		fuzzvar execute(fuzzvar x);

		/** Devuelve un valor (TFuzyVal) dado su descriptor */
		TFuzzyVal& operator[](fzhndl fzId);
		/** Devuelve el valor (TFuzyVal) que ocupa la posición dada */
		TFuzzyVal& operator[](int iIndex);
		/** Devuelve un valor (TFuzyVal) dado su nombre */
		TFuzzyVal& operator[](std::string strName);
	};
	//-------------------------------------------------------------------------

	/**
	 * Mantiene una lista de conjuntos (TFuzzySet) para usar en un modelo
	 * (TFuzzyNModel).
	 *
	 * Un modelo (TFuzzyModel) de lógica difusa usa varios conjuntos y una
	 * serie de reglas (TFuzzyRules) que indican la relación que existe entre
	 * cada uno. Todos los conjuntos que participan en un modelo se agrupan en
	 * esta clase.
	 *
	 * \sa TFuzzySet, TFuzzyModel
	 */
	class TFuzzySets : public TFuzzyBase
	{
	private:
		/** Lista de los conjuntos (TFuzzySet). */
		std::map<fzhndl, TFuzzySet*> m_mSets;

	protected:
	public:
		/** Constructor principal de la clase. */
		TFuzzySets();
		/** Destructor de la clase. */
		virtual ~TFuzzySets();

		/** Número de conjuntos del grupo.
		 *
		 *  Devuelve el número de conjuntos (TFuzzySet) que contiene el grupo.
		 *
		 * \return Número de conjuntos (TFuzzySet) que forman el grupo.
		 * \sa m_mSets, operator[]()
		 */
		inline size_t size() { return m_mSets.size(); }

		/** Añade un nuevo conjunto (TFuzzySet) al grupo. */
		fzhndl add(std::string strName);
		/** Elimina un conjunto (TFuzzySet) del grupo. */
		void   del(fzhndl fhId);
		/** Vacía el grupo de los conjuntos (TFuzzySet) que contenga. */
		void   clear();

		/** Devuelve el conjunto (TFuzzySet) con el descriptor especificado. */
		TFuzzySet& operator[](fzhndl fhId);
		/** Devuelve el conjunto (TFuzzySet) en la posición especificada. */
		TFuzzySet& operator[](int iIndex);
		/** Devuelve el conjunto (TFuzzySet) con el nombre especificado. */
		TFuzzySet& operator[](std::string strName);
	};
	//-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif /* __FZSET_HPP__ */
//-----------------------------------------------------------------------------