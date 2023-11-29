void test(){

	ifstream readIn_0("../Parameter_UP/PositionUP.txt");
	ifstream readIn_1("./Position_atc.txt");
	ifstream readIn_2("./Position_up.txt");
	ifstream readIn_3("./Position_down.txt");

	double x0, y0, z0;
	double x1, y1, z1;
	double x2, y2, z2;
	double x3, y3, z3;
	double tmp;
	for(int i=0;i<112;i++){
		readIn_0 >> tmp >> x0 >> y0 >> z0;
		double r0 = TMath::Sqrt(x0*x0 + y0*y0 + z0*z0);

		readIn_1 >>  x1 >> y1 >> z1;
		double r1 = TMath::Sqrt(x1*x1 + y1*y1 + z1*z1);
		//cout<<"x1 = "<<x1<<"  y1 = "<<y1<<"  z1 = "<<z1<<endl;

		readIn_2 >>  x2 >> y2 >> z2; 
		double r2 = TMath::Sqrt(x2*x2 + y2*y2 + z2*z2);
		cout<<"x2 = "<<x2<<"  y2 = "<<y2<<"  z2 = "<<z2<<endl;

		readIn_3 >>  x3 >> y3 >> z3; 
		double r3 = TMath::Sqrt(x3*x3 + y3*y3 + z3*z3);
		//cout<<"x3 = "<<x3<<"  y3 = "<<y3<<"  z3 = "<<z3<<endl;

		double diff0 = TMath::Abs(r1-r0);
		double diff1 = TMath::Abs(r2-r0);
		double diff2 = TMath::Abs(r3-r0);

		//cout<<"PixelID = "<<i<<"  Diff0 = "<<diff0<<"  Diff1 = "<<diff1<<"  Diff2 = "<<diff2<<endl;
		cout<<"r0 = "<<r0<<"  r1 = "<<r1<<"  r2 = "<<r2<<"  r3 = "<<r3<<endl;
	}

}
