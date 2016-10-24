{
	ofstream f;
	TString name[]={"test1.spe","test2.spe"};
	for(int i = 0;i<2;++i) {
		f.open(name[i].Data());
		f<<"$SPEC_ID:"<<endl;
		f<<"GeMPI4, bg, NOV-2012, w shield, w sample support plate"<<endl;
		f<<"$SPEC_REM:"<<endl;
		f<<"DET# 27"<<endl;
		f<<"DETDESC# GeMPI4 Detector"<<endl;
		f<<"AP# GammaVision Version 6.08"<<endl;
		f<<"$DATE_MEA:"<<endl;
		f<<"11/05/2012 12:09:33"<<endl;
		f<<"$MEAS_TIM:"<<endl;
		f<<"2806052 2806052"<<endl;
		f<<"$DATA:"<<endl;
		f<<"0 8191"<<endl;
		for(int i = 0;i<8192;++i) {
			f<<"       "<<(i>100)*(i<3000)*rand()/100000000<<endl;
		}
		f<<"$ROI:"<<endl;
		f<<"0"<<endl;
		f<<"$PRESETS:"<<endl;
		f<<"None"<<endl;
		f<<"0"<<endl;
		f<<"0"<<endl;
		f<<"$ENER_FIT:"<<endl;
		f<<"-15.214184 0.471975"<<endl;
		f<<"$MCA_CAL:"<<endl;
		f<<"3"<<endl;
		f<<"-1.5214184E+001 4.719750E-001 0.000000E+000 keV"<<endl;
		f<<"$SHAPE_CAL:"<<endl;
		f<<"3"<<endl;
		f<<"1.158641E+001 -2.875630E-003 0.000000E+000"<<endl;
		f.close();
	}
}
