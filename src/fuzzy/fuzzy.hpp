/**
 * \file fuzzy.hpp
 * Main project include file.
 *
 * This files includes general definitions for the fuzzy framework that
 * shall be used throughout the project.
 */

/*    Copyright (C) 2014  Raúl Hermoso Sánchez
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
	/** Descriptor que identifica un objeto de esta librería */
	typedef unsigned int              fzhndl;
	/** Tipo que define una lista de parámetros */
	typedef std::vector<fuzzvar>      fuzzlist;
	/** Tipo que define un mapa de parámetros */
	typedef std::map<fzhndl, fuzzvar> fuzzmap;
	/** Prototipo de función */
	typedef fuzzvar (*FFuzzyFunction)(fuzzlist&, fuzzvar);
	//-------------------------------------------------------------------------
//};
//-----------------------------------------------------------------------------

/** Handle incorrecto o inválido */
#define FL_INVALID_HANDLE	0x80000000
//-----------------------------------------------------------------------------

// Para cada clase derivada de TFuzzyBase debe existir un identificador...

/** Clase no definida */
#define FL_ID_UNDEFINED 0x7FFFFFFF
/** Clase objetos */
#define FL_ID_OBJS      0x01
/** Clase función */
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
/** Clase árbol de módulo */
#define FL_ID_MOD_TREE  0x08
/** Clase módulo */
#define FL_ID_MOD       0x09
/** Clase módulo */
#define FL_ID_MODS      0x0A
/** Clase ejecución */
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
 * \section sec_intro Introducción
 *
 * El nombre Yaflengine viene de <em>Y otro motor de lógica difusa</em> por sus
 * siglas en inglés. Es una librería que le permite realizar cálculos usando
 * <a href="http://es.wikipedia.org/wiki/L%C3%B3gica_difusa">lógica difusa</a>.
 * Está escrita en su totalidad en C++ y compila tanto en Windows como en
 * Linux.
 *
 * Con Yaflengine podrá:
 *
 * \li Crear/modificar en tiempo de ejecución modelos.
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
 * La documentación está dividida en dos partes, cada una subdividida en varias
 * secciones.
 *
 * La primera parte forma la guía de usuario.
 *
 * \todo Rellenar la lista de secciones que componen la guía de usuario
 *
 * La segunda parte forma el manual de referencia (manual API).
 *
 * \todo Rellenar la lista de secciones que componen la API
 *
 * \subsection ssec_find Dónde encontrar Yaflengine
 *
 * Yaflengine es la primera librería que hago accesible al público. Después de
 * mirar diferentes opciones, me decidí por <a href="https://github.com/">GitHub</a>.
 * La página web del proyecto se encuentra en <http://di4l.github.io/yaflengine/>.
 * En la página podrá encontrar enlaces a la documentación que actualmente está
 * leyendo, al código fuente, la última entrega estable y la wiki.
 *
 * Hay varios tipos de descargas que puede realizar. Si pretende compilar el
 * proyecto uested mismo, debe descargar el código fuente, si por el contrario
 * sólo está interesado en usar el motor en alguno de sus proyectos puede
 * descargar los binarios y ficheros cabecera.
 *
 * \li Última entrega: <https://github.com/Di4l/yaflengine/releases/latest>
 * \li Último desarrollo: <https://github.com/Di4l/yaflengine>
 *
 * Existe una Wiki del proyecto (en desarrollo) donde encontrará infromación
 * variada y enlaces a otras aprtes del proyecto. El enlace a la Wiki es
 * <https://github.com/Di4l/yaflengine/wiki>
 *
 * Por último, la documentación que está usted leyendo (en inglés) se encuentra
 * en <http://di4l.github.io/yaflengine/dev_doc/html/index.html>
 *
 * \subsection ssec_copyright Licencia Yaflengine
 *
 *    Copyright (C) 2014  Raúl Hermoso Sánchez
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
 * \page pag_user_manual Guía de usuario
 *
 * Las diferentes secciones que componen este documento le guiarán en las
 * diferentes etapas de uso de Yaflengine en sus proyectos. Con esto esperamos
 * ayudarle a acelerar el proceso de aprendizaje y conseguir que se encuentre
 * cómodo lo más rápido posible usando este motor.
 *
 * \section sec_ug_install Instalación
 *
 * \todo Completar la guía de instalación
 *
 * \section sec_ug_start Comenzando
 *
 * \todo Completar una guía de "comenzando"
 *
 * \page pag_api_ref Manual de la API
 *
 * \todo Rellenar aqui una especie de introducción de la API
 *
 */
