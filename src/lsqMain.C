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
#include "leslie/filewritertxt.hpp"
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

  //////////////////////////// READ FROM FILE //////////////////////////////////
  filereader<scalar_type,vector_type> myreader(filename);
  // set up data pair
  // std::cout << myreader.getFileName() << std::endl;
  vector_type xs, ys;
  myreader(xs,ys);
  //////////////////////////////////////////////////////////////////////////////

  ///////////////////// SETTING UP PURE FUNCTION SPACE /////////////////////////
  // initialize dim-0 space
  FunctionSpace1d<scalar_type,vector_type,matrix_type,
                  Function1d<scalar_type,vector_type,Polynomial1d> > 
    mySpace(0);

  // fill in basis
  for (unsigned int k=0; k<=degree; ++k)
  {
    mySpace.addFunction(Function1d<scalar_type,vector_type,Polynomial1d>(k));
  }

  /////////// ALTERNATIVE OF SETTING UP MIXED FUNCTION SPACE ///////////////////
  vector_type myShape(1);
  myShape(0) = 3;
  std::cout << POLYNOMIAL<scalar_type,vector_type>(5,myShape) << std::endl;
  scalar_type (*myfunpt)(scalar_type,vector_type) = 
    POLYNOMIAL<scalar_type,vector_type>;
  std::cout << myfunpt(6,myShape) << std::endl;

  Function1d_PT<scalar_type,vector_type> myPoly(myfunpt,myShape);
  std::cout << myPoly.evaluate(29) << std::endl;

  // set up space for testing purposes
  FunctionSpace1d<scalar_type,vector_type,matrix_type,
                  Function1d_PT<scalar_type,vector_type> >
  testSpace(0);

  // fill in basis
  // ADD ARRAY OF FUNCTIONS
  for (int p=0; p<=degree; ++p)
  {
    myShape(0) = p;
    scalar_type (*myfunpt)(scalar_type,vector_type) = 
      POLYNOMIAL<scalar_type,vector_type>;
    testSpace.addFunction(
      Function1d_PT<scalar_type,vector_type>(myfunpt,myShape)  );
  }
  // ADD A SINGLE FUNCTION - SPACE ENRICHED WITH LOG (COMPARED TO mySpace)
  scalar_type (*lastpt)(scalar_type,vector_type) = 
      LOG<scalar_type,vector_type>;
  myShape(0) = 1;
  std::cout << myShape << std::endl;
  testSpace.addFunction(Function1d_PT<scalar_type,vector_type>(lastpt,myShape));
  ///////////// END OF ALTERNATIVE OF SETTING UP MIXED FUNCTION SPACE //////////

  // Define matrix

  /*
  // CASE A - mySpace
  matrix_type Mat(xs.size(),mySpace.getDim());
  Mat.setZero();
  vector_type rhs(mySpace.getDim()), sol(mySpace.getDim());
  // Compute matrix
  mySpace.evaluate(xs,Mat);
  */
  
  // CASE B - testSpace
  matrix_type Mat(xs.size(),testSpace.getDim());
  Mat.setZero();
  vector_type rhs(testSpace.getDim()), sol(testSpace.getDim());
  // Compute matrix
  testSpace.evaluate(xs,Mat); 
  

  // compute rhs
  rhs.noalias() = Mat.transpose()*ys;

  // TODO: ADD ROUTINE THAT CHECKS SOLVABILITY OF THE SYSTEM
  // This is an issue especially when MIXED function spaces are allowed.
  Eigen::PartialPivLU<matrix_type> plu;
  plu.compute(Mat.transpose()*Mat);
  sol = plu.solve(rhs);

  // Display solution - SHORT
  std::cout << std::endl;
  std::cout << "Solution:" << std::endl;
  std::cout << sol << std::endl << std::endl;;

  // Display solution - LONG
  std::cout << "Solution:" << std::endl;
  for (unsigned int i=0; i<sol.size(); ++i)
  {
    std::cout << "beta_" << i << "\t=\t" << sol(i) << std::endl;
  }

  // Display solution - CSV OPTIMIZED
  std::cout << "Solution:" << std::endl;
  for (unsigned int i=0; i<sol.size(); ++i)
  {
    std::cout << "beta_" << i << "," << sol(i) << std::endl;
  }
  
  //filewriter<scalar_type,vector_type> mywriter(filename+".OUT");
  //mywriter(sol);

  return 0;


  } catch (TCLAP::ArgException &e)  // catch any exceptions
	{ std::cerr << "error: " << e.error() << " for arg " << e.argId() 
              << std::endl; }
}



