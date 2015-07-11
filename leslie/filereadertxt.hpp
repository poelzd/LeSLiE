// this will become the filereader for basic tetx files

// include guard
#ifndef FILEREADERTXT_HPP
#define FILEREADERTXT_HPP

namespace leslie
{

  // filereader class
  template<typename s_type, typename v_type>
  class filereader
  {
  private:
    char fname[1024];
  public:
    filereader() : fname() {}
    filereader(const std::string& filename)
    {
      strncpy(fname, filename.c_str(), sizeof(fname));
      fname[sizeof(fname) - 1] = 0;
    }

    char* getFileName()
    {
      return fname;
    }
    
    // overload operator() to fill up x-data and y-data
    void operator() (v_type& vec_x, v_type& vec_y)
    {
      //std::fstream filestream(fname, std::ios_base::in);
      std::ifstream filestream;
      filestream.open(fname);
      // set up std vectors of zero length (push_back will expand it later on)
      std::vector<s_type> inpvec(0);
      float a;
      while (filestream >> a)
      {
        inpvec.push_back(a);
      }
      filestream.close();
      
      // TODO: SET UP ERROR WHEN SIZE OF INPVEC IS NOT EVEN!!! (INP FILE IS
      // WRONG)

      // resize input arrays
      vec_x.resize(inpvec.size()/2,1);
      vec_y.resize(inpvec.size()/2,1);

      for (unsigned int p=0; p<vec_x.size(); ++p)
      {
        vec_x(p) = inpvec[p];
        vec_y(p) = inpvec[p+vec_x.size()];
      }
    }
    
  };


}

#endif // FILEREADERTXT_HPP
