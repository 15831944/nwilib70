#include "stdafx.h"
#include "fifo.h"
#include "qdata.h"
#include "equity.h"

IMPLEMENT_DYNAMIC(CFifo, CObject)

void CFifo::WritePageHeading(LPCTSTR Portfolio) 
{
	CString Text;

	Text.Format("Date of Report : %s%36sNWI MANAGEMENT, LP%45sPage : %d\n", 
			(LPCTSTR) m_Date, " ", " ", m_Pages); 
	m_File.WriteString(Text);
	Text.Format("As of : %s%53s      %s  FUND\n", (LPCTSTR) m_Date, " ", Portfolio);
	m_File.WriteString(Text);
	Text.Format("%70sFIFO DEAL PROFITABILITY REPORT\n", " ");
	m_File.WriteString(Text);
	Text.Format("%70s------------------------------\n\n", " ");
	m_File.WriteString(Text);

	Text.Format("%9s %5s %13s %10s %3s %3s %c %16s %13s %13s %13s %14s %9s %13s "
		"%13s %13s %5s %s\n", "Trade","Trans","Ticket","Asset","Dl.", "Cur", 'T', 
		"Nominal", "Price", "Exchange", "Price", "Profit/Loss", "Ori Trade", 
		"Ori Price", "Ori Fxrate", "Adjust", "Trans", "H");
	m_File.WriteString(Text);

	Text.Format("%9s %5s %13s %10s %3s %3s %c %16s %13s %13s %13s %14s %9s %13s "
		"%13s %13s %5s %s\n", "Date", "Num", "Num", "Code", "Tp.", "ncy", 'T', "Amount", 
		"Local Crncy", "Rate", "In USD", "In USD", "Date", "Local Crncy", "Fxrate", 
		"Cost", "Num O", "P");
	m_File.WriteString(Text);
	
	Text.Format("%9s %5s %13s %10s %3s %3s %c %16s %13s %13s %13s %14s %9s %13s "
			"%13s %13s %5s %s\n", "---------","-----","-------------","----------",
			"---", "---", '-', "----------------", "-------------", "-------------", 
			"-------------", "--------------", "---------", "-------------", 
			"-------------", "-------------", "-----", "-");
	m_File.WriteString(Text);

	m_Pages ++;
	m_Lines = 8;
	SetChanged(TRUE);
}

BOOL CFifo::WritePage()
{
	if(m_Lines >= 64)
	{
		m_File.WriteString("\f");
		m_Lines = 1;
		return TRUE;
	}

	return FALSE;
}

void CFifo::WriteBreakLine(LPCTSTR Portfolio, double Profit)
{	
	char* Months[] = {"", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", 
					"AUG", "SEP", "OCT", "NOV", "DEC"};

	if(m_BreakDay == 0 || m_BreakMonth == 0 || m_BreakYear == 0)
		return;

	if(WritePage())
		WritePageHeading(Portfolio);

	CString Text;	
	CQData QData;

	Text.Format("%02d-%s-%02d %5s %13s %10s %3s %3s %c %16s %13s %13s %13s %14s\n", 
				m_BreakDay, Months[m_BreakMonth], m_BreakYear%100, "", "", 
				(LPCTSTR) m_Asset, (LPCTSTR) m_DealType, (LPCTSTR) m_Currency, 
				' ', "", "", "", "", QData.WriteNumber(Profit, TRUE, 2));
	
	m_Lines ++;
	SetChanged(FALSE);	
}

void CFifo::WriteLine(LPCTSTR Portfolio, CFifoData &Data)
{
	CQData QData;	
    
	if(WritePage())
		WritePageHeading(Portfolio);

	CString Text, Text2, frm1, frm2, frm3, frm4, frm5, frm6, frm7, frm8;;
	if(m_Ticket.Find("EX", 0) >= 0)
		Text.Format("%9s %5d %13s %10s %3s %3s %c ", (LPCTSTR) m_TradeDate, 
				m_TransNum, (LPCTSTR) m_Ticket, GetChanged() ? (LPCTSTR) m_Asset : "", 
				GetChanged() ? (LPCTSTR) m_DealType : "", (LPCTSTR) m_Currency, (LPCTSTR) m_Dir);
	else
		Text.Format("%9s %5d %13s %10s %3s %3s %c ", (LPCTSTR) m_TradeDate, 
				m_TransNum, (LPCTSTR) m_Ticket, GetChanged() ? (LPCTSTR) m_Asset : "", 
				GetChanged() ? (LPCTSTR) m_DealType : "", (LPCTSTR) m_Currency, (LPCTSTR) m_Dir);    

	frm1 = QData.WriteNumber(Data.GetAmount(), TRUE, 2);
	frm2 = QData.WriteNumber(m_Price, TRUE, 6);
	frm3 = QData.WriteNumber(m_Fxrate, TRUE, 6);
	frm4 = QData.WriteNumber(m_Price/m_Fxrate, TRUE, 6);
	frm5 = QData.WriteNumber(Data.GetProfit(), TRUE, 2);
	frm6 = QData.WriteNumber(Data.GetPrice(), TRUE, 6);
	frm7 = QData.WriteNumber(Data.GetFxrate(), TRUE, 6);
	frm8 = QData.WriteNumber(Data.GetCost(), TRUE, 6);
	if(m_TransType == SECURITIES || m_TransType == CDS)
	{
		Text2.Format("%16s %13s %13s %13s %14s %9s %13s %13s %13s %5ld %d\n",
				(LPCTSTR) frm1, (LPCTSTR) frm2, (LPCTSTR) frm3, (LPCTSTR) frm4, 
				(LPCTSTR) frm5, Data.GetTradeDate(), (LPCTSTR) frm6, (LPCTSTR) frm7, 
				(LPCTSTR) frm8, Data.GetTransNum(),	Data.GetHolding());
		m_TrType = m_TransType;
	}
	else /* Options */
	{
		Text2.Format("%16s %13s %13s %8s%5s %14s %9s %13s %13s %13s %5ld %d\n", 
				(LPCTSTR) frm1, (LPCTSTR) frm2, (LPCTSTR) frm3,	"Exp Unx.", 
				m_TransType, (LPCTSTR) frm5, Data.GetTradeDate(), (LPCTSTR) frm6, 
				(LPCTSTR) frm7, (LPCTSTR) frm8, Data.GetTransNum(), Data.GetHolding());
		m_TrType = "OPTION EXP UNEX";
	}
	
	Text += Text2;
	m_File.WriteString(Text);

	SetChanged(FALSE);
	m_Lines ++;	
}

void CFifo::WriteSTotalLine(LPCTSTR Portfolio, double Profit, double InProfit)
{
	if(WritePage())
		WritePageHeading(Portfolio);

	CString Text, Frm;
	CQData QData;

	Text.Format("%9s %5s %13s %10s %3s %3s %c %16s %13s %13s %13s %14s %14s\n"
				" ", " ", " ", " ", " ", " ", ' ', " ", " ", " ", " ", 
				"--------------", "--------------");
	m_File.WriteString(Text);

	Frm = QData.WriteNumber(Profit, TRUE, 2);	
	Text.Format("%9s %5s %13s %10s %3s %3s %c %16s %13s %13s %13s %14s %14s\n", 
				" ", " ", " ", " ", " ", " ", ' ', " ", " ", " ", " ", 
				(LPCTSTR) Frm, QData.WriteNumber(InProfit, TRUE, 2));
	m_File.WriteString(Text);
	m_Lines += 2;
}

void CFifo::WriteTotalLine(LPCTSTR Portfolio, double Profit, double InProfit)
{
	if(WritePage())
		WritePageHeading(Portfolio);
	
	CString Text, Frm;
	CQData QData;
	Text.Format("%9s %5s %13s %10s %3s %3s %c %16s %13s %13s %13s %14s %14s\n",
				" ", " ", " ", " ", " ", " ", ' ', " ", " ", " ", " ", 
				"==============", "==============");
	m_File.WriteString(Text);

	Frm = QData.WriteNumber(Profit, TRUE, 2);	
	Text.Format("%9s %5s %13s %10s %3s %3s %c %16s %13s %13s %13s %14s %14s\n\f", 
				" ", " ", " ", " ", " ", " ", ' ', " ", " ", " ", " ", 
				(LPCTSTR) Frm, QData.WriteNumber(InProfit, TRUE, 2));
	m_File.WriteString(Text);
}

void CFifo::AssignEntry(CFifoData &Data)
{
	Data.SetTradeDate(m_OriTradeDate);
	if(m_TransType == SECURITIES || m_TransType == CDS)
	{
		Data.SetTransNum(m_TransNum);
		Data.SetPrice(m_Price);
		Data.SetFxrate(m_Fxrate);
	}
	else /* Option */
	{
		if(m_OptNum <= 0) /* Option Expiration */
		{
			Data.SetTransNum(m_TransNum);
			Data.SetPrice(m_Price);
			Data.SetFxrate(m_Fxrate);
		}
		else
		{
			Data.SetTransNum(m_OptNum);
			Data.SetPrice(m_OptPrice);
			Data.SetFxrate(m_OptFxrate);
		}
	}
	
	Data.SetCost(Data.GetPrice()/Data.GetFxrate());
	Data.SetDir(m_Dir);
	Data.SetAmount(0);
	Data.SetRemain(m_Amount);
	Data.SetAmortFact(m_AmortFact);
	Data.SetProfit(0);	
	Data.SetHolding(-1);
	Data.SetAddable(TRUE);
}

int CFifo::CountEntry()
{
	int count = 0;
	CFifoData *pData;

	for(int i = 0; i < m_Array.GetSize(); i ++)
	{
		pData = m_Array.GetAt(i);
		if(m_Dir != pData->GetDir() && pData->GetRemain() > 0)		
			count++;
	}

	return count;
}

int CFifo::FindEntry()
{
	CFifoData *pData;

	for(int i = 0; i < m_Array.GetSize(); i ++)
	{
		pData = m_Array.GetAt(i);
		if(m_Dir != pData->GetDir() && pData->GetRemain() > 0)
			return i;
	}

	return -1;
}

void CFifo::ComputeEntryPL(CFifoData &Data, double UsdPrice, double BondFact)
{
	if(m_bFuture)
		Data.SetProfit(Data.GetAmount()*(m_Price - Data.GetPrice())*BondFact/m_Fxrate);
	else
		Data.SetProfit(Data.GetAmount()*(UsdPrice - Data.GetPrice()/Data.GetFxrate())*BondFact);
}

void CFifo::AdjustAmortFact(double AmortFact, double BondFact)
{
	CFifoData *pData;

	for(int i = 0; i < m_Array.GetSize() && AmortFact != 1; i ++)
	{
		pData = m_Array.GetAt(i);
		if(pData->GetAmortFact() != AmortFact)
		{
			if(pData->GetAmortFact() > AmortFact)
				pData->SetProfit(pData->GetProfit() + (strcmp(pData->GetDir(), "P") == 0 ? 1 : -1)*
								pData->GetRemain()*(pData->GetAmortFact() - AmortFact)*
								(100-pData->GetPrice())*BondFact);
			else 
				if(pData->GetAmortFact() < AmortFact)
				{
					pData->SetPrice(pData->GetPrice()*pData->GetAmortFact()/AmortFact);
					pData->SetFxrate(pData->GetFxrate()*pData->GetAmortFact()/AmortFact);
				}
		}
	}
}

double CFifo::PriceAdjust()
{
	double UsdPrice;

	if(m_Fxrate <= 0)
		m_Fxrate = GetFxrate(m_ClDate > 0 ? m_ClDate : m_Date);

	UsdPrice = m_Price/m_Fxrate;
	if(m_OptNum > 0)
	{
		m_OptFxrate = GetOptFxrate();
		m_Ticket.Format("%sEX%d", (LPCTSTR) m_Ticket, m_OptNum);

		if(m_TransType == SECURITIES || m_TransType == CDS) /* Securities */
		{
			if(m_ODir == m_Dir)
			{
				m_Price += m_OptPrice;
				UsdPrice += m_OptPrice/m_OptFxrate;
			}
			else 
			{
				m_Price -= m_OptPrice;
				UsdPrice -= m_OptPrice/m_OptFxrate;
			}
		}
	}

	return UsdPrice;
}

int CFifo::RunFifo(COraLoader &OraLoader, LPCTSTR Date, CString &Portfolios, 
					CString &Condition, LPCTSTR BreakDate)
{
	SetOraLoader(OraLoader);
	return RunFifo(Date, Portfolios, Condition, BreakDate);
}

int CFifo::RunFifo(LPCTSTR Date, CString &Portfolio, CString &Condition, LPCTSTR BreakDate)
{
	CString LastDir, LastPort, LastAsset, LastDeal;
	double OldAmortFact = 1, BondFact, UsdPrice;
	double AssetProfit, AssetInProfit, TotalInProfit, TotalProfit;
	CFifoData Data;
	int OldTNum = -1, Index;
   	
	if(!m_File.Open("TAXPROFIT.TXT", CFile::modeWrite))
		return -1;

	m_Date = Date;
	SetBreakDate(BreakDate);
	m_Pages = 1;
	m_Lines = 0;

	SetupPreSql(Portfolio, Condition);
	SetupSql(Portfolio, Condition);

	if(m_OraLoader.Open() != OSUCCESS)
		return -1;
	
	m_OraLoader.MoveFirst();
	while(!m_OraLoader.IsEOF())
	{
		DoFetch();
		if(OldTNum == m_TransNum)
			continue;
		
		if(m_Fxrate <= 0)
			m_Fxrate = GetFxrate(m_ClDate > 0 ? m_ClDate : m_Date);

		UsdPrice = PriceAdjust();
		if(LastPort != m_Portfolio || LastAsset != m_Asset || LastDeal != m_DealType)			 
		{
			if(m_Lines > 0)
			{
				if(m_BreakDay > 0)
					WriteBreakLine(LastPort, AssetProfit);
				WriteSTotalLine(LastPort, AssetProfit, AssetInProfit);
				if(LastPort != m_Portfolio)
					WriteTotalLine(LastPort, TotalProfit, TotalInProfit);
			}

			m_Array.RemoveAll();
			if(LastPort != m_Portfolio)
			{			
				TotalProfit = 0.0;
				TotalInProfit = 0.0;
				WritePageHeading(m_Portfolio);
				LastPort = m_Portfolio;
			}
	    			
			LastAsset = m_Asset;
			LastDeal = m_DealType;

			LastDir.Empty();
			OldAmortFact = m_AmortFact;
			AssetProfit = 0;
			AssetInProfit = 0;
		}

		BondFact = m_AssetType.Find("BOND", 0) >= 0 ? 0.01 : 1;
		if(m_TransType == SECURITIES || m_TransType == CDS)
		{
			BondFact *= (LastDir == "P" ? 1:-1)*m_AmortFact;
			if(LastDir.IsEmpty() || (LastDir == m_Dir && (m_Dir == "S" || m_Dir == "P")))
			{
				AssignEntry(Data);
				m_Array.Add(Data);					
				LastDir = m_Dir;
				Data.SetProfit(0);
				Data.SetAddable(FALSE);
//				if(old_amort_fact != amort_fact)
//				{
//					AdjustAmortFact(data_arr, amort_fact, bond_fact);
//					old_amort_fact = amort_fact;
//				}
			}
			else 
				if(LastDir != m_Dir && (LastDir == "S" || LastDir == "P") && 
					(m_Dir == "P" || m_Dir == "S"))
				{
//					if(old_amort_fact != amort_fact)
//					{
//						AdjustAmortFact(data_arr, amort_fact, bond_fact);
//						old_amort_fact = amort_fact;
//					}

					Index = FindEntry();
					while(Index >= 0 && m_Amount > 0)
					{
						Data = *m_Array.GetAt(Index);
						/* Short sale is short term gain */
						Data.SetHolding(DateHolding(Data.GetTradeDate(), m_TradeDate, 
										strcmp(Data.GetDir(), "S") == 0 ? TRUE :FALSE));
						if(m_Amount >= Data.GetRemain())
						{
							Data.SetAmount(Data.GetRemain());
							ComputeEntryPL(Data, UsdPrice, BondFact);
							m_Amount -= Data.GetAmount();
							Data.SetRemain(0);

							m_Array.SetAt(Index, Data);								
							if(m_Amount > 0)
							{
								if(CompareBreakDate(m_TradeDate) < 0)
								{
									WriteBreakLine(LastPort, AssetProfit);
									WriteLine(LastPort, Data);
									AssetInProfit += Data.GetProfit();
									TotalInProfit += Data.GetProfit();
								}	
		
								SaveResult(Data);
								AssetProfit += Data.GetProfit();
								TotalProfit += Data.GetProfit();
							
								Index = FindEntry();
								if(Index < 0) /* change dir */
								{
									AssignEntry(Data);
									m_Array.RemoveAll();
									m_Array.Add(Data);
									LastDir = m_Dir;
									Data.SetAddable(FALSE);
								}									
							}
							else
							{
								if(CountEntry() == 0)
									LastDir.Empty();
							}
						}
						else      /* if amount < item.remain */
						{
							Data.SetAmount(m_Amount);
							ComputeEntryPL(Data, UsdPrice, BondFact);
							Data.SetRemain(Data.GetRemain() - m_Amount);
							m_Array.SetAt(Index, Data);	/* Update Data Array */								
							m_Amount = 0;
						}
					}
				}
		}
		else  /* Option */
		{
			AssignEntry(Data);
			if(m_OptNum <= 0) /* Option Expiration */
			{
				Data.SetProfit((m_Dir == "P" ? -1 : 1)*m_Amount*m_AmortFact*UsdPrice*BondFact);
				/* written option expiration */
				Data.SetHolding(DateHolding(Data.GetTradeDate(), m_TradeDate, m_Dir == "S" ? 1 : 0));
			}
			else	/* Option Exercised or Unwinded */
			{
				Data.SetProfit((m_Dir == "P" ? -1 : 1)*m_Amount*(UsdPrice - m_OptPrice/m_OptFxrate));
				/* Unwind a written option */	
				Data.SetHolding(DateHolding(Data.GetTradeDate(), m_TradeDate, m_Dir == "P" ? 1 : 0));
			}
			Data.SetAmount(m_Amount);
		}
      
		if(Data.GetAddable())
		{
			if(CompareBreakDate(m_TradeDate) < 0)
			{
				WriteBreakLine(LastPort, AssetProfit);
				WriteLine(LastPort, Data);
				AssetInProfit += Data.GetProfit();
				TotalInProfit += Data.GetProfit();
			}
			
			SaveResult(Data);
			AssetProfit += Data.GetProfit();
			TotalProfit += Data.GetProfit();
		}
		
		OldTNum = m_TransNum;
		m_OraLoader.MoveNext();
	}
	 
	if(!LastPort.IsEmpty())
	{
		WriteBreakLine(LastPort, AssetProfit);
		WriteSTotalLine(LastPort, AssetProfit, AssetInProfit);
		WriteTotalLine(LastPort, TotalProfit, TotalInProfit);
	}

	return 0;
}


int CFifo::SetupPreSql(CString &Portfolios, CString &Condition)
{
	BOOL bPortAll;
	CQData QData;
	LPCTSTR Date;
	
	bPortAll = IsAll(Portfolios);
	if(m_OraLoader.ExecuteSql("DELETE FROM SEMAM.TEMP_EXERCISING_OPTIONS") != OSUCCESS)
		return OFAILURE;

	Date = QData.GetQueryDate(m_Date);
	if(bPortAll)
		m_OraLoader.GetSql().Format("INSERT INTO  SEMAM.TEMP_EXERCISING_OPTIONS (PORTFOLIO, "
			"OPTION_BACKING, NOM_AMOUNT) SELECT A.PORTFOLIO, A.OPTION_BACKING, SUM(B.NOM_AMOUNT) "
			"FROM SEMAM.NW_TR_TICKETS A "
			"JOIN SEMAM.NW_TR_TICKETS B ON (A.OPTION_BACKING = B.TRANS_NUM AND B.OPTION_BACKING IS NULL "
											"AND B.TRANS_TYPE IN ('CALL', 'PUT')) "
			"WHERE A.TRANS_TYPE IN ('PUT','CALL','SECURITIES','CDS') "
			"AND A.TRADE_DATE <= %s "
			"GROUP BY A.PORTFOLIO, A.OPTION_BACKING ", Date);
	else
		m_OraLoader.GetSql().Format("INSERT INTO  SEMAM.TEMP_EXERCISING_OPTIONS (PORTFOLIO, "
			"OPTION_BACKING, NOM_AMOUNT) SELECT A.PORTFOLIO, A.OPTION_BACKING, SUM(B.NOM_AMOUNT) "
			"FROM SEMAM.NW_TR_TICKETS A "
			"JOIN SEMAM.NW_TR_TICKETS B ON (A.OPTION_BACKING = B.TRANS_NUM "
						"AND B.OPTION_BACKING IS NULL AND B.TRANS_TYPE IN ('CALL', 'PUT')) "
			"WHERE A.TRANS_TYPE IN ('PUT','CALL','SECURITIES', 'CDS') "
			"AND A.TRADE_DATE <= %s "
			"AND A.PORTFOLIO IN (%s) "
			"GROUP BY A.PORTFOLIO, A.OPTION_BACKING ", Date, (LPCTSTR) Portfolios);
	return m_OraLoader.ExecuteSql();
}

BOOL CFifo::SetupSql(CString &Portfolios, CString &Condition)
{
	BOOL bPortAll, bCondAll;
	CString Sql;
	CQData QData;
	const char* Date;

	Date = QData.GetQueryText(m_Date);

	bPortAll = IsAll(Portfolios);
	bCondAll = IsAll(Condition);

	m_OraLoader.GetSql().Format(
		"SELECT A.PORTFOLIO, B.ASS_COUNTRY, A.ASSET_CODE, A.DEAL_TYPE, A.TRADE_DATE, "
		"A.TRANS_NUM, A.TICKET_NUM, ACTUAL_CLOSING, A.TRANS_TYPE, NVL(X.NEW_CURRENCY, "
		"A.CURRENCY), A.TRANS_DIRECTION, "
		"A.NOM_AMOUNT/DECODE(B.ASS_TYPE, 'EQUITY TYPE CAL', 1, NVL(X.CONVERSION,1)), "
		"A.PRICE/DECODE(B.ASS_TYPE,'EQUITY TYPE CAL', NVL(X.CONVERSION, 1), 1), "
		"NVL(A.FXRATE/NVL(X.CONVERSION, 1), NVL(D.FXRATE*NVL(G.CROSS_RATE,1)/NVL(X.CONVERSION,1),-1)), "
		"C.TRANS_NUM, C.PRICE, C.TRANS_DIRECTION, NVL(AMORT_FACT,1), B.ASS_TYPE, A.CURRENCY, "
		"ASS_CONV_TYPE, A.TRADE_DATE "
		"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_ASSETS B, SEMAM.NW_TR_TICKETS C, SEMAM.NW_TR_TICKETS D, "
		"SEMAM.NW_ASS_PERIODS E, SEMAM.NW_PORTFOLIOS F, SEMAM.NW_TR_INV G, SEMAM.NW_COUNTRIES X "
		"WHERE A.ASSET_CODE = B.ASS_CODE "
		"AND A.OPTION_BACKING = C.TRANS_NUM(+) "
		"AND G.FOREX_REF_NUM = D.TRANS_NUM(+) "
		"AND D.TRANS_TYPE(+) = 'FOREX' "
		"AND G.ASSET_CODE = E.ASS_CODE(+) "
		"AND E.ASS_FROM(+) <= NVL(ACTUAL_VDATE, TO_DATE(%s, 'DD-MON-YY')) "
		"AND E.ASS_TO(+) + DECODE(NVL(E.ACTION(+), 'A'), 'INCLUSIVE', 1, 0) > NVL(ACTUAL_VDATE, TO_DATE(%s, 'DD-MON-YY')) "
		"AND A.CURRENCY = X.CURRENCY "		
		"AND A.TRANS_TYPE IN ('SECURITIES', 'CDS') "
		"AND A.TRANS_DIRECTION IN ('S','P') "
		"AND A.TRADE_DATE <= %s "
		"AND A.PORTFOLIO = F.PORTFOLIO "
		"AND A.TRANS_NUM = G.TRANS_NUM ", Date, Date, Date);		
	if(bPortAll)
		m_OraLoader.GetSql() += "AND F.STATUS IS NULL ";
	else
		m_OraLoader.GetSql() += "AND A.PORTFOLIO IN (" + Portfolios + ") ";
	if(!bCondAll)
		m_OraLoader.GetSql() += "AND A.ASSET_CODE IN (" + Condition + ") ";			

	m_OraLoader.GetSql() += "UNION ALL ";
	m_OraLoader.GetSql() += 
		"SELECT A.PORTFOLIO, B.ASS_COUNTRY, A.ASSET_CODE, A.DEAL_TYPE, A.OPT_EXPIRATION, "
		"A.TRANS_NUM, A.TICKET_NUM, ACTUAL_CLOSING, A.TRANS_TYPE, A.CURRENCY, A.TRANS_DIRECTION, "
		"A.NOM_AMOUNT - NVL(O.NOM_AMOUNT, 0), A.PRICE, NVL(A.FXRATE, NVL(D.FXRATE, -1)), 0, 0, 'O', "
		"NVL(AMORT_FACT,1), B.ASS_TYPE, A.CURRENCY, ASS_CONV_TYPE, A.TRADE_DATE ";
	m_OraLoader.GetSql() += "FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_ASSETS B, SEMAM.NW_TR_TICKETS D, "
		"SEMAM.NW_ASS_PERIODS E, SEMAM.NW_PORTFOLIOS F, SEMAM.NW_TR_INV G, SEMAM.TEMP_EXERCISING_OPTIONS O ";
	Sql.Format("WHERE A.ASSET_CODE = B.ASS_CODE "
		"AND G.FOREX_REF_NUM = D.TRANS_NUM(+) "
		"AND D.TRANS_TYPE(+) = 'FOREX' "
		"AND A.TRANS_TYPE IN ('CALL','PUT') "
		"AND A.PORTFOLIO = O.PORTFOLIO(+) "
		"AND A.TRANS_NUM = O.OPTION_BACKING(+) "
		"AND A.OPTION_BACKING IS NULL "
		"AND A.NOM_AMOUNT - NVL(O.NOM_AMOUNT, 0) > 0 "
		"AND A.TRADE_DATE <= %s "
		"AND A.OPT_EXPIRATION <= %s "
		"AND G.ASSET_CODE = E.ASS_CODE(+) "
		"AND E.ASS_FROM(+) <= NVL(ACTUAL_VDATE, TO_DATE(%s, 'DD-MON-YY')) "
		"AND E.ASS_TO(+) + DECODE(NVL(E.ACTION(+), 'A'), 'INCLUSIVE', 1, 0) > NVL(ACTUAL_VDATE, TO_DATE(%s, 'DD-MON-YY')) "
		"AND A.PORTFOLIO = F.PORTFOLIO "
		"AND A.TRANS_NUM = G.TRANS_NUM ", Date, Date, Date, Date);
	m_OraLoader.GetSql() += Sql;
	if(bPortAll)
		m_OraLoader.GetSql() += "AND F.STATUS IS NULL ";
	else
		m_OraLoader.GetSql() += "AND A.PORTFOLIO IN (" + Portfolios + ") ";				
	if(!bCondAll)
		m_OraLoader.GetSql() += "AND A.ASSET_CODE IN (" + Condition + ") ";				

	m_OraLoader.GetSql() += "UNION ";
	m_OraLoader.GetSql() += 
		"SELECT A.PORTFOLIO, D.ASS_COUNTRY, NVL(B.ASSET_CODE, A.ASSET_CODE), NULL, A.TRADE_DATE, "
		"A.TRANS_NUM, A.TICKET_NUM, E.ACTUAL_CLOSING, A.TRANS_TYPE, A.CURRENCY, A.TRANS_DIRECTION, "
		"NVL(G.AMORT_FACT, 1)*A.NOM_AMOUNT, A.PRICE/DECODE(D.ASS_TYPE, 'EQUITY TYPE CAL', 1, 100), "
		"NVL(B.FXRATE, NVL(M.FXRATE*NVL(L.CROSS_RATE, 1), C.EXRATE)), B.TRANS_NUM, "
		"NVL(B.PRICE, 0)/DECODE(D.ASS_TYPE,'EQUITY TYPE CAL',1,100), 'O', NVL(H.AMORT_FACT, 1), "
		"ASS_TYPE, A.CURRENCY, ASS_CONV_TYPE, B.TRADE_DATE ";
	m_OraLoader.GetSql() += 
		"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_TR_TICKETS B, SEMAM.NW_EXRATES C, SEMAM.NW_ASSETS D, "
		"SEMAM.NW_TR_INV E, SEMAM.NW_PORTFOLIOS F, SEMAM.NW_ASS_PERIODS G, SEMAM.NW_ASS_PERIODS H, "
		"SEMAM.NW_TR_INV L, SEMAM.NW_TR_TICKETS M, SEMAM.NW_TR_TICKETS N ";
	Sql.Format("WHERE A.CURRENCY = C.CURRENCY(+) "
		"AND C.INDATE(+) = NVL(A.VALUE_DATE, TO_DATE(%s, 'DD-MON-YY')) "
		"AND B.ASSET_CODE = D.ASS_CODE(+) "
		"AND A.TRANS_NUM = E.TRANS_NUM(+) "
		"AND A.TRANS_NUM = N.TRANS_NUM(+) "
		"AND N.TRANS_TYPE(+) = 'FOREX' "
		"AND A.TRANS_TYPE IN ('PUT','CALL') "
		"AND B.TRANS_TYPE IN ('PUT','CALL') "
		"AND A.OPTION_BACKING = B.TRANS_NUM "
		"AND B.TRANS_NUM = L.TRANS_NUM(+) "
		"AND L.FOREX_REF_NUM = M.TRANS_NUM(+) "
		"AND M.TRANS_TYPE(+) = 'FOREX' "
		"AND E.ASSET_CODE = G.ASS_CODE(+) "
		"AND G.ASS_FROM(+) <= NVL(E.ACTUAL_VDATE, TO_DATE(%s, 'DD-MON-YY')) "
		"AND G.ASS_TO(+) + DECODE(NVL(G.ACTION(+), 'A'), 'INCLUSIVE', 1, 0) > NVL(E.ACTUAL_VDATE, TO_DATE(%s, 'DD-MON-YY')) "
		"AND L.ASSET_CODE = H.ASS_CODE(+) "
		"AND H.ASS_FROM(+) <= NVL(L.ACTUAL_VDATE, TO_DATE(%s, 'DD-MON-YY')) "
		"AND H.ASS_TO(+) + DECODE(NVL(H.ACTION(+), 'A'), 'INCLUSIVE', 1, 0) > NVL(L.ACTUAL_VDATE, TO_DATE(%s, 'DD-MON-YY')) "
		"AND A.PORTFOLIO = F.PORTFOLIO ", Date, Date, Date, Date, Date);
	m_OraLoader.GetSql() += Sql;
	if(bPortAll)
		m_OraLoader.GetSql() += "AND F.STATUS IS NULL ";
	else
		m_OraLoader.GetSql() += "AND A.PORTFOLIO IN (" + Portfolios +") ";				
	if(!bCondAll)
		m_OraLoader.GetSql() += "AND A.ASSET_CODE IN (" + Condition + ") ";				

	m_OraLoader.GetSql() += "UNION ";
	m_OraLoader.GetSql() += 
		"SELECT F.PORTFOLIO, B.ASS_COUNTRY, A.ASS_CODE, NULL, A.ASS_FROM, 0, '0', A.ASS_FROM, "
		"DECODE(ASS_INDUSTRY, 'G7 CDS SWAPS', 'CDS', 'EM CDS SWAPS', 'CDS', 'SECURITIES'), "
		"B.ASS_CURRENCY, 'N', 0, 0, C.EXRATE, 0, 0, 'O', A.AMORT_FACT, B.ASS_TYPE, "
		"B.ASS_CURRENCY, ASS_CONV_TYPE, A.ASS_FROM ";
	m_OraLoader.GetSql() += 
		"FROM SEMAM.NW_ASS_PERIODS A, SEMAM.NW_ASSETS B, SEMAM.NW_EXRATES C, SEMAM.NW_PORTFOLIOS F ";
	Sql.Format("WHERE A.ASS_CODE = B.ASS_CODE "
		"AND B.ASS_CURRENCY = C.CURRENCY "
		"AND A.ASS_FROM = C.INDATE "
		"AND A.AMORT_FACT <> 0 "
		"AND A.ASS_FROM <= %s "
		"AND A.ASS_TO + DECODE(NVL(A.ACTION(+), 'A'), 'INCLUSIVE', 1, 0) > %s ", Date, Date);
	m_OraLoader.GetSql() += Sql;
	if(bPortAll)
		m_OraLoader.GetSql() += "AND F.STATUS IS NULL ";
	else
		m_OraLoader.GetSql() += "AND F.PORTFOLIO IN (" + Portfolios + ") ";				
	if(!bCondAll)
		m_OraLoader.GetSql() += "AND A.ASS_CODE IN (" + Condition + ") ";				
	m_OraLoader.GetSql() += "ORDER BY 1, 2, 3, 4, 5, 6 ";

	return TRUE;
}

void CFifo::DoFetch()
{
	OValue Value;
	CString Country;

	m_Portfolio = m_OraLoader.GetDBString(0);
	Country = m_OraLoader.GetDBString(1);
	m_Asset = m_OraLoader.GetDBString(2);
	m_DealType = m_OraLoader.GetDBString(3);
	m_TradeDate = m_OraLoader.GetDBString(4);
	m_OraLoader.GetFieldValue(5, &Value);
	m_TransNum = (int) Value;
	m_Ticket = m_OraLoader.GetDBString(6);
	m_ClDate = m_OraLoader.GetDBString(7);
	m_TransType = m_OraLoader.GetDBString(8);
	m_Currency = m_OraLoader.GetDBString(9);
	m_Dir = m_OraLoader.GetDBString(10);
	m_OraLoader.GetFieldValue(11, &Value);
	m_Amount = (double) Value;
	m_OraLoader.GetFieldValue(12, &Value);
	m_Price = (double) Value;
	m_OraLoader.GetFieldValue(13, &Value);
	m_Fxrate = (double) Value;
	m_OraLoader.GetFieldValue(14, &Value);
	m_OptNum = (int) Value;
	m_OraLoader.GetFieldValue(15, &Value);
	m_OptPrice = (double) Value;
	m_ODir = m_OraLoader.GetDBString(16);
	m_OraLoader.GetFieldValue(17, &Value);
	m_AmortFact = (double) Value;
	m_AssetType = m_OraLoader.GetDBString(18);
	m_OriCurrency = m_OraLoader.GetDBString(19);
	m_OraLoader.GetFieldValue(20, &Value);
	m_bFuture = strcmp((LPCTSTR) Value, "F") == 0 ? TRUE : FALSE;
	m_OriTradeDate = m_OraLoader.GetDBString(21);
}

BOOL CFifo::IsAll(CString &String)
{
	return (String.IsEmpty() || String.Find("'ALL'", 0) >= 0 ||
			(String.GetLength() == 3 && String.Find("ALL", 0) >= 0)) ? TRUE : FALSE;
}

double CFifo::GetFxrate(LPCTSTR Date)
{
	m_OraLoader2.GetSql().Format("SELECT A.EXRATE/nvl(conversion, 1) "
			"FROM SEMAM.NW_EXRATES A, SEMAM.NW_COUNTRIES B "
			"WHERE A.CURRENCY = '%s' AND A.CURRENCY = B.CURRENCY "
			"AND A.INDATE = (SELECT MAX(C.INDATE) FROM SEMAM.NW_EXRATES C "
							"WHERE C.INDATE <= DECODE(SIGN(TO_DATE('%s') - TO_DATE('%s')), "
									"1, TO_DATE('%s'), TO_DATE('%s')) "
							"AND C.CURRENCY = A.CURRENCY " 
							"AND C.EXRATE IS NOT NULL) ", (LPCTSTR) m_OriCurrency, 
							(LPCTSTR) m_Date, Date, Date, (LPCTSTR) m_Date);
	m_OraLoader2.Open();
	OValue Value;
	m_OraLoader2.GetFieldValue(0, &Value);
	m_Fxrate = (double) Value;
	return m_Fxrate;
}
/************************************************
************************************************/
double CFifo::GetOptFxrate()
{
	if(m_OptNum <= 0)
		return m_OptFxrate;

	CString OptClDate;

	m_OraLoader2.GetSql().Format("SELECT NVL(A.FXRATE/NVL(CONVERSION, 1), "
			"NVL(C.FXRATE*NVL(B.CROSS_RATE, 1)/NVL(CONVERSION, 1), -1)), ACTUAL_CLOSING "
			"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_TR_INV B, SEMAM.NW_TR_TICKETS C, "
			"SEMAM.NW_COUNTRIES X "
			"WHERE A.TRANS_NUM = %d "
			"AND A.TRANS_TYPE IN ('CALL', 'PUT') "
			"AND A.TRANS_NUM = B.TRANS_NUM "
			"AND B.FOREX_REF_NUM = C.TRANS_NUM(+) "
			"AND C.TRANS_TYPE = 'FOREX' "
			"AND A.CURRENCY = X.CURRENCY ", m_OptNum);
	m_OraLoader2.Open();
	
	OValue Value;
	m_OraLoader2.GetFieldValue(0, &Value);
	m_OptFxrate = (double) Value;
	OptClDate = m_OraLoader2.GetDBString(1);	

	if(m_OptFxrate <= 0)
		m_OptFxrate = GetFxrate(OptClDate.IsEmpty() ? m_Date : OptClDate);

	return m_OptFxrate;
}


/************************************************
************************************************/
void CFifo::SaveResult(CFifoData &Data)
{	
	m_OraLoader2.GetSql() = "SELECT INDATE, PORTFOLIO, ASSET, DEAL_TYPE, ACTION_DATE, TRANS_NUM, "
				"CURRENCY, TR_TYPE, DIR, NOM_AMOUNT, PRICE, FXRATE, PROFIT, ORI_TRADE_DATE, "
				"ORI_TRANS_NUM, ORI_PRICE, ORI_FXRATE, HOLDING FROM SEMAM.NW_PROFITABILITY_2 ";
	
	CQData QData;
	CDBRec Rec;
	OValue Value;

	Rec.Add(m_Date);
	Rec.Add(m_Portfolio);
	Rec.Add(m_Asset);
	Rec.Add(m_DealType);
	Rec.Add(m_TradeDate);
	Value.SetValue(m_TransNum);
	Rec.Add((LPCTSTR) Value);
	Rec.Add(m_Currency);
	Rec.Add(m_TransType);
	Rec.Add(m_Dir);
	Value.SetValue(Data.GetAmount());
	Rec.Add((LPCTSTR) Value);
	Value.SetValue(m_Price);
	Rec.Add((LPCTSTR) Value);
	Value.SetValue(m_Fxrate);
	Rec.Add((LPCTSTR) Value);
	Value.SetValue(Data.GetProfit());
	Rec.Add((LPCTSTR) Value);
	Rec.Add(Data.GetTradeDate());
	Value.SetValue(Data.GetTransNum());
	Rec.Add((LPCTSTR) Value);
	Value.SetValue(Data.GetPrice());
	Rec.Add((LPCTSTR) Value);
	Value.SetValue(Data.GetFxrate());
	Rec.Add((LPCTSTR) Value);
	Value.SetValue(Data.GetHolding());
	Rec.Add((LPCTSTR) Value);
				
	m_OraLoader2.Open(m_OraLoader2.GetSql(), ODYNASET_DEFAULT);
	m_OraLoader2.UpdateRecord(Rec, TRUE);
}

void CFifo::SetBreakDate(LPCTSTR BreakDate)
{
	GetDayMonthYear(BreakDate, m_BreakDay, m_BreakMonth, m_BreakYear);	
}

int CFifo::CompareBreakDate(LPCTSTR Date)
{
	int Day, Month, Year;

	GetDayMonthYear(Date, Day, Month, Year);

	if(m_BreakYear > Year)
		return 1;
	else
		if(m_BreakYear == Year && m_BreakMonth > Month)
			return 1;
		else
			if(m_BreakYear == Year && m_BreakMonth == Month && m_BreakDay > Day)
				return 1;
			else
				if(m_BreakYear == Year && m_BreakMonth == Month && m_BreakDay == Day)
					return 0;

	return -1;
}

int CFifo::CompareDate(LPCTSTR Date1, LPCTSTR Date2)
{
	int Day1, Month1, Year1;
	int Day2, Month2, Year2;

	GetDayMonthYear(Date1, Day1, Month1, Year1);
	GetDayMonthYear(Date2, Day2, Month2, Year2);

	if(Year1 > Year2)
		return 1;
	else
		if(Year1 == Year2 && Month1 > Month2)
			return 1;
		else
			if(Year1 == Year2 && Month1 == Month2 && Day1 > Day2)
				return 1;
			else
				if(Year1 == Year2 && Month1 == Month2 && Day1 == Day2)
					return 0;

	return -1;
}
/************************************************
************************************************/
int CFifo::DateHolding(LPCTSTR Date1, LPCTSTR Date2, int IsShort)
{
	int Day1, Month1, Year1, Day2, Month2, Year2;

	if(IsShort)
		return 0;

	GetDayMonthYear(Date1, Day1, Month1, Year1);
	GetDayMonthYear(Date2, Day2, Month2, Year2);

	if(Year1 > Year2 || (Year1 == Year2 && Month1 > Month2) || 
		(Year1 == Year2 && Month1 == Month2 && Day1 > Day2))
		return -1;
	else
	{
		if(Year2 - Year1 == 0 || (Year2 - Year1 == 1 && Month2 - Month1 < 0) ||
			(Year2 - Year1 == 1 && Month2 - Month1 == 0 && Day2 - Day1 <= 0))
			return 0;
		else
			if(Year2 - Year1 < 5 || (Year2 - Year1 == 5 && Month2 - Month1 < 0) ||
				(Year2 - Year1 == 5 && Month2 - Month1 == 0 && Day2 - Day1 <= 0))
				return 1;
			else
				return 5;
	}
}
/************************************************
************************************************/
void CFifo::GetDayMonthYear(LPCTSTR Date, int &Day, int &Month, int &Year)
{
	char Buf[100];
	char *p, *q;
	char* Months[] = {"", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", 
					"JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
	int i = 0, j;
	CQData QData;
   
	strcpy_s(Buf, Date);
	p = strtok_s(Buf, "-", &q);
	while(p)
	{
		switch(i)
		{
			case 0:
				Day = atoi(p);
				break;
			case 1:
				Month = 0;
				for(j = 0; j < 13 && !Month; j++)
				{
					if(strcmp(p, Months[j]) == 0)
						Month = j;
				}
				break;
			case 2:
				Year = QData.YYToYYYY(atoi(p));
				break;
		}
      
		i++;
		p = strtok_s(NULL, "-", &q);
	}
}
/************************************************
************************************************/
int CFifo::GetJulianDays(LPCTSTR Date)
{
	int Day, Month, Year;
	char Buf[30];
	int  Mp, Yp, T;
	long Days = 1;

	GetDayMonthYear(Date, Day, Month, Year);

	if(Month <= 2)
	{
		Mp = 0;
		Yp = Year - 1;
	}
	else
	{
		sprintf_s(Buf, "%lf", 0.4*Month + 2.3);
		Mp = atoi(Buf);
		Yp = Year;
	}

	T = Yp/4 - Yp/100 + Yp/400;
	Days = Days*365*Year + 31*(Month - 1) + Day + T - Mp;
  
	return Days;
}
