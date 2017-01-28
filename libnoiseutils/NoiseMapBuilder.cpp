//
// Created by martin on 27/01/17.
//

#include "NoiseMapBuilder.h"

using namespace noise::utils;

/////////////////////////////////////////////////////////////////////////////
// NoiseMapBuilder class

NoiseMapBuilder::NoiseMapBuilder ():
        m_pCallback (NULL),
        m_destHeight (0),
        m_destWidth  (0),
        m_pDestNoiseMap (NULL),
        m_pSourceModule (NULL)
{
}

void NoiseMapBuilder::SetCallback (NoiseMapCallback pCallback)
{
    m_pCallback = pCallback;
}
