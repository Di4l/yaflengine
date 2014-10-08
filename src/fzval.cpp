//-----------------------------------------------------------------------------
#include "fzval.hpp"
#include <cmath>
//-----------------------------------------------------------------------------
using namespace nsFuzzy;
//-----------------------------------------------------------------------------
TFuzzyFunctions TFuzzyVal::m_ffFunctions;
//-----------------------------------------------------------------------------

/**
 * \param [in] strName Nombre identificativo para la instancia
 *
 * Constructor de la clase. Esta forma del constructor necesita como parámetro
 * el nombre con el que se le quiere identificar dentro de un set.
 *
 * \return Nada
 * \sa ~TFuzzyVal()
 */
TFuzzyVal::TFuzzyVal(std::string strName) : TFuzzyBase(FL_ID_VAL)
{
	if(strName.empty())
        throw TFuzzyError("contructor", "Nombre no especificado. El Objeto "
        				  "no puede construirse", __LINE__, __FILE__);

	m_fvMaxF  = TFuzzyBase::invalidNumber();
    m_ffId    = INVALID_FUNC;
    m_strName = strName;

	toLower(trim(m_strName));
}
//-----------------------------------------------------------------------------

/**
 * Destructor de la clase
 *
 * \return Nada
 * \sa TFuzzyVal()
 */
TFuzzyVal::~TFuzzyVal()
{
}
//-----------------------------------------------------------------------------

/**
 *  \param [in] x Valor de entrada a la función
 *
 *  Esta función recibe el valor x, lo aplica la función asociada y retorna el
 *  resultado.
 *
 *  \return Resultado de aplicar el valor x a la función definida para este
 *  valor.
 *  \sa FFuzzyFunction, execute()
 */
fuzzvar TFuzzyVal::exeFunction(fuzzvar x)
{
	if(INVALID_FUNC == m_ffId)
		return 0.0;

    if(!m_vParams.size())
    {
        size(0);
        m_vParams[0] = 0.0;
        m_vParams[1] = 10.0;
    }
    return m_ffFunctions[m_ffId]->Function(m_vParams, x);
}
//-----------------------------------------------------------------------------

/**
 * Calcula el punto donde la función es máxima
 */
fuzzvar TFuzzyVal::calcMaxFunc()
{
	fuzzvar x   = min();
	fuzzvar y   = 0.0;
	fuzzvar dy  = 2.0;
	fuzzvar Dx  = (max() - min()) / 2;
	fuzzvar de2 = 1.0E-6;

	while(dy > de2)
	{
		x += Dx;

		dy  = y;
		y   = exeFunction(x);

		if(y < dy)
			Dx /= -2.0;

		dy = pow(y - dy, 2.0);

		if(x > max())
		{
			x  = max();
			dy = 0.0;
		}
		else if(x < min())
		{
			x  = min();
			dy = 0.0;
		}
	}

	return x;
}
//-----------------------------------------------------------------------------

/**
 * \param [in] szCount Número de parámetros a asignar
 *
 * Asigna el número de parámetros que necesita la función para poder calcular
 * un resultado.
 *
 * La mayoría de las funciones predeterminadas en la librería Fuzzy son
 * expresiones matemáticas bien definidas y no necesitan ningún parámetro
 * extra a parte del valor X sobre el que se quiere calcular el resultado. Para
 * funciones de interpolación, por ejemplo, es necesario suministrar como
 * parámetros los puntos sobre los que debe extrapolar la función.
 *
 * \return Nada
 * \sa operator[](), size()
 */
void TFuzzyVal::size(size_t szCount)
{
	size_t sz_cnt = szCount + 2;

	while(m_vParams.size() > sz_cnt)
		m_vParams.pop_back();
	while(m_vParams.size() < sz_cnt)
		m_vParams.push_back(0.0);
}
//-----------------------------------------------------------------------------

/**
 * \param [in] fvMin Valor mínimo de la función
 *
 * Este método asigna el límite inferior sobre el que la función está definida.
 * Todo aquel valor \a x menor que fvMin devolverá como resultado a una llamada a
 * execute() el mismo valor que si <i>x = fvMin</i>.
 *
 * El propósito de esto es extender fuera de los límites definido el resultado
 * de caulquier función.
 *
 * \return Nada
 * \sa max(), execute()
 */
void TFuzzyVal::min(fuzzvar fvMin)
{
	if(m_vParams.size() > 0)
		m_vParams[0] = fvMin;
	else
		m_vParams.push_back(fvMin);
}
//-----------------------------------------------------------------------------

/**
 * \param [in] fvMax Valor máximo de la función
 *
 * Este método asigna el límite superior sobre el que la función está definida.
 * Todo aquel valor \a x mayor que fvMax devolverá como resultado a una llamada a
 * execute() el mismo valor que si <i>x = fvMax</i>.
 *
 * El propósito de esto es extender fuera de los límites definido el resultado
 * de caulquier función.
 *
 * \return Nada
 * \sa min(), execute()
 */
void TFuzzyVal::max(fuzzvar fvMax)
{
	switch(m_vParams.size())
	{
	case 0:
		m_vParams.push_back(0.0);

	case 1:
		m_vParams.push_back(fvMax);
		break;

	case 2:
	default:
		m_vParams[1] = fvMax;
		break;
	}
}
//-----------------------------------------------------------------------------

/**
 * \param [in] strName Nombre de la función a asignar.
 *
 * Cada valor (TFuzzyVal) de un conjunto (TFuzzySet) está asociado a una
 * función que define su grado de verdad dentro del conjunto. Éste método
 * asigna la función a partir del nombre de la misma.
 *
 * La librería Fuzzy incorpora una serie de funciones consideradas estándar
 * que pueden emplearse de manera nativa. La definición de estas funciones y
 * sus nombres se encuentran en TStdFuzzyFunctions.
 *
 * \return Descriptor a la función con nombre strName, FL_INVALID_HANDLE en
 * caso de no existir la función.
 * \sa TStdFuzzyFunctions, TFuzzyFunctions
 */
fzhndl TFuzzyVal::setFunction(std::string strName)
{
	SFuzzyFunction* ff_func = m_ffFunctions[strName];
    m_ffId = ff_func ? ff_func->handle() : FL_INVALID_HANDLE;

    m_fvMaxF = calcMaxFunc();

    return m_ffId;
}
//-----------------------------------------------------------------------------

/**
 * \param [in] ffId Descriptor de la función a asignar.
 *
 * Cada valor (TFuzzyVal) de un conjunto (TFuzzySet) está asociado a una
 * función que define su grado de verdad dentro del conjunto. Éste método
 * asigna la función a partir del descriptor de la misma.
 *
 * La librería Fuzzy incorpora una serie de funciones consideradas estándar
 * que pueden emplearse de manera nativa. La definición de estas funciones y
 * sus nombres y descriptores se encuentran en TStdFuzzyFunctions.
 *
 * \return Nombre de la función con descriptor ffId, FL_INVALID_HANDLE en caso
 * de no existir el descriptor.
 * \sa TStdFuzzyFunctions, TFuzzyFunctions
 */
std::string TFuzzyVal::setFunction(fzhndl ffId)
{
	SFuzzyFunction* ff_func = m_ffFunctions[ffId];

	m_ffId = ff_func ? ffId : INVALID_FUNC;

    return ff_func ? ff_func->name() : std::string("");
}
//-----------------------------------------------------------------------------

/**
 * \param [in] iIndex Posición del parámetro.
 *
 * Para aquellas funciones con algún parámetro de configuración, este operador
 * devuelve el valor del parámetro en la posición iIndex, siendo el primero el
 * parámetro 0. Para determinar cuántos parámetros tiene una funcion se puede
 * usar el método size().
 *
 * \return Valor del parámetro en la posición iIndex, TFuzzyBase::invalid()
 * en caso de no existir dicha posición.
 * \sa size()
 */
fuzzvar& TFuzzyVal::operator[](int iIndex)
{
	return (iIndex >= 0 && size_t(iIndex) < size())
			? m_vParams[iIndex + 2] : invalidNumber();
}
//-----------------------------------------------------------------------------
