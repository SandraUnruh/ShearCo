//
//  writefile.cpp
//  ShearCo
//
//  Created by Sandra Unruh 2019
//  Copyright © 2019 ShearCo. All rights reserved.
//

#include "writefile.hpp"


// //***// write an output file for the tangential shear gamma(R) as a function of distance from the (stacked) lenses
void write_output_file(const std::vector<tmean_type>& theta, const std::vector<std::complex<double>>& gamma_R, const std::vector<std::complex<double>>& gamma_Rlss, const int numlens, const int numsource, const int N_annuli, const std::string& units_output, const std::string& output_data_dir)
{
  // redirect to terminal
  if(!C::output2file)
    print_result(theta, gamma_R, gamma_Rlss, numlens, numsource, N_annuli, units_output) ;

  std::string Output = output_data_dir ;
  // create file name in output directory
  if (!C::convergence)
    Output += "tshear.ascii" ;
  else 
    Output += "convergence.ascii" ;

  // open file for writing, discard any previous content
  std::ofstream OutputFile(Output.c_str(), std::ios::out | std::ios::trunc) ;

  // create header
  OutputFile << "## num of lenses\t" << numlens << "\t, num of sources\t" << numsource <<  "\n"  ;
  OutputFile << "# R_lin [" << units_output << "]\tR_log [" << units_output << "]\t\t" ;

  if (!C::convergence)
    OutputFile << "gammat\t\t\tgammax\t\t\tgammatlss\t\tgammaxlss\n" ;
  else 
    OutputFile << "kappa\t\t\tkappalss\n" ;
  

  // for each annulus calculate radius of annulus from lens and write plus correspondung gamma(radius) value into file
  for(int ann = 0; ann < N_annuli ; ann++)
  {
    OutputFile << std::fixed << theta[ann].mean[0] << "\t\t"  << theta[ann].mean[1] << "\t\t" ;
    // write to output file radius and gamma(radius)
    if (!C::convergence)
      OutputFile << std::scientific << std::real(gamma_R[ann]) <<  "\t" << std::imag(gamma_R[ann]) << "\t" << std::real(gamma_Rlss[ann]) <<  "\t" << std::imag(gamma_Rlss[ann]) << "\n"  ;
    // write to output file radius and kappa(radius)
    else 
      OutputFile << std::scientific << std::real(gamma_R[ann]) << "\t" << std::real(gamma_Rlss[ann]) << "\n"  ;
  }
    

  // check if writing succeeded
  if(!OutputFile && !C::shut_up)
  {
    std::cerr << "\nWARNING: could not write data to file: " << Output << " please check whether the output directory exists\nfunction >write_output_file< in >writefile.cpp<\n\ninstead: print result in terminal:" << std::endl;

    print_result(theta, gamma_R, gamma_Rlss, numlens, numsource, N_annuli, units_output) ;
  }

  // close output file
  OutputFile.close() ;
}


//***// write an output file filled with zeros to keep consistency
void write_empty_output_file(const std::vector<tmean_type> theta, const int N_annuli, const std::string& units_output, const std::string output_data_dir)
{
  // redirect to terminal
  if (!C::output2file)
    print_empty_result(theta, N_annuli, units_output) ;

  std::string Output = output_data_dir ;
  // create file name in output directory
  if (!C::convergence)
    Output += "tshear.ascii" ;
  else 
    Output += "convergence.ascii" ;

  // open file for writing, discard any previous content
  std::ofstream OutputFile(Output.c_str(), std::ios::out | std::ios::trunc) ;

  // create header
  OutputFile << "## num of lenses \t-1\t, num of sources\t-1\n"  ;
  OutputFile << "# R_lin [" << units_output << "]\tR_log [" << units_output << "]\t\t" ;

  if (!C::convergence)
    OutputFile << "gammat\t\t\tgammax\t\t\tgammatlss\t\tgammaxlss\n" ;
  else 
    OutputFile << "kappa\t\t\tkappalss\n" ;

  // write empty file
  for(int ann = 0; ann < N_annuli ; ann++)
  {
    OutputFile << std::fixed << theta[ann].mean[0] << "\t\t"  << theta[ann].mean[1] << "\t\t" ;
    if (!C::convergence)
      OutputFile << "0\t0\t0\t0\n"  ;
    else 
      OutputFile << "0\t0\t0\n"  ;
  }

  // check if writing succeeded
  if(!OutputFile && !C::shut_up)
    std::cerr << "\nWARNING: could not write data to file: " << Output << "\nfunction >write_empty_output_file< in >writefile.cpp<" << std::endl;

  // close output file
  OutputFile.close() ;
}


//***// output result to terminal for the tangential shear gamma(R) as a function of distance from the (stacked) lenses
void print_result(const std::vector<tmean_type>& theta, const std::vector<std::complex<double>>& gamma_R, const std::vector<std::complex<double>>& gamma_Rlss, const int numlens, const int numsource, const int N_annuli, const std::string& units_output)
{
  std::cout << "\n\nweighted number of lenses:  " << numlens << "\nweighted number of sources: " << numsource << std::endl ;

  std::cout << "R_lin [" << units_output << "]\tR_log [" << units_output << "]\t " ;
  if (!C::convergence)
    std::cout << "<gamma_t> \t <gamma_x>" << std::endl ;
  else 
    std::cout << "<kappa>" << std::endl ;
  

  for (int ann = 0; ann < N_annuli; ++ann)
  {
    std::cout.precision(4) ;
    std::cout << std::defaultfloat << theta[ann].mean[0] << "\t\t" << theta[ann].mean[1] << "\t\t" ;
    if (!C::convergence)
      std::cout << std::scientific << std::real(gamma_R[ann]) - std::real(gamma_Rlss[ann]) << " \t " << std::imag(gamma_R[ann]) - std::imag(gamma_Rlss[ann]) << std::endl;
    else 
      std::cout << std::scientific << std::real(gamma_R[ann]) - std::real(gamma_Rlss[ann]) << std::endl;
  }
  std::cout << std::endl ;
}


//***// output result to terminal with zeros to let people know
void print_empty_result(const std::vector<tmean_type>& theta, const int N_annuli, const std::string& units_output)
{
  std::cout << "R_lin [" << units_output << "]\tR_log [" << units_output << "]\t " ;
  if (!C::convergence)
    std::cout << "<gamma_t> \t <gamma_x>" << std::endl ;
  else 
    std::cout << "<kappa>" << std::endl ;

  for (int ann = 0; ann < N_annuli; ++ann)
  {
    std::cout.precision(4) ;
    std::cout << std::defaultfloat << theta[ann].mean[0] << "\t\t" << theta[ann].mean[1] << "\t\t" ;
    if (!C::convergence)
      std::cout << " 0 \t\t 0" << std::endl;
    else 
      std::cout << " 0" << std::endl;
  }
  std::cout << std::endl ;
}

//***// say hello "this is SHEARCO"
void sayhello()
{
  std::cout << std::endl ;
  std::cout.width(58);
  std::cout.fill('=');
  std::cout << '=' << std::endl;
  std::cout.fill(' ');

  if (!C::convergence)
    std::cout << "SHEARCO\ncalculating galaxy-shear correlation functions, let's go\n" ;
  else
    std::cout << "SHEARCO\ncalculating galaxy-convergence correlation functions, let's go\n" ;

  std::cout.width(58);
  std::cout.fill('=');
  std::cout << '=' << std::endl;
  std::cout.fill(' ');

  std::cout << "\nconfiguring\r" << std::flush ;
}