#include "Spectrum.h"
//#include "TH1D.h"
#include <iostream>
using std::cout;
using std::endl;
using std::cerr;
#include <fstream>
using std::ifstream;
using std::ofstream;

double Spectrum::keV = 1;
double Spectrum::MeV = 1e3;
double Spectrum::eV = 1e-3;

#include <cmath>
Spectrum::Histogram::Histogram(const vector<unsigned int> &raw_spc) :
	gRandom(new TRandom),biny(NULL),binyerr(NULL),
	L(0),R(0),Nbins(0),dx(0),N(0),maximum(0),bin_ready(false)
{
	Nbins = raw_spc.size();
	L = -0.5;
	R = Nbins-0.5;
	dx = 1;
	maximum = 0;
	biny = new vector<double>(Nbins+2,0); // underflow and overflow
	binyerr = new vector<double>(Nbins+2,0);
	bin_ready = true;
	for(int i = 0;i<Nbins;++i) {
		Fill(i,raw_spc.at(i));
	}
}
Spectrum::Histogram::Histogram(unsigned int Nbins_in,double iL,double iR) :
	gRandom(new TRandom),biny(NULL),binyerr(NULL),
	L(iL),R(iR),Nbins(Nbins_in),dx((iR-iL)/Nbins_in),N(0),maximum(0),bin_ready(false)
{
	biny = new vector<double>(Nbins+2,0); // underflow and overflow
	binyerr = new vector<double>(Nbins+2,0);
	bin_ready = true;
}

double Spectrum::Histogram::GetBinContent(int bin) const { 
	try {
		return biny->at(bin);
	} catch(exception &ex) {
		cerr<<"trying to get bin "<<bin<<" while bin ["<<L<<","<<R<<","<<Nbins<<","<<N<<"]: "<<ex.what()<<endl;
		return -1;
	};
}
double Spectrum::Histogram::GetBinError(int bin) const { 
	try {
		return binyerr->at(bin);
	} catch(exception &ex) {
		cerr<<"trying to get bin "<<bin<<" while bin ["<<L<<","<<R<<","<<Nbins<<","<<N<<"]: "<<ex.what()<<endl;
		return -1;
	};
}

unsigned int Spectrum::Histogram::Find(double x) const {
	if((x>R)||(x<L)) 
		throw runtime_error("Fill out of range for histogram");
	return (x-L)/dx+1;
}

bool Spectrum::Histogram::Fill(double x) {
	return Fill(x,1);
}

bool Spectrum::Histogram::Fill(double x,double y) {
	if(!bin_ready)
		throw runtime_error("fill while bin not ready. set L,R,N,dx first.");
	const unsigned int bin = Find(x);
	biny->at(bin)+=y;
	N+=y;
	binyerr->at(bin) = sqrt(biny->at(bin));
	if(biny->at(bin)>maximum) maximum = biny->at(bin);
	return true;
}

double Spectrum::Histogram::GetRandom() const {
	unsigned int bin = 0;
	unsigned int Nsum = 0;
	double Nsample = Rand()*Integral();
	while(Nsum<Nsample)
		Nsum+=GetBinContent(++bin);
	return (bin-(Nsum-Nsample)/GetBinContent(bin))*dx;
}
Spectrum::Spectrum(double out_bin_width) :
	res_par_ready(false), 
	ADC_spc_ready(false),E_spc_ready(true),
	in_N(NULL), in_Npdf(NULL),
	out_N(NULL), out_Npdf(NULL),
	ADC(NULL),
	E(new Histogram(10*MeV/out_bin_width,0.5*out_bin_width,10*MeV+0.5*out_bin_width))
{
	if(int(10*MeV/out_bin_width)!=10*MeV/out_bin_width) {
		throw runtime_error("out bin width should be divider of 10*MeV");
	}
	E_spc_ready = true;
		/*
		if(out_N) delete out_N;
		int Nbins = 10000; // 10 MeV
		out_N = new TH1D("out_N","histogram of output (entries)",Nbins,
				out_bin_width/2,(Nbins+0.5)*out_bin_width);
		if(out_Npdf) delete out_Npdf;
		out_Npdf = new TH1D("out_Npdf","histogram of output (entries/binWidth)",Nbins,
				out_bin_width/2,(Nbins+0.5)*out_bin_width);
				*/
}

Spectrum::~Spectrum() {
	cout<<"here"<<endl;
	delete ADC;
	delete E;
}

void Spectrum::SetSpectrum(const vector<unsigned int> &raw_spc) {
	ADC = new Histogram(raw_spc);
	ADC_spc_ready = true;
};

void Spectrum::SetCalibration(const vector<double> &res_par) {
	if((res_par.size()!=2)&&(res_par.size()!=3)) {
		// throw
		throw -1;
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
	throw runtime_error("This method is not implemented yet.");
	for(int i = 0;i<m_E_spc.size();++i) {
	}
//	for(int i = 1;i<=out_N->GetNbinsX();++i) {
//		double y,y_err;
//		Integrate(y,y_err,in_Npdf,
//				out_N->GetBinLowEdge(i),
//				out_N->GetBinLowEdge(i)+out_N->GetBinWidth(i));
//		out_N->SetBinContent(i,y);
//		out_N->SetBinError(i,y_err);
//	}
}
void Spectrum::FillOutN() {
	for(int i = 0;i<ADC->Integral();++i) {
		E->Fill(ADC_to_E(ADC->GetRandom()));
	}
}
void Spectrum::Convert(string conversion_method="ConvertToOutN") {
	if(conversion_method=="ConvertToOutN")
		ConvertToOutN();
	else if(conversion_method=="FillOutN")
		FillOutN();
	else
		throw runtime_error("Unkown conversion method. use ConvertToOutN or FillOutN");
}

//
//
//
void Spectrum::Write(const string out_f) {
	ofstream out;
	out.open(out_f);
	out<<"ADC to E Model: ["<<m_res_par.at(0)<<"]+["
		<<m_res_par.at(1)<<"]*x+["
		<<m_res_par.at(2)<<"]*x*x"<<endl;
	// data
	for(int i = 1;i<=E->GetNbins();++i)
		out<<E->GetBinContent(i)<<" "<<E->GetBinError(i)<<endl;
	out.close();
}
//
#include <iomanip>
using std::setprecision;
void Spectrum::Show() const {
	// ADC spc
	if(ADC_spc_ready) {
		cout<<"ADC: ";
		int i = 0;
		while(ADC->GetBinContent(i)==0) ++i;
		for(int j = i;j<i+3;++j)
			cout<<"ch["<<j<<"]="<<ADC->GetBinContent(j)<<" ";
		cout<<"..."<<endl;
	}
	// calib
	if(res_par_ready) {
		cout<<"Calib: "<<m_res_par.at(0)<<"+("
			<<m_res_par.at(1)<<")*x+("
			<<m_res_par.at(2)<<")*x*x"<<endl;
	}
	if(E_spc_ready) {
		cout<<"E: ";
		int i = 0;
		while(E->GetBinContent(i)==0) ++i;
		for(int j = i;j<i+3;++j)
			cout<<"ch["<<j<<"]="<<E->GetBinContent(j)<<" ";
		cout<<"..."<<endl;
	}
}
//
#include <cmath>
