/*
 * \file fzbase.hpp
 *
 *  Base class declaration
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
#ifndef __FZBASE_HPP__
#define __FZBASE_HPP__
//-----------------------------------------------------------------------------
#include <map>
#include <vector>
#include <string>
#include "fuzzy.hpp"
//-----------------------------------------------------------------------------

/**
 * Nombre de espacio para todas las clases, estructuras, tipos relacionados
 * con la librería de lógica difusa
 */
namespace nsFuzzy
{
	//-------------------------------------------------------------------------

	/**
	 *  Clase base de la librería fuzzy. Todas las demás clases heredan de esta.
	 */
	class TFuzzyBase
    {
    private:
		/** Número inválido */
		static fuzzvar m_fvInvalid;
		/** Objeto inválido */
        static TFuzzyBase m_fbInvalid;

		/** Lista de objetos "fuzzy" creados */
        static std::map<fzhndl, TFuzzyBase*> m_mObjects;
        /** Último descriptor asignado a un objeto registrado */
        static fzhndl                        m_fhLastHandle;

		/** Objeto padre de éste */
		TFuzzyBase* m_pParent;
        /** Descriptor de este objeto */
        fzhndl m_fzHandle;
        /** Tipo de objeto "fuzzy" */
        int    m_iType;

    protected:
        /** Nombre del objeto */
        std::string m_strName;
        /** Devuelve la lista con todos los objetos registrados.
         *
         * Todos los objetos de tipo "fuzzy" creados se registran en una lista.
         * Esto permite llevar un control de los objetos existentes, memoria
         * usada, e incluso memoria perdida.
         *
         * \return mapa con la lista de objetos registrados del sistema.
         * \sa m_mObjects
         */
        inline std::map<fzhndl, TFuzzyBase*>& objects() { return m_mObjects; }

        /** Convierte una cadena de texto a minúsculas */
		std::string& toLower(std::string& strRule);
		/** Elimina los espacios al principio y fin de una cadena */
		std::string& trim(std::string& strRule);

        /** Devuelve el objeto con handle especificado */
        //TFuzzyBase* operator[](fzhndl fhHandle);

    public:
        /** Constructor de un objeto de tipo "fuzzy" */
        TFuzzyBase(int Tipo = FL_ID_UNDEFINED);
        /** Destructor de un objeto "fuzzy" */
        virtual ~TFuzzyBase();

        /** Devuelve un número que representa un inválido
         *
         */
        static inline fuzzvar&    invalidNumber() { m_fvInvalid = 0xffffffff; return m_fvInvalid; }
        /** Devuelve un objeto que representa un valor inválido
         *
         */
        static inline TFuzzyBase& invalidObject() { return m_fbInvalid; }

        /** Devuelve el nombre del objeto.
         *
         * Retorna el nombre asignado al objeto.
         *
         * \return Nombre del objeto.
         */
        inline std::string name() { return m_strName; }
        /** Devuelve el objeto padre de esta instancia, en caso de existir
         *
         * Muchos de los objetos en esta librería están organizados
         * jerárquicamente. Así por ejemplo, un TFuzzySet contiene una serie de
         * TFuzzyVal.
         *
         * parent() devuelve un puntero al objeto padre en la jerarquía. En
         * caso de no existir dicho padre, retornará NULL. Esto puede ocurrir
         * bien porque no existe jerarquía para el tipo de objeto en cuestión o
         * porque es el objeto padre primero en su jerarquía.
         *
         * \return Puntero al objeto padre en la jerarquía. NULL en caso de
         * tener padre.
         * \sa m_pParent
         */
        inline TFuzzyBase*& parent() { return m_pParent; }
        /** Devuelve el descriptor del objeto instanciado
         *
         * A todos los objetos de tipo "TFuzzyBase" y heredados se les asigna
         * un descriptor único que sirve para identificarlos. Este método
         * permite obtener el descriptor del objeto instanciado.
         *
         * \return Descriptor del objeto.
         * \sa objects()
         */
        inline fzhndl handle() { return m_fzHandle; }
        /** Devuleve el tipo de objeto fuzzy que es.
         *
         * Los objetos "fuzzy" se dividen en varios tipos o categorías. Este
         * método retorna dicho tipo.
         *
         * \return Tipo de objeto "fuzzy"
         */
        inline int type() { return m_iType; }

        /** Cambia el nombre del objeto */
        void name(std::string strName);

        /** Compara dos objetos de tipo Fuzzy y determina si son iguales */
        virtual bool operator==(TFuzzyBase& cmp);
        /** Compara dos objetos de tipo Fuzzy y determina si son diferentes */
        virtual bool operator!=(TFuzzyBase& cmp);
    };
	//-------------------------------------------------------------------------

	/*
	 *
	 */
	class TFuzzyObjects : public TFuzzyBase
	{
	private:
	protected:
	public:
		/** Constructor de la clase */
		TFuzzyObjects();
		/** Destrcutor de la clase */
		virtual ~TFuzzyObjects();

		/** Hace pública la lista de objetos registrados en el sistema
		 *
		 *  Con este método se accede a la lista de objetos registrados en la
		 *  librería.
		 *
		 *  \return lista de objetos registrados en el sistema
		 *  \sa TFuzzyBase::objects()
		 */
        inline std::map<fzhndl, TFuzzyBase*>& objects() { return TFuzzyBase::objects(); }

        /** Devuelve el objeto con Id especificado. */
        TFuzzyBase& operator[](fzhndl fzId);
	};
	//-------------------------------------------------------------------------

	/** Código de error inválido */
	#define FL_INVALID_ERROR_CODE		0x80000000
	/** Código de error no definido */
	#define FL_UNDEFINED_ERROR_CODE		0x40000000
	/**
	 * Clase de manejo de excepciones de lógica difusa.
	 *
	 * Esta clase se usa para la propagación de excpeciones de la librería. La
	 * filosofía de la excepción es almacenar tres parámetros básicos para
	 * cualquier excepción, a saber;
	 *
	 * \li Qué produjo el error
	 * \li Por qué se produjo el error
	 * \li Dónde se produjo el error
	 *
	 * Qué produjo el error (m_strWhat) es una cadena de texto que debe ser
	 * rellenada por el programador en cada caso. Esta variable puede accederse
	 * mediante el método what().
	 *
	 * Por qué se produjo el error se almacena en un código numérico
	 * (m_uiError). Estos códigos numéricos y su correspondiente descripción
	 * están definidos en el archivo fzerrors.hpp. El método error() devuelve
	 * el código de error y why() el texto asociado.
	 *
	 * \note TFuzzyError almacena cualquier código de error que se le pase como
	 * parámetro, los propios de la librería y los del sistema.
	 *
	 * Dónde se produjo el error se almacena mediante dos variables: m_strFile
	 * guarda el nombre del archivo y m_uiLine el número de línea dentro del
	 * archivo donde se produjo el error. Estos dos valores deben ser
	 * proporcionados por el programador a la hora de lanzar una excepción. Se
	 * recomienda el uso de las directivas __FILE__ y __LINE__. Los métodos
	 * file() y line() devuelven el valor de estas variables. El método where()
	 * devuelve ambas variables representadas en una línea de texto con el
	 * formato file:line.
	 *
	 * Para finalizar, el método message() coloca en una cadena de texto toda
	 * la información disponible sobre la excepción: qué, dónde y por qué.
	 *
	 * \sa what(), where(), why(), file(), line(), error()
	 */
	class TFuzzyError
	{
	private:
		/** Línea del código donde se produjo el error */
		unsigned int m_uiLine;
		/** Nombre del archivo fuente donde se produjo el error */
		std::string  m_strFile;
		/** Qué produjo el error */
		std::string  m_strWhat;
		/** Por qué se produjo el error */
		std::string  m_strWhy;
		/** Código de error */
		unsigned int m_uiError;

	protected:
	public:
		/** Constructor básico */
		TFuzzyError();
		/** Constructor con código de error */
		TFuzzyError(const char* What, int iError,
						unsigned int uiLine, const char* cFile);
		/** Constructor con mensaje en formato texto */
		TFuzzyError(const char* What, const char* Why,
						unsigned int uiLine, const char* cFile);
		/** Destructor de la clase */
		virtual ~TFuzzyError();

		/** Devuelve el código de error producido. \sa m_uiError */
		inline unsigned int error() { return m_uiError; }
		/** Devuelve el 'qué' produjo el error. \sa m_strWhat*/
		inline std::string  what()  { return m_strWhat; }
		/** Devuelve el 'por qué' produjo el error. \sa m_strWhy */
		inline std::string  why()   { return m_strWhy;  }
		/** Devuelve el nombre del archivo donde se produjo el error.
		 * \sa m_strFile, line(), where() */
		inline std::string  file()  { return m_strFile; }
		/** Devuelve el número de línea en el archivo donde se produjo el error.
		 * \sa m_uiLine, file(), where() */
		inline unsigned int line()  { return m_uiLine;  }

		/** Devuelve en una cadena de texto 'dónde' se produjo el error. */
		std::string where();
		/** Devuelve una cadena de texto con toda la información completa del
		 * error. */
		std::string message();
	};
	//-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif /* __FZBASE_HPP__ */
//-----------------------------------------------------------------------------
