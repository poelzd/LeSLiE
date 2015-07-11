// templates and specializations for any function in one dimension

// include guard
#ifndef FUNCTIONS1D_HPP
#define FUNCTIONS1D_HPP

namespace leslie
{

  //////////////////////////////////////////////////////////////////////////////
  // FIRST FUNCTION SET UP
  //////////////////////////////////////////////////////////////////////////////
  
  // tag-like definitions of function_type
  struct Polynomial1d{};
  struct Exponential1d{};


  // General set up for a function
  // s_type          type of scalar
  // v_type          type of vector
  // function_type   arguments to be defined as tag-like structures above
  template <typename s_type, typename v_type, typename function_type>
  class Function1d
  {
  public:
    //ctor
    Function1d() {}

    // evaluate function to be defined in specialization
    double evaluate(const double& x);
  };



  //////////////////////////////////////////////////////////////////////////////
  // TEMPLATE SPECIALIZATIONS OF FIRST FUNCTION SET UP
  //////////////////////////////////////////////////////////////////////////////

  // POLYNOMIAL
  template <typename s_type, typename v_type>
  class Function1d <s_type, v_type, Polynomial1d>
  {
  private:
    s_type shape;  
  public:
    // ctor: default shape not defined
    Function1d() : shape() {}
    Function1d(const s_type& shape) {this->shape = shape;}
    // evaluation: x^shape
    s_type evaluate(const s_type& x)
    {
      return pow(x,shape);
    }
  };


  // EXPONENTIAL
  template <typename s_type, typename v_type>
  class Function1d <s_type, v_type, Exponential1d>
  {
  private:
    s_type shape;
  public:
    // ctor: default shape 1
    Function1d() : shape(1) {}
    Function1d(const s_type& shape) {this->shape = shape;}
    // evaluation: exp(shape*x) 
    s_type evaluate(const s_type& x)
    {
      return exp(shape*x);
    }
  };


  //////////////////////////////////////////////////////////////////////////////
  // SECONDARY FUNCTION SET UP
  //////////////////////////////////////////////////////////////////////////////
  
  // Note: The vectorized definition of shape for all functions is not too
  // comfortable, but this definition allows for mixing of functions within one
  // function space.

  template <typename s_type, typename v_type>
  class Function1d_PT
  {
  private:
    // function pointer
    s_type (*function)(s_type,v_type);
    v_type shape;
  public:
    //ctor
    Function1d_PT() {}
    Function1d_PT(s_type (*funtocall)(s_type,v_type), v_type shape) 
    {
      function = funtocall;
      this->shape = shape;
    }

    // evaluate function to be defined in specialization
    double evaluate(const double& x)
    {
      return (*function)(x,shape);
    }
  };
  
  //////////////////////////////////////////////////////////////////////////////
  // FUNCTION DEFINITIONS FOR SECOND FUNCTION SET UP
  //////////////////////////////////////////////////////////////////////////////

  // POLYNOMIAL
  template <typename s_type, typename v_type>
  s_type POLYNOMIAL(const s_type x, v_type power)
  {
    return pow(x,power(0));
  }

  // EXPONENT
  template <typename s_type, typename v_type>
  s_type EXPONENT(const s_type x, v_type mult)
  {
    return exp(x*mult(0));
  }

  // NATURAL LOG
  template <typename s_type, typename v_type>
  s_type LOG(const s_type x, v_type mult)
  {
    return log(x*mult(0));
  }
  
}

#endif // FUNCTIONS1D_HPP
