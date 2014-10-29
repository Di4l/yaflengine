/**
 * \file fzbase.cpp
 *
 *  Base class definition. Almost all other classes shall inherit from this.
 */

/*     Copyright (C) 2014  Raúl Hermoso Sánchez
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
#include "fzbase.hpp"
#include <sstream>
#include <locale>
//-----------------------------------------------------------------------------
using namespace nsFuzzy;
//-----------------------------------------------------------------------------

std::map<fzhndl, TFuzzyBase*> TFuzzyBase::m_mObjects;
fzhndl                        TFuzzyBase::m_fhLastHandle = 0;
fuzzvar                       TFuzzyBase::m_fvInvalid    = 0xffffffff;
TFuzzyBase                    TFuzzyBase::m_fbInvalid;
//-----------------------------------------------------------------------------

/**
 * \param [in] Tipo Tipo de objeto "fuzzy"
 *
 * Inicializa un objeto "fuzzy", se le asigna un descriptor y se inserta en
 * la lista de objetos.
 *
 * Tipo indica el tipo de objeto creado. Útil para realizar búsquedas. Los
 * tipos actualmente definidos son:
 *
 * \li FL_ID_UNDEFINED Tipo NO definido
 * \li FL_ID_VAL       Un valor
 * \li FL_ID_SET       Un conjunto
 * \li FL_ID_FUN       Una función
 * \li FL_ID_RUL       Una regla
 * \li FL_ID_MOD       Un módulo
 *
 * \return nada
 * \sa ~TFuzzyBase(), m_mObjects
 */
TFuzzyBase::TFuzzyBase(int Tipo)
{
	m_fzHandle = ++m_fhLastHandle;
	m_pParent  = NULL;
	m_iType    = Tipo;
	m_strName  = "";

	m_mObjects[m_fzHandle] = this;
}
//-----------------------------------------------------------------------------

/**
 * Libera los recursos usados por un objeto "fuzzy" y se elimina de la lista de
 * objetos.
 *
 * Lo primero que se hace es asignar el tipo a "no definido", de manera que, si
 * por alguna razón el objeto no es sacado de la lista, se puede identificar
 * los objetos vagabundos buscando aquellos cuyo tipo sea FL_ID_UNDEFINED.
 *
 * \return Nada
 * \sa TFuzzyBase()
 */
TFuzzyBase::~TFuzzyBase()
{
	std::map<fzhndl, TFuzzyBase*>::iterator found = m_mObjects.find(m_fzHandle);

	m_iType = FL_ID_UNDEFINED;
	if(found != m_mObjects.end() && found->second == this)
		m_mObjects.erase(found);
}
//-----------------------------------------------------------------------------

/**
 * \param [in] strText Cadena a convertir
 *
 * \return Devuelve strText convertida a minúsculas
 */
std::string& TFuzzyBase::toLower(std::string& strText)
{
	std::locale loc;
	std::string aux = "";

	for(size_t i = 0; i < strText.size(); ++i)
		aux += (tolower(strText[i], loc));
	strText = aux;

	return strText;
}
//-----------------------------------------------------------------------------

/**
 * \param [in] strText Cadena de texto a ajustar
 *
 * \return strText sin los espacios al principio y final de la cadena
 */
std::string& TFuzzyBase::trim(std::string& strText)
{
	size_t pos = strText.find_first_not_of(" ");
	if(pos != std::string::npos)
		strText = strText.substr(pos);

	pos = strText.size() - 1;
	while(strText[pos] == ' ' && pos != 0)
		--pos;
	if(pos >= 0)
		strText = strText.substr(0, pos + 1);

	return strText;
}
//-----------------------------------------------------------------------------

/**
 * \param [in] fhHandle Descriptor del objeto a obtener
 *
 * Este operador devuelve el objeto cuyo descriptor sea fhHandle. El objeto
 * debe ser de tipo TFuzzyBase o derivado, de lo contrario no se encontrará en
 * la lista de objetos registrados por la librería.
 *
 * \return puntero al objeto "fuzzy" con descriptor fhHandle. NULL en caso de
 * no encontrarse.
 * \sa m_mObjects
 */
/*
TFuzzyBase* TFuzzyBase::operator[](fzhndl fhHandle)
{
	std::map<fzhndl, TFuzzyBase*>::iterator found = m_mObjects.find(fhHandle);
	return found == m_mObjects.end() ? NULL : found->second;
}*/
//-----------------------------------------------------------------------------

/**
 *  \param [in] strName Nombre a asignar a la función
 *
 * Este método asigna un nuevo nommbre al objeto.
 */
void TFuzzyBase::name(std::string strName)
{
	m_strName = strName;
	toLower(trim(m_strName));
}
//-----------------------------------------------------------------------------

/**
 * \param [in] cmp Objeto "fuzzy" con el que compararse
 *
 * Este operador primero comprueba si se trata del MISMO objeto (ambos tienen
 * la misma dirección de memoria). En caso contrario compara los tipos.
 *
 * \return true en caso de ser el mismo objeto o que los tipos sean iguales.
 * false en caso contrario
 * \sa m_iType, TFuzzyBase()
 */
bool TFuzzyBase::operator==(TFuzzyBase& cmp)
{
	return this == &cmp ? true : (m_iType == cmp.m_iType && m_strName == cmp.m_strName);
}
//-----------------------------------------------------------------------------

/**
 * \param [in] cmp Objeto "fuzzy" con el que compararse
 *
 * Este operador primero comprueba si se trata del MISMO objeto (ambos tienen
 * la misma dirección de memoria). En caso contrario compara los tipos y
 * nombres.
 *
 * \return true en caso de ser objetos diferentes o que los tipos o nombres
 * sean diferentes.
 * false en caso contrario
 * \sa m_iType, TFuzzyBase()
 */
bool TFuzzyBase::operator!=(TFuzzyBase& cmp)
{
	return this == &cmp ? false : (m_iType != cmp.m_iType || m_strName != cmp.m_strName);
}
//-----------------------------------------------------------------------------










/**
 *  Constrcutor único de la clase, no necesita inicializar nada, pues no tiene
 *  variables particulares, a parte de las que ya tiene TFuzzyBase.
 */
TFuzzyObjects::TFuzzyObjects() : TFuzzyBase(FL_ID_OBJS)
{
}
//-----------------------------------------------------------------------------

/**
 *  Libera los recursos empleados por la instancia.
 */
TFuzzyObjects::~TFuzzyObjects()
{
}
//-----------------------------------------------------------------------------

/**
 * \param [in] fzId Identificador del objeto a obtener.
 *
 * Devuelve el objeto especificado por el parámetro <i>fzId</i>
 *
 * \return Objeto con identificador <i>fzId</i>. En caso de no existir,
 * devuelve TFuzzyBase::invalid.
 *
 * \sa TFuzzyBase::invalid
 *
 */
TFuzzyBase& TFuzzyObjects::operator[](fzhndl fzId)
{
	std::map<fzhndl, TFuzzyBase*>::iterator found = objects().find(fzId);

	return found == objects().end() ? invalidObject() : *found->second;
}
//-----------------------------------------------------------------------------










/**
 * Constructor básico de la clase. Inicializa todas las variables a los
 * valores por defecto.
 *
 * \return Nada
 * \sa TFuzzyError(), ~TFuzzyError()
 */
TFuzzyError::TFuzzyError()
{
	m_uiError = FL_INVALID_ERROR_CODE;
	m_uiLine  = 0;
	m_strFile = "";
	m_strWhat = "";
	m_strWhy  = "";
}
//-----------------------------------------------------------------------------

/**
 * \param [in] What Qué es lo que ha producido el error
 * \param [in] iError Código del error producido
 * \param [in] uiLine Número de línea donde se ha producido el error
 * \param [in] cFile Archivo donde se produjo el error
 *
 * Este constructor recibe cuatro parámetros. 'What' es una cadena de texto que
 * indica qué es lo que falló. 'iError' identifica el por qué ha fallado (NULL
 * pointer, etc...). 'cFile' es el nombre del archivo fuente donde se ha
 * producido el error y 'uiLine' es la línea en el archivo. Juntos forman el
 * 'dónde'.
 *
 * \return Nada
 * \sa TFuzzyError(), ~TFuzzyError()
 */
TFuzzyError::TFuzzyError(const char* What, int iError,
						unsigned int uiLine, const char* cFile)
{
	m_uiError = iError;
	m_uiLine  = uiLine;
	m_strFile = std::string(cFile);
	m_strWhat = std::string(What);
	m_strWhy  = "";
}
//-----------------------------------------------------------------------------

/**
 * \param [in] What Qué es lo que ha producido el error
 * \param [in] Why  Por qué se ha producido el error
 * \param [in] uiLine Número de línea donde se ha producido el error
 * \param [in] cFile Archivo donde se produjo el error
 *
 * Este constructor recibe cuatro parámetros. 'What' es una cadena de texto que
 * indica qué es lo que falló. 'Why' determina el por qué ha fallado (NULL
 * pointer, etc...). 'cFile' es el nombre del archivo fuente donde se ha
 * producido el error y 'uiLine' es la línea en el archivo. Juntos forman el
 * 'dónde'.
 *
 * \return Nada
 * \sa TFuzzyError(), ~TFuzzyError()
 */
TFuzzyError::TFuzzyError(const char* What, const char* Why,
						unsigned int uiLine, const char* cFile)
{
	m_uiError = FL_UNDEFINED_ERROR_CODE;
	m_uiLine  = uiLine;
	m_strFile = std::string(cFile);
	m_strWhat = std::string(What);
	m_strWhy  = std::string(Why);
}
//-----------------------------------------------------------------------------

/**
 * Destructor de la clase.
 *
 * \return Nada.
 * \sa TFuzzyError()
 */
TFuzzyError::~TFuzzyError()
{
	m_uiError = FL_INVALID_ERROR_CODE;
	m_uiLine  = 0;
	m_strFile = "";
	m_strWhat = "";
	m_strWhy  = "";
}
//-----------------------------------------------------------------------------

/**
 * Devuelve el 'dónde' como una cadena de texto en un formato más o menos
 * estándar. <nombre_fichero>:<linea>
 *
 * \return Cadena de texto con el lugar donde se produjo el error.
 * \sa line(), file()
 */
std::string TFuzzyError::where()
{
	std::stringstream ss;
	ss << m_strFile << ":" << m_uiLine;

	return ss.str();
}
//-----------------------------------------------------------------------------

/**
 * Devuelve el 'dónde' como una cadena de texto en un formato más o menos
 * estándar. <nombre_fichero>:<linea>
 *
 * \return Cadena de texto con el lugar donde se produjo el error.
 * \sa line(), file()
 */
std::string TFuzzyError::message()
{
	std::stringstream ss;
	ss << m_strFile << ":" << m_uiLine << ": (error " << m_uiError << ") "
	   << m_strWhat << ": " << m_strWhy;

	return ss.str();
}
//-----------------------------------------------------------------------------
