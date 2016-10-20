#include "Spectrum.h"
/*
#include "TF1.h"
#include "TH1I.h"
#include "TH1D.h"
#include "TString.h"
#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <string>
using std::string;
#include <sstream>
using std::stringstream;

double Spectrum::keV = 1;
double Spectrum::MeV = 1e3;
double Spectrum::eV = 1e-3;

Spectrum::Spectrum() :
  in_model(NULL), in_N(NULL), in_Npdf(NULL),
  out_model(NULL), out_N(NULL), out_Npdf(NULL) {
}

void Spectrum::LoadSpectrum(const char *in_f) {
  if(!in_model) {
    cout<<"input model not set. call SetInputModel and ReadInputPar first."<<endl;
    throw -1;
  }
  ifstream in;
  in.open(in_f);
  if(!in.is_open()) {
    cout<<"File ["<<in_f<<"] not found."<<endl;
    throw -1;
  }
  // get entry of in_N
  int in_N_begin,in_N_end; {
    char buff[255];
    while(true) {
      in.getline(buff,255);
      TString buff_tstr(buff);
      if(buff_tstr.Contains("DATA")) break;
    }
    in.getline(buff,255);
    stringstream buff_s;
    buff_s<<buff;
    buff_s>>in_N_begin>>in_N_end; 
  }
  // read the histogram in_N
  { 
    double bin_edges[in_N_end+1];
    for(int i = 0;i<=in_N_end;++i)
//      bin_edges[i] = in_model->Eval(i);
      bin_edges[i] = in_model->Eval(i+0.5);
//      bin_edges[i] = in_model->Eval(i+1);
    if(in_N) delete in_N;
    in_N = new TH1I("in_N","histogram of input (entries)",in_N_end,bin_edges);
    if(in_Npdf) delete in_Npdf;
    in_Npdf = new TH1D("in_Npdf","histogram of input (entries/binWidth)",in_N_end,bin_edges);
    char buff[255];
    for(int i = 1;i<= in_N_end; ++i) {
      in.getline(buff,255);
      stringstream buff_s;
      int n_i;
      buff_s<<buff;
      buff_s>>n_i;
      in_N->SetBinContent(i,n_i);
    }
    in_N->Sumw2();
  }
  in.close();
}

void Spectrum::LoadEnergyCalibration(const char *in_f) {
  SetInputModel("[0]+[1]*x+[2]*x*x",3); 
  ifstream in;
  in.open(in_f);
  if(!in.is_open()) {
    cout<<"File ["<<in_f<<"] not found."<<endl;
    throw -1;
  }
  // get entry of in_N
  double in_pars[3]; {
    char buff[255];
    while(true) {
      in.getline(buff,255);
      TString buff_tstr(buff);
      if(buff_tstr.Contains("MCA_CAL")) break;
    }
    in.getline(buff,255);
    in.getline(buff,255);
    stringstream buff_s;
    buff_s<<buff;
    buff_s>>in_pars[0]>>in_pars[1]>>in_pars[2];
    TString unit; {
      buff_s>>unit;
      if(unit!="keV") {
	cout<<"unit of pars of input model not kev. quit."<<endl;
	throw -1;
      }
    }
  }
  in.close();
  in_model->SetParameters(in_pars);
}

void Spectrum::SetInputModel(const char *in_smodel,const int in_npar) {
  if(in_model) delete in_model;
  in_model = new TF1("in_model",in_smodel,0,10*MeV); // it's validate outside 10 MeV
}

void Spectrum::SetOutputModel(const char *out_smodel,const int out_npar) {
  if(out_model) delete out_model;
  // it's validate outside 10 MeV
  out_model = new TF1("out_model",out_smodel,0,10*MeV); 
}

void Spectrum::SetOutputPar(const double *out_pars) {
  out_model->SetParameters(out_pars);
}

void Spectrum::Convert() {
  in_N_to_in_Npdf();
  in_Npdf_to_out_N();
  out_N_to_out_Npdf();
}

void Spectrum::in_N_to_in_Npdf() {
  for(int i = 1;i<=in_N->GetNbinsX();++i) {
    in_Npdf->SetBinContent(i,in_N->GetBinContent(i)/in_N->GetBinWidth(i));
    in_Npdf->SetBinError(i,in_N->GetBinError(i)/in_N->GetBinWidth(i));
  }
}

void Spectrum::in_Npdf_to_out_N() {
  for(int i = 1;i<=out_N->GetNbinsX();++i) {
    double y,y_err;
    Integrate(y,y_err,in_Npdf,
	  out_N->GetBinLowEdge(i),
	  out_N->GetBinLowEdge(i)+out_N->GetBinWidth(i));
    out_N->SetBinContent(i,y);
    out_N->SetBinError(i,y_err);
  }
}

void Spectrum::out_N_to_out_Npdf() {
  for(int i = 1;i<=out_N->GetNbinsX();++i) {
    out_Npdf->SetBinContent(i,out_N->GetBinContent(i)/out_N->GetBinWidth(i));
    out_Npdf->SetBinError(i,out_N->GetBinError(i)/out_N->GetBinWidth(i));
  }
}


void Spectrum::SetOutput(const int Nbins) {
  SetOutputModel("[0]*x",1);
  const double out_pars[] = {1.0*Spectrum::keV};
  SetOutputPar(out_pars);
  if(out_N) delete out_N;
  out_N = new TH1D("out_N","histogram of output (entries)",Nbins,0.5*keV,(Nbins+0.5)*keV);
  if(out_Npdf) delete out_Npdf;
  out_Npdf = new TH1D("out_Npdf","histogram of output (entries/binWidth)",Nbins,
      0.5*keV,(Nbins+0.5)*keV);
}

void Spectrum::Write(const char *out_f) {
  ofstream out;
  out.open(out_f);
  out<<"Output:\nModel: "<<out_model->GetExpFormula()<<", ";
  for(int i = 0;i<out_model->GetNpar();++i)
    out<<out_model->GetParameter(i)<<", ";
  out<<endl;
  // data
  for(int i = 1;i<=out_Npdf->GetNbinsX();++i)
  out<<out_Npdf->GetBinContent(i)<<" "<<out_Npdf->GetBinError(i)<<endl;
  out.close();
}

#include <iomanip>
using std::setprecision;
void Spectrum::Show() const {
  // input
  // model
  if(!(in_model&&in_N&&in_Npdf&&out_model&&out_N&&out_Npdf)) {
    cout<<"Models not initialized."<<endl;
    return;
  }

  cout<<setprecision(10);

  cout<<"Input:\nModel: "<<in_model->GetExpFormula()<<", ";
  for(int i = 0;i<in_model->GetNpar();++i)
    cout<<in_model->GetParameter(i)<<", ";
  cout<<endl;
  // data
  cout<<"N "<<in_N->Integral()<<" Npdf "<<in_Npdf->Integral("width")<<endl;
  // model
  cout<<"Output:\nModel: "<<out_model->GetExpFormula()<<", ";
  for(int i = 0;i<out_model->GetNpar();++i)
    cout<<out_model->GetParameter(i)<<", ";
  cout<<endl;
  // data
  cout<<"N "<<out_N->Integral()<<" Npdf "<<out_Npdf->Integral("width")<<endl;
}

#include <cmath>
void Spectrum::Integrate(double &y,double &y_err,
	const TH1D* Npdf,const double x_l,const double x_r) const {
  y = 0; y_err = 0;
  for(int i = 1;i<=Npdf->GetNbinsX();++i) {
    double bin_l(Npdf->GetBinLowEdge(i));
    double bin_r(Npdf->GetBinLowEdge(i)+Npdf->GetBinWidth(i));
    double l = (bin_l>x_l)?bin_l:x_l;
    double r = (bin_r<x_r)?bin_r:x_r;
    if(r<l) continue;
    y+= Npdf->GetBinContent(i)*(r-l);
    y_err += pow(Npdf->GetBinError(i)*(r-l),2);
  }
  y_err = sqrt(y_err);
}
*/
