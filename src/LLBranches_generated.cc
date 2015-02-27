#ifdef FOOBARBAD
// vim: ts=4:sw=4:et:sta
#include "../interface/LLBranches.h"

using namespace roast;

void
ll::Branches::Clear()
{
    roast::Branches::Clear();

    // >>> Begin clear <<<
    L1_Charge->clear();
    L1_CorrectedD0->clear();
    L1_CorrectedDZ->clear();
    L1_EventWeight->clear();
    L1_GenMatchDaughter0Id->clear();
    L1_GenMatchDaughter0Status->clear();
    L1_GenMatchDaughter1Id->clear();
    L1_GenMatchDaughter1Status->clear();
    L1_GenMatchGrandmother00Id->clear();
    L1_GenMatchGrandmother00Status->clear();
    L1_GenMatchGrandmother01Id->clear();
    L1_GenMatchGrandmother01Status->clear();
    L1_GenMatchGrandmother10Id->clear();
    L1_GenMatchGrandmother10Status->clear();
    L1_GenMatchGrandmother11Id->clear();
    L1_GenMatchGrandmother11Status->clear();
    L1_GenMatchId->clear();
    L1_GenMatchMother0Id->clear();
    L1_GenMatchMother0Status->clear();
    L1_GenMatchMother1Id->clear();
    L1_GenMatchMother1Status->clear();
    L1_GenMatchStatus->clear();
    L1_ImpactParameter->clear();
    L1_IsElectron->clear();
    L1_IsLoose->clear();
    L1_IsMuon->clear();
    L1_IsTight->clear();
    L1_P->clear();
    L1_RelIso->clear();
    L2_Charge->clear();
    L2_CorrectedD0->clear();
    L2_CorrectedDZ->clear();
    L2_EventWeight->clear();
    L2_GenMatchDaughter0Id->clear();
    L2_GenMatchDaughter0Status->clear();
    L2_GenMatchDaughter1Id->clear();
    L2_GenMatchDaughter1Status->clear();
    L2_GenMatchGrandmother00Id->clear();
    L2_GenMatchGrandmother00Status->clear();
    L2_GenMatchGrandmother01Id->clear();
    L2_GenMatchGrandmother01Status->clear();
    L2_GenMatchGrandmother10Id->clear();
    L2_GenMatchGrandmother10Status->clear();
    L2_GenMatchGrandmother11Id->clear();
    L2_GenMatchGrandmother11Status->clear();
    L2_GenMatchId->clear();
    L2_GenMatchMother0Id->clear();
    L2_GenMatchMother0Status->clear();
    L2_GenMatchMother1Id->clear();
    L2_GenMatchMother1Status->clear();
    L2_GenMatchStatus->clear();
    L2_ImpactParameter->clear();
    L2_IsElectron->clear();
    L2_IsLoose->clear();
    L2_IsMuon->clear();
    L2_IsTight->clear();
    L2_P->clear();
    L2_RelIso->clear();
    // >>> End clear <<<
}

void
ll::Branches::Delete()
{
    roast::Branches::Delete();

    // >>> Begin delete <<<
    delete L1_Charge;
    delete L1_CorrectedD0;
    delete L1_CorrectedDZ;
    delete L1_EventWeight;
    delete L1_GenMatchDaughter0Id;
    delete L1_GenMatchDaughter0Status;
    delete L1_GenMatchDaughter1Id;
    delete L1_GenMatchDaughter1Status;
    delete L1_GenMatchGrandmother00Id;
    delete L1_GenMatchGrandmother00Status;
    delete L1_GenMatchGrandmother01Id;
    delete L1_GenMatchGrandmother01Status;
    delete L1_GenMatchGrandmother10Id;
    delete L1_GenMatchGrandmother10Status;
    delete L1_GenMatchGrandmother11Id;
    delete L1_GenMatchGrandmother11Status;
    delete L1_GenMatchId;
    delete L1_GenMatchMother0Id;
    delete L1_GenMatchMother0Status;
    delete L1_GenMatchMother1Id;
    delete L1_GenMatchMother1Status;
    delete L1_GenMatchStatus;
    delete L1_ImpactParameter;
    delete L1_IsElectron;
    delete L1_IsLoose;
    delete L1_IsMuon;
    delete L1_IsTight;
    delete L1_P;
    delete L1_RelIso;
    delete L2_Charge;
    delete L2_CorrectedD0;
    delete L2_CorrectedDZ;
    delete L2_EventWeight;
    delete L2_GenMatchDaughter0Id;
    delete L2_GenMatchDaughter0Status;
    delete L2_GenMatchDaughter1Id;
    delete L2_GenMatchDaughter1Status;
    delete L2_GenMatchGrandmother00Id;
    delete L2_GenMatchGrandmother00Status;
    delete L2_GenMatchGrandmother01Id;
    delete L2_GenMatchGrandmother01Status;
    delete L2_GenMatchGrandmother10Id;
    delete L2_GenMatchGrandmother10Status;
    delete L2_GenMatchGrandmother11Id;
    delete L2_GenMatchGrandmother11Status;
    delete L2_GenMatchId;
    delete L2_GenMatchMother0Id;
    delete L2_GenMatchMother0Status;
    delete L2_GenMatchMother1Id;
    delete L2_GenMatchMother1Status;
    delete L2_GenMatchStatus;
    delete L2_ImpactParameter;
    delete L2_IsElectron;
    delete L2_IsLoose;
    delete L2_IsMuon;
    delete L2_IsTight;
    delete L2_P;
    delete L2_RelIso;
    // >>> End delete <<<
}

void
ll::Branches::Null()
{
    roast::Branches::Null();

    // >>> Begin null <<<
    L1_Charge = 0;
    L1_CorrectedD0 = 0;
    L1_CorrectedDZ = 0;
    L1_EventWeight = 0;
    L1_GenMatchDaughter0Id = 0;
    L1_GenMatchDaughter0Status = 0;
    L1_GenMatchDaughter1Id = 0;
    L1_GenMatchDaughter1Status = 0;
    L1_GenMatchGrandmother00Id = 0;
    L1_GenMatchGrandmother00Status = 0;
    L1_GenMatchGrandmother01Id = 0;
    L1_GenMatchGrandmother01Status = 0;
    L1_GenMatchGrandmother10Id = 0;
    L1_GenMatchGrandmother10Status = 0;
    L1_GenMatchGrandmother11Id = 0;
    L1_GenMatchGrandmother11Status = 0;
    L1_GenMatchId = 0;
    L1_GenMatchMother0Id = 0;
    L1_GenMatchMother0Status = 0;
    L1_GenMatchMother1Id = 0;
    L1_GenMatchMother1Status = 0;
    L1_GenMatchStatus = 0;
    L1_ImpactParameter = 0;
    L1_IsElectron = 0;
    L1_IsLoose = 0;
    L1_IsMuon = 0;
    L1_IsTight = 0;
    L1_P = 0;
    L1_RelIso = 0;
    L2_Charge = 0;
    L2_CorrectedD0 = 0;
    L2_CorrectedDZ = 0;
    L2_EventWeight = 0;
    L2_GenMatchDaughter0Id = 0;
    L2_GenMatchDaughter0Status = 0;
    L2_GenMatchDaughter1Id = 0;
    L2_GenMatchDaughter1Status = 0;
    L2_GenMatchGrandmother00Id = 0;
    L2_GenMatchGrandmother00Status = 0;
    L2_GenMatchGrandmother01Id = 0;
    L2_GenMatchGrandmother01Status = 0;
    L2_GenMatchGrandmother10Id = 0;
    L2_GenMatchGrandmother10Status = 0;
    L2_GenMatchGrandmother11Id = 0;
    L2_GenMatchGrandmother11Status = 0;
    L2_GenMatchId = 0;
    L2_GenMatchMother0Id = 0;
    L2_GenMatchMother0Status = 0;
    L2_GenMatchMother1Id = 0;
    L2_GenMatchMother1Status = 0;
    L2_GenMatchStatus = 0;
    L2_ImpactParameter = 0;
    L2_IsElectron = 0;
    L2_IsLoose = 0;
    L2_IsMuon = 0;
    L2_IsTight = 0;
    L2_P = 0;
    L2_RelIso = 0;
    // >>> End null <<<
}

void
ll::Branches::SetBranchAddresses()
{
    roast::Branches::SetBranchAddresses();

    // >>> Begin address <<<
    fChain->SetBranchAddress("L1_Charge", &L1_Charge);
    fChain->SetBranchAddress("L1_CorrectedD0", &L1_CorrectedD0);
    fChain->SetBranchAddress("L1_CorrectedDZ", &L1_CorrectedDZ);
    fChain->SetBranchAddress("L1_EventWeight", &L1_EventWeight);
    fChain->SetBranchAddress("L1_GenMatchDaughter0Id", &L1_GenMatchDaughter0Id);
    fChain->SetBranchAddress("L1_GenMatchDaughter0Status", &L1_GenMatchDaughter0Status);
    fChain->SetBranchAddress("L1_GenMatchDaughter1Id", &L1_GenMatchDaughter1Id);
    fChain->SetBranchAddress("L1_GenMatchDaughter1Status", &L1_GenMatchDaughter1Status);
    fChain->SetBranchAddress("L1_GenMatchGrandmother00Id", &L1_GenMatchGrandmother00Id);
    fChain->SetBranchAddress("L1_GenMatchGrandmother00Status", &L1_GenMatchGrandmother00Status);
    fChain->SetBranchAddress("L1_GenMatchGrandmother01Id", &L1_GenMatchGrandmother01Id);
    fChain->SetBranchAddress("L1_GenMatchGrandmother01Status", &L1_GenMatchGrandmother01Status);
    fChain->SetBranchAddress("L1_GenMatchGrandmother10Id", &L1_GenMatchGrandmother10Id);
    fChain->SetBranchAddress("L1_GenMatchGrandmother10Status", &L1_GenMatchGrandmother10Status);
    fChain->SetBranchAddress("L1_GenMatchGrandmother11Id", &L1_GenMatchGrandmother11Id);
    fChain->SetBranchAddress("L1_GenMatchGrandmother11Status", &L1_GenMatchGrandmother11Status);
    fChain->SetBranchAddress("L1_GenMatchId", &L1_GenMatchId);
    fChain->SetBranchAddress("L1_GenMatchMother0Id", &L1_GenMatchMother0Id);
    fChain->SetBranchAddress("L1_GenMatchMother0Status", &L1_GenMatchMother0Status);
    fChain->SetBranchAddress("L1_GenMatchMother1Id", &L1_GenMatchMother1Id);
    fChain->SetBranchAddress("L1_GenMatchMother1Status", &L1_GenMatchMother1Status);
    fChain->SetBranchAddress("L1_GenMatchStatus", &L1_GenMatchStatus);
    fChain->SetBranchAddress("L1_ImpactParameter", &L1_ImpactParameter);
    fChain->SetBranchAddress("L1_IsElectron", &L1_IsElectron);
    fChain->SetBranchAddress("L1_IsLoose", &L1_IsLoose);
    fChain->SetBranchAddress("L1_IsMuon", &L1_IsMuon);
    fChain->SetBranchAddress("L1_IsTight", &L1_IsTight);
    fChain->SetBranchAddress("L1_P", &L1_P);
    fChain->SetBranchAddress("L1_RelIso", &L1_RelIso);
    fChain->SetBranchAddress("L2_Charge", &L2_Charge);
    fChain->SetBranchAddress("L2_CorrectedD0", &L2_CorrectedD0);
    fChain->SetBranchAddress("L2_CorrectedDZ", &L2_CorrectedDZ);
    fChain->SetBranchAddress("L2_EventWeight", &L2_EventWeight);
    fChain->SetBranchAddress("L2_GenMatchDaughter0Id", &L2_GenMatchDaughter0Id);
    fChain->SetBranchAddress("L2_GenMatchDaughter0Status", &L2_GenMatchDaughter0Status);
    fChain->SetBranchAddress("L2_GenMatchDaughter1Id", &L2_GenMatchDaughter1Id);
    fChain->SetBranchAddress("L2_GenMatchDaughter1Status", &L2_GenMatchDaughter1Status);
    fChain->SetBranchAddress("L2_GenMatchGrandmother00Id", &L2_GenMatchGrandmother00Id);
    fChain->SetBranchAddress("L2_GenMatchGrandmother00Status", &L2_GenMatchGrandmother00Status);
    fChain->SetBranchAddress("L2_GenMatchGrandmother01Id", &L2_GenMatchGrandmother01Id);
    fChain->SetBranchAddress("L2_GenMatchGrandmother01Status", &L2_GenMatchGrandmother01Status);
    fChain->SetBranchAddress("L2_GenMatchGrandmother10Id", &L2_GenMatchGrandmother10Id);
    fChain->SetBranchAddress("L2_GenMatchGrandmother10Status", &L2_GenMatchGrandmother10Status);
    fChain->SetBranchAddress("L2_GenMatchGrandmother11Id", &L2_GenMatchGrandmother11Id);
    fChain->SetBranchAddress("L2_GenMatchGrandmother11Status", &L2_GenMatchGrandmother11Status);
    fChain->SetBranchAddress("L2_GenMatchId", &L2_GenMatchId);
    fChain->SetBranchAddress("L2_GenMatchMother0Id", &L2_GenMatchMother0Id);
    fChain->SetBranchAddress("L2_GenMatchMother0Status", &L2_GenMatchMother0Status);
    fChain->SetBranchAddress("L2_GenMatchMother1Id", &L2_GenMatchMother1Id);
    fChain->SetBranchAddress("L2_GenMatchMother1Status", &L2_GenMatchMother1Status);
    fChain->SetBranchAddress("L2_GenMatchStatus", &L2_GenMatchStatus);
    fChain->SetBranchAddress("L2_ImpactParameter", &L2_ImpactParameter);
    fChain->SetBranchAddress("L2_IsElectron", &L2_IsElectron);
    fChain->SetBranchAddress("L2_IsLoose", &L2_IsLoose);
    fChain->SetBranchAddress("L2_IsMuon", &L2_IsMuon);
    fChain->SetBranchAddress("L2_IsTight", &L2_IsTight);
    fChain->SetBranchAddress("L2_P", &L2_P);
    fChain->SetBranchAddress("L2_RelIso", &L2_RelIso);
    // >>> End address <<<
}
#endif
