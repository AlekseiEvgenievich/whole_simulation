// **************************************************************
//
//      ------------ CrystalEyeCalorimeterSD  ------
//           by Libo Wu (20 Sep 2021)
//           Email: libo.wu@gssi.it
//
// **************************************************************

#include "G4RunManager.hh"
#include "CrystalEyeCalorimeterHit.hh"
#include "CrystalEyeCalorimeterSD.hh"
#include "CrystalEyeDetectorConstruction.hh"

#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"



//---------------------------------------------
CrystalEyeCalorimeterSD::CrystalEyeCalorimeterSD(const G4String name):G4VSensitiveDetector(name)
{
	G4RunManager* runManager = G4RunManager::GetRunManager();
	Detector = (CrystalEyeDetectorConstruction*)(runManager->GetUserDetectorConstruction());

	fNofCells = 2*Detector->GetNbOfPixel();
	fBkgTimeU = Detector->GetBackgroundTimeU();//Second
	fBkgTimeD = Detector->GetBackgroundTimeD();//Second
	fThrTime  = Detector->GetThresholdTime();//Microsecond

	fCHitID = new G4int[fNofCells];

	collectionName.insert("CalorimeterCollection");

	fhit_order = 0;
	tmp_pixelID= -1;
	flag_order = new G4bool[fNofCells];
}

//---------------------------------------------
CrystalEyeCalorimeterSD::~CrystalEyeCalorimeterSD()
{
	delete [] fCHitID;
	delete [] flag_order;
}

//---------------------------------------------
void CrystalEyeCalorimeterSD::Initialize(G4HCofThisEvent*)
{
	CalorimeterCollection = new CrystalEyeCalorimeterHitsCollection(
			SensitiveDetectorName, collectionName[0]);

	for(G4int i=0; i<fNofCells; i++){
		fCHitID[i]    = -1;
		flag_order[i] = false;
	}

}

//---------------------------------------------
G4bool CrystalEyeCalorimeterSD::ProcessHits(G4Step *aStep, G4TouchableHistory* )
{
	G4double edep = 0.;
	G4double slenght = 0.;
	G4double dedx = 0.;
	edep  = aStep->GetTotalEnergyDeposit();
	slenght = aStep->GetStepLength();

	G4TouchableHistory* theTouchable = 
		(G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());

	G4VPhysicalVolume*  crystal_bar = theTouchable->GetVolume();
	G4String pixeName = crystal_bar->GetName();
	G4int pixelId     = crystal_bar->GetCopyNo();

	G4String fparticleName = aStep->GetTrack()->GetDefinition()->GetParticleName();
	G4String fphysicsName  = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
	G4StepPoint* fpreStepPoint = aStep->GetPreStepPoint();
	G4StepPoint* fpostStepPoint= aStep->GetPostStepPoint();

	G4int    ftrackid = aStep->GetTrack()->GetTrackID();
	G4int    fstepid  = aStep->GetTrack()->GetCurrentStepNumber();
	G4double fedep    = aStep->GetTotalEnergyDeposit();
	G4double ftime    = fpreStepPoint->GetGlobalTime();
	G4LogicalVolume* fpreVolum = fpreStepPoint->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
	G4String fvolumName = fpreVolum->GetName();

	if (edep == 0.) return false;

	if( fCHitID[pixelId] == -1 ){
		CrystalEyeCalorimeterHit* CalorimeterHit = new CrystalEyeCalorimeterHit;
		fCHitID[pixelId] = CalorimeterCollection->insert(CalorimeterHit)-1;

		//G4ThreeVector txyz = aStep->GetPreStepPoint()->GetPosition();
		//G4ThreeVector worldPos = aStep->GetPreStepPoint()->GetPosition();
		//G4ThreeVector txyz = theTouchable->GetHistory()->GetTopTransform().TransformPoint(worldPos);
		//G4ThreeVector txyz = crystal_bar->GetTranslation();
		//CalorimeterHit->SetPos( txyz );
		//G4cout
		//	<<"------  CollectionID = "
		//	<<fCHitID[pixelId]
		//	<<"  PixelName: "<<pixeName.c_str()<<"  ======"
		//	<<"  pixelId = "<<pixelId
		//	<<"  X = "<<txyz.x()
		//	<<"  Y = "<<txyz.y()
		//	<<"  Z = "<<txyz.z()
		//	<<"  ------"
		//	<<G4endl;


	}
	if( fCHitID[pixelId] != -1 ){
		if( ftime < fThrTime ){
			(*CalorimeterCollection)[fCHitID[pixelId]]->AddEnergy(edep);
			(*CalorimeterCollection)[fCHitID[pixelId]]->AddLength(slenght);
			(*CalorimeterCollection)[fCHitID[pixelId]]->SetCrystalID(pixelId);
		}

		if( ftime>=fBkgTimeD && ftime<=fBkgTimeU ){
			(*CalorimeterCollection)[fCHitID[pixelId]]->AddEnergyDelay( edep );
			//G4cout
			//	<<"ftrackid: "<<ftrackid<<"  "
			//	<<"fstepid: "<<fstepid<<"  "
			//	<<"Volume Name: "<<fvolumName.c_str()<<"  "
			//	<<"Partcle Name: "<<fparticleName.c_str()<<"  "
			//	<<"StepTime: "<<ftime<<"  "
			//	<<G4endl;
		}

		if(slenght!=0){
			dedx = edep/slenght;
			(*CalorimeterCollection)[fCHitID[pixelId]]->AddnSteps();
			(*CalorimeterCollection)[fCHitID[pixelId]]->Add_dE_dx(dedx);
		}

		if( tmp_pixelID!=pixelId && flag_order[pixelId]==false ){
			tmp_pixelID = pixelId;
			(*CalorimeterCollection)[fCHitID[pixelId]]->SetCrystalOrder(fhit_order);
			flag_order[pixelId] = true;
			fhit_order++;

			//G4cout
			//	<<"  fhit_order = "<<fhit_order
			//	<<"  TrackID = "<<ftrackid
			//	<<"  StepID = "<<fstepid
			//	<<"  PixelID = "<<pixelId
			//	<<"  ParticleName = "<<fparticleName.c_str()
			//	<<"  PhysiscsName: "<<fphysicsName.c_str()
			//	<<"  edep = "<<edep
			//	<<G4endl;
		}

	}

	return true;
}

//---------------------------------------------
void CrystalEyeCalorimeterSD::EndOfEvent(G4HCofThisEvent* HCE){

	//G4cout<<"CalorimeterSD: EndOfEvent!!!"<<G4endl;

	static G4int HCID = -1;
	if(HCID<0){
		HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	}
	HCE -> AddHitsCollection(HCID, CalorimeterCollection);

	for(G4int i=0;i<fNofCells;i++){
		fCHitID[i]    = -1;
		flag_order[i] = false;
	}
	fhit_order  = 0;
	tmp_pixelID = -1;
}

//---------------------------------------------
void CrystalEyeCalorimeterSD::clear(){
}

//---------------------------------------------
void CrystalEyeCalorimeterSD::DrawAll(){
}

//---------------------------------------------
void CrystalEyeCalorimeterSD::PrintAll(){
}

//---------------------------------------------
