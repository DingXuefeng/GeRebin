#ifndef Spectrum_H
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <exception>
using std::exception;
using std::runtime_error;
#include "TRandom.h"
class Spectrum {
	public:
		Spectrum(double out_bin_width);
		~Spectrum();
		void SetSpectrum(const vector<unsigned int> &raw_spc);
		void SetCalibration(const vector<double> &res_par);
		void Convert(string conversion_method);
		void Write(const string out_f);
		void Show() const;
	private:
		void ConvertToOutN();
		void FillOutN();
		double ADC_to_E(double ADC);

	private:
		bool res_par_ready;
		bool ADC_spc_ready;
		bool E_spc_ready;
		vector<double> m_res_par;
		vector<unsigned int> m_ADC_spc;
		vector<double> m_E_spc;
		vector<double> m_E_spc_err;
	public:
		static double keV;
		static double MeV;
		static double eV;
		class Histogram {
			public:
				Histogram(const vector<unsigned int> &raw_spc);
				Histogram(unsigned int Nbins,double L,double R);
				bool Fill(double x);
				bool Fill(double x,double y);
				unsigned int Integral() const { return N; }
				const unsigned int GetNbins() const { return Nbins; };
				double GetRandom() const;
				double GetBinContent(int bin) const;
				double GetBinError(int bin) const;
				unsigned int GetMaximum() const { return maximum; };
			private:
				double Rand() const { return gRandom->Rndm(); };
				unsigned int Find(double x) const;
				TRandom *gRandom;
				vector<double> *biny;
				vector<double> *binyerr;
				double L;
				double R;
				unsigned int Nbins;
				double dx;
				unsigned int N;
				unsigned int maximum;
				bool bin_ready;
		};
	private:
		Histogram *ADC;
		Histogram *E;
};
#endif
#define Spectrum_H
