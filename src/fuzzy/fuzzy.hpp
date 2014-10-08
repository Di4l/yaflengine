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
