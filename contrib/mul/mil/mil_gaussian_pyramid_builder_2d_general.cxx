// This is mul/mil/mil_gaussian_pyramid_builder_2d_general.cxx
#include "mil_gaussian_pyramid_builder_2d_general.h"
//:
//  \file
//  \brief Specialisations of is_a() function
//  \author Tim Cootes

#include <vil1/vil1_byte.h>

// Specialise the is_a() for vil1_byte
template<> vcl_string mil_gaussian_pyramid_builder_2d_general<vil1_byte>::is_a() const
{  return vcl_string("mil_gaussian_pyramid_builder_2d_general<vil1_byte>"); }

// Specialise the is_a() for int
template<> vcl_string mil_gaussian_pyramid_builder_2d_general<int>::is_a() const
{  return vcl_string("mil_gaussian_pyramid_builder_2d_general<int>"); }

// Specialise the is_a() for float
template<> vcl_string mil_gaussian_pyramid_builder_2d_general<float>::is_a() const
{  return vcl_string("mil_gaussian_pyramid_builder_2d_general<float>"); }
