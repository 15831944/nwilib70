#include "stdafx.h"
#include "avcost.h"
#include "assetval.h"
#include "nwivar.h"


const int AVCOST_NEW = 10000;

IMPLEMENT_DYNAMIC(CAvCost, CObject)
CAvCost::CAvCost()
{
	m_RunIDRec.RemoveAll();
}

CAvCost::CAvCost(CAvCost &AvCost)
{
	m_OraLoader = AvCost.GetOraLoader();
	GetRunIDRec().Copy(AvCost.GetRunIDRec());
}

CAvCost::CAvCost(COraLoader &OraLoader, CStringArray &RunIDRec)
{
	m_OraLoader = OraLoader;
	GetRunIDRec().Copy(RunIDRec);
}

int CAvCost::IsDiff()
{
	if(GetRunIDRec().GetSize() <= 0)
		return AVCOST_NEW;

	if(GetRunIDRec().GetSize() <= GetIDRec().GetSize())
	{
		for(int i = 0; i < GetRunIDRec().GetSize(); i++)
			if(strcmp(GetIDRecAt(GetRunIDRec(), i), GetIDRecAt(GetIDRec(), i)))
				return i + 1;
	}

	return 0;
}

const char* CAvCost::GetIDRecAt(CStringArray &IDRec, int Index)
{
	if(Index >= 0 && Index < IDRec.GetSize())
		return (const char*) IDRec.GetAt(Index);
	else
		return EMPTYSTRING;
}

double CAvCost::PriceAdjust(double &Price, LPCTSTR Dir, double OptPrice, LPCTSTR OptDir)
{
	if(OptDir && (strcmp(OptDir, P) == 0 || strcmp(OptDir, S) == 0)) // Option Exist
	{
		if(strcmp(Dir, OptDir) == 0)
			Price += OptPrice;
		else
			Price -= OptPrice;
	}

	SetPrice(Price);
	return Price;
}

void CAvCost::AmortFactAdjust()
{
	if(GetRunAmortFact() == GetAmortFact())
		return;

	if(GetRunAmortFact() > GetAmortFact())
	{
		SetCapProfit(GetnRunDir()*GetRunBondFact()*GetRunNomAmount()*
					(GetRunAmortFact() - GetAmortFact())*(100 - GetRunPrice())/GetFxRate());
		SetFxProfit(GetnRunDir()*GetRunBondFact()*GetRunNomAmount()*
					(GetRunAmortFact() - GetAmortFact())*
					(100/GetRunFxRate() - GetRunPrice()/GetFxRate()));
		SetFxProfit(GetFxProfit() - GetCapProfit());
		SetNetCapProfit(GetNetCapProfit() + GetCapProfit());
		SetNetFxProfit(GetNetFxProfit() + GetFxProfit());
	}
	else
	{
		SetRunPrice(GetRunPrice()*GetRunAmortFact()/GetAmortFact());
		SetRunHostPrice(GetRunHostPrice()*GetRunAmortFact()/GetAmortFact());
	}

	SetRunAmortFact(GetAmortFact());
}

void CAvCost::Assign(CStringArray &IDRec, LPCTSTR Type, double NomAmount, double Price, 
					 double FxRate, LPCTSTR Dir, double OptPrice, LPCTSTR OptDir, 
					 double BondFact, double AmortFact, BOOL bFuture)
{
	CEquity Eq;

	PriceAdjust(Price, Dir, OptPrice, OptDir);
		
	m_TransType = Eq.DefineTransType(Type, Dir);
	GetIDRec().Copy(IDRec);
	SetNomAmount(NomAmount);
	SetPrice(Price);
	SetFxRate(FxRate);
	SetHostPrice(Price/FxRate);
	SetBondFact(BondFact);
	SetAmortFact(AmortFact);
	SetDir(Dir);
	if(bFuture)
		m_TransType = FUTURE;
}

int CAvCost::Update()
{
	if(m_TransType == SECURITIES || m_TransType == CDS || m_TransType == FUTURE)
	{
		GetRunIDRec().Copy(GetIDRec());
		SetRunNomAmount(GetNomAmount());
		SetRunPrice(GetPrice());
		SetRunFxRate(GetFxRate());
		SetRunHostPrice(GetHostPrice());
		SetRunBondFact(GetBondFact());
		SetRunAmortFact(GetAmortFact());
		SetRunDir(GetDir());
	}
	else
		if(m_TransType == CALL || m_TransType == PUT)
			SetCapProfit(-1*GetnDir(GetDir())*GetNomAmount()*GetAmortFact()*
						GetPrice()*GetBondFact()/GetFxRate());
		else
			if(m_TransType == CASH)
				SetCapProfit(GetnDir(GetDir())*(GetNomAmount()*GetAmortFact() + GetPrice()));
			else
				SetCapProfit(GetnDir(GetDir())*GetNomAmount()*GetAmortFact()*
							GetPrice()*GetBondFact()/GetFxRate());

	SetNetCapProfit(GetCapProfit());
	SetNetFxProfit(GetFxProfit());

	return 0;
}

int CAvCost::Compute()
{
	double FxProf, Prof, Amount;
	int Ret;
	CAmortBond Val, CurrVal;

	SetCapProfit(0);
	SetFxProfit(0);
	if((Ret = IsDiff()) > 0)
		return Ret;

	if(m_TransType == SECURITIES || m_TransType == CDS || m_TransType == FUTURE)
	{
		AmortFactAdjust();
		if(m_Dir == N)				
			return 0;

		switch(CompareDir())
		{
			case 1: // Dir == "N", No Previous Position or Previous Position is zero
				Update();
				break;
			case 2: // Same Dir
				Amount = GetRunNomAmount() + GetNomAmount();
				if(Amount != 0)
				{
					SetRunPrice((GetRunNomAmount()*GetRunPrice() + GetNomAmount()*GetPrice())/Amount);
					SetRunHostPrice((GetRunNomAmount()*GetRunHostPrice() + GetNomAmount()*GetPrice()/GetFxRate())/Amount);
					SetRunNomAmount(Amount);
				}
				break;
			case 3: // Different Dir
				CurrVal.Setup(GetOraLoader(), SECURITIES, GetNomAmount(), GetPrice(), 
						GetFxRate(), GetBondFact(), GetAmortFact(), GetDir());
				Val.Setup(GetOraLoader(), SECURITIES, GetRunNomAmount(), GetRunPrice(), 
						GetRunFxRate(), GetRunBondFact(), GetRunAmortFact(), 
						(LPCTSTR) GetRunDir());
				if(GetNomAmount() >= GetRunNomAmount())
				{
					CurrVal.SetNomAmount(GetRunNomAmount());
					Prof = GetnRunDir()*(CurrVal.GetValue() - Val.GetValue())/GetFxRate();
					if(m_TransType == FUTURE)
						FxProf = GetnRunDir()*(CurrVal.GetValue() - Val.GetValue())/GetFxRate();
					else
						FxProf = GetnRunDir()*(CurrVal.GetValue(FALSE) - Val.GetValue(FALSE));
					SetCapProfit(GetCapProfit() + Prof);
					SetFxProfit(GetFxProfit() + Prof - FxProf); 
					SetRunNomAmount(GetNomAmount() - GetRunNomAmount()); // Set to current Transaction
					if(GetNomAmount() != 0)
					{
						SetRunDir(GetDir());
						SetRunPrice(GetPrice());
						SetRunHostPrice(GetHostPrice());
						SetRunFxRate(GetFxRate());
					}
					else
						SetRunDir(N);
				}
				else
				{
					Val.SetNomAmount(GetNomAmount());
					Prof = GetnRunDir()*(CurrVal.GetValue() - Val.GetValue())/GetFxRate();
					if(m_TransType == FUTURE)
						FxProf = GetnRunDir()*(CurrVal.GetValue() - Val.GetValue())/GetFxRate();
					else
						FxProf = GetnRunDir()*(CurrVal.GetValue(FALSE) - Val.GetValue(FALSE));
					SetCapProfit(GetCapProfit() + Prof);
					SetFxProfit(GetFxProfit() + Prof - FxProf);
					SetRunNomAmount(GetRunNomAmount() - GetNomAmount()); // Set Position
				}
				break;
			default:
				break;
		}
	}
	else
		if(m_TransType == CALL || m_TransType == PUT)
			SetCapProfit(-1*GetnDir(GetDir())*GetNomAmount()*GetAmortFact()*
						GetPrice()*GetBondFact()/GetFxRate());
		else
			if(m_TransType == CASH)
				SetCapProfit(GetnDir(GetDir())*(GetNomAmount()*GetAmortFact() + GetPrice()));
			else
				SetCapProfit(GetnDir(GetDir())*GetNomAmount()*GetAmortFact()*
							GetPrice()*GetBondFact()/GetFxRate());
	
	SetNetCapProfit(GetNetCapProfit() + GetCapProfit());
	SetNetFxProfit(GetNetFxProfit() + GetFxProfit());
	
	return 0;
}			

int CAvCost::CompareDir()
{
	if(m_Dir == P || m_Dir == S)
	{
		if(m_RunDir == N)
			return 1;
		else
			if(m_RunDir == m_Dir)
				return 2;
			else
				return 3;
	}

	return 0;
}

int CAvCost::GetnDir(LPCTSTR Dir)
{
	if(strcmp(Dir, P) == 0 || strcmp(Dir, B) == 0)
		return 1;
	if(strcmp(Dir, S) == 0 || strcmp(Dir, L) == 0)
		return -1;

	return 0;
}

