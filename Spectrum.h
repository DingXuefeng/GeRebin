#ifndef Spectrum_H
class TH1I;
class TH1D;
class TF1;
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <exception>
using std::runtime_error;
class Spectrum {
	public:
		Spectrum(double out_bin_width);
		void SetSpectrum(vector<unsigned int> raw_spc);
		void SetCalibration(vector<double> res_par);
		void Convert(string conversion_method);
		void WriteROOT(const string out_f);
		void Write(const string out_f);
		void Show() const;
	private:
		void CheckData();
		void ConvertToOutN();
		void FillOutN();
		void Integrate(double &y,double &y_err,
				const TH1D* Npdf,const double x_l,const double x_r) const;

	private:
		int m_nbit;
		TF1* ADC_to_E;
		TF1* out_model;
		TH1D* in_N;
		TH1D* in_Npdf;
		TH1D* out_N;
		TH1D* out_Npdf;
	public:
		static double keV;
		static double MeV;
		static double eV;
};
#endif
#define Spectrum_H
