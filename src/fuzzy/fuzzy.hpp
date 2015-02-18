/**
 * \file fuzzy.hpp
 * Main project include file.
 *
 * This files includes general definitions for the fuzzy framework that
 * shall be used throughout the project.
 */

/*    Copyright (C) 2014  Ra�l Hermoso S�nchez
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
 *    e-mail: <mailto:raul.hermoso@gmail.com>
 */
//-----------------------------------------------------------------------------
#ifndef __FUZZY_HPP__
#define __FUZZY_HPP__
//-----------------------------------------------------------------------------
#ifdef WIN32
	#include <windows.h>
	#define DCAST(x,y)	dynamic_cast<x>(y)
#else
	#define DCAST(x,y) (x)y
#endif
//-----------------------------------------------------------------------------
#include <vector>
#include <map>
//-----------------------------------------------------------------------------

//namespace nsFuzzy
//{
	//-------------------------------------------------------------------------
	/** Tipo que define un valor */
	typedef float                     fuzzvar;
	/** Descriptor que identifica un objeto de esta librer�a */
	typedef unsigned int              fzhndl;
	/** Tipo que define una lista de par�metros */
	typedef std::vector<fuzzvar>      fuzzlist;
	/** Tipo que define un mapa de par�metros */
	typedef std::map<fzhndl, fuzzvar> fuzzmap;
	/** Prototipo de funci�n */
	typedef fuzzvar (*FFuzzyFunction)(fuzzlist&, fuzzvar);
	//-------------------------------------------------------------------------
//};
//-----------------------------------------------------------------------------

/** Descriptor incorrecto o inv�lido */
#define FL_INVALID_HANDLE	0x80000000
//-----------------------------------------------------------------------------

// Para cada clase derivada de TFuzzyBase debe existir un identificador...

/** Clase no definida */
#define FL_ID_UNDEFINED 0x7FFFFFFF
/** Clase objetos */
#define FL_ID_OBJS      0x01
/** Clase funci�n */
#define FL_ID_FUN       0x02
/** Clase funciones */
#define FL_ID_FUNS      0x03
/** Clase valor */
#define FL_ID_VAL       0x04
/** Clase conjunto/set */
#define FL_ID_SET       0x05
/** Clase lista de sets */
#define FL_ID_SETS      0x06
/** Clase regla */
#define FL_ID_RUL       0x07
/** Clase �rbol de m�dulo */
#define FL_ID_MOD_TREE  0x08
/** Clase m�dulo */
#define FL_ID_MOD       0x09
/** Clase m�dulo */
#define FL_ID_MODS      0x0A
/** Clase ejecuci�n */
#define FL_ID_EXEC		0x0B
//-----------------------------------------------------------------------------

#ifdef __BCPLUSPLUS__
#include <string>
//-------------------------------------------------------------------------

#ifndef DLL_FUNC_PROTO
	#ifdef FZ_BUILD
		#define DLL_FUNC_PROTO	__declspec(dllexport)
	#else
		#define DLL_FUNC_PROTO __declspec(dllexport)
	#endif
#endif
//-------------------------------------------------------------------------


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
	//-------------------------------------------------------------------------

	struct FZ_INIT
	{
		std::string library;
		std::string author;
		std::string version;
	};
	//-------------------------------------------------------------------------

	struct FZ_OBJECT
	{
		fzhndl      id;
		std::string name;
		int         type;
	};
	//-------------------------------------------------------------------------

	struct FZ_LIST
	{
		FZ_OBJECT* object;
		FZ_LIST*   next;
	};
	//-------------------------------------------------------------------------

	FZ_INIT* DLL_FUNC_PROTO fuzzy_init();
	void     DLL_FUNC_PROTO fuzzy_close();
	FZ_LIST* DLL_FUNC_PROTO fuzzy_list_create();
	void     DLL_FUNC_PROTO fuzzy_list_free(FZ_LIST* fzLst);
	fzhndl   DLL_FUNC_PROTO fuzzy_use_model(fzhndl fzId);
	fzhndl   DLL_FUNC_PROTO fuzzy_model_create();
	void     DLL_FUNC_PROTO fuzzy_model_free(fzhndl fzId);
	void     DLL_FUNC_PROTO fuzzy_model_load(std::string& strFileName,
										fzhndl fzId = FL_INVALID_HANDLE);
	void     DLL_FUNC_PROTO fuzzy_model_save(std::string& strFileName,
										fzhndl fzId = FL_INVALID_HANDLE);
	void     DLL_FUNC_PROTO fuzzy_model_input(fzhndl fzSetId, fuzzvar fvValue);
	void     DLL_FUNC_PROTO fuzzy_model_calculate(fzhndl fzId = FL_INVALID_HANDLE);
	fuzzvar  DLL_FUNC_PROTO fuzzy_model_output(fzhndl fzSetId);
	size_t   DLL_FUNC_PROTO fuzzy_model_get_sets(fzhndl fzId, FZ_LIST*& fzList);
	size_t   DLL_FUNC_PROTO fuzzy_set_get_values(fzhndl fzSetId, FZ_LIST*& fzList);
	//-------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif /* __cplusplus */
//-----------------------------------------------------------------------------
#endif /* __BCPLUSPLUS__ */
//-----------------------------------------------------------------------------
#endif /* __FUZZY_HPP__ */
//-----------------------------------------------------------------------------

/**
 * \mainpage
 *
 * \section sec_intro Introducci�n
 *
 * El nombre Yaflengine viene de <em>Y otro motor de l�gica difusa</em> por sus
 * siglas en ingl�s. Es una librer�a que le permite realizar c�lculos usando
 * <a href="http://es.wikipedia.org/wiki/L%C3%B3gica_difusa">l�gica difusa</a>.
 * Est� escrita en su totalidad en C++ y compila tanto en Windows como en
 * Linux.
 *
 * Con Yaflengine podr�:
 *
 * \li Crear/modificar en tiempo de ejecuci�n modelos.
 * \li Guardar/Cargar modelos. El formato de los archivos es el de los
 * <a href="http://es.wikipedia.org/wiki/INI_(extensi%C3%B3n_de_archivo)">archivos .ini</a>
 * de Windows.
 * \li Concatenar \c N modelos.
 *
 * Yaflengine esta escrito completamente en C++ y compila tanto en Windows como
 * en Linux. El autor ha compilado el motor en una
 * <a href="http://es.wikipedia.org/wiki/Raspberry_Pi">Rasberry Pi</a> corriendo
 * <a href="http://es.wikipedia.org/wiki/Arch_Linux">Arch Linux</a>.
 *
 * La documentaci�n est� dividida en dos partes, cada una subdividida en varias
 * secciones.
 *
 * La primera parte forma la gu�a de usuario.
 *
 * \todo Rellenar la lista de secciones que componen la gu�a de usuario
 *
 * La segunda parte forma el manual de referencia (manual API).
 *
 * \todo Rellenar la lista de secciones que componen la API
 *
 * \subsection ssec_find D�nde encontrar Yaflengine
 *
 * Yaflengine es la primera librer�a que hago accesible al p�blico. Despu�s de
 * mirar diferentes opciones, me decid� por <a href="https://github.com/">GitHub</a>.
 * La p�gina web del proyecto se encuentra en <http://di4l.github.io/yaflengine/>.
 * En la p�gina podr� encontrar enlaces a la documentaci�n que actualmente est�
 * leyendo, al c�digo fuente, la �ltima entrega estable y la wiki.
 *
 * Hay varios tipos de descargas que puede realizar. Si pretende compilar el
 * proyecto uested mismo, debe descargar el c�digo fuente, si por el contrario
 * s�lo est� interesado en usar el motor en alguno de sus proyectos puede
 * descargar los binarios y ficheros cabecera.
 *
 * \li �ltima entrega: <https://github.com/Di4l/yaflengine/releases/latest>
 * \li �ltimo desarrollo: <https://github.com/Di4l/yaflengine>
 *
 * Existe una Wiki del proyecto (en desarrollo) donde encontrar� infromaci�n
 * variada y enlaces a otras aprtes del proyecto. El enlace a la Wiki es
 * <https://github.com/Di4l/yaflengine/wiki>
 *
 * Por �ltimo, la documentaci�n que est� usted leyendo (en ingl�s) se encuentra
 * en <http://di4l.github.io/yaflengine/dev_doc/html/index.html>
 *
 * \subsection ssec_copyright Licencia Yaflengine
 *
 *    Copyright (C) 2014  Ra�l Hermoso S�nchez
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
 *    You should have received a copy of the \ref pag_license "GNU General Public License"
 *    along with this program; if not, write to the Free Software Foundation,
 *    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * \page pag_license Yaflengine License
 * \verbinclude LICENSE
 *
 * \page pag_user_manual Gu�a de usuario
 *
 * Las diferentes secciones que componen este documento le guiar�n en las
 * diferentes etapas de uso de Yaflengine en sus proyectos. Con esto esperamos
 * ayudarle a acelerar el proceso de aprendizaje y conseguir que se encuentre
 * c�modo lo m�s r�pido posible usando este motor.
 *
 * Esta p�gina est� dividida en varias secciones:
 *
 * \li \ref sec_ug_install indica c�mo instalar el c�digo fuente y/o los
 * binarios de Yaflengine para poder usarlos en sus proyectos.
 * \li \ref sec_ug_start le gu�a en la creacion de su primer proyecto con
 * Yaflengine.
 *
 * \ref sec_ug_install "Siguiente"
 *
 * \section sec_ug_install Instalaci�n
 *
 * \todo Completar la gu�a de instalaci�n
 * \ref pag_user_manual "Anterior" | \ref sec_ug_start "Siguiente"
 *
 * \section sec_ug_start Comenzando
 *
 * \todo Completar una gu�a de "comenzando"
 *
 * \ref sec_ug_install "Anterior"
 *
 *
 *
 * \page pag_api_ref Manual de la API
 *
 * Yaflengine se compone de una serie de clases y estructuras relacionadas
 * entre s� mediante dependencias, m�todos y jerarqu�as. Para sentar una base
 * com�n al paquete, existe una clase base (nsFuzzy::TFuzzyBase) de la que
 * heredan todas las dem�s (o casi todas). Esta clase contiene informaci�n
 * com�n a todas las clases de la librer�a as� como algunos m�todos comunes.
 *
 * Yaflengine gestiona los errores producidos mediante excepciones. La clase
 * nsFuzzy::TFuzzyError es la otra estructura fundamental en el entorno del
 * motor. Contiene m�todos y miembros que permiten, utilizada correctamente,
 * mostrar informaci�n detallada de los errores producidos.
 *
 * Todos los errores gestionados por Yaflengine contienen tres datos
 * fundamentales a la hora de localizar y analizar el problema:
 *
 * \li <b>Qu�</b>. Indica qu� es lo que ha fallado o qu� ha producido el error.
 * \li <b>Por qu�</b>. Describe la raz�n por la que se produjo el error.
 * \li <b>D�nde</b>. Indica d�nde se ha producido el error. Identifica la l�nea
 * y archivo donde se lanz� la excepci�n.
 *
 * Sobre estas dos clases se construye el resto de la librer�a. Es importante
 * que eche un vistazo a la documentaci�n de estas dos bestias antes de
 * continuar. No es necesario que se las aprenda de memoria, con tener una
 * ligera idea de qu� miembros y m�todos contiene es suficiente para empezar.
 *
 * Para entender mejor el rol que el resto de las clases desempe�an,
 * imaginaremos dos catgeor�as diferenciadas: Definici�n y Ejecuci�n. El primer
 * grupo se encarga de definir un modelo l�gico. El segundo grupo se encarga de
 * operar sobre esos modelos para realizar c�lculos. Casi todas las clases del
 * motor pertenecen al primer grupo.
 *
 * Un modelo (nsFuzzy::TFuzzyModel) es una serie de conjuntos (nsFuzzy::TFuzzySet)
 * interrelacionados entre s� por una serie de reglas (nsFuzzy::TFuzzyRules).
 * Para entender mejor esto, debemos definir <em>conjunto</em> y <em>regla</em>.
 * Un <em>conjunto</em> es una propiedad mensurable y/o manipulable de un
 * sistema. Por ejemplo <em>Temperatura</em>, <em>Voltaje</em> o <em>Tama�o</em>.
 * El hecho de ser mensurable y/o anipulables depende del sistema a modelar
 * (caso de estudio) y la raz�n de ser mensurable y/o manipulables es simple
 * l�gica. No tiene sentido definir el conjunto <em>Temperatura</em> si no se
 * va a poder medir o modificar su valor.
 *
 */
