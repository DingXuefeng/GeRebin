//#define bg
void compare() {
  TH1F *bg_new = new TH1F("bg_new","new histogram",3000,0.5,3000.5);
  ifstream f_new;
#ifdef bg
  f_new.open("bg_1keV.dat");
#else
  f_new.open("v50_1keV.dat");
#endif
  char buff[255];
  for(int i =1;i<=3000;++i) {
    double y,yerr;
    f_new>>y>>yerr;
    bg_new->SetBinContent(i,y);
    bg_new->SetBinError(i,yerr);
  }
  f_new.close();

  TH1F *bg_old = new TH1F("bg_old","old histogram",3000,1,3001);
  ifstream f_old;
#ifdef bg
  f_old.open("../data/20150202/bg_1kev.dat");
#else
  f_old.open("../data/20150202/v_1kev_8k.dat");
#endif
  for(int i =1;i<=3000;++i) {
    double y;
    f_old>>y;
    bg_old->SetBinContent(i,y);
  }
  f_old.close();

  TH1F *bg_compare = new TH1F("bg_compare","compare histogram",3000,0,3000);
  for(int i =1;i<=3000;++i) {
	  double x = bg_compare->GetBinCenter(i);
	  double y1 = bg_new->GetBinContent(bg_new->FindBin(x));
	  double y2 = bg_old->GetBinContent(bg_old->FindBin(x));
	  if(y1>0)
		  //bg_compare->SetBinContent(i,(y2-y1)/y1);
		  bg_compare->SetBinContent(i,y2-y1);
  }

  TCanvas* cc = new TCanvas("cc","cc",800,800);
  TPad *pad1 = new TPad("pad1","pad1",0.03,0.42,0.98,0.98,0);
  TPad *pad2 = new TPad("pad2","pad2",0.03,0.02,0.98,0.40,0);
  pad1->Draw();
  pad2->Draw();
  pad1->cd();
  bg_new->SetLineColor(kBlue);
  bg_old->SetLineColor(kRed);
  bg_new->Draw("HIST");
  bg_new->GetXaxis()->SetTitle("Energy (keV)");
  bg_new->GetYaxis()->SetTitle("Entries/keV");
  bg_old->Draw("HISTsame");
  TLegend *leg = new TLegend(0.6,0.6,0.9,0.9);
  leg->AddEntry(bg_new,"NEW","l");
  leg->AddEntry(bg_old,"OLD","l");
  leg->Draw();
  pad2->cd();
  bg_compare->Draw("HIST");
  bg_compare->GetXaxis()->SetTitle("Energy (keV)");
  bg_compare->GetYaxis()->SetTitle("old-new");
  pad2->SetLogy(false);
  cc->Print("cc.pdf");
}

