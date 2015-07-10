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

template <typename s_type, typename v_type, typename m_type,
          typename base_function>
class Basis
{
private:
  Eigen::Matrix<base_function,Eigen::Dynamic,1> baseFunctions;
  unsigned int dimension;
public:
  // ctor
  Basis() : baseFunctions() {}
  Basis(int i) : baseFunctions(i), dimension(i) {}

  // member functions
  unsigned int getDim()
  {
    return dimension;
  }

  void addFunction(const base_function& func, int i)
  {
    baseFunctions(i) = func;
  }

  // evaluate overloads
  s_type evaluate(const int& i, const s_type& x)
  {
    return baseFunctions(i).evaluate(x);
  }
  void evaluate(const int& i, const s_type& x, s_type& y)
  {
    y = baseFunctions(i).evaluate(x);
  }
  void evaluate(const s_type& x, v_type& y)
  {
    for (unsigned int k=0; k<dimension; ++k)
      y(k) = baseFunctions(k).evaluate(x);
  }
  // evaluation of entire basis along discrete axis (use to compute matrix)
  void evaluate(const v_type& x, m_type& y)
  {
    for (unsigned int k=0; k<dimension; ++k)
      for (unsigned int l=0; l<x.size(); ++l)
        y(l,k) = baseFunctions(k).evaluate(x(l));
  }

};

template<typename s_type>
class base_function
{
private:
  s_type shape;
public:
  // ctor
  base_function() : shape() {}
  base_function(s_type mshape) : shape(mshape) {}
  s_type evaluate(const s_type& x)
  {
    return pow(x,shape);
  }
};


///////////////////////////////// MAIN //////////////////////////////////////
int main( int argc, char* argv[] )
{
  // Wrap everything in a try block.  Do this every time, 
	// because exceptions will be thrown for problems.
	try {  
    
  // Define the command line object.
  TCLAP::CmdLine cmd("Solve linear least square fit.", 
                       ' ', "LSQ 0.1"); 

  // add the input file as a required unlabeled value to the command line

  TCLAP::UnlabeledValueArg<int> 
  lsqOrder( "order", "integer indicating maximal degree of polynomial basis",
           true, 1, "int", cmd );

  TCLAP::UnlabeledValueArg<std::string> 
  inpFile( "filename", ".csv file containing x and y values",
           true, "default", "string", cmd );
  
  
  //cmd.add( inpFile );
  //cmd.add( nameArg );

  // Parse the args.
  cmd.parse( argc, argv );

  const int degree = lsqOrder.getValue();
  const std::string filename = inpFile.getValue();
  std::cout << degree << std::endl;
  std::cout << filename << std::endl;

  // typedefs
  typedef double scalar_type;
  typedef Eigen::Matrix<scalar_type,Eigen::Dynamic,Eigen::Dynamic>
          matrix_type;
  typedef Eigen::Matrix<scalar_type,Eigen::Dynamic,1>
          vector_type;
  

  // set up data pair for testing purposes
  matrix_type X;
  vector_type xs, ys;
  
  /*
  int i=0;
  std::cout << "\ncmdline args count = " << argc << std::endl;

  // First argument is executable name only
  std::cout << "\nexe name= " << argv[0] << std::endl;

  for (i=1; i < argc; i++) {
    std::cout << "\narg " <<  i << " = " << argv[i] << std::endl;
  }
  */
  
  //////////////////////////// READ FROM FILE ////////////////////////////

  // TODO: WRITE FILEREADER

  // convert string to char-array for fstream
  char fname[1024];
  strncpy(fname, filename.c_str(), sizeof(fname));
  fname[sizeof(fname) - 1] = 0;
  std::fstream myfile(fname, std::ios_base::in);
  //std::fstream myfile("../input/input.csv", std::ios_base::in);

  // set up std vectors of zero length (push_back will expand it later on)
  std::vector<scalar_type>*inpvec = new std::vector<scalar_type>(0);
  float a;
  while (myfile >> a)
  {
    inpvec->push_back(a);
  }

  std::cout << (*inpvec)[0] << std::endl;

  vector_type inx(inpvec->size()/2), iny(inpvec->size()/2);

  for (unsigned int p=0; p<inx.size(); ++p)
  {
    inx(p) = (*inpvec)[p];
    iny(p) = (*inpvec)[p+inx.size()];
  }

  delete inpvec;

  std::cout << "inx" << std::endl;
  std::cout << inx << std::endl;
  std::cout << "iny" << std::endl;
  std::cout << iny << std::endl;
  std::cout << std::endl;

  xs = inx;
  ys = iny;

  ///////////////////////////////////////////////////////////////////////

  Eigen::Matrix<base_function<scalar_type>,Eigen::Dynamic,1> mypoint(degree+1);
  for (unsigned int k=0; k<mypoint.size(); ++k)
  {
    mypoint(k) = base_function<scalar_type>(k);
  }

  matrix_type Mat(xs.rows(),degree+1);
  matrix_type S(Mat.cols(),Mat.cols());
  vector_type rslt(Mat.cols()), right(Mat.cols());
  
  // build basis
  Basis<scalar_type, vector_type, matrix_type, base_function<scalar_type> >
    myBase(degree+1);

  //std::cout << myBase.getDim() << std::endl;

  // fill basis
  for (unsigned int l=0; l<=degree; ++l)
    myBase.addFunction(mypoint(l),l);

  myBase.evaluate(xs,Mat);
  std::cout << Mat << std::endl;

  // compute rhs
  right.noalias() = Mat.transpose()*ys;

  Eigen::PartialPivLU<matrix_type> plu;
  plu.compute(Mat.transpose()*Mat);
  rslt = plu.solve(right);

  std::cout << "Solution" << std::endl;
  std::cout << rslt << std::endl;

  return 0;


  } catch (TCLAP::ArgException &e)  // catch any exceptions
	{ std::cerr << "error: " << e.error() << " for arg " << e.argId() 
              << std::endl; }
}



