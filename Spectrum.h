#ifndef Spectrum_H
class TH1I;
class TH1D;
class TF1;
class Spectrum {
	 /*
  public:
    Spectrum();
    void LoadSpectrum(const char *in_f);
    void LoadEnergyCalibration(const char *in_f);
    void SetInputModel(const char *in_smodel,const int in_npar);
    void SetOutput(const int Nbins);
    void Convert();
    void Write(const char *out_f);
    void Show() const;
  private:
    void SetOutputModel(const char *out_smodel,const int out_npar);
    void SetOutputPar(const double *out_pars);
    void in_N_to_in_Npdf();
    void in_Npdf_to_out_N();
    void out_N_to_out_Npdf();
    void Integrate(double &y,double &y_err,
	const TH1D* Npdf,const double x_l,const double x_r) const;

  private:
    TF1* in_model;
    TF1* out_model;
    TH1I* in_N;
    TH1D* in_Npdf;
    TH1D* out_N;
    TH1D* out_Npdf;
  public:
    static double keV;
    static double MeV;
    static double eV;
	 */
};
#endif
#define Spectrum_H
