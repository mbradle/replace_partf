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
//! \brief Code to replace partition function data in a network xml file
//!        with csv data.
////////////////////////////////////////////////////////////////////////////////

//##############################################################################
// Includes.
//##############################################################################

#include <fstream>
#include <iostream>

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

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
      " ../../data_pub/my_net.xml data.csv updated_net.xml" <<
      std::endl << std::endl;
    exit( EXIT_FAILURE );
  }

  if( argc < 4 || argc > 5 )
  {
    fprintf(
      stderr,
      "\nUsage: %s nuc_xml partf_csv output_xml\n\n",
      argv[0]
    );
    fprintf(
      stderr, "  nuc_xml = nuclear data xml filename\n\n"
    );
    fprintf(
      stderr, "  partf_csv = csv partf file\n\n"
    );
    fprintf(
      stderr, "  output_xml = output xml file\n\n"
    );
    fprintf(
      stderr, "  source = additional source string (optional)\n\n"
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

  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

  Libnucnet__Nuc * p_nuc;
  Libnucnet__Species * p_species;
  std::string line;
  std::ifstream infile;
  unsigned int i_z, i_a;
  double d_spin;
  std::vector<double> t9;
  gsl_vector * p_t9_new, * p_log10_partf_new;

  //============================================================================
  // Check input.
  //============================================================================

  check_input( argc, argv );

  //============================================================================
  // Read and store input.
  //============================================================================

  p_nuc = Libnucnet__Nuc__new_from_xml( argv[1], NULL );

  //============================================================================
  // Open input partf file.
  //============================================================================

  infile.open( argv[2] );

  //============================================================================
  // Read in temperatures and set vector.
  //============================================================================

  std::getline( infile, line, '\n' );

  boost::char_separator<char> sep(",");
  tokenizer tok( line, sep);
  tokenizer::iterator it = tok.begin();

  it++;
  it++;

  for( ; it != tok.end(); it++ )
  {
    std::string s_tmp = *it;
    boost::algorithm::trim( s_tmp );
    t9.push_back( boost::lexical_cast<double>( s_tmp ) );
  }

  p_t9_new = gsl_vector_calloc( t9.size() );
  p_log10_partf_new = gsl_vector_calloc( t9.size() );

  for( size_t i = 0; i < t9.size(); i++ )
  {
    gsl_vector_set( p_t9_new, i, t9[i] );
  }

  //============================================================================
  // Loop over lines to end.
  //============================================================================

  while( std::getline( infile, line ) )
  {

    tokenizer tok( line, sep );

    it = tok.begin();

    i_z = boost::lexical_cast<unsigned int>( *(it++) );  
    i_a = boost::lexical_cast<unsigned int>( *(it++) );  

    std::vector<double> v_p;

    for( ; it != tok.end(); it++ )
    {
      std::string s_tmp = *it;
      boost::algorithm::trim( s_tmp );
      v_p.push_back( boost::lexical_cast<double>( s_tmp ) );
    }  

    d_spin = ( v_p[0] - 1. ) / 2.;

    for( size_t i = 0; i < v_p.size(); i++ )
    {
      gsl_vector_set( p_log10_partf_new, i, log10( v_p[i] / v_p[0] ) );
    }

    p_species =
      Libnucnet__Nuc__getSpeciesByZA(
        p_nuc,
        i_z,
        i_a,
        ""
      );

    if( p_species )
    {

      Libnucnet__Species__updateSpin(
        p_species,
        d_spin
      );

      Libnucnet__Species__updatePartitionFunctionData(
        p_species,
        p_t9_new,
        p_log10_partf_new
	);

      if( argc == 5 )
      {
        std::string s = Libnucnet__Species__getSource( p_species );
        s += std::string( argv[4] );
        Libnucnet__Species__updateSource( p_species, s.c_str() );
      }

    }

  }

  //============================================================================
  // Write to output.
  //============================================================================

  Libnucnet__Nuc__writeToXmlFile(
    p_nuc,
    argv[3]
  );

  //============================================================================
  // Clean up and exit.
  //============================================================================

  infile.close();
  gsl_vector_free( p_t9_new );
  gsl_vector_free( p_log10_partf_new );
  Libnucnet__Nuc__free( p_nuc );
  return EXIT_SUCCESS;

}

