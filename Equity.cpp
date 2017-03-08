#include "stdafx.h"
#include "equity.h"
#include "oradate.h"
#include "qdata.h"

double CEquity::m_SecFeeRate = 0;
double CEquity::m_OrFeeRate = 0;

IMPLEMENT_DYNAMIC(CEquity, CObject)
CEquity::CEquity(CEquity& Eq) 
{
	Setup(Eq.GetType(), Eq.GetNomAmount(), Eq.GetPrice(), Eq.GetFxRate(), Eq.GetDir()); 
}

void CEquity::Setup(LPCTSTR Type, double NomAmount, double Price, double FxRate, LPCTSTR Dir)
{
	CQData QData;
	
	SetOutPrec();
	SetNomAmount(NomAmount);
	SetFxRate(FxRate);
	SetDir(Dir);
	QData.SetString(m_Type, DefineTransType(Type, Dir));
	SetPrice(Price);  // Note SetPrice must be CALLed after SetDir and DefineTransType.
}

LPCTSTR CEquity::DefineTransType(LPCTSTR TransType, LPCTSTR Dir)
{
	CString sType, sDir;

	sType = TransType;
	sDir = Dir;

	if(TransType == SECURITIES)
	{
		if(Dir == B)
			return BORROW;
		else
			if(Dir == L)
				return LEND;
	}

	return TransType;
}

void CEquity::SetFxRate(double FxRate)
{
	m_FxRate = FxRate;
	if(m_FxRate <= 0)
		m_FxRate = 1;
}

void CEquity::SetPrice(double Price)
{
	m_Price = Price;
	if(m_Type == FOREX || m_Type == LEVERAGE || m_Type == CASH || m_Type == FEES)
		m_CalPrice = 1;
	else
		if(m_Type == CDS)
			m_CalPrice = m_Price - 100;
		else
			m_CalPrice = m_Price;
}

void CEquity::SetDir(LPCTSTR Dir)
{ // "N" Neutral dir, used for when position is zero
	if(!Dir)
		m_Dir = N;
	else
		m_Dir = Dir;
	
	if(m_Dir == P || m_Dir == S || m_Dir == L || m_Dir == B || m_Dir == N)
		return;
	else
		m_Dir = N;

	m_nDir = GetnDir();
}

int CEquity::GetnDir(LPCTSTR Dir)
{
	if(Dir)
		SetDir(Dir);
	
	if(strcmp(GetDir(), P) == 0 || strcmp(GetDir(), B) == 0)
		return 1;
	if(strcmp(GetDir(), S) == 0 || strcmp(GetDir(), L) == 0)
		return -1;

	return 0;
}

double CEquity::ComputeValue(double NomAmount, double Price)
{
	NomAmount <= 0 ? GetNomAmount() : NomAmount;
	Price <= 0 ? GetCalPrice() : Price;
	return NomAmount*Price;
}

double CEquity::GetValue(BOOL bLocal)
{
	if(bLocal)
		return ComputeValue(GetNomAmount(), GetCalPrice());
	else
		return ComputeValue(GetNomAmount(), GetCalPrice())/GetFxRate();
}

double CEquity::GetOutput(double OutVal)
{
	CQData QData;

	return QData.Round(OutVal, GetOutPrec());
}

double CEquity::GetSecFees(double GrossPrice, BOOL bSec)
{
	CQData QData;

	if(strcmp(GetDir(), S) == 0 && bSec)
		return QData.Round(GrossPrice*GetNomAmount()*m_SecFeeRate + .0049, 2);
	return 0;
}

double CEquity::GetOrFees(double Par, BOOL bOr)
{
	CQData QData;

	if(Par > 0 && (GetType() == CALL || GetType() == PUT))
		return QData.Round(GetNomAmount()/Par*m_OrFeeRate, 2);
	return 0;
}