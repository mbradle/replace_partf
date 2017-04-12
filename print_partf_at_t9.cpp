////////////////////////////////////////////////////////////////////////////////
// This file was originally written by Bradley S. Meyer.
//
// This is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//! \file
//! \brief Code to print out partition functions at given T9.
////////////////////////////////////////////////////////////////////////////////

//##############################################################################
// Includes.
//##############################################################################

#include <iostream>

#include <nnt/auxiliary.h>

#include <Libnucnet.h>

//##############################################################################
// check_input().
//##############################################################################

void
check_input( int argc, char **argv )
{

  if( argc == 2 && strcmp( argv[1], "--example" ) == 0 )
  {
    std::cout << std::endl;
    std::cout << argv[0] <<
      " updated_nuc.xml 4. \"[z = 26]\"" <<
      std::endl << std::endl;
    exit( EXIT_FAILURE );
  }

  if( argc < 3 || argc > 4 )
  {
    fprintf(
      stderr,
      "\nUsage: %s nuc_xml t9 nuc_xpath\n\n",
      argv[0]
    );
    fprintf(
      stderr, "  nuc_xml = nuclear data xml filename\n\n"
    );
    fprintf(
      stderr, "  t9 = temperature at which to compute partition functions\n\n"
    );
    fprintf(
      stderr, "  nuc_xpath = XPath to select nuclei (optional)\n\n"
    );
    std::cout << "For an example usage, type " << std::endl << std::endl;
    std::cout << argv[0] << " --example" << std::endl << std::endl;
    exit( EXIT_FAILURE );
  }

}

//##############################################################################
// main().
//##############################################################################

int main( int argc, char * argv[] ) {

  Libnucnet__Nuc * p_nuc;
  double d_t9;

  //============================================================================
  // Check input.
  //============================================================================

  check_input( argc, argv );

  //============================================================================
  // Read and store input.
  //============================================================================

  if( argc == 3 )
    p_nuc = Libnucnet__Nuc__new_from_xml( argv[1], NULL );
  else
    p_nuc = Libnucnet__Nuc__new_from_xml( argv[1], argv[3] );

  d_t9 = boost::lexical_cast<double>( argv[2] );

  //============================================================================
  // Printout.
  //============================================================================

  std::cout << "\nPartition functions at T9 = " << d_t9 << "\n\n";

  boost::format fmt( "%s\t  %e\n" );

  BOOST_FOREACH( nnt::Species species, nnt::make_species_list( p_nuc ) )
  {

    Libnucnet__Species * p_species = species.getNucnetSpecies();

    fmt % 
      Libnucnet__Species__getName( p_species ) %
      Libnucnet__Species__computePartitionFunction( p_species, d_t9 );

    std::cout << fmt.str();
  
  }

  std::cout << "\n";
    
  //============================================================================
  // Done.
  //============================================================================

  Libnucnet__Nuc__free( p_nuc );
  return EXIT_SUCCESS;

}

