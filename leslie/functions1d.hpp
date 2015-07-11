// templates and specializations for any function in one dimension

namespace leslie
{
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
  // TEMPLATE SPECIALIZATIONS
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
}
