// classes that set up the function space of the llsq

// include guard
#ifndef FUNCTIONSPACE1D_HPP
#define FUNCTIONSPACE1D_HPP

namespace leslie
{
  template <typename s_type, typename v_type, typename m_type,
            typename BaseFunction>
  class FunctionSpace1d
  {
  private:
    //Eigen::Matrix<BaseFunction,Eigen::Dynamic,1> basis;
    //Eigen::Matrix<BaseFunction,Eigen::Dynamic,1> basis;
    std::vector<BaseFunction> basis;
  public:
    //ctor
    FunctionSpace1d() : basis() {}
    FunctionSpace1d(unsigned int N) : basis(N) {}

    // member functions
    unsigned int getDim()
    {
      return basis.size();
    }

    void setFunction(const BaseFunction& func, int i)
    {
      basis(i) = func;
    }

    void addFunction(const BaseFunction& func)
    {
      basis.push_back(func);
    }

    ////////////////////////////////////////////////////////////////////////////
    // EVALUATES

    // evaluate a single function at a single point and return it
    s_type evaluate(const int& i, const s_type& x)
    {
      return basis[i].evaluate(x);
    }

    // evaluate a single function at a single point and write res to reference
    void evaluate(const int& i, const s_type& x, s_type& y)
    {
      y = basis[i].evaluate(x);
    }

    // evaluate a single function at many points (vector - discrete axis)
    void evaluate(const int& i, const v_type& x, v_type& y)
    {
      for (unsigned int l=0; x.size(); ++l)
        y(l) = basis[i].evaluate(x(l));
    }

    // evaluate entire basis at one single point
    void evaluate(const s_type& x, v_type& y)
    {
      for (unsigned int k=0; k<basis.size(); ++k)
        y(k) = basis[k].evaluate(x);
    }

    // evaluate entire basis at many points (vector - discrete axis)
    void evaluate(const v_type& x, m_type& y)
    {
      for (unsigned int k=0; k<basis.size(); ++k)
        for (unsigned int l=0; l<x.size(); ++l)
          y(l,k) = basis[k].evaluate(x(l));
    }

  };

}

#endif // FUNCTIONSPACE1D_HPP
