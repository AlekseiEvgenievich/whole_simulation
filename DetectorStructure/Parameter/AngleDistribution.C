void AngleDistribution(){

	ifstream readIn_1("./Position_atc.txt");

	double x, y, z;
	TH1D* h1_the = new TH1D("theta","theta", 90, 0, 90);
	TH2D* h2_tp  = new TH2D("tp","tp",90,0,90,360,0,360);
	for(int i=0;i<112;i++){
		double pi = TMath::Pi();

		readIn_1 >>  x >> y >> z;
		double r = TMath::Sqrt(x*x + y*y + z*z);
		cout<<"x = "<<x<<"  y = "<<y<<"  z = "<<z<<endl;
		
		double dx = x/r;
		double dy = y/r;
		double dz = z/r;

		double cos_theta = dz;
		double tan_phi   = dy/dx;
		double theta = TMath::ACos( cos_theta );
		double phi   = TMath::ATan( tan_phi );

		if(x<0 && y>0)
			phi = pi + phi;
		if(x<0 && y<0)
			phi = pi + phi;
		//phi = phi - pi;
		if(x>0 && y<0)
			phi = 2*pi + phi;

		theta = theta/pi*180.;
		phi   = phi/pi*180.;


		h1_the->Fill( theta );
		h2_tp ->Fill( theta, phi );
	}

	TCanvas* can = new TCanvas();
	can->cd();
	h1_the->Draw();
	TCanvas* can1 = new TCanvas();
	can1->cd();
	h2_tp->Draw();

}
