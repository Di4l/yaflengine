/**
 * \file fzmodel.cpp
 *
 *  A model binds a group of Sets and Rules. Rules define how sets relate to
 *  each other.
 */

/*
 *     Copyright (C) 2014  Raúl Hermoso Sánchez
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
#include "fzmodel.hpp"
#include "ini_file.hpp"
#include <sstream>
#include <iomanip>
#include <iostream>
#ifdef __BORLANDC__
#include <stdio.h>
#else
#include <cstdio>
#endif
//-----------------------------------------------------------------------------
using namespace nsFuzzy;
//-----------------------------------------------------------------------------
#define FL_SCRIPT_MODEL     "model"
#define FL_SCRIPT_SETS		"sets"
#define FL_SCRIPT_RULES     "rules"
//-----------------------------------------------------------------------------

/**
 *
 */
TFuzzyModel::TFuzzyModel() : TFuzzyBase(FL_ID_MODS), m_frRules(m_fsSets)
{
	m_fsSets.parent()  = this;
	m_frRules.parent() = this;
}
//-----------------------------------------------------------------------------

/**
 *
 */
TFuzzyModel::~TFuzzyModel()
{
	clear();
	//-- El propio destructor de TFuzzyRules y TFuzzySets libera los recursos
	//   empleados. No hay necesidad de llamar a clear()
}
//-----------------------------------------------------------------------------

/**
 *
 */
std::string& TFuzzyModel::configurationHint(std::string& strHint)
{
	strHint.clear();

	strHint =
	"  **********************************************************\n"
	"      Guía de configuración de un modelo para 'libfuzzy'\n"
	"  **********************************************************\n"
	"  Este archivo define un modelo para trabajar con la librería de lógica difusa\n"
	"'libfuzzy'. Si es ud. novato en el uso de esta librería o en el concepto de\n"
	"lógica difusa en general, le recomendamos que lea primero el manual\n"
	"'fuzzy_logic.chm'. En él aprenderá los conceptos básicos y cómo se implementa\n"
	"en 'libfuzzy'.\n\n"
	"  En los comentarios que siguen, se explica cómo se configura un modelo para\n"
	"ser usado por 'libfuzzy'.\n\n"
	"  0.  Introducción\n"
	"  -------------------------------------\n"
	"  El formato del archivo es el de un archivo '.ini' tradicional de Windows. Ver\n"
	"http://es.wikipedia.org/wiki/INI_(extensi%C3%B3n_de_archivo) para más\n"
	"información.\n\n"
	"  El archivo debe contener las siguientes secciones de manera obligatoria:\n\n"
	"\t1. [model]  Donde se añade información general del modelo, como nombre,\n"
	"\t            descripción, etc...\n"
	"\t2. [sets]   Donde se definen los conjuntos (sets) que forman parte del modelo.\n"
	"\t3. [rules]  Donde se define las reglas que interrelacionan los diferentes sets.\n\n"
	"  Para cada 'set' definido en [sets] se debe crear una sección cuyo nombre\n"
	"sea el mismo que el del set, donde se definirá los valores y funciones que lo\n"
	"componen.\n\n"
	"  1.  Sección [model]\n"
	"  -------------------------------------\n"
	"  Esta sección sólo contiene el parámetro 'name' que sirve para proporcionar\n"
	"un nombre al modelo. Este nombre puede contener espacios, como en el ejemplo\n"
	" siguiente:\n\n"
	"\t[model]\n"
	"\tname=Modelo de ejemplo\n\n"
	"  2.  Sección [sets]\n"
	"  -------------------------------------\n"
	"  Aquí se enumeran los conjuntos o 'sets' que componen el modelo. Cada set se\n"
	"enumera en un parámetro diferente, cuyo formato es: <nombre_set>=<num_vals>\n\n"
	"\tnombre_set    Nombre del set. No debe contener espacios en blanco, ni\n"
	"\t              caracteres especiales (/\\^*&%...), pero si puede contener\n"
	"\t              guiones bajos.\n"
	"\tnum_vals      Número de valores que componen el set.\n\n"
	"  A continuación se muestra un ejemplo de configuración de la sección [sets]\n\n"
	"\t[sets]\n"
	"\ttemperatura=3\n"
	"\tpresion=3\n"
	"\tvoltaje=3\n\n"
	"  2.1 Definiendo conjuntos (o sets)\n"
	"  -------------------------------------\n"
	"  Para cada uno de los sets enumerados, debe crearse una sección particular\n"
	"donde se enumerará los valores de los que está compuesto. Así, siguiendo el\n"
	"ejemplo de arriba, debería crearse tres secciones llamadas [temperatura],\n"
	"[presion] y [voltaje] y, cada una de ellas enumerar los valores que lo componen.\n\n"
	"  Para cada valor del conjunto existirá un parámetro con el formato siguiente:\n\n"
	"\t\t<nombre_valor>=<nombre_funcion>\n\n"
	"\tnombre_valor    Es el nombre que identifica a este valor dentro del set.\n"
	"\tnombre_funcion  Es el nombre de la función que define el valor. Existen varias\n"
	"\t                funciones predeterminadas que 'libfuzzy' comprende. Estas son:\n\n"
	"\t            1. Gaussian Bell\n"
	"\t            2. S-Curve\n"
	"\t            3. Inverted S-Curve"
	"\t            4. Triangle\n"
	"\t            5. Inverted Triangle\n"
	"\t            6. Interpolate\n\n"
	"\t                El propósito de cada una de las funciones está fuera del\n"
	"\t                alcance de esta guía. Para más detalles consulte el manual.\n\n"
	"  Se muestra a continuación un ejemplo:\n"
	"\t[temperatura]\n"
	"\tfrio=Inverted S-Curve\n"
	"\ttemplado=Gaussian Bell\n"
	"\tcaliente=S-Curve\n\n"
	"  2.2 Definiendo los valores de un set\n"
	"  -------------------------------------\n"
	"  Cada valor de cada set debe ser definido mediante una sección propia. El\n"
	"nombre de la sección sigue el formato siguiente:\n\n"
	"\t\t[<nombre_set>_<nombre_valor>]\n\n"
	"\tnombre_set       Es el nombre del set al que pertenece el valor.\n"
	"\tnombre_valor     Es el nombre del valor a definir.\n\n"
	"  Estas secciones deben contener tres parámetros únicamente. A saber:\n\n"
	"\t1. min   Indica el valor mínimo de la función asociada (definida en la\n"
	"\t         sección descrita en 2.1).\n"
	"\t2. max   Indica el valor máximo de la función asociada (definida en la\n"
	"\t         sección descrita en 2.1).\n"
	"\t3. count Indica el número de parámetros extra que necesita la función\n"
	"\t         asociada. Para las funciones predefinidas por 'libfuzzy' este\n"
	"\t         parámetro es siempre 0, excepto para la función Interpolate.\n"
	"\t         Para más información consulte el manual.\n\n"
	"  Para terminar con una sección, mostramos un ejemplo:\n\n"
	"\t[temperatura_frio]\n"
	"\tmin=-10\n"
	"\tmax=15\n"
	"\tcount=0\n\n"
	"  3.  Sección [rules]\n"
	"  -------------------------------------\n"
	"  En esta sección se definen las reglas que gobiernan el comportamiento del\n"
	"modelo. Estas reglas definen cómo el/los conjunto/s de salida dependen de las\n"
	"entradas. Para ello se usa un lenguaje particular muy sencillo.\n\n"
	"\t\tif <input1> [and <input2> [and <input3 [and ...]]] then <output>\n\n"
	"\tinputN          Es una construcción semántica con el siguiente formato:\n\n"
	"\t            <nombre_set>[.<modificador>].<nombre_valor>\n\n"
	"\t  nombre_set    Es el nombre de uno de los sets enumerados en [sets].\n"
	"\t  nombre_valor  Es el nombre de uno de los valores del set especificado por\n"
	"\t                parámetro anterior.\n"
	"\t  modificador   Es una de las siguientes palabras clave:\n\n"
	"\t            very, slightly, little, few, muy, ligeramente, algo\n\n"
	"\t                El propósito de estos modificadores está fuera del alcance\n"
	"\t                de esta guía. Baste decir que sirve para modificar ligeramente\n"
	"\t                el resultado obtenido por los valores de un set.\n"
	"\toutput          Es una construcción semántica con el formato siguiente:\n\n"
	"\t            <nombre_set>.<nombre_valor>\n\n"
	"\t  nombre_set    Es el nombre de uno de los sets enumerados en [sets].\n"
	"\t  nombre_valor  Es el nombre de uno de los valores del set especificado por\n"
	"\t                parámetro anterior.\n\n"
	"  El ejemplo siguiente servirá para clarificar un poco todo lo anterior:\n\n"
	"\t\tif temperatura.muy.frio and presion.baja then voltaje.normal\n\n"
	"  Con esto concluye la guía de configuración de modelos para 'libfuzzy'.";

	return strHint;
}
//-----------------------------------------------------------------------------

/**
 *
 */
void TFuzzyModel::clear()
{
	m_frRules.clear();
	m_fsSets.clear();
}
//-----------------------------------------------------------------------------

/**
 *
 */
void TFuzzyModel::loadFromFile(std::string strFileName)
{
	clear();

	nsIniFile::TIniFile  file;
	nsIniFile::TSection* section = NULL;
	nsIniFile::TParam*   param   = NULL;
	fzhndl               fh_val  = FL_INVALID_HANDLE;
	std::stringstream    ss_aux;
	std::string          str_aux;
	char                 c_aux[512];
	fuzzvar              fv_aux;

	file.loadFile(strFileName);

	//-- Recuperamos el nombre del modelo
	section = file[FL_SCRIPT_MODEL];
	param   = (*section)["name"];
	name(param->value().str());
	//-- Se crean los sets
	section = file[FL_SCRIPT_SETS];
	for(int i = 0; i < int(section->size()); ++i)
	{
		param = (*section)[i];
		m_fsSets.add(param->name());
	}

	//-- Para cada set, creamos los valores
	for(int i = 0; i < int(m_fsSets.size()); ++i)
	{
		section = file[m_fsSets[i].name()];
		if(!section)
			throw TFuzzyError("loadFile", "No existe la sección",
					__LINE__, __FILE__);
		for(int j = 0; j < int(section->size()); ++j)
		{
			param = (*section)[j];
			fh_val = m_fsSets[i].add(param->name());
			if(FL_INVALID_HANDLE == fh_val)
				throw TFuzzyError("loadFile",
						"No se ha creado el valor para el set",
						__LINE__, __FILE__);
			m_fsSets[i][fh_val].setFunction(param->value().str());
		}
	}

	//-- Ahora asignamos las particularidades de cada valor en cada set
	for(int i = 0; i < int(m_fsSets.size()); ++i)
	{
		for(int j = 0; j < int(m_fsSets[i].size()); ++j)
		{
			sprintf(c_aux, "%s_%s", m_fsSets[i].name().c_str(), m_fsSets[i][j].name().c_str());
			str_aux = c_aux;

			section = file[str_aux];
			if(!section)
				throw TFuzzyError("loadFile", "No se encuentra la sección",
						__LINE__, __FILE__);

			param = (*section)["min"];
			if(!param)
				throw TFuzzyError("loadFile", "No se encuentra el parametro 'min'",
						__LINE__, __FILE__);
			param->value() >> fv_aux;
			m_fsSets[i][j].min(fv_aux);

			param = (*section)["max"];
			if(!param)
				throw TFuzzyError("loadFile", "No se encuentra el parametro 'max'",
						__LINE__, __FILE__);
			param->value() >> fv_aux;
			m_fsSets[i][j].max(fv_aux);

			param = (*section)["count"];
			if(!param)
				throw TFuzzyError("loadFile", "No se encuentra el parametro 'count'",
						__LINE__, __FILE__);
			param->value() >> fv_aux;
			m_fsSets[i][j].size(size_t(fv_aux));

			if(m_fsSets[i][j].size() > 0)
			{
				char c_par[10];
				for(size_t k = 0; k < m_fsSets[i][j].size(); ++k)
				{
					sprintf(c_par, "param_%04u", k);
					param = (*section)[c_par];
					param->value() >> m_fsSets[i][j][k];
				}
			}
		}
	}

	//-- Finalmente se cargan las reglas
	section = file[FL_SCRIPT_RULES];
	if(!section)
		throw TFuzzyError("loadFile", "No se encuentran reglas para este modelo",
				__LINE__, __FILE__);

	for(size_t i = 0; i < section->size(); ++i)
	{
		m_frRules.add((*section)[i]->value().str());
	}

	file.close();
}
//-----------------------------------------------------------------------------

/**
 *
 */
void TFuzzyModel::saveToFile(std::string strFileName, bool bAddComments)
{
	char                 c_aux[32];
	std::string          str_aux;
	std::stringstream    ss_aux;
	nsIniFile::TIniFile  file;
	nsIniFile::TSection* section = NULL;
	nsIniFile::TParam*   param   = NULL;

	//-- Se añade la cabecera al archivo de configuración del modelo
	if(bAddComments)
		configurationHint(file.header());
	//-- Se guarda el nombre del modelo
	section = file.add(FL_SCRIPT_MODEL);
	if(bAddComments)
		section->comment() = m_strDescription;
	param   = section->add("name");
	param->value() << m_strName;

	//-- Se guarda la lista de sets
	section = file.add(FL_SCRIPT_SETS);
	if(bAddComments)
		section->comment() = "Lista los sets que componen el modelo";
	for(int i = 0; i < int(m_fsSets.size()); ++i)
	{
		param = section->add(m_fsSets[i].name());
		param->value() << m_fsSets[i].size();
	}

	//-- Se guarda las características de cada set
	for(int i = 0; i < int(m_fsSets.size()); ++i)
	{
		section = file.add(m_fsSets[i].name());
		if(bAddComments)
			section->comment() = "Definición del conjunto " + section->name();
		for(int j = 0; j < int(m_fsSets[i].size()); ++j)
		{
			param = section->add(m_fsSets[i][j].name());
			param->value() << m_fsSets[i][j].function()->name();
		}
	}

	//-- Se guarda los valores de cada set
	for(int i = 0; i < int(m_fsSets.size()); ++i)
	{
		for(int j = 0; j < int(m_fsSets[i].size()); ++j)
		{
			str_aux = m_fsSets[i].name() + std::string("_") + m_fsSets[i][j].name();
			section = file.add(str_aux);
			if(bAddComments)
				section->comment() = "Definición del valor " + m_fsSets[i].name() + "." + m_fsSets[i][j].name();
			param   = section->add("min");
			param->value() << m_fsSets[i][j].min();
			if(bAddComments)
				param->comment() = "Valor mínimo que toma";
			param   = section->add("max");
			param->value() << m_fsSets[i][j].max();
			if(bAddComments)
				param->comment() = "Valor máximo que toma";
			param   = section->add("count");
			param->value() << m_fsSets[i][j].size();
			if(bAddComments)
				param->comment() = "Número de parámetros que necesita la función";

			if(m_fsSets[i][j].size() > 0)
			{
				char c_par[11];
				for(size_t k = 0; k < m_fsSets[i][j].size(); ++k)
				{
					sprintf(c_par, "param_%04u", k);
					param = section->add(c_par);
					param->value() << m_fsSets[i][j][k];
				}
			}
		}
	}

	//-- Se guarda las reglas
	section = file.add(FL_SCRIPT_RULES);
	if(bAddComments)
		section->comment() = "Establece la relación que hay entre los diferentes conjuntos del modelo";
	for(size_t i = 0; i < m_frRules.size(); ++i)
	{
		sprintf(c_aux, "rule_%03u", i + 1);

		param = section->add(c_aux);
		param->value() << m_frRules[i].String;
	}

	file.saveFile(strFileName);
	file.close();
}
//-----------------------------------------------------------------------------








/**
 *
 */
TFuzzyModels::TFuzzyModels() : TFuzzyBase(FL_ID_MODS)
{
}
//-----------------------------------------------------------------------------

/**
 *
 */
TFuzzyModels::~TFuzzyModels()
{
	clear();
}
//-----------------------------------------------------------------------------

/**
 *
 */
fzhndl TFuzzyModels::add()
{
	TFuzzyModel* fm_model = new TFuzzyModel;
	fm_model->parent() = this;
	m_mModels[fm_model->handle()] = fm_model;
	return fm_model->handle();
}
//-----------------------------------------------------------------------------

/**
 *
 */
void TFuzzyModels::del(fzhndl fzId)
{
	std::map<fzhndl, TFuzzyModel*>::iterator found = m_mModels.find(fzId);

	if(found != m_mModels.end())
	{
		delete found->second;
		found->second = NULL;
		m_mModels.erase(found);
	}
}
//-----------------------------------------------------------------------------

/**
 *
 */
void TFuzzyModels::clear()
{
	while(m_mModels.size() > 0)
	{
		delete m_mModels.begin()->second;
		m_mModels.erase(m_mModels.begin());
	}
}
//-----------------------------------------------------------------------------

/**
 *
 */
TFuzzyModel& TFuzzyModels::operator[](fzhndl fzId)
{
	std::map<fzhndl, TFuzzyModel*>::iterator found = m_mModels.find(fzId);

	return found == m_mModels.end() ? *((TFuzzyModel*)&invalidObject()) : *found->second;
}
//-----------------------------------------------------------------------------

/**
 *
 */
TFuzzyModel& TFuzzyModels::operator[](int iIndex)
{
	std::map<fzhndl, TFuzzyModel*>::iterator iter = m_mModels.begin();
	int i_ndx = 0;

	while(iter != m_mModels.end() && i_ndx++ < iIndex)
		++iter;

	return iter == m_mModels.end() ? *((TFuzzyModel*)&invalidObject()) : *iter->second;
}
//-----------------------------------------------------------------------------

/**
 *
 */
TFuzzyModel& TFuzzyModels::operator[](std::string strName)
{
	std::map<fzhndl, TFuzzyModel*>::iterator iter = m_mModels.begin();
	strName = toLower(trim(strName));

	while(iter != m_mModels.end() && iter->second->name() != strName)
		++iter;

	return iter == m_mModels.end() ? *((TFuzzyModel*)&invalidObject()) : *iter->second;
}
//-----------------------------------------------------------------------------
