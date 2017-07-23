/***************************************************************************
 * Author: Xuefeng Ding<xuefeng.ding.physics@gmail.com>
 * Date: 2017.07.22
 * Copyrighted, all rights reserved
 * ***********************************************************************/
#include "stdafx.h"

#include "Spectrum.h"
#include <iostream>
#include <fstream>

const double Spectrum::keV = 1;
const double Spectrum::MeV = 1e3;
const double Spectrum::eV = 1e-3;

#include <cmath>
#include "Histogram.h"
Spectrum::Spectrum(double out_bin_width) :
	res_par_ready(false), 
	ADC_spc_ready(false),E_spc_ready(true),
	ADC(NULL),
	E(new Histogram(10*MeV/out_bin_width,0.5*out_bin_width,10*MeV+0.5*out_bin_width))
{
	if(int(10*MeV/out_bin_width)!=10*MeV/out_bin_width) {
		throw std::runtime_error("out bin width should be divider of 10*MeV");
	}
	E_spc_ready = true;
}

Spectrum::~Spectrum() {
	delete ADC;
	delete E;
}

void Spectrum::SetSpectrum(const std::vector<unsigned int> &raw_spc) {
	ADC = new Histogram(raw_spc);
	ADC_spc_ready = true;
};

void Spectrum::SetCalibration(const std::vector<double> &res_par) {
	if((res_par.size()!=2)&&(res_par.size()!=3)) {
		// throw
		throw std::runtime_error("calibration parameters empty.");
	}
	m_res_par = res_par;
	res_par_ready = true;
}

double Spectrum::ADC_to_E(double ADC) {
	double p0 = m_res_par.at(0);
	double p1 = m_res_par.at(1);
	double p2 = m_res_par.at(2);
	return p0+p1*ADC+p2*ADC*ADC;
}

void Spectrum::ConvertToOutN() {
	throw std::runtime_error("This method is not implemented yet.");
	for(int i = 0;i<m_E_spc.size();++i) {
	}
}
void Spectrum::FillOutN() {
	for(int i = 0;i<ADC->Integral();++i) {
		E->Fill(ADC_to_E(ADC->GetRandom()));
	}
}
void Spectrum::Convert(const std::string &conversion_method) {
	if(conversion_method=="ConvertToOutN")
		ConvertToOutN();
	else if(conversion_method=="FillOutN")
		FillOutN();
	else
		throw std::runtime_error("Unkown conversion method. use ConvertToOutN or FillOutN");
}

//
//
//
void Spectrum::Write(const std::string &out_f) {
  std::ofstream out;
	out.open(out_f.c_str());
	std::cout<<"E spectrum written to ["<<out_f<<"] successfully."<<std::endl;
	std::cout<<std::endl;
	Show();
	std::cout<<"output: on [N] line there is the entries in [-0.5keV+N*keV,+0.5keV+N*keV)"<<std::endl;
	std::cout<<"eg: first line is the entries in [0.5keV, 1.5keV)"<<std::endl;
	// data
	for(int i = 1;i<=E->GetNbins();++i)
		out<<E->GetBinContent(i)<<" "<<E->GetBinError(i)<<std::endl;
	out.close();
}
//
void Spectrum::Show() const {
	// ADC spc
	if(ADC_spc_ready) {
		std::cout<<"ADC: "<<ADC->GetNbins()<<" ";
		int i = 0;
		while(ADC->GetBinContent(i)==0) ++i;
		for(int j = i;j<i+3;++j)
			std::cout<<"ch["<<j<<"]="<<ADC->GetBinContent(j)<<" ";
		std::cout<<"..."<<std::endl;
	}
	// calib
	if(res_par_ready) {
		std::cout<<"Calib("<<m_res_par.size()<<"): "<<m_res_par.at(0)<<"+("
			<<m_res_par.at(1)<<")*x+("
			<<m_res_par.at(2)<<")*x*x"<<std::endl;
	}
	if(E_spc_ready) {
		std::cout<<"E: "<<E->GetNbins()<<" ";
		int i = 0;
		while(E->GetBinContent(i)==0) ++i;
		for(int j = i;j<i+3;++j)
			std::cout<<"ch["<<j<<"]="<<E->GetBinContent(j)<<" ";
		std::cout<<"..."<<std::endl;
	}
}

#include "FileLoader.h"
void Spectrum::LoadRawSpectrum(const std::string &inputname) {
  loader->ParseRawDataFile(inputname);
  SetSpectrum(loader->get_raw());
  SetCalibration(loader->get_res_par());
}
