/*
 This program computes the coefficients of a linear least square fit in 1D for
 any choice of basis functions.
 */

#include <iostream>
#include <Eigen/Dense>
// debug stuff - to be dumped in sub files later on
#include <typeinfo>
#include <fstream>
#include <vector>
#include <string>

// command line parser tclap
#include <tclap/CmdLine.h>
//
#include <algorithm>
//
#include "leslie/filereadertxt.hpp"
//
#include "leslie/functionspace1d.hpp"
//
#include "leslie/functions1d.hpp"

using namespace leslie;

///////////////////////////////// MAIN /////////////////////////////////////////
int main( int argc, char* argv[] )
{
  // Wrap everything in a try block.  Do this every time, 
	// because exceptions will be thrown for problems.
	try {  
    
  // Define the command line object.
  TCLAP::CmdLine cmd("Solve linear least square fit.", 
                       ' ', "LeSLiE 0.1"); 

  // add the input file as a required unlabeled value to the command line

  TCLAP::UnlabeledValueArg<int> 
  lsqOrder( "order", "integer indicating maximal degree of polynomial basis",
           true, 1, "int", cmd );

  TCLAP::UnlabeledValueArg<std::string> 
  inpFile( "filename", ".csv file containing x and y values",
           true, "default", "string", cmd );
  
  // Parse the args.
  cmd.parse( argc, argv );

  const int degree = lsqOrder.getValue();
  const std::string filename = inpFile.getValue();

  // typedefs
  typedef double scalar_type;
  typedef Eigen::Matrix<scalar_type,Eigen::Dynamic,Eigen::Dynamic>
          matrix_type;
  typedef Eigen::Matrix<scalar_type,Eigen::Dynamic,1>
          vector_type;
  

  // set up data pair
  vector_type xs, ys;
    
  //////////////////////////// READ FROM FILE //////////////////////////////////
  filereader<scalar_type,vector_type> myreader(filename);
  myreader.printFileName();
  myreader(xs,ys);
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////// SET UP FUNCTION SPACE ///////////////////////////
  // initialize dim-0 space
  FunctionSpace1d<scalar_type,vector_type,matrix_type,
                  Function1d<scalar_type,vector_type,Polynomial1d> > 
    mySpace(0);

  // in basis
  for (unsigned int k=0; k<=degree; ++k)
  {
    mySpace.addFunction(Function1d<scalar_type,vector_type,Polynomial1d>(k));
  }

  // Define matrix
  matrix_type Mat(xs.size(),degree+1);
  Mat.setZero();
  // Define right hand side and solution
  vector_type rhs(degree+1), sol(degree+1);

  // Compute matrix
  mySpace.evaluate(xs,Mat);

  // compute rhs
  rhs.noalias() = Mat.transpose()*ys;

  Eigen::PartialPivLU<matrix_type> plu;
  plu.compute(Mat.transpose()*Mat);
  sol = plu.solve(rhs);

  std::cout << "Solution" << std::endl;
  std::cout << sol << std::endl;

  return 0;


  } catch (TCLAP::ArgException &e)  // catch any exceptions
	{ std::cerr << "error: " << e.error() << " for arg " << e.argId() 
              << std::endl; }
}



