//
// Created by martin on 27/01/17.
//

#include "NoiseMapBuilder.h"

using namespace noise::utils;

/////////////////////////////////////////////////////////////////////////////
// NoiseMapBuilder class

NoiseMapBuilder::NoiseMapBuilder ():
        m_pCallback (NULL),
        _destHeight (0),
        _destWidth  (0),
        _destNoiseMap (NULL),
        _source (NULL)
{
}

void NoiseMapBuilder::SetCallback (NoiseMapCallback pCallback)
{
    m_pCallback = pCallback;
}
