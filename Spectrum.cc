#include "Spectrum.h"
#include "TF1.h"
#include "TH1D.h"
#include "TString.h"
#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
using std::ofstream;

double Spectrum::keV = 1;
double Spectrum::MeV = 1e3;
double Spectrum::eV = 1e-3;

Spectrum::Spectrum(double out_bin_width) :
	ADC_to_E(NULL), in_N(NULL), in_Npdf(NULL),
	out_N(NULL), out_Npdf(NULL) {
		if(out_N) delete out_N;
		int Nbins = 10000; // 10 MeV
		out_N = new TH1D("out_N","histogram of output (entries)",Nbins,
				out_bin_width/2,(Nbins+0.5)*out_bin_width);
		if(out_Npdf) delete out_Npdf;
		out_Npdf = new TH1D("out_Npdf","histogram of output (entries/binWidth)",Nbins,
				out_bin_width/2,(Nbins+0.5)*out_bin_width);
	}

void Spectrum::SetSpectrum(vector<unsigned int> raw_spc) {
	in_N = new TH1D("in_N","histogram of input (entries)",raw_spc.size(),-0.5,raw_spc.size()-0.5);
	for(int i = 0;i<raw_spc.size();++i)
		in_N->Fill(i+1,raw_spc.at(i));
	//	in_N->Sumw2();
	in_Npdf = new TH1D("in_Npdf","histogram of input (entries/binWidth)",in_N->GetNbinsX(),-0.5,in_N->GetNbinsX()-0.5);
	for(int i = 1;i<=in_N->GetNbinsX();++i) {
		in_Npdf->SetBinContent(i,in_N->GetBinContent(i)/in_N->GetBinWidth(i));
		in_Npdf->SetBinError(i,in_N->GetBinError(i)/in_N->GetBinWidth(i));
	}
};

void Spectrum::SetCalibration(vector<double> res_par) {
	if((res_par.size()!=2)&&(res_par.size()!=3)) {
		// throw
		throw -1;
	}
	// in model: input(ADC) to E conversion
	ADC_to_E = new TF1("ADC_to_E","[0]+[1]*x+[2]*x*x",-0.5,in_N->GetNbinsX()-0.5);
	for(int i = 0;i<res_par.size();++i)
		ADC_to_E->FixParameter(i,res_par.at(i));
	if(res_par.size()==2) 
		ADC_to_E->FixParameter(2,0);
}

void Spectrum::ConvertToOutN() {
	for(int i = 1;i<=out_N->GetNbinsX();++i) {
		double y,y_err;
		Integrate(y,y_err,in_Npdf,
				out_N->GetBinLowEdge(i),
				out_N->GetBinLowEdge(i)+out_N->GetBinWidth(i));
		out_N->SetBinContent(i,y);
		out_N->SetBinError(i,y_err);
	}
}
void Spectrum::FillOutN() {
	for(int i = 0;i<in_N->Integral();++i) {
		out_N->Fill(ADC_to_E->Eval(in_Npdf->GetRandom()));
	}
	out_N->Sumw2();
}
void Spectrum::Convert(string conversion_method="ConvertToOutN") {
	if(conversion_method=="ConvertToOutN")
		ConvertToOutN();
	else if(conversion_method=="FillOutN")
		FillOutN();
	else
		throw runtime_error("Unkown conversion method. use ConvertToOutN or FillOutN");
	for(int i = 1;i<=out_N->GetNbinsX();++i) {
		out_Npdf->SetBinContent(i,out_N->GetBinContent(i)/out_N->GetBinWidth(i));
		out_Npdf->SetBinError(i,out_N->GetBinError(i)/out_N->GetBinWidth(i));
	}
}

//
//
//
void Spectrum::Write(const string out_f) {
	CheckData();
	ofstream out;
	out.open(out_f);
//	out<<"ADC to E Model: "<<ADC_to_E->GetExpFormula()<<", ";
//	for(int i = 0;i<ADC_to_E->GetNpar();++i)
//		out<<ADC_to_E->GetParameter(i)<<", ";
//	out<<endl;
	// data
	for(int i = 1;i<=out_Npdf->GetNbinsX();++i)
		out<<out_Npdf->GetBinContent(i)<<" "<<out_Npdf->GetBinError(i)<<endl;
	out.close();
}
#include "TFile.h"
void Spectrum::WriteROOT(const string out_f) {
	TFile *f = TFile::Open(out_f.c_str(),"RECREATE");
	in_N->Write();
	in_Npdf->Write();
	out_N->Write();
	out_Npdf->Write();
	f->Close();
}
//
#include <iomanip>
using std::setprecision;
void Spectrum::Show() const {
	// input
	// model
	if(!(ADC_to_E&&in_N&&in_Npdf&&out_N&&out_Npdf)) {
		cout<<"Models not initialized."<<endl;
		return;
	}

	//  cout<<setprecision(10);

	cout<<"ADC to E Model: "<<ADC_to_E->GetExpFormula()<<", ";
	for(int i = 0;i<ADC_to_E->GetNpar();++i)
		cout<<ADC_to_E->GetParameter(i)<<", ";
	cout<<endl;
	cout<<"ADC N "<<in_N->Integral()<<" Npdf "<<in_Npdf->Integral("width")<<endl;
	cout<<"E   N "<<out_N->Integral()<<" Npdf "<<out_Npdf->Integral("width")<<endl;
}
//
#include <cmath>
void Spectrum::Integrate(double &N_E,double &N_E_err,
		const TH1D* Npdf,const double E_l,const double E_r) const {
	N_E = 0; N_E_err = 0;
	int i_start = 0,i_end = 0;
	for(i_start = 1;i_start<=Npdf->GetNbinsX();++i_start) {
		if(ADC_to_E->Eval(Npdf->GetBinLowEdge(i_start)+Npdf->GetBinWidth(i_start))>E_l)
			break;
	}
	for(i_end = Npdf->GetNbinsX();i_end>=1;--i_end) {
		if(ADC_to_E->Eval(Npdf->GetBinLowEdge(i_end))<E_r)
			break;
	}
	if(i_start>i_end) return;
	//cout<<i_start<<" "<<i_end<<endl;
	for(int i = i_start; i<=i_end; ++i) {
		double bin_l(Npdf->GetBinLowEdge(i));
		double bin_r(Npdf->GetBinLowEdge(i)+Npdf->GetBinWidth(i));
		double bin_E_l(ADC_to_E->Eval(bin_l));
		double bin_E_r(ADC_to_E->Eval(bin_r));
		double l = (bin_E_l>E_l)?bin_E_l:E_l;
		double r = (bin_E_r<E_r)?bin_E_r:E_r;
		if(r<l) continue;
		double conversion_factor= (bin_r-bin_l)/(ADC_to_E->Eval(bin_r)-ADC_to_E->Eval(bin_l));
		N_E+= Npdf->GetBinContent(i)*conversion_factor*(r-l);
		N_E_err += pow(Npdf->GetBinError(i)*conversion_factor*(r-l),2); // err might be wrong.
	}
	N_E_err = sqrt(N_E_err);
}
void Spectrum::CheckData() {
	if(!(in_N&&in_Npdf&&ADC_to_E&&out_N&&out_Npdf))
		throw runtime_error("not ready for show/write");
}
