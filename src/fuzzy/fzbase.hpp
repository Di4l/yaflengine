/**
 * \file fzbase.hpp
 *
 *  Base class declaration
 */

/*     Copyright (C) 2014  Ra�l Hermoso S�nchez
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
 * con la librer�a de l�gica difusa
 */
namespace nsFuzzy
{
	//-------------------------------------------------------------------------

	/**
	 *  Clase base de la librer�a fuzzy. Todas las dem�s clases heredan de esta.
	 */
	class TFuzzyBase
    {
    private:
		/** N�mero inv�lido */
		static fuzzvar m_fvInvalid;
		/** Objeto inv�lido */
        static TFuzzyBase m_fbInvalid;

		/** Lista de objetos "fuzzy" creados */
        static std::map<fzhndl, TFuzzyBase*> m_mObjects;
        /** �ltimo descriptor asignado a un objeto registrado */
        static fzhndl                        m_fhLastHandle;

		/** Objeto padre de �ste */
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

        /** Convierte una cadena de texto a min�sculas */
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

        /** Devuelve un n�mero que representa un inv�lido
         *
         */
        static inline fuzzvar&    invalidNumber() { m_fvInvalid = 0xffffffff; return m_fvInvalid; }
        /** Devuelve un objeto que representa un valor inv�lido
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
         * Muchos de los objetos en esta librer�a est�n organizados
         * jer�rquicamente. As� por ejemplo, un TFuzzySet contiene una serie de
         * TFuzzyVal.
         *
         * parent() devuelve un puntero al objeto padre en la jerarqu�a. En
         * caso de no existir dicho padre, retornar� NULL. Esto puede ocurrir
         * bien porque no existe jerarqu�a para el tipo de objeto en cuesti�n o
         * porque es el objeto padre primero en su jerarqu�a.
         *
         * \return Puntero al objeto padre en la jerarqu�a. NULL en caso de
         * tener padre.
         * \sa m_pParent
         */
        inline TFuzzyBase*& parent() { return m_pParent; }
        /** Devuelve el descriptor del objeto instanciado
         *
         * A todos los objetos de tipo "TFuzzyBase" y heredados se les asigna
         * un descriptor �nico que sirve para identificarlos. Este m�todo
         * permite obtener el descriptor del objeto instanciado.
         *
         * \return Descriptor del objeto.
         * \sa objects()
         */
        inline fzhndl handle() { return m_fzHandle; }
        /** Devuleve el tipo de objeto fuzzy que es.
         *
         * Los objetos "fuzzy" se dividen en varios tipos o categor�as. Este
         * m�todo retorna dicho tipo.
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

		/** Hace p�blica la lista de objetos registrados en el sistema
		 *
		 *  Con este m�todo se accede a la lista de objetos registrados en la
		 *  librer�a.
		 *
		 *  \return lista de objetos registrados en el sistema
		 *  \sa TFuzzyBase::objects()
		 */
        inline std::map<fzhndl, TFuzzyBase*>& objects() { return TFuzzyBase::objects(); }

        /** Devuelve el objeto con Id especificado. */
        TFuzzyBase& operator[](fzhndl fzId);
	};
	//-------------------------------------------------------------------------

	/** C�digo de error inv�lido */
	#define FL_INVALID_ERROR_CODE		0x80000000
	/** C�digo de error no definido */
	#define FL_UNDEFINED_ERROR_CODE		0x40000000
	/**
	 * Clase de manejo de excepciones de l�gica difusa.
	 *
	 * Esta clase se usa para la propagaci�n de excpeciones de la librer�a. La
	 * filosof�a de la excepci�n es almacenar tres par�metros b�sicos para
	 * cualquier excepci�n, a saber;
	 *
	 * \li Qu� produjo el error
	 * \li Por qu� se produjo el error
	 * \li D�nde se produjo el error
	 *
	 * Qu� produjo el error (m_strWhat) es una cadena de texto que debe ser
	 * rellenada por el programador en cada caso. Esta variable puede accederse
	 * mediante el m�todo what().
	 *
	 * Por qu� se produjo el error se almacena en un c�digo num�rico
	 * (m_uiError). Estos c�digos num�ricos y su correspondiente descripci�n
	 * est�n definidos en el archivo fzerrors.hpp. El m�todo error() devuelve
	 * el c�digo de error y why() el texto asociado.
	 *
	 * \note TFuzzyError almacena cualquier c�digo de error que se le pase como
	 * par�metro, los propios de la librer�a y los del sistema.
	 *
	 * D�nde se produjo el error se almacena mediante dos variables: m_strFile
	 * guarda el nombre del archivo y m_uiLine el n�mero de l�nea dentro del
	 * archivo donde se produjo el error. Estos dos valores deben ser
	 * proporcionados por el programador a la hora de lanzar una excepci�n. Se
	 * recomienda el uso de las directivas __FILE__ y __LINE__. Los m�todos
	 * file() y line() devuelven el valor de estas variables. El m�todo where()
	 * devuelve ambas variables representadas en una l�nea de texto con el
	 * formato file:line.
	 *
	 * Para finalizar, el m�todo message() coloca en una cadena de texto toda
	 * la informaci�n disponible sobre la excepci�n: qu�, d�nde y por qu�.
	 *
	 * \sa what(), where(), why(), file(), line(), error()
	 */
	class TFuzzyError
	{
	private:
		/** L�nea del c�digo donde se produjo el error */
		unsigned int m_uiLine;
		/** Nombre del archivo fuente donde se produjo el error */
		std::string  m_strFile;
		/** Qu� produjo el error */
		std::string  m_strWhat;
		/** Por qu� se produjo el error */
		std::string  m_strWhy;
		/** C�digo de error */
		unsigned int m_uiError;

	protected:
	public:
		/** Constructor b�sico */
		TFuzzyError();
		/** Constructor con c�digo de error */
		TFuzzyError(const char* What, int iError,
						unsigned int uiLine, const char* cFile);
		/** Constructor con mensaje en formato texto */
		TFuzzyError(const char* What, const char* Why,
						unsigned int uiLine, const char* cFile);
		/** Destructor de la clase */
		virtual ~TFuzzyError();

		/** Devuelve el c�digo de error producido. \sa m_uiError */
		inline unsigned int error() { return m_uiError; }
		/** Devuelve el 'qu�' produjo el error. \sa m_strWhat*/
		inline std::string  what()  { return m_strWhat; }
		/** Devuelve el 'por qu�' produjo el error. \sa m_strWhy */
		inline std::string  why()   { return m_strWhy;  }
		/** Devuelve el nombre del archivo donde se produjo el error.
		 * \sa m_strFile, line(), where() */
		inline std::string  file()  { return m_strFile; }
		/** Devuelve el n�mero de l�nea en el archivo donde se produjo el error.
		 * \sa m_uiLine, file(), where() */
		inline unsigned int line()  { return m_uiLine;  }

		/** Devuelve en una cadena de texto 'd�nde' se produjo el error. */
		std::string where();
		/** Devuelve una cadena de texto con toda la informaci�n completa del
		 * error. */
		std::string message();
	};
	//-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif /* __FZBASE_HPP__ */
//-----------------------------------------------------------------------------
