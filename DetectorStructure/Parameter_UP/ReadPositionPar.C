
void ReadPositionPar(){


	ifstream readIn("./readInParPosPixelUP.txt");
	double pos[112][3];
	for(int i=0;i<112;i++){
		for(int j=0;j<3;j++){
			pos[i][j] = -1;
			readIn>>pos[i][j];
		}
	}

	ofstream output("./PositionUP.txt");
	for(int i=0;i<112;i++){
		output<<setiosflags(ios::left)<<setw(5) <<i+1<<"  ";
		output<<setiosflags(ios::left)<<setw(14)<<pos[i][0]<<"  ";
		output<<setiosflags(ios::left)<<setw(14)<<pos[i][1]<<"  ";
		output<<setiosflags(ios::left)<<setw(14)<<pos[i][2]<<endl;
	}


}
