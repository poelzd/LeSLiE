// filewriter for basic txt documents

// include guard
#ifndef FILEWRITERTXT_HPP
#define FILEWRITERTXT_HPP

namespace leslie
{

  // filewriter class
  template<typename s_type, typename v_type>
  class filewriter
  {
  private:
    char fname[1024];
  public:
    filewriter() : fname() {}
    filewriter(const std::string& filename)
    {
      strncpy(fname, filename.c_str(), sizeof(fname));
      fname[sizeof(fname) - 1] = 0;
    }

    char* getFileName()
    {
      return fname;
    }
    
    // overload operator() to write result
    void operator() (v_type& solution)
    {
      //std::fstream filestream(fname, std::ios_base::out);
      std::ofstream filestream;
      filestream.open(fname);
      
      for (unsigned int i=0; i<solution.size(); ++i)
      {
        filestream << solution(i) << '\n';
      }
      filestream.close();
    }       
  };

}

#endif // FILEWRITERTXT_HPP
