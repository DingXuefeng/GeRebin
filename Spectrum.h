/***************************************************************************
 * Author: Xuefeng Ding<xuefeng.ding.physics@gmail.com>
 * Date: 2017.07.22
 * Copyrighted, all rights reserved
 * ***********************************************************************/
#ifndef Spectrum_H
#define Spectrum_H
#include <vector>
#include <string>
class FileLoader;
class Histogram;
class Spectrum {
	public:
		Spectrum(double out_bin_width);
		~Spectrum();
		void SetSpectrum(const std::vector<unsigned int> &raw_spc);
		void SetCalibration(const std::vector<double> &res_par);
		void Convert(const std::string &conversion_method);
		void Write(const std::string &out_f);
		void Show() const;
        void SetLoader(FileLoader *loader_) { loader = loader_; }
        void Reset() {}
        void LoadRawSpectrum(const std::string &inputname);
	private:
		void ConvertToOutN();
		void FillOutN();
		double ADC_to_E(double ADC);

	private:
        FileLoader *loader = nullptr;
		bool res_par_ready;
		bool ADC_spc_ready;
		bool E_spc_ready;
        std::vector<double> m_res_par;
		std::vector<unsigned int> m_ADC_spc;
		std::vector<double> m_E_spc;
		std::vector<double> m_E_spc_err;
	public:
		static const double keV;
		static const double MeV;
		static const double eV;
	private:
		Histogram *ADC;
		Histogram *E;
};
#endif
